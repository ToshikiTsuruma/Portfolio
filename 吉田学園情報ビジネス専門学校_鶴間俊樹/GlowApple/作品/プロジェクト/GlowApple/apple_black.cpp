//=============================================================================
//
// 黒林檎処理 [apple_black.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "apple_black.h"
#include "appleTree.h"
#include "manager.h"
#include "gameScene.h"
#include "enemySpawner.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CAppleBlack::CAppleBlack()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleBlack::CAppleBlack(D3DXVECTOR3 pos, CAppleTree* pTree) : CGlowApple(pos, pTree)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleBlack::~CAppleBlack()
{

}

//=============================================================================
// 黒林檎の生成処理
//=============================================================================
CAppleBlack* CAppleBlack::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	CAppleBlack* pAppleBlack;
	pAppleBlack = new CAppleBlack(pos, pTree);
	if (pAppleBlack == nullptr) return nullptr;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	CGameScene* pGame = nullptr;
	CEnemySpawner* pEnemySpawner = nullptr;
	//ゲームシーンの取得
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//敵のスポナーの取得
	if (pGame != nullptr) pEnemySpawner = pGame->GetEnemySpawner();

	//敵の間隔を減らす
	if (pEnemySpawner != nullptr) {
		pEnemySpawner->AddDistEnemy(-400);
	}

	pAppleBlack->Init();

	return pAppleBlack;
}

//=============================================================================
// 黒林檎の初期化処理
//=============================================================================
HRESULT CAppleBlack::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//モデルの色を変更
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::BLACK), 0);
	}

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// 黒林檎の終了処理
//=============================================================================
void CAppleBlack::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// 黒林檎の更新処理
//=============================================================================
void CAppleBlack::Update(void) {

	CGlowApple::Update();
}

//=============================================================================
// 黒林檎の描画処理
//=============================================================================
void CAppleBlack::Draw(void) {
	CGlowApple::Draw();
}