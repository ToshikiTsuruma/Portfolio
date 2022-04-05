//=============================================================================
//
// メッシュシリンダー処理 [meshcylinder.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CMeshcylinder::CMeshcylinder() : m_nNumXZ(0), m_nNumY(0)
{

}

//=============================================================================
//オーバーロードされたコンストラクタ
//=============================================================================
CMeshcylinder::CMeshcylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadius, int nNumY, float fHeight, bool bOut, D3DXCOLOR col) : m_nNumXZ(nNumXZ), m_nNumY(nNumY)
{
	SetPos(pos);
	SetRot(rot);
	m_fRadius = fRadius;
	m_fHeight = fHeight;
	m_bOut = bOut;
	m_col = col;
	SetNumVtx((nNumXZ + 1) * (nNumY + 1));	//(XZ方向のポリゴン数 + 1) * (Y方向のポリゴン数 + 1)
	SetNumIdx((2 * (nNumXZ + 1)) * nNumY + (nNumY - 1) * 2);	//XZ方向で使う頂点数 * Y方向のポリゴン数 + 縮退ポリゴンの数
}


//=============================================================================
// デストラクタ
//=============================================================================
CMeshcylinder::~CMeshcylinder()
{

}

//=============================================================================
// メッシュシリンダーの生成処理
//=============================================================================
CMeshcylinder* CMeshcylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadius, int nNumY, float fHeight,  bool bOut, D3DXCOLOR col) {
	CMeshcylinder* pMeshcylinder;
	pMeshcylinder = new CMeshcylinder(pos, rot, nNumXZ, fRadius, nNumY, fHeight, bOut, col);
	if (pMeshcylinder != nullptr) {
		pMeshcylinder->Init();
	}

	return pMeshcylinder;
}

//=============================================================================
// メッシュシリンダーの初期化処理
//=============================================================================
HRESULT CMeshcylinder::Init(void) {
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

	//頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9* ppVtxBuff = GetPtrVtxBuff();

	int nNumVtx = GetNumVtx();	//頂点数の取得

	//頂点バッファの生成
	if (*ppVtxBuff == nullptr) {
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			ppVtxBuff,
			nullptr);
	}

	//頂点バッファの設定
	if (*ppVtxBuff != nullptr) {
		VERTEX_3D *pVtx;	//頂点バッファのポインタ
		//頂点バッファをロック
		(*ppVtxBuff)->Lock(0, 0, (void**)&pVtx, 0);
		//頂点の設定
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++) {
			float fRotXZ = ((nCntVtx % (m_nNumXZ + 1)) / (float)m_nNumXZ) * (D3DX_PI * 2);	//Y軸から見た角度

			//ポリゴンの向きが外向きの場合
			if (m_bOut) {
				//逆向きに回転
				fRotXZ *= -1.0f;
			}

			//頂点座標の設定
			pVtx[nCntVtx].pos = D3DXVECTOR3(sinf(fRotXZ) * m_fRadius,
				(nCntVtx / (m_nNumXZ + 1)) * -m_fHeight + m_fHeight * m_nNumY,
				cosf(fRotXZ) * m_fRadius);

			//法線ベクトルの設定
			D3DXVECTOR3 vecNor = D3DXVECTOR3(pVtx[nCntVtx].pos.x, 0.0f, pVtx[nCntVtx].pos.z);	//法線方向のベクトル
			D3DXVec3Normalize(&vecNor, &vecNor);	//ベクトルの正規化
			pVtx[nCntVtx].nor = vecNor;	//正規化したベクトルを設定

			//円柱が内向きの場合
			if (!m_bOut) {
				//法線を逆に設定
				pVtx[nCntVtx].nor.x *= -1;
				pVtx[nCntVtx].nor.y *= -1;
				pVtx[nCntVtx].nor.z *= -1;
			}

			pVtx[nCntVtx].col = m_col;

			pVtx[nCntVtx].tex = D3DXVECTOR2((float)(nCntVtx % (m_nNumXZ + 1)), (float)nCntVtx / (float)(m_nNumXZ + 1));
		}
		//頂点バッファをアンロックする
		(*ppVtxBuff)->Unlock();
	}

	//インデックスバッファのの取得
	LPDIRECT3DINDEXBUFFER9* ppIdxBuff = GetPtrIdxBuff();

	int nNumIdx = GetNumIdx();	//インデックス数の取得

	//インデックスバッファの生成
	if (*ppIdxBuff == nullptr) {
		pDevice->CreateIndexBuffer(sizeof(WORD) * nNumIdx,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			ppIdxBuff,
			nullptr);
	}
	//インデックスバッファの設定
	if (*ppIdxBuff != nullptr) {
		WORD *pIdx;	//インデックス情報へのポインタ
		//インデックスバッファをロックし、番号データへのポインタを取得
		(*ppIdxBuff)->Lock(0, 0, (void**)&pIdx, 0);
		//番号データ設定
		for (int nCntIdx = 0; nCntIdx < (m_nNumXZ + 1) * 2 * m_nNumY + (m_nNumY - 1) * 2; nCntIdx++) {
			if (nCntIdx == (m_nNumXZ + 2) * 2 * (1 + nCntIdx / ((m_nNumXZ + 2) * 2)) - 2) {
				pIdx[nCntIdx] = pIdx[nCntIdx - 1];
			}
			else if (nCntIdx == (m_nNumXZ + 2) * 2 * (1 + nCntIdx / ((m_nNumXZ + 2) * 2)) - 1) {
				pIdx[nCntIdx] = (m_nNumXZ + 1) * (2 + nCntIdx / ((m_nNumXZ + 1) * 2 + 2));
			}
			else {
				pIdx[nCntIdx] = (m_nNumXZ + 1 + nCntIdx / 2) - (nCntIdx / ((m_nNumXZ + 2) * 2)) - ((m_nNumXZ + 1)*(nCntIdx % 2));//下　縮退　上 縮退に修正
			}
		}
		//インデックスバッファをアンロックする
		(*ppIdxBuff)->Unlock();
	}

	CScene3D::Init();

	return S_OK;
}

//=============================================================================
// メッシュシリンダーの終了処理
//=============================================================================
void CMeshcylinder::Uninit(void) {
	CScene3D::Uninit();
}

//=============================================================================
// メッシュシリンダーの更新処理
//=============================================================================
void CMeshcylinder::Update(void) {
	CScene3D::Update();
}

//=============================================================================
// メッシュシリンダーの描画処理
//=============================================================================
void CMeshcylinder::Draw(void) {
	//描画
	CScene3D::Draw();
}

//=============================================================================
// メッシュシリンダーの半径の取得
//=============================================================================
float CMeshcylinder::GetRadius(void) {
	return m_fRadius;
}

//=============================================================================
// 色の設定
//=============================================================================
void CMeshcylinder::SetColor(D3DXCOLOR col) {
	//頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9* ppVtxBuff = GetPtrVtxBuff();
	//頂点数の取得
	int nNumVtx = GetNumVtx();	

	//頂点バッファの設定
	if (*ppVtxBuff != nullptr) {
		//色の設定
		m_col = col;

		VERTEX_3D *pVtx;	//頂点バッファのポインタ
		//頂点バッファをロック
		(*ppVtxBuff)->Lock(0, 0, (void**)&pVtx, 0);
		//頂点の設定
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++) {
			pVtx[nCntVtx].col = m_col;
		}
		//頂点バッファをアンロックする
		(*ppVtxBuff)->Unlock();
	}
}

//=============================================================================
// 色の取得
//=============================================================================
D3DXCOLOR CMeshcylinder::GetColor(void) {
	return m_col;
}