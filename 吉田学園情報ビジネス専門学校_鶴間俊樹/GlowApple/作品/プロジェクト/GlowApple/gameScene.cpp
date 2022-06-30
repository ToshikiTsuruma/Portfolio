//=============================================================================
//
// ゲームシーン処理 [gameScene.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "gameScene.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "timer.h"
#include "stage.h"
#include "wallCylinder.h"
#include "player.h"
#include "appleTree.h"
#include "selectMenu2D.h"
#include "enemy_human.h"
#include "enemy_normal.h"
#include "enemySpawner.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define GAME_TIME (300)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGameScene::CGameScene()
{
	m_pStage = nullptr;
	m_pAppleTree = nullptr;
	m_pTimer = nullptr;
	m_pMenuGameEnd = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGameScene::~CGameScene()
{

}

//=============================================================================
// ゲームシーンの初期化処理
//=============================================================================
void CGameScene::Init(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//カメラの取得
	CCamera* pCamera = pManager->GetCamera();
	if (pCamera == nullptr) return;
	//サウンドの取得
	CSound* pSound = pManager->GetSound();

	//カメラの初期設定
	pCamera->Init();

	//ステージの生成
	if(m_pStage == nullptr) m_pStage = new CStage;
	if(m_pStage != nullptr) m_pStage->CreateStage(TEXT_FILE_NAME_STAGE_GAME);

	//モーション情報のロード
	CPlayer::LoadMotionInfo();
	CEnemyNormal::LoadMotionInfo();
	CEnemyHuman::LoadMotionInfo();

	//林檎の木の生成
	m_pAppleTree = CAppleTree::Create(D3DXVECTOR3(0.0f, -190.0f, 0.0f));

	//円柱の壁の生成
	CWallCylinder::Create(D3DXVECTOR3(0.0f, 60.0f, 0.0f), 1500.0f, 40.0f, CTexture::TEXTURE_TYPE::NONE, true);

	//プレイヤーの生成
	CPlayer* pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, -300.0f, -1000.0f));
	//シーンのプレイヤーの設定
	SetPlayer(pPlayer);

	//タイマーの生成
	m_pTimer = CTimer::Create(GAME_TIME, 3, CTexture::TEXTURE_TYPE::NUMBER_003, D3DXVECTOR3(SCREEN_WIDTH - 10.0f, 10.0f, 0.0f), 50.0f, 50.0f);

	//最初の敵の配置
	CEnemy::SetEnemyCircle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1000.0f, 2000, 2000);

	//敵のスポナーの生成
	m_pEnemySpawner = CEnemySpawner::Create(750, 2500, 3500);

	//BGMの再生
	if (pSound != nullptr) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_GAME);
		pSound->SetBGM(CSound::SOUND_LABEL::BGM_GAME);
	}
}

//=============================================================================
// ゲームシーンの終了処理
//=============================================================================
void CGameScene::Uninit(void) {
	//ステージの破棄
	if (m_pStage != nullptr) {
		delete m_pStage;
		m_pStage = nullptr;
	}

	//モーション情報のアンロード
	CPlayer::UnloadMotionInfo();
	CEnemyNormal::UnloadMotionInfo();
	CEnemyHuman::UnloadMotionInfo();

	//シーンのプレイヤーの設定
	SetPlayer(nullptr);

	//シーンの終了処理
	CScene::Uninit();

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//BGMの停止
	if (pSound != nullptr) {
		pSound->StopSound(pSound->GetBGM());
	}
}

//=============================================================================
// ゲームシーンの更新処理
//=============================================================================
void CGameScene::Update(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	CInput* pInput = nullptr;	//入力デバイスへのポインタ
	CFade* pFade = nullptr;		//フェードへのポインタ

	if (pManager != nullptr) {
		//現在の入力デバイスの取得
		pInput = pManager->GetInputCur();
		//フェードの取得
		pFade = pManager->GetFade();
	}

	//残り時間減少によるスポーン間隔の減少
	if (m_pEnemySpawner != nullptr) {
		//残り200秒になった瞬間
		if (m_pTimer->GetTime() == 200 && m_pTimer->GetCountTimer() == 0) {
			m_pEnemySpawner->AddSpan(-60);
		}
		//残り100秒になった瞬間
		if (m_pTimer->GetTime() == 100 && m_pTimer->GetCountTimer() == 0) {
			m_pEnemySpawner->AddSpan(-60);
		}
		//残り60秒になった瞬間
		if (m_pTimer->GetTime() == 60 && m_pTimer->GetCountTimer() == 0) {
			m_pEnemySpawner->AddSpan(-60);
		}
		//残り30秒になった瞬間
		if (m_pTimer->GetTime() == 30 && m_pTimer->GetCountTimer() == 0) {
			m_pEnemySpawner->AddSpan(-60);
		}
	}

	//ゲーム終了していないときにタイマーが０になった場合
	if (m_pTimer != nullptr && !m_bGameClear && !m_bGameOver) {
		if (m_pTimer->GetTime() <= 0) {
			//ゲームクリア
			GameClear();
		}
	}

	//ゲーム終了時
	if ((m_bGameClear || m_bGameOver) && m_pMenuGameEnd != nullptr && pInput != nullptr) {
		//選択時
		if (pInput->GetTrigger(CInput::CODE::SELECT)) {
			switch (m_pMenuGameEnd->GetIdxCurSelect())
			{
			case 0:
				//タイトルへシーン遷移
				if (pFade != nullptr) pFade->SetFade(CScene::SCENE_TYPE::TITLE, 0.02f);
				break;
			case 1:
				//ゲームを再プレイ
				if (pFade != nullptr) pFade->SetFade(CScene::SCENE_TYPE::GAME, 0.02f);
				break;
			}
		}
	}
}

//=============================================================================
// ゲームクリア
//=============================================================================
void CGameScene::GameClear(void) {
	if (m_bGameClear || m_bGameOver) return;

	m_bGameClear = true;

	//敵をすべて死亡させる
	CObject::DeadObjtype(CObject::OBJ_TYPE::ENEMY);

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//クリア音を再生
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::GAMECLEAR);

	//ゲームクリアテキストの表示
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GAMECLEAR, 600.0f, 150.0f);

	//ゲーム終了メニューの生成
	CreateMenuEndGame();

	//敵のスポナーの破棄
	if (m_pEnemySpawner != nullptr) {
		m_pEnemySpawner->Uninit();
		m_pEnemySpawner = nullptr;
	}
}

//=============================================================================
// ゲームオーバー
//=============================================================================
void CGameScene::GameOver(void) {
	if (m_bGameClear || m_bGameOver) return;

	m_bGameOver = true;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//ゲームオーバー音を再生
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::GAMEOVER);

	//ゲームオーバーテキストの表示
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GAMEOVER, 600.0f, 150.0f);

	//ゲーム終了メニューの生成
	CreateMenuEndGame();

	//タイマーを止める
	if (m_pTimer != nullptr) m_pTimer->SetStop(true);

	//敵のスポナーの破棄
	if (m_pEnemySpawner != nullptr) {
		m_pEnemySpawner->Uninit();
		m_pEnemySpawner = nullptr;
	}
}

//=============================================================================
// ゲーム終了時の選択メニューの生成
//=============================================================================
void CGameScene::CreateMenuEndGame(void) {
	//すでに生成されている場合終了
	if (m_pMenuGameEnd != nullptr) return;

	//メニューの生成
	m_pMenuGameEnd = CSelectMenu2D::Create(2);
	if (m_pMenuGameEnd == nullptr) return;

	//横選択
	m_pMenuGameEnd->SetSelectType(CSelectMenu::SELECT_TYPE::HORIZONTAL);
	//選択肢UIの詳細設定
	m_pMenuGameEnd->SetSelectUI(0, D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 130.0f, 600.0f, 0.0f), 220.0f, 40.0f, CTexture::TEXTURE_TYPE::TEXT_QUIT);
	m_pMenuGameEnd->SetSelectUI(1, D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 130.0f, 600.0f, 0.0f), 220.0f, 40.0f, CTexture::TEXTURE_TYPE::TEXT_RETRY);
	//選択肢アイコンの生成
	m_pMenuGameEnd->CreateSelectIcon(D3DXVECTOR3(-110.0f, 0.0f, 0.0f), 50.0f, 50.0f, CTexture::TEXTURE_TYPE::SELECT_ICON);
	//背景の生成
	m_pMenuGameEnd->CreateMenuBG(CTexture::TEXTURE_TYPE::MENU_BG , D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 600.0f, 0.0f), 600.0f, 80.0f);

}