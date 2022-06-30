//=============================================================================
//
// 3Dゲージ処理 [gauge3D.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "gauge3D.h"
#include "billboard.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGauge3D::CGauge3D()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CGauge3D::CGauge3D(int nMaxValue, int nValue, int nDrawLifeMax) : CGauge(nMaxValue, nValue, nDrawLifeMax)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CGauge3D::~CGauge3D()
{

}

//=============================================================================
// 3Dゲージの生成処理
//=============================================================================
CGauge3D* CGauge3D::Create(int nMaxValue, int nValue, int nDrawLifeMax) {
	CGauge3D* pGauge3D;
	pGauge3D = new CGauge3D(nMaxValue, nValue, nDrawLifeMax);
	if (pGauge3D == nullptr) return nullptr;

	pGauge3D->Init();

	return pGauge3D;
}

//=============================================================================
// 3Dゲージの初期化処理
//=============================================================================
void CGauge3D::Init(void) {
	CGauge::Init();
}

//=============================================================================
// 3Dゲージの終了処理
//=============================================================================
void CGauge3D::Uninit(void) {
	CGauge::Uninit();
}

//=============================================================================
// 3Dゲージの更新処理
//=============================================================================
void CGauge3D::Update(void) {
	CGauge::Update();
}

//=============================================================================
// 3Dゲージを生成
//=============================================================================
void CGauge3D::CreateGauge(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//ゲージのダブルポインタを取得
	CObject** ppGauge = GetGaugePtr();

	//ゲージを生成するポインタ変数のアドレスがnullの場合終了
	if (ppGauge == nullptr) return;

	//すでに生成されている場合終了
	if (*ppGauge != nullptr) return;

	//ゲージの生成
	*ppGauge = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (*ppGauge != nullptr) (*ppGauge)->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
}

//=============================================================================
// 3Dゲージの背景を生成
//=============================================================================
void CGauge3D::CreateGaugeBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//ゲージの背景のダブルポインタを取得
	CObject** ppGaugeBG = GetGaugeBGPtr();

	//ゲージの背景を生成するポインタ変数のアドレスがnullの場合終了
	if (ppGaugeBG == nullptr) return;

	//すでに生成されている場合終了
	if (*ppGaugeBG != nullptr) return;

	//背景の生成
	*ppGaugeBG = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (*ppGaugeBG != nullptr) (*ppGaugeBG)->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
}

//=============================================================================
// 3Dゲージの枠を生成
//=============================================================================
void CGauge3D::CreateGaugeFrame(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//ゲージの背景のダブルポインタを取得
	CObject** ppGaugeFrame = GetGaugeFramePtr();

	//ゲージの背景を生成するポインタ変数のアドレスがnullの場合終了
	if (ppGaugeFrame == nullptr) return;

	//すでに生成されている場合終了
	if (*ppGaugeFrame != nullptr) return;

	//背景の生成
	*ppGaugeFrame = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (*ppGaugeFrame != nullptr) (*ppGaugeFrame)->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
}