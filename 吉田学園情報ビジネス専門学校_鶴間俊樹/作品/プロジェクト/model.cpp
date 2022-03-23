//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_FILE_NAME_MODEL "data/TEXT/load_model.txt"
#define OUTLINE_SIZE (1.0f)	//輪郭のサイズ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CModel::ModelData CModel::m_aModelData[(int)MODELTYPE::ENUM_MAX] = {};
char CModel::m_aFilePath[(int)MODELTYPE::ENUM_MAX][MAX_LOAD_TEXT_MODEL] = {};
D3DXMATERIAL CModel::m_aMatDefault[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL] = {};
CTexture::TEXTURE_TYPE CModel::m_aTexType[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL] = {};

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CModel::CModel()
{
	m_modelType = (MODELTYPE) 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pParent = NULL;
	m_nIdxParent = -1;
	m_bOutline = false;
	m_pCloneMesh = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// モデルの生成処理
//=============================================================================
CModel* CModel::Create(MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine) {
	CModel* pModel;
	pModel = new CModel;
	if (pModel != NULL) {
		pModel->m_modelType = type;
		pModel->m_pos = pos;
		pModel->m_rot = rot;
		pModel->m_pParent = pParent;
		pModel->m_bOutline = bOutLine;
		//マテリアルの設定
		for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
		{
			pModel->m_aMat[nCntMat] = m_aMatDefault[(int)type][nCntMat];
		}

		pModel->Init();
	}

	return pModel;
}

//=============================================================================
//モデルデータの読み込み
//=============================================================================
HRESULT CModel::Load(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return S_OK;

	//-------------------------------------
	//ファイルの読み込み
	//-------------------------------------
	FILE *pFile;		//ファイルへのポインタ
	char sModelText[MAX_LOAD_TEXT_MODEL];//一行ずつ読み込むファイルのテキスト
	char *pModelText;	//ファイルのテキストを分割した文字列
	int nIdxType = 0;	//次に読み込むモデルの種類

	pFile = fopen(TEXT_FILE_NAME_MODEL, "r");

	if (pFile != NULL) {
		while (fgets(sModelText, MAX_LOAD_TEXT_MODEL, pFile) != NULL) //一行ごとに文字列を取得
		{
			pModelText = strtok(sModelText, " =\t\n");	//文字列の分割（空白 タブ 改行 ＝）
			if (pModelText != NULL) {
				//コメント
				if (strstr(pModelText, "//") != NULL) {
					continue;
				}
				//ディレクトリ名のコピー
				strcpy(m_aFilePath[nIdxType], pModelText);
				nIdxType++;	//インデックスを加算
				if (nIdxType >= (int)MODELTYPE::ENUM_MAX) 	break;	//読み込むモデルの最大数を超えたら終了	
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else {
		//ファイルの読み込み失敗時
		return S_OK;
	}

	//-------------------------------------
	//モデルの読み込み
	//-------------------------------------
	for (int nCntModel = 0; nCntModel < (int)MODELTYPE::ENUM_MAX; nCntModel++) {
		//すでにモデルが生成されていた場合
		//マテリアルバッファの解放
		if (m_aModelData[nCntModel].pBuffMat != NULL) {
			m_aModelData[nCntModel].pBuffMat->Release();
			m_aModelData[nCntModel].pBuffMat = NULL;
		}
		//メッシュの解放
		if (m_aModelData[nCntModel].pMesh != NULL) {
			m_aModelData[nCntModel].pMesh->Release();
			m_aModelData[nCntModel].pMesh = NULL;
		}

		//モデルの生成
		D3DXLoadMeshFromX(m_aFilePath[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,		//ここまで固定
			&m_aModelData[nCntModel].pBuffMat,
			NULL,
			&m_aModelData[nCntModel].nNumMat,
			&m_aModelData[nCntModel].pMesh);

		//テクスチャタイプの設定	
		if (m_aModelData[nCntModel].pBuffMat == NULL || m_aModelData[nCntModel].nNumMat <= 0) continue;	//マテリアルバッファがNULLか、マテリアル数が0以下なら以降の処理を飛ばす

		D3DXMATERIAL *pMat;	//マテリアルへのポインタ
		pMat = (D3DXMATERIAL*)m_aModelData[nCntModel].pBuffMat->GetBufferPointer();	//マテリアル情報に対するポインタを取得

		for (int nCntMat = 0; nCntMat < (int)m_aModelData[nCntModel].nNumMat; nCntMat++) {
			//デフォルトのマテリアルを取得
			m_aMatDefault[nCntModel][nCntMat] = pMat[nCntMat];

			if (pMat[nCntMat].pTextureFilename != NULL) {
				//テクスチャクラスのパスと比較
				for (int nCntTex = 1; nCntTex < (int)CTexture::TEXTURE_TYPE::ENUM_MAX; nCntTex++) {
					//文字列が一致でテクスチャの番号を決める
					if (strcmp(pMat[nCntMat].pTextureFilename, CTexture::GetPathName((CTexture::TEXTURE_TYPE)nCntTex)) == 0) {
						m_aTexType[nCntModel][nCntMat] = (CTexture::TEXTURE_TYPE)nCntTex;
						break;
					}
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
//モデルデータの解放
//=============================================================================
void CModel::Unload(void) {
	for (int nCnt = 0; nCnt < (int)MODELTYPE::ENUM_MAX; nCnt++) {
		//マテリアルバッファの解放
		if (m_aModelData[nCnt].pBuffMat != NULL) {
			m_aModelData[nCnt].pBuffMat->Release();
			m_aModelData[nCnt].pBuffMat = NULL;
		}
		//メッシュの解放
		if (m_aModelData[nCnt].pMesh != NULL) {
			m_aModelData[nCnt].pMesh->Release();
			m_aModelData[nCnt].pMesh = NULL;
		}
	}
}

//=============================================================================
// モデルデータの取得
//=============================================================================
CModel::ModelData CModel::GetModelData(MODELTYPE type) {
	return m_aModelData[(int)type];
}

//=============================================================================
// モデルのパスの取得
//=============================================================================
char* CModel::GetPathName(MODELTYPE type) {
	return m_aFilePath[(int)type];
}
//=============================================================================
// モデルの初期化処理
//=============================================================================
HRESULT CModel::Init(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	//輪郭がある場合
	if (m_bOutline == true && pDevice != nullptr) {
		//メッシュの複製
		if (m_aModelData[(int)m_modelType].pMesh != NULL) {
			//普通の複製だと法線の取得方法を知らないのでFVFを指定して複製
			m_aModelData[(int)m_modelType].pMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX_3D, pDevice, &m_pCloneMesh);
		}

		if (m_pCloneMesh != NULL) {
			//頂点数の取得
			int nNumVtx = 0;
			nNumVtx = m_pCloneMesh->GetNumVertices();

			VERTEX_3D *pVtx;	//FVFを変更して複製したのでそれに合わせた構造体の頂点バッファのポインタ

			//頂点バッファのロック
			m_pCloneMesh->LockVertexBuffer(0, (void**)&pVtx);

			for (int nCnt = 0; nCnt < nNumVtx; nCnt++) {
				//全頂点を法線方向へ膨張
				pVtx[nCnt].pos = D3DXVECTOR3(pVtx[nCnt].pos.x + pVtx[nCnt].nor.x * OUTLINE_SIZE,
					pVtx[nCnt].pos.y + pVtx[nCnt].nor.y * OUTLINE_SIZE,
					pVtx[nCnt].pos.z + pVtx[nCnt].nor.z * OUTLINE_SIZE);

				//複製の仕方のせいかマテリアルが反映されないのでここで色を指定
				pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			//頂点バッファのアンロック
			m_pCloneMesh->UnlockVertexBuffer();
		}
	}

	return S_OK;
}

//=============================================================================
// モデルの終了処理
//=============================================================================
void CModel::Uninit(void) {
	//複製メッシュの破棄
	if (m_pCloneMesh != NULL) {
		m_pCloneMesh->Release();
		m_pCloneMesh = NULL;
	}
}

//=============================================================================
// モデルの更新処理
//=============================================================================
void CModel::Update(void) {

}

//=============================================================================
// モデルの描画処理
//=============================================================================
void CModel::Draw(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return;

	//モデルの情報が正しく確保されていない場合描画終了
	if (m_aModelData[(int)m_modelType].pBuffMat == NULL || m_aModelData[(int)m_modelType].pMesh == NULL || m_aModelData[(int)m_modelType].nNumMat <= 0)return;

	//----------------------------------------------------
	//マトリックスの設定
	//----------------------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxParent;			//親のマトリックス

	//各パーツの親のマトリックスを設定
	if (m_pParent != NULL) {
		mtxParent = m_pParent->GetMtxWorld();	//親のモデルからマトリックスを取得
	}
	else {
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	//最後にセットしたワールドマトリックスを親のマトリックスとして設定
	}

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//算出したワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//----------------------------------------------------
	//モデルの描画
	//----------------------------------------------------
	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//輪郭の描画
	if (m_bOutline == true && m_pCloneMesh != NULL) {
		//裏面
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		//ライトを無効
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		/* 色は頂点で設定
		D3DMATERIAL9 matOutline;	//輪郭用マテリアル
		//マテリアル情報の設定
		matOutline.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

		//マテリアルの設定
		pDevice->SetMaterial(&matOutline);
		*/

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		for (int nCntMat = 0; nCntMat < (int)m_aModelData[(int)m_modelType].nNumMat; nCntMat++) {
			//モデル（パーツ）の描画	
			m_pCloneMesh->DrawSubset(nCntMat);
		}

		// 表面
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	
		//ライトを有効
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	//モデルの描画
	if (m_aModelData[(int)m_modelType].pMesh != NULL) {
		for (int nCntMat = 0; nCntMat < (int)m_aModelData[(int)m_modelType].nNumMat; nCntMat++) {
			//マテリアルの設定
			pDevice->SetMaterial(&m_aMat[nCntMat].MatD3D);
			//テクスチャの設定
			pDevice->SetTexture(0, CTexture::GetTexture(m_aTexType[(int)m_modelType][nCntMat]));
			//モデル（パーツ）の描画	
			m_aModelData[(int)m_modelType].pMesh->DrawSubset(nCntMat);
		}
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// モデルの位置情報の設定
//=============================================================================
void CModel::SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

//=============================================================================
// モデルの位置情報の取得
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void) { return m_pos; }

//=============================================================================
// モデルの角度の設定
//=============================================================================
void CModel::SetRot(D3DXVECTOR3 rot) { m_rot = rot; }

//=============================================================================
// モデルの角度の取得
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void) { return m_rot; }

//=============================================================================
// ワールドマトリックスの更新
//=============================================================================
void CModel::UpdateMtxWorld(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return;

	//----------------------------------------------------
	//マトリックスの設定
	//----------------------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxParent;			//親のマトリックス

	//各パーツの親のマトリックスを設定
	if (m_pParent != NULL) {
		mtxParent = m_pParent->GetMtxWorld();	//親のモデルからマトリックスを取得
	}
	else {
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	//最後にセットしたワールドマトリックスを親のマトリックスとして設定
	}

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//算出したワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=============================================================================
//ワールドマトリックスの取得
//=============================================================================
D3DXMATRIX CModel::GetMtxWorld(void) {
	return m_mtxWorld;
}

//=============================================================================
// ワールドマトリックスのポインタの取得
//=============================================================================
D3DXMATRIX* CModel::GetPtrMtxWorld(void) {
	return &m_mtxWorld;
}

//=============================================================================
// モデルのタイプの取得
//=============================================================================
CModel::MODELTYPE CModel::GetModelType(void) {
	return m_modelType;
}

//=============================================================================
// マテリアルのアルファ値を設定
//=============================================================================
void CModel::SetMaterialAlpha(float fAlpha) {
	for (int nCnt = 0; nCnt < MAX_MATERIAL; nCnt++)
	{
		m_aMat[nCnt].MatD3D.Diffuse.a = fAlpha;
	}
}

//=============================================================================
// マテリアルのアルファ値を取得
//=============================================================================
float CModel::GetMaterialAlpha(void) {
	return m_aMat[0].MatD3D.Diffuse.a;
}

//=============================================================================
// 輪郭の色の指定
//=============================================================================
void CModel::SetColorOutline(D3DXCOLOR col) {
	if (m_pCloneMesh == NULL) return;
	//頂点数の取得
	int nNumVtx = 0;
	nNumVtx = m_pCloneMesh->GetNumVertices();

	VERTEX_3D *pVtx;	//FVFを変更して複製したのでそれに合わせた構造体の頂点バッファのポインタ

						//頂点バッファのロック
	m_pCloneMesh->LockVertexBuffer(0, (void**)&pVtx);

	for (int nCnt = 0; nCnt < nNumVtx; nCnt++) {
		pVtx[nCnt].col = col;
	}

	//頂点バッファのアンロック
	m_pCloneMesh->UnlockVertexBuffer();
}

//=============================================================================
// 輪郭の表示設定
//=============================================================================
void CModel::SetDrawOutline(bool bDraw) {
	m_bOutline = bDraw;
	//モデル生成時に輪郭を生成していない場合は新たに作る必要あり
}