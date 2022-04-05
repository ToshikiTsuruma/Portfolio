//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "fade.h"
#include "renderer.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CFade::CFade()
{
	m_pVtxBuff = NULL;
	m_fFadeSpeed = 0.0f;
	m_bFadein = false;
	m_nextMode = CManager::MODE::TITLE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// フェードの初期化処理
//=============================================================================
HRESULT CFade::Init(void) {
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

	VERTEX_2D *pVtx;
	m_bFadein = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// フェードの終了処理
//=============================================================================
void CFade::Uninit(void) {
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// フェードの更新処理
//=============================================================================
void CFade::Update(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	

	//モード切り替え時の終了
	if (m_bChangeFade) m_bChangeFade = false;

	//フェードアウト
	if (!m_bFadein && m_colA > 0.0f) {
		m_colA -= m_fFadeSpeed;
		if (m_colA < 0.0f) {
			m_colA = 0.0f;
		}
	}

	//フェードイン
	if (m_bFadein && m_colA <= 1.0f) {
		m_colA += m_fFadeSpeed;
		//フェード完了時
		if (m_colA >= 1.0f) {
			m_colA = 1.0f;
			m_bFadein = false;
			//シーン遷移
			if (pManager != nullptr) {
				m_bChangeFade = true;	//モード切り替え時
				//次のモードが範囲内だった場合モード切替
				if((int)m_nextMode >= 0 && m_nextMode < CManager::MODE::ENUM_MAX) pManager->SetMode(m_nextMode);
			}
		}
	}

	//フェード中のみ更新
	if (m_bFadein || m_colA > 0.0f) {
		VERTEX_2D *pVtx;

		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点情報を設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colA);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colA);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colA);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colA);
		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// フェードの描画処理
//=============================================================================
void CFade::Draw(void) {
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

	//フェード中のみ描画
	if (m_bFadein || m_colA > 0.0f) {
		pDevice->SetFVF(FVF_VERTEX_2D);			//頂点フォーマットの設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));	//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetTexture(0, NULL);		//テクスチャの設定
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
			0,
			2);
	}
}

//=============================================================================
//フェードの設定
//=============================================================================
void CFade::SetFade(CManager::MODE mode, float fFadeSpeed) {
	if (!m_bFadein) {
		m_bFadein = true;
		m_nextMode = mode;
		m_fFadeSpeed = fFadeSpeed;
	}
}

//=============================================================================
// フェードアウトをスキップさせる
//=============================================================================
void CFade::SkipFade(void) {
	m_colA = 0.0f;
	m_bFadein = false;
}

//=============================================================================
// フェード中かどうかを取得
//=============================================================================
bool CFade::GetFade(void) {
	if (m_bFadein || m_colA > 0.0f) {
		return true;
	}
	return false;
}

//=============================================================================
// フェードインからフェードアウトに切り替わったタイミングかどうか
//=============================================================================
bool CFade::GetChangeFade(void) {
	return m_bChangeFade;
}