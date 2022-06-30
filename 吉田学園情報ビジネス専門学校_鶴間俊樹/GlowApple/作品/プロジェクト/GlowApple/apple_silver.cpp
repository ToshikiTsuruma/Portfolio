//=============================================================================
//
// 銀林檎処理 [apple_silver.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "apple_silver.h"
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
CAppleSilver::CAppleSilver()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleSilver::CAppleSilver(D3DXVECTOR3 pos, CAppleTree* pTree) : CGlowApple(pos, pTree)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleSilver::~CAppleSilver()
{

}

//=============================================================================
// 銀林檎の生成処理
//=============================================================================
CAppleSilver* CAppleSilver::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	CAppleSilver* pAppleSilver;
	pAppleSilver = new CAppleSilver(pos, pTree);
	if (pAppleSilver == nullptr) return nullptr;

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

	pAppleSilver->Init();

	return pAppleSilver;
}

//=============================================================================
// 銀林檎の初期化処理
//=============================================================================
HRESULT CAppleSilver::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//モデルの色を変更
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::SILVER), 0);
	}

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// 銀林檎の終了処理
//=============================================================================
void CAppleSilver::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// 銀林檎の更新処理
//=============================================================================
void CAppleSilver::Update(void) {

	CGlowApple::Update();
}

//=============================================================================
// 銀林檎の描画処理
//=============================================================================
void CAppleSilver::Draw(void) {
	CGlowApple::Draw();
}