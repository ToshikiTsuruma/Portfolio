//=============================================================================
//
// ナンバー処理 [number.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "number.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CNumber::CNumber()
{
	m_pVtxBuff = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// ナンバーの生成処理
//=============================================================================
CNumber* CNumber::Create(CTexture::TEXTURE_TYPE type, int nNumDigit) {
	CNumber* pNumberArray;
	pNumberArray = new CNumber[nNumDigit];

	if (pNumberArray != nullptr) {
		LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
		//マネージャーの取得
		CManager* pManager = CManager::GetManager();	
		//レンダラーの取得
		CRenderer* pRenderer = nullptr;			
		if (pManager != nullptr) pRenderer = pManager->GetRenderer();
		//デバイスの取得
		if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

		if (pDevice != nullptr) {
			for (int nCnt = 0; nCnt < nNumDigit; nCnt++)
			{
				//頂点バッファの生成
				pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_2D,
					D3DPOOL_MANAGED,
					&(pNumberArray[nCnt].m_pVtxBuff),
					nullptr);

				pNumberArray[nCnt].m_texType = type;
				pNumberArray[nCnt].Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
			}
		}
	}

	return pNumberArray;
}

//=============================================================================
// ナンバーの初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fSize) {
	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点情報を設定 ポリゴンの右上が基準
		pVtx[0].pos = D3DXVECTOR3(pos.x - fSize, pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x, pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fSize, pos.y + fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x, pos.y + fSize, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ナンバーの終了処理
//=============================================================================
void CNumber::Uninit(void) {
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// ナンバーの更新処理
//=============================================================================
void CNumber::Update(void) {

}

//=============================================================================
// ナンバーの描画処理
//=============================================================================
void CNumber::Draw(void) {
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

	//テクスチャの取得
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(m_texType);

	pDevice->SetFVF(FVF_VERTEX_2D);			//頂点フォーマットの設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetTexture(0, pTexture);		//テクスチャの設定
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0,
		2);
}


//=============================================================================
// 数字の設定
//=============================================================================
void CNumber::SetNumber(int nNumber) {
	VERTEX_2D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].tex = D3DXVECTOR2((float)nNumber / 10.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)(nNumber + 1) / 10.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)nNumber / 10.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)(nNumber + 1) / 10.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//数字の色の設定
//=============================================================================
void CNumber::SetColor(D3DXCOLOR col) {
	VERTEX_2D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
