//=============================================================================
//
// 白林檎処理 [apple_white.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "apple_white.h"
#include "appleTree.h"
#include "manager.h"
#include "gameScene.h"
#include "player.h"
#include "effect.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CAppleWhite::CAppleWhite()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleWhite::CAppleWhite(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleWhite::~CAppleWhite()
{

}

//=============================================================================
// 白林檎の生成処理
//=============================================================================
CAppleWhite* CAppleWhite::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	CAppleWhite* pAppleWhite;
	pAppleWhite = new CAppleWhite(pTree);
	if (pAppleWhite == nullptr) return nullptr;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	CGameScene* pGame = nullptr;
	CPlayer* pPlayer = nullptr;
	//ゲームシーンの取得
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//プレイヤーの取得
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	//プレイヤーの衝撃波数を増やす
	if (pPlayer != nullptr) {
		pPlayer->AddNumShockwave(1);
	}

	//風の渦のエフェクトを生成
	pAppleWhite->m_pEffectWind = CEffect::Create(pos + D3DXVECTOR3(0.0f, -15.0f, 0.0f), CEffect::EFFECT_TYPE::WIND, 40.0f, 40.0f, true);

	pAppleWhite->SetPos(pos);
	pAppleWhite->Init();

	return pAppleWhite;
}

//=============================================================================
// 白林檎の初期化処理
//=============================================================================
HRESULT CAppleWhite::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//モデルの色を変更
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::WHITE), 0);
	}

	//発光表現のためにパーティクルを生成
	CreateGlowParticle(APPLE_TYPE::WHITE);

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// 白林檎の終了処理
//=============================================================================
void CAppleWhite::Uninit(void) {
	//風の渦のエフェクトの破棄
	if (m_pEffectWind != nullptr) {
		m_pEffectWind->Uninit();
		m_pEffectWind = nullptr;
	}

	CGlowApple::Uninit();
}

//=============================================================================
// 白林檎の更新処理
//=============================================================================
void CAppleWhite::Update(void) {

	CGlowApple::Update();
}

//=============================================================================
// 白林檎の描画処理
//=============================================================================
void CAppleWhite::Draw(void) {
	CGlowApple::Draw();
}