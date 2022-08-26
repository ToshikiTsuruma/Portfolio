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
#include "pauseMenu.h"
#include "stage.h"
#include "wallCylinder.h"
#include "object2D.h"

#include "player.h"
#include "enemy_human.h"
#include "enemy_normal.h"
#include "enemy_sorcerer.h"

#include "appleTree.h"
#include "glowApple.h"
#include "selectMenu2D.h"
#include "enemySpawner.h"
#include "effect.h"
#include "shockWaveEmitter.h"

#include "apple_red.h"
#include "apple_green.h"
#include "apple_white.h"
#include "apple_black.h"
#include "apple_silver.h"
#include "apple_gold.h"

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

	m_nCntGameClear = 0;
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

	//モーション情報のロード
	CPlayer::LoadMotionInfo();
	CEnemyNormal::LoadMotionInfo();
	CEnemyHuman::LoadMotionInfo();
	CEnemySorcerer::LoadMotionInfo();

	//オブジェクトのポーズが無いように設定
	CObject::SetUpdatePauseLevel(0);

	//ステージの生成
	if(m_pStage == nullptr) m_pStage = new CStage;
	if(m_pStage != nullptr) m_pStage->CreateStage(TEXT_FILE_NAME_STAGE_GAME);

	//ゴールドラッシュ無効
	CEnemy::SetGoldRush(false);

	//林檎の木の生成
	m_pAppleTree = CAppleTree::Create(D3DXVECTOR3(0.0f, -190.0f, 0.0f));

	//円柱の壁の生成
	CWallCylinder::Create(D3DXVECTOR3(0.0f, 60.0f, 0.0f), 1600.0f, 40.0f, CTexture::TEXTURE_TYPE::NONE, true);

	//プレイヤーの生成
	CPlayer* pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, -300.0f, -1000.0f));
	//シーンのプレイヤーの設定
	SetPlayer(pPlayer);

	//タイマーの生成
	m_pTimer = CTimer::Create(GAME_TIME, 3, CTexture::TEXTURE_TYPE::NUMBER_003, D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 75.0f, 30.0f, 0.0f), 50.0f);

	//最初の敵の配置
	CEnemyNormal::Create(D3DXVECTOR3(900.0f, -200.0f, 300.0f));
	CEnemyNormal::Create(D3DXVECTOR3(-800.0f, -200.0f, 500.0f));
	CEnemyNormal::Create(D3DXVECTOR3(-600.0f, -200.0f, -700.0f));

	//敵のスポナーの生成
	m_pEnemySpawner = CEnemySpawner::Create(900, 1800.0f, 500, 1500);

	//BGMの再生
	if (pSound != nullptr) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_GAME);
		pSound->SetBGM(CSound::SOUND_LABEL::BGM_GAME);
	}

#ifdef _DEBUG
	//Zバッファテクスチャの表示
	CObject2D* pZBuff = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 70.0f, 0.0f + 70.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 100.0f, 100.0f);
	if (pZBuff != nullptr) {
		pZBuff->SetDrawPriority(CObject::DRAW_PRIORITY::FRONT);
		pZBuff->SetUseZBuffTexture(true);
	}
#endif

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
	CEnemySorcerer::UnloadMotionInfo();

	//シーンのプレイヤーの設定
	SetPlayer(nullptr);

	//シーンの終了処理
	CScene::Uninit();

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//音の停止
	if (pSound != nullptr) {
		pSound->StopSound();
	}

	//ゴールドラッシュ無効
	CEnemy::SetGoldRush(false);
}

//=============================================================================
// ゲームシーンの更新処理
//=============================================================================
void CGameScene::Update(void) {
#ifdef _DEBUG
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	if (pManager == nullptr) return;
	//現在の入力デバイスの取得
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;

	//ゲームクリア
	if (pInput->GetTrigger(CInput::CODE::DEBUG_0)) {
		GameClear();
	}
	//ゲームオーバー
	if (pInput->GetTrigger(CInput::CODE::DEBUG_1)) {
		GameOver();
	}

	//タイム追加
	if (pInput->GetTrigger(CInput::CODE::DEBUG_3)) {
		if (m_pTimer != nullptr) m_pTimer->AddScore(50);
	}
#endif

	//ゲームクリア時
	if (m_bGameClear) {
		UpdateGameClear();
	}
	//ゲームオーバー時
	else if (m_bGameOver) {
		UpdateGameOver();
	}
	//ゲーム中
	else
	{
		UpdateGame();
	}
}

//=============================================================================
// ゲーム中の更新
//=============================================================================
void CGameScene::UpdateGame(void) {
	//ポーズメニューがある場合は更新なし
	if (m_pMenuPause != nullptr) return;

	//ゲーム終了していないときにタイマーが０になった場合
	if (m_pTimer != nullptr && !m_bGameClear && !m_bGameOver) {
		if (m_pTimer->GetScore() <= 0) {
			//ゲームクリア
			GameClear();
		}
	}

	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	if (pManager == nullptr) return;
	//現在の入力デバイスの取得
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;
	//サウンドの取得
	CSound* pSound = pManager->GetSound();	//サウンドへのポインタ
	if (pSound == nullptr) return;
	//フェードの取得
	CFade* pFade = pManager->GetFade();	//フェードのポインタ
	if (pFade == nullptr) return;

	//ポーズ
	if (pInput->GetTrigger(CInput::CODE::PAUSE) && !pFade->GetFade()) {
		//ポーズメニュークラスを生成
		m_pMenuPause = CPauseMenu::Create();
		//サウンドを再生
		pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
	}
}

//=============================================================================
// ゲームクリア時の更新
//=============================================================================
void CGameScene::UpdateGameClear(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	if (pManager == nullptr) return;
	//現在の入力デバイスの取得
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;
	//カメラの取得
	CCamera* pCamera = pManager->GetCamera();	//カメラへのポインタ
	if (pCamera == nullptr) return;
	//フェードの取得
	CFade* pFade = pManager->GetFade();		//フェードへのポインタ
	if (pFade == nullptr) return;
	//サウンドの取得
	CSound* pSound = pManager->GetSound();	//サウンドへのポインタ
	if (pSound == nullptr) return;


	if (m_nCntGameClear == 60) {
		pFade->SetFade(CScene::SCENE_TYPE(-1), 0.005f, 100);
	}

	//タイトルへのフェード以前の、遷移なしフェード切り替えタイミング
	if (pFade->GetChangeFade() && m_nCntGameClear < 600) {
		//リンゴの木から生成したリンゴの数と種類を取得
		int nNumCreateApple = MAX_NUM_CREATE_APPLE;	//現状クリアするにはすべてのリンゴがなっているのでとりあえず最大数
		//最大数を超えないように
		if (nNumCreateApple > MAX_NUM_CREATE_APPLE) nNumCreateApple = MAX_NUM_CREATE_APPLE;

		CGlowApple::APPLE_TYPE aTypeApple[MAX_NUM_CREATE_APPLE];

		for (int nCnt = 0; nCnt < MAX_NUM_CREATE_APPLE; nCnt++)
		{
			aTypeApple[nCnt] = m_pAppleTree->GetCreateAppleType(nCnt);
		}
		//木とリンゴの破棄
		CObject::ReleaseObjtype((CObject::OBJTYPE)(CObject::OBJTYPE_APPLE_TREE));

		//木を生成
		for (int nCntTree = 0; nCntTree < nNumCreateApple; nCntTree++)
		{
			//木の配置
			D3DXVECTOR3 posTree = D3DXVECTOR3(sinf(D3DX_PI * 2 * (nCntTree / (float)nNumCreateApple) + D3DX_PI) * 700.0f, -80.0f, cosf(D3DX_PI * 2 * (nCntTree / (float)nNumCreateApple) + D3DX_PI) * 700.0f);
			
			//木の生成
			CObjectModel* pTree = CObjectModel::Create(CModel::MODELTYPE::OBJ_APPLE_TREE, posTree, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);
			
			if (pTree == nullptr) continue;

			CModel* pModelTree = pTree->GetPtrModel();
			if (pModelTree == nullptr) continue;

			//輪郭色の設定
			pModelTree->SetColorGlow(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));

			//木にリンゴの実をつける
			for (int nCntApple = 0; nCntApple < MAX_NUM_CREATE_APPLE; nCntApple++)
			{
				D3DXVECTOR3 posApple = posTree + CAppleTree::GetOffsetPosApple(nCntApple);

				//林檎を生成
				switch (aTypeApple[nCntTree])
				{
				case CGlowApple::APPLE_TYPE::RED:
					CAppleRed::Create(posApple, nullptr);
					break;
				case CGlowApple::APPLE_TYPE::GREEN:
					CAppleGreen::Create(posApple, nullptr);
					break;
				case CGlowApple::APPLE_TYPE::WHITE:
					CAppleWhite::Create(posApple, nullptr);
					break;
				case CGlowApple::APPLE_TYPE::BLACK:
					CAppleBlack::Create(posApple, nullptr);
					break;
				case CGlowApple::APPLE_TYPE::SILVER:
					CAppleSilver::Create(posApple, nullptr);
					break;
				case CGlowApple::APPLE_TYPE::GOLD:
					 CAppleGold::Create(posApple, nullptr);
					break;
				}
			}
		}

		//何かしらの装飾


		//カメラの位置設定
		pCamera->Init();
		//カメラの距離設定
		pCamera->SetDistance(2000.0f);
		//カメラの動き固定
		pCamera->SetLockControll(true);

		//プレイヤーの取得
		CPlayer* pPlayer = GetPlayer();
		if (pPlayer != nullptr) {
			//プレイヤーの位置設定
			pPlayer->SetPos(D3DXVECTOR3(0.0f, -800.0f, 0.0f));	//いい感じになったので埋もれさせる
			pPlayer->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	if (m_nCntGameClear == 540) {
		//ゲームクリアテキストの表示
		CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GAMECLEAR, 600.0f, 150.0f);
		//クリア音を再生
		pSound->PlaySound(CSound::SOUND_LABEL::GAMECLEAR);
	}
	if (m_nCntGameClear == 900) {
		//カメラの動きの固定を解除
		pCamera->SetLockControll(false);
		//タイトルへフェード
		pFade->SetFade(CScene::SCENE_TYPE::TITLE, 0.01f, 60);
	}

	//ゲームクリア後のカウント
	m_nCntGameClear++;
}

//=============================================================================
// ゲームオーバー時の更新
//=============================================================================
void CGameScene::UpdateGameOver(void) {
	if (m_pMenuGameEnd == nullptr) return;

	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	if (pManager == nullptr) return;
	//現在の入力デバイスの取得
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;
	//フェードの取得
	CFade* pFade = pManager->GetFade();		//フェードへのポインタ
	if (pFade == nullptr) return;
	//サウンドの取得
	CSound* pSound = pManager->GetSound();	//サウンドへのポインタ
	if (pSound == nullptr) return;

	//選択時
	if (pInput->GetTrigger(CInput::CODE::SELECT)) {
		switch (m_pMenuGameEnd->GetIdxCurSelect())
		{
		case 0:
			//タイトルへシーン遷移
			pFade->SetFade(CScene::SCENE_TYPE::TITLE, 0.02f, 60);
			break;
		case 1:
			//ゲームを再プレイ
			pFade->SetFade(CScene::SCENE_TYPE::GAME, 0.02f, 60);
			break;
		}

		//選択決定音の再生
		pSound->PlaySound(CSound::SOUND_LABEL::TITLE_START);
	}
}

//=============================================================================
// ポーズメニューの破棄
//=============================================================================
void CGameScene::DeletePauseMenu(void) {
	//ポーズメニューの破棄
	if (m_pMenuPause != nullptr) {
		m_pMenuPause->Uninit();
		m_pMenuPause = nullptr;
	}
}

//=============================================================================
// ゲームクリア
//=============================================================================
void CGameScene::GameClear(void) {
	if (m_bGameClear || m_bGameOver) return;

	m_bGameClear = true;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//プレイヤーの取得
	CPlayer* pPlayer = GetPlayer();
	if (pPlayer != nullptr) {
		//プレイヤーのゲームクリア時の処理
		pPlayer->GameClear();
	}

	//敵をすべて死亡させる
	CObject::DeadObjtype(CObject::OBJTYPE_ENEMY);
	//敵消滅音を再生
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::ENDGAME);
	//衝撃波
	CShockWaveEmitter::Create(3, 6, D3DXVECTOR3(0.0f, -190.0f, 0.0f), 60.0f, 10.0f, 15.0f, 400.0f, -2.0f, 120, D3DX_PI * 0.01f);

	//タイマーを破棄
	if (m_pTimer != nullptr) {
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	//敵のスポナーの破棄
	if (m_pEnemySpawner != nullptr) {
		m_pEnemySpawner->Uninit();
		m_pEnemySpawner = nullptr;
	}

	//オブジェクトのポーズが無いように設定（念のため）
	CObject::SetUpdatePauseLevel(0);
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


	//プレイヤーの取得
	CPlayer* pPlayer = GetPlayer();
	if (pPlayer != nullptr) {
		//プレイヤーのゲームオーバー時の処理
		pPlayer->GameOver();
	}

	//ゲームオーバーテキストの表示
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GAMEOVER, 600.0f, 150.0f);

	//ゲーム終了メニューの生成
	CreateMenuEndGame();

	//タイマーを破棄
	if (m_pTimer != nullptr) {
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	//敵のスポナーの破棄
	if (m_pEnemySpawner != nullptr) {
		m_pEnemySpawner->Uninit();
		m_pEnemySpawner = nullptr;
	}

	//オブジェクトのポーズが無いように設定（念のため）
	CObject::SetUpdatePauseLevel(0);
}

//=============================================================================
// ゲーム終了時の選択メニューの生成
//=============================================================================
void CGameScene::CreateMenuEndGame(void) {
	//すでに生成されている場合終了
	if (m_pMenuGameEnd != nullptr) return;

	//メニューの生成
	m_pMenuGameEnd = CSelectMenu2D::Create(2, true);
	if (m_pMenuGameEnd == nullptr) return;

	//背景の設定
	m_pMenuGameEnd->SetMenuBG(CTexture::TEXTURE_TYPE::MENU_BG, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 600.0f, 0.0f), 600.0f, 80.0f);
	//横選択
	m_pMenuGameEnd->SetSelectType(CSelectMenu::SELECT_TYPE::HORIZONTAL);
	//選択肢UIの詳細設定
	m_pMenuGameEnd->SetSelectUI(0, D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 130.0f, 600.0f, 0.0f), 220.0f, 40.0f, CTexture::TEXTURE_TYPE::TEXT_QUIT);
	m_pMenuGameEnd->SetSelectUI(1, D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 130.0f, 600.0f, 0.0f), 220.0f, 40.0f, CTexture::TEXTURE_TYPE::TEXT_RETRY);
	//選択肢アイコンの生成
	m_pMenuGameEnd->CreateSelectIcon(D3DXVECTOR3(-80.0f, 0.0f, 0.0f), 50.0f, 50.0f, CTexture::TEXTURE_TYPE::SELECT_ICON);
	m_pMenuGameEnd->SetIconPosOffset(1, D3DXVECTOR3(-105.0f, 0.0f, 0.0f));
}