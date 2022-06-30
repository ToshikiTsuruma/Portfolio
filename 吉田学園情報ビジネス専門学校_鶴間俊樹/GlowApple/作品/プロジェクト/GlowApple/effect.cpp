//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEffect::CEffect()
{
	m_nCntAnim = 0;
	m_nSpanAnim = 0;
	m_nPatternAnim = 0;
	m_nPatternU = 0;
	m_nPatternV = 0;
	m_bDestBlendOne = false;

	//深度関係なく描画できるようにする
	SetZtestAlways(true);
}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CEffect* CEffect::Create(D3DXVECTOR3 pos, EFFECT_TYPE type, float fWidth, float fHeight) {
	CEffect* pEffect;
	pEffect = new CEffect;
	if (pEffect == nullptr) return nullptr;

	pEffect->m_type = type;
	pEffect->SetPos(pos);
	pEffect->SetSize(D3DXVECTOR3(fWidth, fHeight, 0.0f));

	switch ((EFFECT_TYPE)type)
	{
	case EFFECT_TYPE::EXPLOSION:
		pEffect->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_EXPLOSION);
		pEffect->m_nSpanAnim = 2;
		pEffect->m_nPatternU = 7;
		pEffect->m_nPatternV = 1;
		break;
	case EFFECT_TYPE::HEAL_APPLE:
		pEffect->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_HEAL_APPLE);
		pEffect->m_nSpanAnim = 1;
		pEffect->m_nPatternU = 9;
		pEffect->m_nPatternV = 1;
		pEffect->m_bDestBlendOne = true;
		break;
	}
	pEffect->Init();

	return pEffect;
}

//=============================================================================
// エフェクトの初期化処理
//=============================================================================
HRESULT CEffect::Init(void) {
	//メンバ変数の初期化
	m_nCntAnim = 0;
	m_nPatternAnim = 0;
	//ビルボードの初期化
	CBillboard::Init();
	//テクスチャ座標の設定
	CBillboard::SetTexPos(0.0f, 0.0f, 1.0f / (float)m_nPatternU, 1.0f / (float)m_nPatternV);

	return S_OK;
}

//=============================================================================
// エフェクトの終了処理
//=============================================================================
void CEffect::Uninit(void) {
	CBillboard::Uninit();
}

//=============================================================================
// エフェクトの更新処理
//=============================================================================
void CEffect::Update(void) {
	CBillboard::Update();

	//アニメーション切り替え
	if (m_nCntAnim >= m_nSpanAnim) {
		m_nCntAnim = 0;
		m_nPatternAnim++;

		//アニメーションを終了した場合
		if (m_nPatternAnim >= m_nPatternU * m_nPatternV) {
			Uninit();
			return;
		}

		//テクスチャ座標の設定
		CBillboard::SetTexPos(
			(1.0f / (float)m_nPatternU) * (m_nPatternAnim % m_nPatternU),
			(1.0f / (float)m_nPatternV) * (m_nPatternAnim / m_nPatternU),
			(1.0f / (float)m_nPatternU) * ((m_nPatternAnim % m_nPatternU) + 1),
			(1.0f / (float)m_nPatternV) * ((m_nPatternAnim / m_nPatternU) + 1)
		);
	}
	else {
		m_nCntAnim++;	//カウンターの加算
	}
}

//=============================================================================
// エフェクトの描画処理
//=============================================================================
void CEffect::Draw(void) {
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

	//フォグを無効
	pRenderer->SetEffectFogEnable(false);
	//アルファテスト　有効／無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//アルファ値の参照値
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	if (m_bDestBlendOne) {
		//αブレンディングの設定
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	CBillboard::Draw();

	//フォグを有効
	pRenderer->SetEffectFogEnable(true);
	//アルファテスト　有効／無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//アルファ値の参照値
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	if (m_bDestBlendOne) {
		//αブレンディングを通常に戻す
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}