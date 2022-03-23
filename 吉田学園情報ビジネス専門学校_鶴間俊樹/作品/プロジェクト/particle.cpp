//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CParticle::CParticle()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CParticle::CParticle(float fDecSize) {
	//描画順の設定
	SetDrawPriority(DRAW_PRIORITY::EFFECT);

	m_fDecSize = fDecSize;
}

//=============================================================================
// デストラクタ
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CParticle* CParticle::Create(D3DXVECTOR3 pos, float fSize, float fDecSize, D3DXCOLOR col) {
	CParticle* pParticle;
	pParticle = new CParticle(fDecSize);
	if (pParticle != NULL) {
		pParticle->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_PARTICLE);
		pParticle->SetPos(pos);
		pParticle->SetSize(D3DXVECTOR3(fSize, fSize, 0.0f));

		//初期化
		pParticle->Init();

		//色の変更
		pParticle->SetColor(col);
	}

	return pParticle;
}

//=============================================================================
// パーティクルの初期化処理
//=============================================================================
HRESULT CParticle::Init(void) {
	//ビルボードの初期化
	CBillboard::Init();

	return S_OK;
}

//=============================================================================
// パーティクルの終了処理
//=============================================================================
void CParticle::Uninit(void) {
	CBillboard::Uninit();
}

//=============================================================================
// パーティクルの更新処理
//=============================================================================
void CParticle::Update(void) {
	CBillboard::Update();

	D3DXVECTOR3 size = GetSize();
	//サイズの減算
	size.x -= m_fDecSize;
	size.y -= m_fDecSize;
	//サイズがマイナスになったら消える
	if (size.x <= 0) {
		Uninit();
		return;
	}
	//サイズの設定
	SetSize(size);
	return;
}

//=============================================================================
// パーティクルの描画処理
//=============================================================================
void CParticle::Draw(void) {
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

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CBillboard::Draw();

	//αブレンディングを通常に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}