//=============================================================================
//
// 2Dオブジェクト処理 [scene2D.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CScene2D::CScene2D()
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fRatioWidth = 1.0f;
	m_fHeight = 0.0f;
	m_fRatioHeight = 1.0f;
	m_fAngle = 0.0f;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	SetDrawPriority(DRAW_PRIORITY::UI);
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// 2Dオブジェクトの生成処理
//=============================================================================
CScene2D* CScene2D::Create(D3DXVECTOR3 pos, CTexture::TEXTURE_TYPE type, float fWidth, float fHeight) {
	CScene2D* pScene2D;
	pScene2D = new CScene2D;
	if (pScene2D != nullptr) {
		LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
		//マネージャーの取得
		CManager* pManager = CManager::GetManager();	
		//レンダラーの取得
		CRenderer* pRenderer = nullptr;			
		if (pManager != nullptr) pRenderer = pManager->GetRenderer();
		//デバイスの取得
		if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

		if (pDevice != nullptr) {
			//頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&(pScene2D->m_pVtxBuff),
				nullptr);
		}

		pScene2D->m_pos = pos;
		pScene2D->m_fWidth = fWidth;
		pScene2D->m_fHeight = fHeight;
		pScene2D->SetTexType(type);
		pScene2D->Init();
	}

	return pScene2D;
}

//=============================================================================
// 2Dオブジェクトの初期化処理
//=============================================================================
HRESULT CScene2D::Init(void) {
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y - m_fHeight / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth / 2.0f, m_pos.y - m_fHeight / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

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
// 2Dオブジェクトの終了処理
//=============================================================================
void CScene2D::Uninit(void) {
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//オブジェクトの破棄
   	Release();
}

//=============================================================================
// 2Dオブジェクトの更新処理
//=============================================================================
void CScene2D::Update(void) {

	//頂点バッファの更新
	//SetVtxPos();
}

//=============================================================================
// 2Dオブジェクトの描画処理
//=============================================================================
void CScene2D::Draw(void) {
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
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(GetTexType());

	if (m_pVtxBuff != nullptr && pDevice != nullptr) {
		pDevice->SetFVF(FVF_VERTEX_2D);		//頂点フォーマットの設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));	//頂点バッファをデバイスのデータストリームに設定		
		pDevice->SetTexture(0, pTexture);	//テクスチャの設定
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
			0,
			2);
	}
}

//=============================================================================
// 2Dオブジェクトの位置情報の設定
//=============================================================================
void CScene2D::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
	//頂点バッファの更新
	SetVtxPos();
}

//=============================================================================
// 2Dオブジェクトの位置情報の取得
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void) { return m_pos; }

//=============================================================================
// 2Dオブジェクトの移動量の設定
//=============================================================================
void CScene2D::SetMove(D3DXVECTOR3 move) { m_move = move; }

//=============================================================================
// 2Dオブジェクトの移動量の取得
//=============================================================================
D3DXVECTOR3 CScene2D::GetMove(void) { return m_move; }

//=============================================================================
// 2Dオブジェクトのサイズの設定
//=============================================================================
void CScene2D::SetSize(D3DXVECTOR3 size) {
	m_fWidth = size.x;
	m_fHeight = size.y;
	//頂点バッファの更新
	SetVtxPos();
}

//=============================================================================
// 2Dオブジェクトのサイズの取得
//=============================================================================
D3DXVECTOR3 CScene2D::GetSize(void) { return D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f); }

//=============================================================================
// 2Dオブジェクトの幅の割合の設定
//=============================================================================
void CScene2D::SetRatioWidth(float fRatio) {
	m_fRatioWidth = fRatio;

	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//頂点バッファのロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f, 0.0f);

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 2Dオブジェクトの幅の割合の設定
//=============================================================================
void CScene2D::SetRatioHeight(float fRatio) {
	m_fRatioHeight = fRatio;

	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//頂点バッファのロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f, 0.0f);

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//角度の設定
//=============================================================================
void CScene2D::SetAngle(float fAngle) {
	m_fAngle = fAngle;
	//頂点バッファの更新
	SetVtxPos();
}

//=============================================================================
//角度の取得
//=============================================================================
float CScene2D::GetAngle(void) { return m_fAngle; }

//=============================================================================
//各頂点の位置を更新
//=============================================================================
void CScene2D::SetVtxPos(void) {
	if (m_pVtxBuff != nullptr) {
		float fRadius = sqrtf(powf(m_fWidth / 2.0f, 2.0f) + powf(m_fHeight / 2.0f, 2.0f));	//ポリゴンの対角線の半分
		float fAngleDelta = atan2f(m_fWidth, m_fHeight);	//基準の角度からの差分（ラジアン）

		VERTEX_2D *pVtx;
		//頂点バッファのロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle * D3DX_PI - fAngleDelta) * fRadius, m_pos.y + cosf((m_fAngle + 1.0f) * D3DX_PI - fAngleDelta) * fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle * D3DX_PI + fAngleDelta) * fRadius, m_pos.y + cosf((m_fAngle + 1.0f) * D3DX_PI + fAngleDelta) * fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf((m_fAngle + 1.0f) * D3DX_PI + fAngleDelta) * fRadius, m_pos.y + cosf(m_fAngle * D3DX_PI + fAngleDelta) * fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf((m_fAngle + 1.0f) * D3DX_PI - fAngleDelta) * fRadius, m_pos.y + cosf(m_fAngle * D3DX_PI - fAngleDelta) * fRadius, 0.0f);
		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//カラーの設定
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR col) {
	m_col = col;

	//頂点バッファの更新
	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//頂点バッファのロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点情報を設定
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//カラーの取得
//=============================================================================
D3DXCOLOR CScene2D::GetColor(void) { return m_col; }

//=============================================================================
//テクスチャ座標の設定
//=============================================================================
void CScene2D::SetTexPos(float startU, float startV, float endU, float endV) {
	VERTEX_2D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].tex = D3DXVECTOR2(startU, startV);
	pVtx[1].tex = D3DXVECTOR2(endU, startV);
	pVtx[2].tex = D3DXVECTOR2(startU, endV);
	pVtx[3].tex = D3DXVECTOR2(endU, endV);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}