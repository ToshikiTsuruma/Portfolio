//=============================================================================
//
// 金林檎処理 [apple_gold.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "apple_gold.h"
#include "appleTree.h"
#include "manager.h"
#include "gameScene.h"
#include "player.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CAppleGold::CAppleGold()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleGold::CAppleGold(D3DXVECTOR3 pos, CAppleTree* pTree) : CGlowApple(pos, pTree)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleGold::~CAppleGold()
{

}

//=============================================================================
// 金林檎の生成処理
//=============================================================================
CAppleGold* CAppleGold::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	CAppleGold* pAppleGold;
	pAppleGold = new CAppleGold(pos, pTree);
	if (pAppleGold == nullptr) return nullptr;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	CGameScene* pGame = nullptr;
	CPlayer* pPlayer = nullptr;
	//ゲームシーンの取得
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//プレイヤーの取得
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	//プレイヤーの能力を上昇
	if (pPlayer != nullptr) {
		//移動速度UP
		pPlayer->AddMoveSpeed(3.0f);
		//攻撃力UP
		pPlayer->AddAttackDamage(5);
	}

	pAppleGold->Init();

	return pAppleGold;
}

//=============================================================================
// 金林檎の初期化処理
//=============================================================================
HRESULT CAppleGold::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//モデルの色を変更
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::GOLD), 0);
	}

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// 金林檎の終了処理
//=============================================================================
void CAppleGold::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// 金林檎の更新処理
//=============================================================================
void CAppleGold::Update(void) {

	CGlowApple::Update();
}

//=============================================================================
// 金林檎の描画処理
//=============================================================================
void CAppleGold::Draw(void) {
	CGlowApple::Draw();
}