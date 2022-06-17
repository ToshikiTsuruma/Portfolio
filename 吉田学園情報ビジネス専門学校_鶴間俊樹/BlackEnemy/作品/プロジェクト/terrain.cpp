//=============================================================================
//
// 地形処理 [terrain.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "terrain.h"
#include "manager.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTerrain::CTerrain()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CTerrain::CTerrain(CModel::MODELTYPE typeModel) : CSceneModel(typeModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), nullptr, false)
{
	SetObjType(OBJ_TYPE::TERRAIN);	//オブジェクトタイプの設定

}

//=============================================================================
// デストラクタ
//=============================================================================
CTerrain::~CTerrain()
{

}

//=============================================================================
// 地形の生成処理
//=============================================================================
CTerrain* CTerrain::Create(TERRAIN_TYPE typeTerrain) {
	CModel::MODELTYPE typeModel = CModel::MODELTYPE::TERRAIN_START;
	switch (typeTerrain)
	{
	case TERRAIN_TYPE::START:
		typeModel = CModel::MODELTYPE::TERRAIN_START;
		break;

	case TERRAIN_TYPE::BOSS:
		typeModel = CModel::MODELTYPE::TERRAIN_BOSS;
		break;
	}

	CTerrain* pTerrain;
	pTerrain = new CTerrain(typeModel);
	if (pTerrain != nullptr) {
		pTerrain->Init();
	}

	return pTerrain;
}

//=============================================================================
// 地形の初期化処理
//=============================================================================
HRESULT CTerrain::Init(void) {
	CSceneModel::Init();

	return S_OK;
}

//=============================================================================
// 地形の終了処理
//=============================================================================
void CTerrain::Uninit(void) {
	CSceneModel::Uninit();
}

//=============================================================================
// 地形の更新処理
//=============================================================================
void CTerrain::Update(void) {
	CSceneModel::Update();
}

//=============================================================================
// 地形の描画処理
//=============================================================================
void CTerrain::Draw(void) {
	CSceneModel::Draw();
}

//=============================================================================
// 地形との衝突判定
//=============================================================================
bool CTerrain::Collision(D3DXVECTOR3* pPosCollision, D3DXVECTOR3 vecStart, D3DXVECTOR3 vecEnd) {
	CScene* pScene;	//オブジェクトへのポインタ
	pScene = GetSceneTopObjtype(OBJ_TYPE::TERRAIN);	//地形のオブジェクトタイプのリストの先頭を取得

	while (pScene != nullptr) {
		CScene* pSceneNext = GetSceneNextObjtype(pScene);	//リストの次のオブジェクトのポインタを取得
		CTerrain* pTerrain = dynamic_cast<CTerrain*>(pScene);	//地形クラスにキャスト
		//キャスト成功時
		if (pTerrain != nullptr) {
			//死亡フラグが立っていない場合
			if (pTerrain->GetDeath() == false) {
				//地形のモデルデータの取得
				CModel::MODELTYPE modelType = pTerrain->GetModelType();	//モデルの種類の取得
				CModel::ModelData modelData = CModel::GetModelData(modelType);	//モデルのデータの取得
				LPD3DXMESH pMesh = modelData.pMesh;	//メッシュデータの取得
				if (pMesh == nullptr) return false;	//メッシュデータが無効の場合終了			

				D3DXVECTOR3 posTerrain = pTerrain->GetPos();//地形の位置				
				D3DXVECTOR3 rotTerrain = pTerrain->GetRot();//地形の回転				

				D3DXMATRIX mtxWorld;	//地形のワールドマトリックス
				D3DXMATRIX mtxTrans, mtxRot;	//計算用マトリックス
				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxWorld);	
				//地形の回転を行列にかける
				D3DXMatrixRotationYawPitchRoll(&mtxRot, rotTerrain.y, rotTerrain.x, rotTerrain.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
				//地形の位置座標を行列にかける
				D3DXMatrixTranslation(&mtxTrans, posTerrain.x, posTerrain.y, posTerrain.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

				//地形の行列の逆行列を計算
				D3DXMatrixInverse(&mtxWorld, nullptr, &mtxWorld);
				//レイに地形の逆行列をかける
				D3DXVec3TransformCoord(&vecStart, &vecStart, &mtxWorld);
				D3DXVec3TransformCoord(&vecEnd, &vecEnd, &mtxWorld);

				D3DXVECTOR3 vecDir = vecEnd - vecStart;	//レイの方向ベクトル

				BOOL bHit;		//衝突判定フラグ
				DWORD faceIdx;	//交差時に一番近い面のインデックス
				float fU, fV;	//交差した三角形の重心座標?

				//当たり判定
				D3DXIntersect(pMesh, &vecStart, &vecDir, &bHit, &faceIdx, &fU, &fV, nullptr, nullptr, nullptr);

				//レイ衝突時
				if (bHit == TRUE) {
					//-------------------------------------------------
					//判定があった三角形の頂点のインデックスの取得
					//-------------------------------------------------
					int nIdx[3];	//三角形の頂点のインデックス
					WORD *pIdx;		//インデックス情報へのポインタ
					//インデックスバッファのロック
					pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIdx);
					for (int nCnt = 0; nCnt < 3; nCnt++) {
						nIdx[nCnt] = pIdx[(faceIdx * 3) + nCnt];
					}

					//インデックスバッファのアンロック
					pMesh->UnlockIndexBuffer();

					//-------------------------------------------------
					//判定があった三角形の頂点の位置の取得
					//-------------------------------------------------
					D3DXVECTOR3 posVtx[3];	//三角形の頂点の位置座標

					DWORD dwStride = pMesh->GetNumBytesPerVertex();	//頂点ごとのバイト数

					BYTE* pbVtx = nullptr;
					float* pfVtx = nullptr;

					//頂点バッファのロック
					pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pbVtx);
				
					for (int nCntPoly = 0; nCntPoly < 3; nCntPoly++) {
						//インデックスに応じた頂点座標を取得
						pfVtx = (float*) &pbVtx[dwStride * nIdx[nCntPoly]];

						posVtx[nCntPoly].x = pfVtx[0];
						posVtx[nCntPoly].y = pfVtx[1];
						posVtx[nCntPoly].z = pfVtx[2];
					}

					//頂点バッファのアンロック
					pMesh->UnlockVertexBuffer();

					D3DXVECTOR3 posCol = D3DXVECTOR3(posVtx[0] + fU * (posVtx[1] - posVtx[0]) + fV * (posVtx[2] - posVtx[0]));	//地形のローカル座標での衝突位置

					//地形との衝突位置をワールド座標に変換
					D3DXMATRIX mtxWorld;	//地形のワールドマトリックス
					D3DXMATRIX mtxTrans, mtxRot;	//計算用マトリックス
					//ワールドマトリックスの初期化
					D3DXMatrixIdentity(&mtxWorld);
					//地形の回転を行列にかける
					D3DXMatrixRotationYawPitchRoll(&mtxRot, rotTerrain.y, rotTerrain.x, rotTerrain.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
					//地形の位置座標を行列にかける
					D3DXMatrixTranslation(&mtxTrans, posTerrain.x, posTerrain.y, posTerrain.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
					//位置にマトリックスを反映
					D3DXVec3TransformCoord(&posCol, &posCol, &mtxWorld);
					//衝突したオブジェクトの位置を設定
					*pPosCollision = posCol;
					return true;
				}
			}
		}
		pScene = pSceneNext;	//リストの次のオブジェクトを代入
	}

	return false;
}