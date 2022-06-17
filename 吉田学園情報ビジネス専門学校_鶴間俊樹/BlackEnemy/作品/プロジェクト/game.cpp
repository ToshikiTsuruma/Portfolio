//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "game.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sound.h"
#include "pause.h"
#include "fade.h"
#include "timer.h"
#include "score.h"
#include "player.h"
#include "scene2D.h"
#include "billboard.h"
#include "terrain.h"
#include "meshfield.h"
#include "wallCylinder.h"
#include "boss.h"
#include "enemy_normal.h"
#include "enemy_human.h"
#include "tree.h"
#include "compass.h"

#include "particleEffect.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define GAME_TIME (120)			//ゲームの制限時間
#define COUNT_FADE_GAMEOVER (180)		//ゲームオーバー時のフェードまでのカウント
#define COUNT_FADE_GAMECLEAR (180)		//ゲームクリア時のフェードまでのカウント
#define FADE_SPEED (0.02f)		//フェードする速度
#define GAMECLEAR_START (60)	//ゲームクリア時に表示が現れるまでのカウント
#define GAMEOVER_START (120)	//ゲームオーバー時に表示が現れるまでのカウント
#define GAMEOVER_TIME (120)		//ゲームオーバーの表示が透明から不透明になるまでの時間
#define WALL_RADIUS_STAGE_START (5000.0f)	//スタート時のステージの壁の半径
#define WALL_RADIUS_STAGE_BOSS (2500.0f)	//ボス戦時のステージの壁の半径

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CGame::m_nScoreLate = 0;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGame::CGame() {
	m_bChangeStage = false;
	m_pTimer = nullptr;
	m_pPlayer = nullptr;
	m_pRaft = nullptr;
	m_pIconInteract = nullptr;
	m_pBoss = nullptr;
	m_pGameClear = nullptr;
	m_pGameOver = nullptr;
	m_bGameOver = false;
	m_bGameClear = false;
	m_nCntFinish = 0;
	m_pFogBillboard = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame() {

}

//=============================================================================
// ゲーム終了時のスコアの設定
//=============================================================================
void CGame::SetScoreLate(int nScore) {
	m_nScoreLate = nScore;
}

//=============================================================================
// ゲーム終了時のスコアの取得
//=============================================================================
int CGame::GetScoreLast(void) {
	return m_nScoreLate;
}

//=============================================================================
// ゲームの初期化処理
//=============================================================================
HRESULT CGame::Init(void) {
	//----------------------------------
	//モーションのロード
	//----------------------------------
	//プレイヤーモーションのロード
	CPlayer::LoadMotionInfo();
	//敵のモーションのロード
	CEnemyNormal::LoadMotionInfo();
	CEnemyHuman::LoadMotionInfo();
	//ボスのモーションのロード
	CBoss::LoadMotionInfo();
	//木のモーションのロード
	CTree::LoadMotionInfo();

	//----------------------------------
	//オブジェクトのインスタンス生成
	//----------------------------------
	//タイマーの枠の生成
	CScene2D* pTimerFrame = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH -20.0f - 75.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::TIMER_FRAME, 200.0f, 80.0f);
	if (pTimerFrame != nullptr) {
		pTimerFrame->SetDrawPriority(CScene::DRAW_PRIORITY::UI);
	}
	//タイマーの生成
	m_pTimer = CTimer::Create(GAME_TIME, 3, CTexture::TEXTURE_TYPE::NUMBER_004, D3DXVECTOR3(SCREEN_WIDTH - 20.0f, 25.0f, 0.0f), 50.0f, 50.0f);
	//地形の生成
	CTerrain* pTerrain = CTerrain::Create(CTerrain::TERRAIN_TYPE::START);
	if (pTerrain != nullptr) pTerrain->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//描画順の設定
	//海の生成
	CMeshfield* pMeshField = CMeshfield::Create(D3DXVECTOR3(0.0f, HEIGHT_SEA, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, 30000.0f, 30000.0f);
	if (pMeshField != nullptr) {
		pMeshField->SetColor(D3DXCOLOR(0.0f, 0.2f, 0.6f, 1.0f));
		pMeshField->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//描画順の設定
	}

	//フォグを描画させるためビルボード
	m_pFogBillboard = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 15000.0f, 12000.0f);
	//フォグを描画させるビルボードの設定
	if (m_pFogBillboard != nullptr) {
		m_pFogBillboard->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//描画順の設定
		m_pFogBillboard->SetEnableFog(true);		//フォグが有効
		m_pFogBillboard->SetZtestAlways(false);		//深度が影響する
		m_pFogBillboard->SetDrawAllDistance(true);	//距離に関係なく描画できる
	}

	//円柱の壁の生成
	CWallCylinder* pWall = CWallCylinder::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), WALL_RADIUS_STAGE_START);
	//円柱の壁の設定
	if (pWall != nullptr) pWall->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//描画順の設定

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, HEIGHT_SEA, -3750.0f));
	//木の生成
	CTree::SetTreeCircle(D3DXVECTOR3(0.0f, HEIGHT_SEA, 0.0f), 8, 500.0f, 800, 1200);
	//敵の生成
	CEnemy::SetEnemyCircle(D3DXVECTOR3(0.0f, HEIGHT_SEA, 0.0f), 3, 1000.0f, 500, 800);
	//筏を生成
	m_pRaft = CSceneModel::Create(CModel::MODELTYPE::RAFT, D3DXVECTOR3(0.0f, HEIGHT_SEA, 4700.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.1f, 0.0f));
	if (m_pRaft != nullptr) m_pRaft->SetObjType(CScene::OBJ_TYPE::OTHER);
	//筏の位置のパーティクルエフェクトの生成
	CParticleEffect::PARTICLE_INFO particleInfo = { 120, 50.0f, -0.5f, 2.0f, D3DXVECTOR3(0.0f, -0.0f, 0.0f), D3DXCOLOR(0.2f, 0.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.2f, 1.0f) };
	CParticleEffect::Create(particleInfo, D3DXVECTOR3(0.0f, HEIGHT_SEA, 4700.0f), 0, 10, 0.04f * D3DX_PI);
	//インタラクトキー有効時のアイコンの生成	筏の上の位置
	m_pIconInteract = CBillboard::Create(D3DXVECTOR3(0.0f, 100.0f, 4700.0f), CTexture::TEXTURE_TYPE::NONE, 50.0f, 50.0f);
	if (m_pIconInteract != nullptr) {
		m_pIconInteract->SetObjType(CScene::OBJ_TYPE::OTHER);
		m_pIconInteract->SetDrawPriority(CScene::DRAW_PRIORITY::UI);
		m_pIconInteract->SetDraw(false);
	}
	//コンパスの生成
	CCompass* pCompass = CCompass::Create();
	if (pCompass != nullptr) pCompass->SetObjType(CScene::OBJ_TYPE::OTHER);

	//----------------------------------
	//メンバ変数の初期化
	//----------------------------------
	m_nScoreLate = 0;
	m_bGameOver = false;
	m_bGameClear = false;
	m_nCntFinish = 0;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//曲の再生
	if (pSound != nullptr) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_GAME);
		pSound->SetBGM(CSound::SOUND_LABEL::BGM_GAME);
	}

	return S_OK;
}

//=============================================================================
// ゲームの終了処理
//=============================================================================
void CGame::Uninit(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	CSound *pSound = nullptr;
	CPause* pPause = nullptr;

	if (pManager != nullptr) {
		//サウンドの取得
		pSound = pManager->GetSound();
		//ポーズの取得
		pPause = pManager->GetPause();
	}

	//ポーズなし
	if (pPause != nullptr) {
		pPause->SetPause(false);
	}

	//曲の停止
	if (pSound != nullptr) {
		pSound->StopSound(CSound::GetBGM());
		pSound->SetBGM(CSound::SOUND_LABEL::NONE);
	}

	//オブジェクトの破棄
	CScene::ReleaseAll();

	//----------------------------------
	//モーションのアンロード
	//----------------------------------
	//プレイヤーモーションのアンロード
	CPlayer::UnloadMotionInfo();
	//ボスのモーションのアンロード
	CBoss::UnloadMotionInfo();
	//敵のモーションのアンロード
	CEnemyNormal::UnloadMotionInfo();
	CEnemyHuman::UnloadMotionInfo();
	//木のモーションのアンロード
	CTree::UnloadMotionInfo();
}

//=============================================================================
// ゲームの更新処理
//=============================================================================
void CGame::Update(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	CPause* pPause = nullptr;
	CInput* pInput = nullptr;
	CFade* pFade = nullptr;
	CSound* pSound = nullptr;
	CCamera* pCamera = nullptr;

	if (pManager != nullptr) {
		//ポーズの取得
		pPause = pManager->GetPause();
		//現在の入力デバイスの取得
		pInput = pManager->GetInputCur();
		//フェードの取得
		pFade = pManager->GetFade();
		//サウンドの取得
		pSound = pManager->GetSound();
		//カメラの取得
		pCamera = pManager->GetCamera();
	}

	if (pPause != nullptr) {
		//ポーズ中だった場合更新しない
		if (pPause->GetPause()) return;
	}

	if (pFade != nullptr) {
		//ステージの切り替え時のフェード中
		if (m_bChangeStage) {
			//ステージの切り替え
			if (pFade->GetChangeFade()) SetNextStage();
		}
	}

	//霧用のビルボードの位置を設定
	if (pCamera != nullptr && m_pFogBillboard != nullptr) {
		D3DXVECTOR3 posCameraV = pCamera->GetPosV();
		D3DXVECTOR3 posCameraR = pCamera->GetPosR();
		D3DXVECTOR3 vecCamera = posCameraR - posCameraV;	//カメラの視点から注視点へのベクトル
		D3DXVec3Normalize(&vecCamera, &vecCamera);	//正規化
		D3DXVECTOR3 posFogBillboard = posCameraV + vecCamera * MAX_FOG_DISTANCE;

		m_pFogBillboard->SetPos(posFogBillboard);
	}

	//インタラクトキー有効時のアイコンのテクスチャの更新
	if (m_pIconInteract != nullptr && pInput != nullptr) {
		CInputKeyboard* pInputKeyboard = dynamic_cast<CInputKeyboard*>(pInput);
		//現在の入力デバイスがキーボードだった場合
		if (pInputKeyboard != nullptr) {
			m_pIconInteract->SetTexType(CTexture::TEXTURE_TYPE::ICON_INTERACT_KEYBOARD);
		}
		//ゲームパッド
		else {
			m_pIconInteract->SetTexType(CTexture::TEXTURE_TYPE::ICON_INTERACT_GAMEPAD);
		}
	}

	//タイマーが０以下でゲームオーバー
	if (!m_bGameOver && m_pTimer != nullptr) {
		if (m_pTimer->GetTime() <= 0) {
			GameOver();
		}
	}

	//ゲームオーバー時、ゲームクリア時ではない場合
	if (!m_bGameOver && !m_bGameClear) {
		//ポーズ
		if (pFade != nullptr) {
			if (!pFade->GetFade() && pInput->GetTrigger(CInput::CODE::PAUSE)) {
				pPause->SetPause(true);
			}
		}
	}

	//ゲームクリア時
	if (m_bGameClear) {
		//ゲームクリアから数フレーム後
		if (m_nCntFinish == GAMECLEAR_START) {

		}
	}

	//ゲームオーバー時
	if (m_bGameOver) {
		if (m_nCntFinish == GAMEOVER_START) {
			//効果音の再生
			if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::GAMEOVER);
			//ゲームオーバー画面の生成
			m_pGameOver = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::GAMEOVER, SCREEN_WIDTH, SCREEN_HEIGHT);
			if (m_pGameOver != nullptr) {
				m_pGameOver->SetDrawPriority(CScene2D::DRAW_PRIORITY::FRONT);	//描画順を最前
				m_pGameOver->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));		//透明
			}
		}
		//透明度を上げていく
		if (m_pGameOver != nullptr && m_nCntFinish <= (GAMEOVER_START + GAMEOVER_TIME)) {
			m_pGameOver->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(m_nCntFinish - GAMEOVER_START) / (float)GAMEOVER_TIME));
		}
	}

	//ゲームクリア時かゲームオーバー時のフェードまでのカウント
	if (pFade != nullptr && (m_bGameClear || m_bGameOver)) {
		//ゲームクリア時
		if (m_bGameClear) {
			//フェード開始
			if (m_nCntFinish == COUNT_FADE_GAMECLEAR) {
				pFade->SetFade(CManager::MODE::RESULT, FADE_SPEED);
			}
		}
		//ゲームオーバー時
		else if (m_bGameOver) {
			//フェード開始
			if (m_nCntFinish == COUNT_FADE_GAMEOVER + GAMEOVER_TIME) {
				pFade->SetFade(CManager::MODE::TITLE, FADE_SPEED);
			}
		}
		m_nCntFinish++;
	}
}

//=============================================================================
// ステージ変更中かどうかの取得
//=============================================================================
bool CGame::GetChangeStage(void) {
	return m_bChangeStage;
}

//=============================================================================
// 次のステージへ移行するためのフェードを設定
//=============================================================================
void CGame::SetFadeNextStage(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//フェードの取得
	CFade* pFade = nullptr;
	if (pManager != nullptr) pFade = pManager->GetFade();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	if (pFade != nullptr) {
		//フェードの設定
		pFade->SetFade((CManager::MODE) -1, 0.01f);
		m_bChangeStage = true;
		//フェード音を再生
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CHANGE_STAGE);
		//壁の破棄
		CScene::ReleaseObjtype(CScene::OBJ_TYPE::WALL);
	}
}

//=============================================================================
// 次のステージを設定
//=============================================================================
void CGame::SetNextStage(void) {
	m_bChangeStage = false;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//カメラの取得
	CCamera* pCamera = nullptr;
	if (pManager != nullptr) pCamera = pManager->GetCamera();

	//音楽の停止
	if (pSound != nullptr) {
		pSound->StopSound(CSound::GetBGM());
		pSound->SetBGM(CSound::SOUND_LABEL::NONE);
	}
	//全敵の破棄
	CScene::ReleaseObjtype(CScene::OBJ_TYPE::ENEMY);
	//全木の破棄
	CScene::ReleaseObjtype(CScene::OBJ_TYPE::TREE);
	//全木の破棄
	CScene::ReleaseObjtype(CScene::OBJ_TYPE::ITEM);
	//地形の破棄
	CScene::ReleaseObjtype(CScene::OBJ_TYPE::TERRAIN);
	//その他オブジェクトの破棄
	CScene::ReleaseObjtype(CScene::OBJ_TYPE::OTHER);
	//筏のポインタをnullにする その他のオブジェクトとして破棄されるので
	m_pRaft = nullptr;
	m_pIconInteract = nullptr;

	//地形の生成
	CTerrain* pTerrain = CTerrain::Create(CTerrain::TERRAIN_TYPE::BOSS);
	if (pTerrain != nullptr) pTerrain->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//描画順の設定
	//壁の生成
	CWallCylinder* pWall = CWallCylinder::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), WALL_RADIUS_STAGE_BOSS);
	if (pWall != nullptr) pWall->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//描画順の設定
	//ボスの生成
	CBoss::Create(D3DXVECTOR3(0.0f, HEIGHT_SEA, 400.0f));

	//プレイヤーの設定
	if (m_pPlayer != nullptr) {
		//プレイヤーの位置を設定
		m_pPlayer->SetPos(D3DXVECTOR3(0.0f, HEIGHT_SEA, -1500.0f));
		//プレイヤーの角度を設定
		m_pPlayer->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		//カメラの設定
		if (pCamera != nullptr) {
			//カメラの位置の設定
			pCamera->SetPos(m_pPlayer->GetPos());
			//カメラの角度の設定
			pCamera->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, D3DX_PI, 0.0f));
		}
	}
}

//=============================================================================
// ボス死亡時の処理
//=============================================================================
void CGame::DeadBoss(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	if (m_pTimer != nullptr) {
		//タイマーの停止
		m_pTimer->SetStop(true);
	}

	if (pSound != nullptr) {
		//曲の停止
		pSound->StopSound(CSound::GetBGM());
	}
}

//=============================================================================
// ゲームオーバー時の処理
//=============================================================================
void CGame::GameOver(void) {
	//タイマーの停止
	if (m_pTimer != nullptr) m_pTimer->SetStop(true);

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//曲の停止
	if (pSound != nullptr) {
		pSound->StopSound(CSound::GetBGM());	//BGMの停止
		pSound->SetBGM(CSound::SOUND_LABEL::NONE);
		pSound->PlaySound(CSound::SOUND_LABEL::STUN);	//ゲームオーバー効果音の再生
	}

	//プレイヤーにゲームオーバー時の処理を行わせる
	if (m_pPlayer != nullptr) {
		m_pPlayer->GameOver();
	}

	m_bGameOver = true;	//ゲームオーバーの設定
}

//=============================================================================
//ゲームオーバーかどうかの取得
//=============================================================================
bool CGame::GetGameOver(void) { return m_bGameOver; }		

//=============================================================================
// ゲームクリア時の処理
//=============================================================================
void CGame::GameClear(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	int nTimeClear = 0;	//クリア時のタイム
	if (m_pTimer != nullptr) {
		//クリア時のタイムの取得
		nTimeClear = m_pTimer->GetTime();
	}
	//最後のゲームのスコアを設定
	SetScoreLate(nTimeClear);

	if (pSound != nullptr) {
		//ゲームクリア効果音の再生
		pSound->PlaySound(CSound::SOUND_LABEL::GAMECLEAR);
	}

	//ゲームクリア表示の生成
	m_pGameClear = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GAMECLEAR, 800.0f, 200.0f);
	//描画順を最前
	if (m_pGameClear != nullptr) m_pGameClear->SetDrawPriority(CScene2D::DRAW_PRIORITY::FRONT);
	//プレイヤーにゲームクリア時の処理を行わせる
	if (m_pPlayer != nullptr) {
		m_pPlayer->GameClear();
	}

	//ゲームクリアの設定
	m_bGameClear = true;		
}

//=============================================================================
//ゲームオーバーかどうかの取得
//=============================================================================
bool CGame::GetGameClear(void) { return m_bGameClear; }

//=============================================================================
// プレイヤーの取得
//=============================================================================
CPlayer* CGame::GetPlayer(void) {
	return m_pPlayer;
}

//=============================================================================
// ゲームが持つプレイヤーへのポインタをNULLにする
//=============================================================================
void CGame::ReleasePlayer(void) {
	m_pPlayer = nullptr;
}

//=============================================================================
// ボスの取得
//=============================================================================
CBoss* CGame::GetBoss(void) {
	return m_pBoss;
}

//=============================================================================
// ゲームが持つボスへのポインタをNULLにする
//=============================================================================
void CGame::ReleaseBoss(void) {
	m_pBoss = nullptr;
}

//=============================================================================
// タイマーの取得
//=============================================================================
CTimer* CGame::GetTimer(void) {
	return m_pTimer;
}

//=============================================================================
// 筏の取得
//=============================================================================
CSceneModel* CGame::GetRaft(void) {
	return m_pRaft;
}

//=============================================================================
// インタラクトキー有効時のアイコンの取得
//=============================================================================
CBillboard* CGame::GetIconInteract(void) {
	return m_pIconInteract;
}