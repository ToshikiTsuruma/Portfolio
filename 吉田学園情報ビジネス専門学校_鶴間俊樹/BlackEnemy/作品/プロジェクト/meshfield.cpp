//=============================================================================
//
// メッシュフィールド処理 [meshfield.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CMeshfield::CMeshfield()
{
	m_nNumX = 0;
	m_nNumZ = 0;
	m_fWidth = 0.0f;
	m_fDepth = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshfield::~CMeshfield()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CMeshfield* CMeshfield::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumX, int nNumZ, float fWidth, float fDepth) {
	CMeshfield* pMeshfield;
	pMeshfield = new CMeshfield;
	if (pMeshfield != nullptr) {
		pMeshfield->SetPos(pos);
		pMeshfield->SetRot(rot);
		pMeshfield->m_nNumX = nNumX;
		pMeshfield->m_nNumZ = nNumZ;
		pMeshfield->m_fWidth = fWidth;
		pMeshfield->m_fDepth = fDepth;
		pMeshfield->SetNumVtx((nNumX + 1) * (nNumZ + 1));	//(X方向のポリゴン数 + 1) * (Z方向のポリゴン数 + 1)
		pMeshfield->SetNumIdx((2 * (nNumX + 1)) * nNumZ + (nNumZ - 1) * 2);	//X方向で使う頂点数 * Z方向のポリゴン数 + 縮退ポリゴンの数
		pMeshfield->Init();
	}

	return pMeshfield;
}

//=============================================================================
// メッシュフィールドの初期化処理
//=============================================================================
HRESULT CMeshfield::Init(void) {
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

	//テクスチャの設定
	SetTexType(CTexture::TEXTURE_TYPE::NONE);

	//頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9* ppVtxBuff = GetPtrVtxBuff();

	int nNumVtx = GetNumVtx();	//頂点数の取得
	int nNumIdx = GetNumIdx();	//インデックス数の取得

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

			//頂点座標の設定
			pVtx[nCntVtx].pos = D3DXVECTOR3((nCntVtx % (m_nNumX + 1)) * m_fWidth - (m_fWidth / 2.0f) * m_nNumX,
				0.0f,
				(nCntVtx / (m_nNumX + 1)) * -m_fDepth + (m_fDepth) * m_nNumZ / 2);

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[nCntVtx].tex = D3DXVECTOR2((float)(nCntVtx % (m_nNumX + 1)), (float)nCntVtx / (float)(m_nNumX + 1));
		}
		//頂点バッファをアンロックする
		(*ppVtxBuff)->Unlock();
	}

	//インデックスバッファのの取得
	LPDIRECT3DINDEXBUFFER9* ppIdxBuff = GetPtrIdxBuff();

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
		for (int nCntIdx = 0; nCntIdx < nNumIdx; nCntIdx++) {
			if (nCntIdx == (m_nNumX + 2) * 2 * (1 + nCntIdx / ((m_nNumX + 2) * 2)) - 2) {
				pIdx[nCntIdx] = pIdx[nCntIdx - 1];
			}
			else if (nCntIdx == (m_nNumX + 2) * 2 * (1 + nCntIdx / ((m_nNumX + 2) * 2)) - 1) {
				pIdx[nCntIdx] = (m_nNumX + 1) * (2 + nCntIdx / ((m_nNumX + 1) * 2 + 2));
			}
			else {
				pIdx[nCntIdx] = (m_nNumX + 1 + nCntIdx / 2) - (nCntIdx / ((m_nNumX + 2) * 2)) - ((m_nNumX + 1)*(nCntIdx % 2));//下　縮退　上 縮退に修正
			}
		}
		//インデックスバッファをアンロックする
		(*ppIdxBuff)->Unlock();
	}

	CScene3D::Init();

	return S_OK;
}

//=============================================================================
// メッシュフィールドの終了処理
//=============================================================================
void CMeshfield::Uninit(void) {
	CScene3D::Uninit();
}

//=============================================================================
// メッシュフィールドの更新処理
//=============================================================================
void CMeshfield::Update(void) {
	CScene3D::Update();
}

//=============================================================================
// メッシュフィールドの描画処理
//=============================================================================
void CMeshfield::Draw(void) {
	CScene3D::Draw();
}