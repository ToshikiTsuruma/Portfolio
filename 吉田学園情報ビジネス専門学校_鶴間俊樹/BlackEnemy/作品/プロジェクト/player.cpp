//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sound.h"
#include "input.h"
#include "game.h"
#include "wallCylinder.h"
#include "terrain.h"
#include "item.h"
#include "effect.h"
#include "particle.h"
#include "particleEffect.h"
#include "scene2D.h"
#include "billboard.h"
#include "sword.h"
#include "katana.h"	 
#include "score.h"
#include "timer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_player.txt"
#define COLLISION_RADIUS (15.0f)		//当たり判定の半径
#define NUM_COLLISION (10)				//当たり判定の数
#define ADD_MOVE_SPEED (0.8f)			//加速
#define DEC_MOVE_SPEED_GROUND (0.3f)	//地上での減速
#define DEC_MOVE_SPEED_AIR (0.05f)		//空中での減速
#define MAX_MOVE_SPEED_WALK (3.0f)		//歩行速度
#define MAX_MOVE_SPEED_DASH (6.0f)		//走行速度
#define MAX_MOVE_SPEED_AIR (2.0f)		//空中速度
#define ROTATE_SPEED (0.1f)				//回転速度
#define DODGE_TIME_FRONT (10)			//前方回避時間
#define DODGE_TIME_BACK (6)				//後方回避時間
#define DODGE_TIME_SHEATH (4)			//納刀回避時間
#define DODGE_TIME_DRAWN_SWORD (4)		//抜刀回避時間
#define DODGE_SPEED_FRONT (18.0f)		//前方回避速度
#define DODGE_SPEED_BACK (14.0f)		//後方回避速度
#define DODGE_SPEED_SHEATH (22.0f)		//納刀回避速度
#define DODGE_SPEED_DRAWN_SWORD (20.0f)	//抜刀回避速度
#define INDEX_MODEL_PUNCH_1 (5)			//パンチ攻撃時(1)の当たり判定があるモデルのインデックス
#define INDEX_MODEL_PUNCH_2 (3)			//パンチ攻撃時(2)の当たり判定があるモデルのインデックス
#define MAX_LIFE (12)					//体力の最大値
#define DANGER_LIFE (MAX_LIFE / 3)		//体力の危険値
#define COLOR_LIFE_GAUGE_SAFE (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//体力バーの安全時の色
#define COLOR_LIFE_GAUGE_DANGER (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//体力バーの危険時の色
#define START_HEAL_STUN_TIME (60)	//スタン回復開始時間
#define END_STUN_TIME (600)			//スタンしている時間
#define MAX_ITEM_OWN (10)			//アイテム所有上限数
#define SPAN_ITEM_USE_SOUND (40)	//アイテム使用時の音の間隔
#define TIME_ITEM_USE (120)			//アイテム使用完了時間
#define HEAL_VALUE_APPLE (6)		//リンゴアイテムの回復量
#define COLOR_ITEM_DEFAULT (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//アイテムカウンタの色のデフォルト
#define COLOR_ITEM_NONE (D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f))		//アイテム無所持のカウンタの色
#define COLOR_ITEM_MAX (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))		//アイテム上限時のカウンタの色
#define COLOR_OUTLINE (D3DXCOLOR(0.2f, 0.5f, 1.0f, 1.0f))	//モデルの輪郭の色
#define DISTANCE_USE_RAFT (300.0f)			//ステージ移動できる筏との距離
#define CAMERA_DISTANCE (500.0f)			//プレイヤーからカメラの距離
#define CAMERA_ROTATE_SPEED_YAW (0.008f)	//カメラの回転速度(Y軸)
#define CAMERA_ROTATE_SPEED_PITCH (0.006f)	//カメラの回転速度(X軸)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CPlayer::m_nNumParts = 0;
CSceneMotion::PARTS_INFO* CPlayer::m_pPartsInfoArray = nullptr;
CSceneMotion::MOTION_INFO CPlayer::m_aMotionInfo[(int)CPlayer::MOTION_TYPE::ENUM_MAX] = {};
int CPlayer::m_nIdxWeapon = -1;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CPlayer::CPlayer() : CSceneMotion(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, true)
{
	SetObjType(OBJ_TYPE::PLAYER);	//オブジェクトタイプの設定
	SetUpdatePriority(UPDATE_PRIORITY::PLAYER);	//更新順の設定
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//描画順の設定

	SetColorOutlineAll(COLOR_OUTLINE);	//輪郭の色の設定

	m_lastPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_destRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bLand = false;
	m_bValidAttack = false;
	m_nNumKillEnemy = 0;
	m_bLockAct = false;
	m_nCntDodge = 0;
	m_nLife = MAX_LIFE;
	m_bStun = false;
	m_nCntStun = 0;

	//体力ゲージの生成
	m_pGaugeLife = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 550.0f, 20.0f);	//ゲージ
	CScene2D* pGaugeLifeBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 550.0f, 20.0f);			//背景
	CScene2D* pGaugeLifeFrame = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f), CTexture::TEXTURE_TYPE::GAUGE_LIFE_PLAYER, 600.0f, 30.0f);	//枠
	//体力ゲージの設定 順番厳守
	if (pGaugeLifeBg != nullptr) {	//背景
		pGaugeLifeBg->SetDrawPriority(DRAW_PRIORITY::UI);
		pGaugeLifeBg->SetColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
	}
	if (m_pGaugeLife != nullptr) {	//ゲージ
		m_pGaugeLife->SetDrawPriority(DRAW_PRIORITY::UI);
		m_pGaugeLife->SetColor(COLOR_LIFE_GAUGE_SAFE);
	}
	if (pGaugeLifeFrame != nullptr) {	//枠
		pGaugeLifeFrame->SetDrawPriority(DRAW_PRIORITY::UI);
	}

	//アイテムの枠の生成
	CScene2D* pFrameItem = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::ITEM_FRAME, 100.0f, 100.0f);	//枠
	CScene2D* pFrameItemBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 90.0f, 90.0f);		//背景
	CScene2D* pTextItem = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 125.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_ITEM, 100.0f, 30.0f);	//文字
	CScene2D* pIconItem = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::ICON_APPLE, 75.0f, 75.0f);	//アイテムのアイコン

	m_bUseItem = false;
	m_nCntUseItem = 0;
	//アイテム使用ゲージの生成
	m_pGaugeUseItem = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 90.0f, 90.0f);
	//描画順の設定
	//アイテムの背景
	if (pFrameItemBg != nullptr) {
		pFrameItemBg->SetDrawPriority(DRAW_PRIORITY::UI);
		pFrameItemBg->SetColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
	}
	//アイテムのアイコン
	if (pIconItem != nullptr) {
		pIconItem->SetDrawPriority(DRAW_PRIORITY::UI);
	}
	//アイテム使用ゲージ
	if (m_pGaugeUseItem != nullptr) {
		m_pGaugeUseItem->SetDrawPriority(DRAW_PRIORITY::UI);
		m_pGaugeUseItem->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.2f, 0.5f));
		m_pGaugeUseItem->SetRatioHeight(0.0f);
	}
	//アイテムの枠
	if (pFrameItem != nullptr) {
		pFrameItem->SetDrawPriority(DRAW_PRIORITY::UI);
	}
	//アイテムの文字
	if (pTextItem != nullptr) {
		pTextItem->SetDrawPriority(DRAW_PRIORITY::UI);
	}

	//アイテムカウンターの生成
	m_pCounterItem = CScore::Create(2, CTexture::TEXTURE_TYPE::NUMBER_004, D3DXVECTOR3(SCREEN_WIDTH - 20.0f, SCREEN_HEIGHT - 45.0f, 0.0f), 25.0f, 25.0f, 0);
	if (m_pCounterItem != nullptr) m_pCounterItem->SetColor(COLOR_ITEM_NONE);

	m_typeWeapon = WEAPONTYPE::PUNCH;
	m_pWeapon = nullptr;

	//武器の枠の生成
	CScene2D* pFrameWeapon = CScene2D::Create(D3DXVECTOR3(60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::WEAPON_FRAME, 100.0f, 100.0f);	//枠
	CScene2D* pFrameWeaponBg = CScene2D::Create(D3DXVECTOR3(60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 90.0f, 90.0f);			//背景
	CScene2D* pTextWeapon = CScene2D::Create(D3DXVECTOR3(60.0f, SCREEN_HEIGHT - 125.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_WEAPON, 100.0f, 30.0f);	//文字
	m_pIconWeapon = CScene2D::Create(D3DXVECTOR3(60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::ICON_NONE, 75.0f, 75.0f);						//武器のアイコン
	//描画順の設定
	//武器の背景
	if (pFrameWeaponBg != nullptr) {
		pFrameWeaponBg->SetDrawPriority(DRAW_PRIORITY::UI);
		pFrameWeaponBg->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	}
	//武器のアイコン
	if (m_pIconWeapon != nullptr) {
		m_pIconWeapon->SetDrawPriority(DRAW_PRIORITY::UI);
	}
	//武器の枠
	if (pFrameWeapon != nullptr) {
		pFrameWeapon->SetDrawPriority(DRAW_PRIORITY::UI);
	}
	//武器の文字
	if (pTextWeapon != nullptr) {
		pTextWeapon->SetDrawPriority(DRAW_PRIORITY::UI);
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// プレイヤーの生成処理
//=============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos) {
	CPlayer* pPlayer;
	pPlayer = new CPlayer();
	if (pPlayer != nullptr) {
		pPlayer->CSceneMotion::SetPos(pos);
		pPlayer->Init();
	}

	return pPlayer;
}

//=============================================================================
// モーション情報をロード
//=============================================================================
void CPlayer::LoadMotionInfo(void) {
	//ロードする前に動的確保したメモリをdeleteする
	UnloadMotionInfo();

	Load(TEXT_FILE_NAME_LOAD_MOTION, &m_pPartsInfoArray, &m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, &m_nIdxWeapon);
}

//=============================================================================
// モーション情報をアンロード
//=============================================================================
void CPlayer::UnloadMotionInfo(void) {
	m_nNumParts = 0;
	m_nIdxWeapon = -1;

	//パーツ情報の解放
	if (m_pPartsInfoArray != nullptr) {
		delete[] m_pPartsInfoArray;
		m_pPartsInfoArray = nullptr;
	}
	//モーション情報の解放
	for (int nCntMotionInfo = 0; nCntMotionInfo < (int)MOTION_TYPE::ENUM_MAX; nCntMotionInfo++)
	{
		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
		{
			if (m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray != nullptr) {
				delete[] m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray;
				m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray = nullptr;
			}
		}
	}
}

//=============================================================================
// プレイヤーの初期化処理
//=============================================================================
HRESULT CPlayer::Init(void) {
	//プレイヤーの初期設定
	CSceneMotion::SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	m_destRot.y =  D3DX_PI;	//奥向き

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//カメラの取得
	CCamera* pCamera = nullptr;
	if (pManager != nullptr) pCamera = pManager->GetCamera();

	//カメラの初期設定
	if (pCamera != nullptr) {
		pCamera->SetPos(CSceneMotion::GetPos());
		pCamera->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, D3DX_PI, 0.0f));
		pCamera->SetDistance(CAMERA_DISTANCE);
	}

	CSceneMotion::Init();
	SetMotion((int)MOTION_TYPE::NEUTRAL);	//モーションの初期設定

	return S_OK;
}

//=============================================================================
// プレイヤーの終了処理
//=============================================================================
void CPlayer::Uninit(void) {
	//武器の破棄
	if (m_pWeapon != nullptr) {
		m_pWeapon->Uninit();
		m_pWeapon = nullptr;
	}
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//ゲームの取得
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//ゲームが保持しているプレイヤーのポインタをnullptrにする
	if (pGame != nullptr) pGame->ReleasePlayer();
	//終了処理
	CSceneMotion::Uninit();
}

//=============================================================================
// プレイヤーの更新処理
//=============================================================================
void CPlayer::Update(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	CInput* pInput = nullptr;
	CSound *pSound = nullptr;
	CCamera* pCamera = nullptr;
	CGame* pGame = nullptr;

	if (pManager != nullptr) {
		//現在の入力デバイスの取得
		pInput = pManager->GetInputCur();
		//サウンドの取得
		pSound = pManager->GetSound();
		//カメラの取得
		pCamera = pManager->GetCamera();
		//ゲームの取得
		pGame = pManager->GetGame();
	}

	//位置情報のポインタの取得
	D3DXVECTOR3* pPosPlayer = GetPtrPos();

	//----------------------------
	//ゲームクリア時かステージ変更時
	//----------------------------
	if (pGame != nullptr) {
		if (pGame->GetGameClear() || pGame->GetChangeStage()) {
			//モーションの更新
			CSceneMotion::Update();
			return;
		}
	}

	//----------------------------
	//カメラの設定
	//----------------------------
	float fRotCameraY = 0.0f;	//カメラの角度
	if (pCamera != nullptr) {
		D3DXVECTOR3 posCamera = CSceneMotion::GetPos();
		posCamera.y += 50.0f;
		//カメラの位置を更新
		pCamera->SetPos(posCamera);

		D3DXVECTOR3 rotCamera = pCamera->GetRot();
		if (pInput != nullptr) {
			if (pInput->GetPress(CInput::CODE::CAMERA_UP)) {
				//視点を下げる
				rotCamera.x -= CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
				if (rotCamera.x < -0.05f * D3DX_PI) {
					rotCamera.x = -0.05f * D3DX_PI;
				}
			}
			if (pInput->GetPress(CInput::CODE::CAMERA_DOWN)) {
				//視点を上げる
				rotCamera.x += CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
				if (rotCamera.x > 0.25f * D3DX_PI) {
					rotCamera.x = 0.25f * D3DX_PI;
				}
			}
			if (pInput->GetPress(CInput::CODE::CAMERA_LEFT)) {
				//視点を左回転
				rotCamera.y -= CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
				if (rotCamera.y < -D3DX_PI) {
					rotCamera.y = rotCamera.y + D3DX_PI * 2;
				}
			}
			if (pInput->GetPress(CInput::CODE::CAMERA_RIGHT)) {
				//視点を右回転
				rotCamera.y += CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
				if (rotCamera.y > D3DX_PI) {
					rotCamera.y = rotCamera.y - D3DX_PI * 2;
				}
			}
		}
		pCamera->SetRot(rotCamera);

		fRotCameraY = pCamera->GetRot().y;	//カメラの角度を取得
		fRotCameraY += D3DX_PI;	//角度を逆にする
	}


	//----------------------------
	//スタン時 (ゲームオーバー時含む)
	//----------------------------
	if (m_bStun) {
		if (pGame != nullptr) {
			//ゲームオーバー時なら死亡モーションのまま動かない
			if (pGame->GetGameOver()) {
				//モーションの更新
				CSceneMotion::Update();
				return;
			}
		}

		//スタンカウンタの加算
		m_nCntStun++;

		//体力ゲージの回復
		if (m_pGaugeLife != nullptr && m_nCntStun >= START_HEAL_STUN_TIME && m_nCntStun <= END_STUN_TIME) m_pGaugeLife->SetRatioWidth((float)(m_nCntStun - START_HEAL_STUN_TIME) / (float)(END_STUN_TIME - START_HEAL_STUN_TIME));

		//スタン回復開始時間
		if (m_nCntStun == START_HEAL_STUN_TIME) {
			//気絶する音
			if (pSound != nullptr)pSound->PlaySound(CSound::SOUND_LABEL::STUN);
		}

		//スタン回復終了時
		if (m_nCntStun == END_STUN_TIME) {
			//体力のリセット
			m_nLife = MAX_LIFE;
			//モーションの設定
			SetMotion((int)MOTION_TYPE::NEUTRAL);
		}
		//ニュートラルモーションへの移行が終わった場合
		if (GetMotionType() != (int)MOTION_TYPE::STUN && !GetTransMotion()) {
			//スタン解除、そのまま更新
			m_bStun = false;
			return;
		}
		else {
			//モーションの更新
			CSceneMotion::Update();
			return;
		}
	}

	//----------------------------
	//硬直中
	//----------------------------
	if (m_bLockAct) {
		//モーションの遷移が完了した場合
		if (!GetTransMotion()) {
			//硬直の解除
			m_bLockAct = false;
		}
	}

	//----------------------------
	//回避中
	//----------------------------
	if (GetMotionCategory() == MOTION_CATEGORY::DODGE) {
		m_nCntDodge--;

		//回避解除
		if (m_nCntDodge <= 0) {
			//ニュートラルモーションを設定
			SetMotion((int)MOTION_TYPE::NEUTRAL);
			//硬直状態にする
			m_bLockAct = true;

			if (m_bLand) {
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);	//大きく重力をかける
			}
			else {
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY, 0.0f);	//重力をかける
			}
		}
		else {
			//パーティクルの生成
			D3DXVECTOR3 posParticle = *pPosPlayer;	//パーティクルの位置
			posParticle.y += 10.0f;
			CParticle::Create(posParticle, 20, 25.0f, -0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}
	}

	//----------------------------
	//攻撃中の移動
	//----------------------------
	if ((GetMotionType() == (int)MOTION_TYPE::SHEATH || GetMotionType() == (int)MOTION_TYPE::ATTACK_KATANA_DRAW) && m_nCntAttackMove > 0) {
		m_nCntAttackMove--;

		//移動終了
		if (m_nCntAttackMove <= 0) {
			if (m_bLand) {
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);	//大きく重力をかける
			}
			else {
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY, 0.0f);	//重力をかける
			}
		}
	}

	if (pInput != nullptr) {
		//----------------------------
		//攻撃
		//----------------------------
		if (pInput->GetTrigger(CInput::CODE::ATTACK) && GetMotionCategory() != MOTION_CATEGORY::DODGE && !m_bLockAct) {
			//まだ攻撃していない場合
			if (GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
				//モーションの設定
				switch (m_typeWeapon)
				{
				case WEAPONTYPE::PUNCH:
					SetMotion((int)MOTION_TYPE::PUNCH_1);
					break;
				case WEAPONTYPE::SWORD:
					SetMotion((int)MOTION_TYPE::ATTACK_SWORD_1);
					break;
				case WEAPONTYPE::KATANA:
					SetMotion((int)MOTION_TYPE::ATTACK_KATANA_1);
					break;
				}
				//移動量を重力のみにする
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			}
			//攻撃中の場合
			else {
				switch (m_typeWeapon)
				{
				case WEAPONTYPE::PUNCH:
					//1回目の攻撃の最後のキーの場合
					if (GetMotionType() == (int)MOTION_TYPE::PUNCH_1 && (GetCurrentKey() == 1 || GetCurrentKey() == 2)) {
						//モーションの設定
						SetMotion((int)MOTION_TYPE::PUNCH_2);
					}
					break;
				case WEAPONTYPE::SWORD:
					//1回目の攻撃の最後のキーの場合
					if (GetMotionType() == (int)MOTION_TYPE::ATTACK_SWORD_1 && GetCurrentKey() == 2) {
						//モーションの設定
						SetMotion((int)MOTION_TYPE::ATTACK_SWORD_2);
					}
					//2回目の攻撃の最後のキーの場合
					else if (GetMotionType() == (int)MOTION_TYPE::ATTACK_SWORD_2 && GetCurrentKey() == 2) {
						//モーションの設定
						SetMotion((int)MOTION_TYPE::ATTACK_SWORD_3);
					}
					break;
				case WEAPONTYPE::KATANA:
					//1回目の攻撃の最後のキーの場合
					if (GetMotionType() == (int)MOTION_TYPE::ATTACK_KATANA_1 && GetCurrentKey() == 3) {
						//モーションの設定
						SetMotion((int)MOTION_TYPE::ATTACK_KATANA_2);
					}
					//2回目の攻撃の最後のキーの場合
					else if (GetMotionType() == (int)MOTION_TYPE::ATTACK_KATANA_2 && GetCurrentKey() == 3) {
						//モーションの設定
						SetMotion((int)MOTION_TYPE::ATTACK_KATANA_3);
					}
					//納刀時の最後のキーの場合
					else if (GetMotionType() == (int)MOTION_TYPE::SHEATH && GetCurrentKey() == 2) {
						//モーションの設定
						SetMotion((int)MOTION_TYPE::ATTACK_KATANA_DRAW);
						//回避時間の設定
						m_nCntAttackMove = DODGE_TIME_DRAWN_SWORD;
						//回避速度の設定
						float rotY = GetRot().y;
						rotY += D3DX_PI;	//角度を前方にする
						m_move = D3DXVECTOR3(DODGE_SPEED_DRAWN_SWORD * sinf(rotY), 0.0f, DODGE_SPEED_DRAWN_SWORD * cosf(rotY));	//移動量を前方にする
					}
					break;
				}
			}
		}

		//----------------------------
		//納刀中の輪郭表示
		//----------------------------
		if (GetMotionType() == (int)MOTION_TYPE::SHEATH && GetCurrentKey() == 2) {
			//抜刀攻撃可能時は輪郭表示
			if (m_pWeapon != nullptr) m_pWeapon->SetDrawOutline(true);
		}
		else {
			//輪郭非表示
			if (m_pWeapon != nullptr) m_pWeapon->SetDrawOutline(false);
		}

		//----------------------------
		//武器の変更
		//----------------------------
		if (pInput->GetTrigger(CInput::CODE::CHANGE_WEAPON) && GetMotionCategory() != MOTION_CATEGORY::ATTACK && !m_bLockAct && GetMotionCategory() != MOTION_CATEGORY::DODGE) {
			int nTypeWeapon = (int)m_typeWeapon;	//武器の種類
			nTypeWeapon++;	//種類を次に進める
			if (nTypeWeapon >= (int)WEAPONTYPE::ENUM_MAX) nTypeWeapon = 0;	//種類数の最大まで行ったら０に戻す
			//武器を変更
			ChangeWeaponType((WEAPONTYPE)nTypeWeapon);
		}

		//----------------------------
		//バックステップ回避
		//----------------------------
		if (pInput->GetTrigger(CInput::CODE::DODGE) && GetMotionCategory() == MOTION_CATEGORY::ATTACK) {
			bool bDodge = false;
			switch (m_typeWeapon)
			{
			case WEAPONTYPE::PUNCH:
				//1回目の攻撃の最後のキーの場合
				if (GetMotionType() == (int)MOTION_TYPE::PUNCH_1 && (GetCurrentKey() == 1 || GetCurrentKey() == 2)) {
					bDodge = true;
				}
				//1回目の攻撃の最後のキーの場合
				if (GetMotionType() == (int)MOTION_TYPE::PUNCH_2 && GetCurrentKey() == 2) {
					bDodge = true;
				}
				break;
			case WEAPONTYPE::SWORD:
				//1回目の攻撃の最後のキーの場合
				if (GetMotionType() == (int)MOTION_TYPE::ATTACK_SWORD_1 && GetCurrentKey() == 2) {
					bDodge = true;
				}
				//2回目の攻撃の最後のキーの場合
				else if (GetMotionType() == (int)MOTION_TYPE::ATTACK_SWORD_2 && GetCurrentKey() == 2) {
					bDodge = true;
				}
				break;
			}
			//回避
			if (bDodge) {
				m_nCntDodge = DODGE_TIME_BACK;
				//後方回避モーションの設定
				SetMotion((int)MOTION_TYPE::DODGE_BACK);
				//角度の取得
				float rotY = GetRot().y;
				//移動量を後方にする
				m_move = D3DXVECTOR3(DODGE_SPEED_BACK * sinf(rotY), -POWER_GRAVITY_GROUND, DODGE_SPEED_BACK * cosf(rotY));
				//回避音の再生
				if (pSound != nullptr)pSound->PlaySound(CSound::SOUND_LABEL::DODGE);
			}
		}

		//----------------------------
		//納刀
		//----------------------------
		if (pInput->GetTrigger(CInput::CODE::DODGE) && (GetMotionType() == (int)MOTION_TYPE::ATTACK_KATANA_1 || GetMotionType() == (int)MOTION_TYPE::ATTACK_KATANA_2) && GetCurrentKey() == 3) {
			m_nCntAttackMove = DODGE_TIME_SHEATH;
			//納刀モーションの設定
			SetMotion((int)MOTION_TYPE::SHEATH);
			//角度の取得
			float rotY = GetRot().y;
			//移動量を後方にする
			m_move = D3DXVECTOR3(DODGE_SPEED_SHEATH * sinf(rotY), -POWER_GRAVITY_GROUND, DODGE_SPEED_SHEATH * cosf(rotY));
			//回避音の再生
			if (pSound != nullptr)pSound->PlaySound(CSound::SOUND_LABEL::DODGE);
		}

		//----------------------------
		//回避
		//----------------------------
		if (GetMotionCategory() != MOTION_CATEGORY::ATTACK  && GetMotionCategory() != MOTION_CATEGORY::DODGE && !m_bLockAct) {
			if (m_bLand && pInput->GetTrigger(CInput::CODE::DODGE)) {
				m_nCntDodge = DODGE_TIME_FRONT;
				float rotY = GetRot().y;
				rotY += D3DX_PI;	//角度を前方にする
				m_move = D3DXVECTOR3(DODGE_SPEED_FRONT * sinf(rotY), -POWER_GRAVITY_GROUND, DODGE_SPEED_FRONT * cosf(rotY));	//移動量を前方にする
				SetMotion((int)MOTION_TYPE::DODGE_FRONT);	//前方回避モーション
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::DODGE);	//回避音の再生
			}
		}

		//----------------------------
		//アイテム使用
		//----------------------------
		if (m_pCounterItem != nullptr) {
			if (pInput->GetTrigger(CInput::CODE::USE_ITME) && (m_pCounterItem->GetScore() <= 0 || m_nLife >= MAX_LIFE)) {
				//アイテム使用不可音の再生
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CANSEL);
			}

			if (GetMotionCategory() != MOTION_CATEGORY::ATTACK && GetMotionCategory() != MOTION_CATEGORY::DODGE && !m_bLockAct) {
				//アイテム使用開始
				if (pInput->GetTrigger(CInput::CODE::USE_ITME) && m_pCounterItem->GetScore() > 0 && m_nLife < MAX_LIFE) {
					m_bUseItem = true;
					m_nCntUseItem = 0;
				}
			}

			//アイテム使用中
			if (m_bUseItem) {
				//アイテム使用継続
				if (pInput->GetPress(CInput::CODE::USE_ITME) && GetMotionCategory() != MOTION_CATEGORY::ATTACK && GetMotionCategory() != MOTION_CATEGORY::DODGE && !m_bLockAct) {
					if (m_nCntUseItem % SPAN_ITEM_USE_SOUND == 0) {
						//アイテムゲージ進行音の再生
						if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::GAUGE);
					}

					//カウンタの加算
					m_nCntUseItem++;

					//アイテム使用完了
					if (m_nCntUseItem >= TIME_ITEM_USE) {
						m_bUseItem = false;
						m_nCntUseItem = 0;

						//アイテムの効果
						m_nLife += HEAL_VALUE_APPLE;	//体力を回復
						if (m_nLife > MAX_LIFE) {
							m_nLife = MAX_LIFE;
						}
						if (m_pGaugeLife != nullptr) {
							//体力ゲージの割合の更新
							m_pGaugeLife->SetRatioWidth((float)m_nLife / (float)MAX_LIFE);
							//体力の色の変更
							if (m_nLife >= DANGER_LIFE) m_pGaugeLife->SetColor(COLOR_LIFE_GAUGE_SAFE);
						}

						//所有上限までアイテムを持っていた場合
						if (m_pCounterItem->GetScore() == MAX_ITEM_OWN) {
							//色の変更
							m_pCounterItem->SetColor(COLOR_ITEM_DEFAULT);
						}
						//アイテムの残量を減らす
						m_pCounterItem->AddScore(-1);
						//アイテムを使い切った場合
						if (m_pCounterItem->GetScore() == 0) {
							//色の変更
							m_pCounterItem->SetColor(COLOR_ITEM_NONE);
						}

						if (pSound != nullptr) {
							//アイテムゲージ進行音の停止
							pSound->StopSound(CSound::SOUND_LABEL::GAUGE);
							//アイテム使用音の再生
							pSound->PlaySound(CSound::SOUND_LABEL::HEAL);
						}

						//パーティクルエフェクトの生成
						D3DXVECTOR3 posParticle = *pPosPlayer;	//パーティクルの位置
						posParticle.y += 10.0f;	//位置の調整
						//パーティクルの情報
						CParticleEffect::PARTICLE_INFO particleInfo = { 30, 50.0f, -1.5f, 9.0f, D3DXVECTOR3(0.0f, -0.6f, 0.0f), D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f), D3DXCOLOR(0.8f, 1.0f, 0.2f, 1.0f) };
						//エフェクトの生成
						CParticleEffect::EmitCircle(particleInfo, posParticle, 12, 0.1f * D3DX_PI);
					}
					//アイテム使用ゲージの更新
					if (m_pGaugeUseItem != nullptr) m_pGaugeUseItem->SetRatioHeight((float)m_nCntUseItem / (float)TIME_ITEM_USE);
				}
				//アイテム使用中断
				else {
					m_bUseItem = false;
					m_nCntUseItem = 0;
					//アイテム使用ゲージの更新
					if (m_pGaugeUseItem != nullptr) m_pGaugeUseItem->SetRatioHeight(0.0f);
					//アイテムゲージ進行音の停止
					if (pSound != nullptr) pSound->StopSound(CSound::SOUND_LABEL::GAUGE);
				}
			}
		}
	}

	//----------------------------
	//移動
	//----------------------------
	//落下
	if (!m_bLand && GetMotionCategory() != MOTION_CATEGORY::DODGE) {
		m_move.y -= POWER_GRAVITY;	//重力を加算
	}

	if (pInput != nullptr && GetMotionCategory() != MOTION_CATEGORY::ATTACK && GetMotionCategory() != MOTION_CATEGORY::DODGE && !m_bLockAct) {
		//上下左右キー入力状態の取得
		const bool bPressUp = pInput->GetPress(CInput::CODE::UP);
		const bool bPressDown = pInput->GetPress(CInput::CODE::DOWN);
		const bool bPressLeft = pInput->GetPress(CInput::CODE::LEFT);
		const bool bPressRight = pInput->GetPress(CInput::CODE::RIGHT);

		bool bDiagonalMove = (bPressUp != bPressDown) && (bPressLeft != bPressRight);	//斜め移動
		bool bRotateUp, bRotateDown, bRotateLeft, bRotateRight;	//回転する方向
		bRotateUp = bRotateDown = bRotateLeft = bRotateRight = false;

		D3DXVECTOR3 moveAddSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//追加する移動量
		D3DXVECTOR3 moveMaxSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//追加する移動量の最大

		//------------------------
		//移動速度の最大を計算
		//------------------------
		float fMaxSpeed;	//移動速度の最大
		//地上
		if (pInput->GetPress(CInput::CODE::DASH)) {
			fMaxSpeed = MAX_MOVE_SPEED_DASH;
		}
		else {
			fMaxSpeed = MAX_MOVE_SPEED_WALK;
		}
		//空中
		if (!m_bLand) {
			fMaxSpeed = MAX_MOVE_SPEED_AIR;
		}

		//------------------------
		//移動量の決定
		//------------------------
		//前後移動のどちらかのみ押されている場合
		if (bPressUp != bPressDown) {
			if (bPressUp) {
				moveAddSpeed.z = ADD_MOVE_SPEED;
				moveMaxSpeed.z = fMaxSpeed;
				bRotateUp = true;
			}
			else if (bPressDown) {
				moveAddSpeed.z = -ADD_MOVE_SPEED;
				moveMaxSpeed.z = -fMaxSpeed;
				bRotateDown = true;
			}
			//斜め移動
			if (bDiagonalMove) {
				moveAddSpeed.z *= 0.7f;
				moveMaxSpeed.z *= 0.7f;
			}
		}
		//左右移動のどちらかのみ押されている場合
		if (bPressLeft != bPressRight) {
			if (bPressLeft) {
				moveAddSpeed.x = -ADD_MOVE_SPEED;
				moveMaxSpeed.x = -fMaxSpeed;
				bRotateLeft = true;
			}
			else if (bPressRight) {
				moveAddSpeed.x = ADD_MOVE_SPEED;
				moveMaxSpeed.x = fMaxSpeed;
				bRotateRight = true;
			}
			//斜め移動
			if (bDiagonalMove) {
				moveAddSpeed.x *= 0.7f;
				moveMaxSpeed.x *= 0.7f;
			}
		}

		//------------------------
		//カメラの角度に合わせて移動量の最大量を設定
		//------------------------
		D3DXVECTOR3 moveMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動の最大量
		moveMax.x += moveMaxSpeed.x * sinf(fRotCameraY + 0.5f * D3DX_PI);
		moveMax.x += moveMaxSpeed.z * sinf(fRotCameraY);

		moveMax.z += moveMaxSpeed.x * -sinf(fRotCameraY);
		moveMax.z += moveMaxSpeed.z * cosf(fRotCameraY);

		//------------------------
		//加速
		//------------------------
		//X方向の移動量が最大量未満の場合
		if ((moveMax.x > 0 && m_move.x < moveMax.x) || (moveMax.x < 0 && m_move.x > moveMax.x)) {
			//カメラの角度に合わせて移動量を追加
			m_move.x += moveAddSpeed.x * sinf(fRotCameraY + 0.5f * D3DX_PI);
			m_move.x += moveAddSpeed.z * sinf(fRotCameraY);
			//加速時に移動量が最大を超えた場合
			if (moveMax.x > 0) {
				if (m_move.x > moveMax.x) {
					m_move.x = moveMax.x;
				}
			}
			else if (moveMax.x < 0) {
				if (m_move.x < moveMax.x) {
					m_move.x = moveMax.x;
				}
			}
		}
		//Z方向の移動量が最大量未満の場合
		if ((moveMax.z > 0 && m_move.z < moveMax.z) || (moveMax.z < 0 && m_move.z > moveMax.z)) {
			//カメラの角度に合わせて移動量を追加
			m_move.z += moveAddSpeed.x * -sinf(fRotCameraY);
			m_move.z += moveAddSpeed.z * cosf(fRotCameraY);
			//加速時に移動量が最大を超えた場合
			if (moveMax.z > 0) {
				if (m_move.z > moveMax.z) {
					m_move.z = moveMax.z;
				}
			}
			else if (moveMax.z < 0) {
				if (m_move.z < moveMax.z) {
					m_move.z = moveMax.z;
				}
			}
		}

		//------------------------
		//移動量の減速
		//------------------------
		float fDecSpeed = DEC_MOVE_SPEED_GROUND;	//減速量
		if (!m_bLand) fDecSpeed = DEC_MOVE_SPEED_AIR;	//空中
		D3DXVECTOR2 vecMoveDec = D3DXVECTOR2(-m_move.x, -m_move.z);	//移動量ベクトルの逆
		D3DXVec2Normalize(&vecMoveDec, &vecMoveDec);	//正規化
		vecMoveDec *= fDecSpeed;	//移動量の加算

		//減速
		m_move.x += vecMoveDec.x;
		m_move.z += vecMoveDec.y;

		//X方向の移動量の停止
		if (vecMoveDec.x > 0.0f) {
			if (m_move.x > 0.0f) {
				m_move.x = 0.0f;
			}
		}
		else if (vecMoveDec.x < 0.0f) {
			if (m_move.x < 0.0f) {
				m_move.x = 0.0f;
			}
		}
		//Z方向の移動量の停止
		if (vecMoveDec.y > 0.0f) {
			if (m_move.z > 0.0f) {
				m_move.z = 0.0f;
			}
		}
		else if (vecMoveDec.x < 0.0f) {
			if (m_move.z < 0.0f) {
				m_move.z = 0.0f;
			}
		}

		//------------------------
		//回転方向の決定
		//------------------------		
		bool bInputMove = false;	//移動キーを押しているかどうか
		bInputMove = bRotateUp || bRotateDown || bRotateLeft || bRotateRight;	//どれかが押されている場合
		//移動キー押下中
		if (bInputMove) {
			//キーを押していた方向を目標の角度にする
			//上
			if (bRotateUp) {
				if (bRotateRight) {
					m_destRot.y = D3DX_PI * -0.75f + fRotCameraY;
				}
				else if (bRotateLeft) {
					m_destRot.y = D3DX_PI * 0.75f + fRotCameraY;
				}
				else {
					m_destRot.y = D3DX_PI + fRotCameraY;
				}
			}
			//下
			else if (bRotateDown) {
				if (bRotateRight) {
					m_destRot.y = D3DX_PI * -0.25f + fRotCameraY;
				}
				else if (bRotateLeft) {
					m_destRot.y = D3DX_PI * 0.25f + fRotCameraY;
				}
				else {
					m_destRot.y = 0.0f + fRotCameraY;
				}
			}
			//左右
			else {
				if (bRotateRight) {
					m_destRot.y = D3DX_PI * -0.5f + fRotCameraY;
				}
				else if (bRotateLeft) {
					m_destRot.y = D3DX_PI * 0.5f + fRotCameraY;
				}
			}

			//パイ超過時
			if (m_destRot.y > D3DX_PI) {
				m_destRot.y = -D3DX_PI * 2 + m_destRot.y;
			}
			else if (m_destRot.y < -D3DX_PI) {
				m_destRot.y = D3DX_PI * 2 + m_destRot.y;
			}

			D3DXVECTOR3 rotPlayer = CSceneMotion::GetRot();//角度の取得
			float fdeltaRot;	//角度の差分
			//現在の角度と目標の角度の差分の計算
			if (m_destRot.y >= 0.0f) {
				if (rotPlayer.y >= 0.0f) {
					fdeltaRot = m_destRot.y - rotPlayer.y;
				}
				else if (rotPlayer.y < 0.0f) {
					if (m_destRot.y - rotPlayer.y >= D3DX_PI) {
						fdeltaRot = -D3DX_PI - rotPlayer.y - D3DX_PI + m_destRot.y;
					}
					else if (m_destRot.y - rotPlayer.y < D3DX_PI) {
						fdeltaRot = m_destRot.y - rotPlayer.y;
					}
				}
			}
			else if (m_destRot.y < 0.0f) {
				if (rotPlayer.y >= 0.0f) {
					if (rotPlayer.y - m_destRot.y >= D3DX_PI) {
						fdeltaRot = D3DX_PI - rotPlayer.y + D3DX_PI + m_destRot.y;
					}
					else if (rotPlayer.y - m_destRot.y < D3DX_PI) {
						fdeltaRot = m_destRot.y - rotPlayer.y;
					}
				}
				else if (rotPlayer.y < 0.0f) {
					fdeltaRot = m_destRot.y - rotPlayer.y;
				}
			}

			//回転の反映
			rotPlayer.y += fdeltaRot * ROTATE_SPEED;

			//パイ超過時
			if (rotPlayer.y > D3DX_PI) {
				rotPlayer.y = -D3DX_PI;
			}
			else if (rotPlayer.y < -D3DX_PI) {
				rotPlayer.y = D3DX_PI;
			}

			//角度の設定
			CSceneMotion::SetRot(rotPlayer);
		}

		//------------------------
		//地上にいる時のモーション切り替え
		//------------------------
		if (m_bLand) {
			//移動キーを押している時のモーション切り替え
			if (bInputMove) {
				if (GetMotionCategory() != MOTION_CATEGORY::MOVE) {
					SetMotion((int)MOTION_TYPE::WALK);
				}
				//歩きモーションと走りモーションの切り替え
				if (GetMotionCategory() == MOTION_CATEGORY::MOVE) {
					if (GetMotionType() == (int)MOTION_TYPE::WALK && pInput->GetPress(CInput::CODE::DASH)) {
						SetMotion((int)MOTION_TYPE::DASH);
					}
					else if (GetMotionType() == (int)MOTION_TYPE::DASH && !pInput->GetPress(CInput::CODE::DASH)) {
						SetMotion((int)MOTION_TYPE::WALK);
					}
				}
			}
			//移動キーを押していない場合のモーション切り替え
			if (!bInputMove) {
				//モーションの設定
				if (GetMotionCategory() == MOTION_CATEGORY::MOVE) {
					SetMotion((int)MOTION_TYPE::NEUTRAL);
				}
			}
		}
	} //pInput


	//----------------------------
	//移動の反映
	//----------------------------
	//最後の位置座標の保存
	m_lastPos = *pPosPlayer;
	//移動
	*pPosPlayer += m_move;

	//----------------------------
	//当たり判定
	//----------------------------
	//アイテムとの当たり判定
	CItem::Collision(this);
	//死亡フラグが立った場合終了
	if (GetDeath()) return;

	//壁との当たり判定
	CWallCylinder::Collision(pPosPlayer, m_lastPos, COLLISION_RADIUS);

	//地面との当たり判定
	bool bLand = false;	//接地しているかどうか
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//接触位置、開始ベクトル、終了ベクトル
	vecStart = *pPosPlayer;	//プレイヤーの位置を取得
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//上向きのベクトル

	bLand = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);

	//接地時
	if (bLand) {
		*pPosPlayer = posColTerrain;	//位置の移動
		//着地
		if (!m_bLand) {
			m_bLand = true;
		}
	}
	//落下
	else if (!bLand) {
		//地上から空中
		if (m_bLand) {
			m_bLand = false;
		}
	}

	//----------------------------
	//モーションの更新
	//----------------------------
	CSceneMotion::Update();

	//----------------------------
	//モーション終了時
	//----------------------------
	if (GetEndMotion()) {
		switch ((MOTION_TYPE)GetMotionType())
		{
			//--------------------------
			//攻撃モーション
			//--------------------------
			//硬直ありのモーション
		case MOTION_TYPE::ATTACK_SWORD_1:
		case MOTION_TYPE::ATTACK_SWORD_2:
		case MOTION_TYPE::ATTACK_SWORD_3:
		case MOTION_TYPE::ATTACK_KATANA_1:
		case MOTION_TYPE::ATTACK_KATANA_2:
		case MOTION_TYPE::ATTACK_KATANA_3:
		case MOTION_TYPE::ATTACK_KATANA_DRAW:
		case MOTION_TYPE::SHEATH:
			m_bLockAct = true;	//硬直の設定

			//硬直なしのモーション
		case MOTION_TYPE::PUNCH_1:
		case MOTION_TYPE::PUNCH_2:
			SetMotion((int)MOTION_TYPE::NEUTRAL);
			//移動量を重力のみにする
			m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//武器を攻撃していない状態にする
				m_pWeapon->SetAttack(false);
			}
			break;
		}
	}

	//----------------------------
	//モーション中の処理
	//----------------------------
	int nDamageAttack = 0;		//攻撃のダメージ量
	bool bBeginAttack = false;	//攻撃開始時かどうか

	switch ((MOTION_TYPE)GetMotionType())
	{
		//-------------------------
		//パンチの攻撃
		//-------------------------
		//パンチの攻撃 1
	case MOTION_TYPE::PUNCH_1:
		//攻撃のタイミングだった場合
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 1) && !GetTransMotion()) {
			//攻撃開始時
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 1;
		}
		else {
			m_bValidAttack = false;
		}
		break;

		//パンチの攻撃 2
	case MOTION_TYPE::PUNCH_2:
		//攻撃のタイミングだった場合
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 1) && !GetTransMotion()) {
			//攻撃開始時
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 2;
		}
		else {
			m_bValidAttack = false;
		}
		break;

		//-------------------------
		//剣の攻撃
		//-------------------------
		//剣の攻撃 1
	case MOTION_TYPE::ATTACK_SWORD_1:
		//攻撃のタイミングだった場合
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 1) && !GetTransMotion()) {
			//攻撃開始時
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 2;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//武器を攻撃していない状態にする
				m_pWeapon->SetAttack(false);
			}
		}
		break;

		//剣の攻撃 2
	case MOTION_TYPE::ATTACK_SWORD_2:
		//攻撃のタイミングだった場合
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 1) && !GetTransMotion()) {
			//攻撃開始時
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 2;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//武器を攻撃していない状態にする
				m_pWeapon->SetAttack(false);
			}
		}
		break;


		//剣の攻撃 3
	case MOTION_TYPE::ATTACK_SWORD_3:
		//攻撃のタイミングだった場合
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 1 || GetCurrentKey() == 2) && !GetTransMotion()) {
			//攻撃開始時
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 5;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//武器を攻撃していない状態にする
				m_pWeapon->SetAttack(false);			
			}
		}
		break;

		//-------------------------
		//刀の攻撃
		//-------------------------
		//刀の攻撃 1
	case MOTION_TYPE::ATTACK_KATANA_1:
		//攻撃のタイミングだった場合
		if (GetCurrentKey() == 1 || GetCurrentKey() == 2) {
			//攻撃開始時
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 3;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//武器を攻撃していない状態にする
				m_pWeapon->SetAttack(false);
			}
		}
		break;
		//刀の攻撃 2
	case MOTION_TYPE::ATTACK_KATANA_2:
		//攻撃のタイミングだった場合
		if (GetCurrentKey() == 1 || GetCurrentKey() == 2) {
			//攻撃開始時
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 3;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//武器を攻撃していない状態にする
				m_pWeapon->SetAttack(false);
			}
		}
		break;
		//刀の攻撃 3
	case MOTION_TYPE::ATTACK_KATANA_3:
		//攻撃のタイミングだった場合
		if (GetCurrentKey() == 1) {
			//攻撃開始時
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 6;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//武器を攻撃していない状態にする
				m_pWeapon->SetAttack(false);
			}
		}
		break;
		//刀の攻撃 抜刀攻撃
	case MOTION_TYPE::ATTACK_KATANA_DRAW:
		//攻撃のタイミングだった場合
		if (GetCurrentKey() == 0 || GetCurrentKey() == 1 || GetCurrentKey() == 4 || GetCurrentKey() == 5) {
			//攻撃開始時
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 5;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//武器を攻撃していない状態にする
				m_pWeapon->SetAttack(false);
			}
		}
		break;

	default:
		m_bValidAttack = false;
		break;
	}

	//攻撃開始時
	if (bBeginAttack) {
		InitObjAttacked();	//攻撃済みリストの初期化
	}

	//----------------------------
	//攻撃のタイミング
	//----------------------------
	if (m_bValidAttack) {
		int nNumKillEnemy = 0;	//敵を倒した数

		//オブジェクトの全モデルのワールドマトリックスを更新
		UpdateMtxWorldAll();	//当たり判定取得時に位置のズレが生じるため

		switch (m_typeWeapon)
		{
			//パンチの攻撃
		case WEAPONTYPE::PUNCH:
		//変数宣言のために{}で囲う
		{
			const int nNumCol = 3;	//パンチの攻撃の当たり判定の数
			D3DXVECTOR3 aPosCol[nNumCol] = {};	//当たり判定の位置
			const float fRadiusCol = 10.0f;	//当たり判定の半径
			int nIdxModel = -1;	//モデルの番号
			D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//モデルからのオフセット

			//モーションによってモデルを取得
			if (GetMotionType() == (int)MOTION_TYPE::PUNCH_1) {
				nIdxModel = INDEX_MODEL_PUNCH_1;	//左手
			}
			else if (GetMotionType() == (int)MOTION_TYPE::PUNCH_2) {
				nIdxModel = INDEX_MODEL_PUNCH_2;	//右手
			}

			for (int nCntCol = 0; nCntCol < nNumCol; nCntCol++)
			{
				//当たり判定の位置の取得
				GetPosCollision(&aPosCol[nCntCol], posOffset, nIdxModel);
				//オフセットを加算
				if (nIdxModel == INDEX_MODEL_PUNCH_1) posOffset += D3DXVECTOR3(10.0f, 0.0f, 0.0f);
				if (nIdxModel == INDEX_MODEL_PUNCH_2) posOffset += D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
			}

			//当たり判定の数分攻撃する
			for (int nCntCollision = 0; nCntCollision < nNumCol; nCntCollision++)
			{
				//攻撃
				Attack(OBJ_TYPE::ENEMY, aPosCol[nCntCollision], fRadiusCol, nDamageAttack, DAMAGE_TYPE::PUNCH, &nNumKillEnemy);	//敵
				Attack(OBJ_TYPE::TREE, aPosCol[nCntCollision], fRadiusCol, nDamageAttack, DAMAGE_TYPE::PUNCH, nullptr);		//木
			}
			//攻撃の先端にパーティクルの生成
			CParticle::Create(aPosCol[nNumCol - 1], 20, 15.0f, -0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

			//攻撃開始時
			if (bBeginAttack) {
				//拳を振る音の再生
				if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_PUNCH);
			}
		}
		break;

		//剣の攻撃
		case WEAPONTYPE::SWORD:
			if (m_pWeapon != nullptr) {
				D3DXVECTOR3* pPosColArray = nullptr;	//当たり判定の位置の配列
				int nNumCol = 0;	//当たり判定の数
				const float fRadiusCol = 10.0f;	//当たり判定の半径

				//当たり判定の位置の取得
				m_pWeapon->GetPosCollision(&pPosColArray, &nNumCol);

				if (pPosColArray != nullptr && nNumCol != 0) {
					//当たり判定の数分攻撃する
					for (int nCntAttack = 0; nCntAttack < nNumCol; nCntAttack++)
					{
						//攻撃
						Attack(OBJ_TYPE::ENEMY, pPosColArray[nCntAttack], fRadiusCol, nDamageAttack, DAMAGE_TYPE::SWORD, &nNumKillEnemy);	//敵
						Attack(OBJ_TYPE::TREE, pPosColArray[nCntAttack], fRadiusCol, nDamageAttack, DAMAGE_TYPE::SWORD, nullptr);		//木
					}
					//攻撃開始時
					if (bBeginAttack) {
						//剣を振る音の再生
						if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_SWORD);
						//武器を攻撃している状態にする
						if (!m_pWeapon->GetAttack()) {
							m_pWeapon->SetAttack(true);
						}
					}
				}

				//動的確保した配列の破棄
				if (pPosColArray != nullptr) {
					delete pPosColArray;
					pPosColArray = nullptr;
				}
			}
			break;

			//刀の攻撃
		case WEAPONTYPE::KATANA:
			if (m_pWeapon != nullptr) {
				D3DXVECTOR3* pPosColArray = nullptr;	//当たり判定の位置の配列
				int nNumCol = 0;	//当たり判定の数
				const float fRadiusCol = 10.0f;	//当たり判定の半径

				//当たり判定の位置の取得
				m_pWeapon->GetPosCollision(&pPosColArray, &nNumCol);

				if (pPosColArray != nullptr && nNumCol != 0) {
					//当たり判定の数分攻撃する
					for (int nCntAttack = 0; nCntAttack < nNumCol; nCntAttack++)
					{
						//攻撃
						Attack(OBJ_TYPE::ENEMY, pPosColArray[nCntAttack], fRadiusCol, nDamageAttack, DAMAGE_TYPE::SWORD, &nNumKillEnemy);	//敵
						Attack(OBJ_TYPE::TREE, pPosColArray[nCntAttack], fRadiusCol, nDamageAttack, DAMAGE_TYPE::SWORD, nullptr);		//木
					}
					//攻撃開始時
					if (bBeginAttack) {
						//剣を振る音の再生
						if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_KATANA);
						//武器を攻撃している状態にする
						if (!m_pWeapon->GetAttack()) {
							m_pWeapon->SetAttack(true);
						}
					}
				}

				//動的確保した配列の破棄
				if (pPosColArray != nullptr) {
					delete pPosColArray;
					pPosColArray = nullptr;
				}
			}
			break;
		}

		if (nNumKillEnemy > 0 && m_nNumKillEnemy < 20) {
			//敵を倒した数の加算
			m_nNumKillEnemy += nNumKillEnemy;
		}
	}

	//----------------------------
	//ステージ移動
	//----------------------------
	if (pInput != nullptr && pGame != nullptr) {
		//筏の取得
		CSceneModel* pRaft = pGame->GetRaft();
		//インタラクトボタンのアイコンの取得
		CBillboard* pIconInteract = pGame->GetIconInteract();
		if (pRaft != nullptr) {

			D3DXVECTOR3 vecRaft = *pPosPlayer - pRaft->GetPos();	//プレイヤーから筏の位置へのポインタ
			//一定の距離以内の場合
			if (D3DXVec3Length(&vecRaft) <= DISTANCE_USE_RAFT) {
				//インタラクトボタンのアイコンを表示
				if (pIconInteract != nullptr) pIconInteract->SetDraw(true);

				//ボタン押下時
				if (pInput->GetTrigger(CInput::CODE::INTERACT)) {
					//ステージの移行
					pGame->SetFadeNextStage();
					//ニュートラルモーションの設定
					SetMotion((int)MOTION_TYPE::NEUTRAL);
					//筏の位置に移動
					SetPos(pRaft->GetPos());
					//筏の角度に合わせる
					D3DXVECTOR3 rotRaft = pRaft->GetRot();	//筏の角度を取得
					//プレイヤーのモデルの方向に修正
					rotRaft.y += D3DX_PI;
					if (rotRaft.y > D3DX_PI * 2) rotRaft.y -= D3DX_PI * 2;
					//角度を設定
					SetRot(rotRaft);
					//カメラの位置を設定
					if (pCamera != nullptr) {
						D3DXVECTOR3 posCamera = CSceneMotion::GetPos();
						posCamera.y += 50.0f;
						//カメラの位置を更新
						pCamera->SetPos(posCamera);
					}
					//インタラクトボタンのアイコンを非表示
					if (pIconInteract != nullptr) pIconInteract->SetDraw(false);
				}
			}
			else {
				//インタラクトボタンのアイコンを非表示
				if (pIconInteract != nullptr) pIconInteract->SetDraw(false);
			}
		}
	}
}

//=============================================================================
// プレイヤーの描画処理
//=============================================================================
void CPlayer::Draw(void) {
	CSceneMotion::Draw();
}


//=============================================================================
// プレイヤーのゲームクリア時の処理
//=============================================================================
void CPlayer::GameClear(void) {
	//ニュートラルモーションの設定
	SetMotion((int)MOTION_TYPE::NEUTRAL);

	//移動量を重力のみにする
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bLockAct = false;

	if (m_pWeapon != nullptr) {
		//武器を攻撃していない状態にする
		m_pWeapon->SetAttack(false);
		//輪郭非表示
		m_pWeapon->SetDrawOutline(false);
	}

	//アイテム使用中断
	m_bUseItem = false;
	m_nCntUseItem = 0;
	//アイテム使用ゲージの更新
	if (m_pGaugeUseItem != nullptr) m_pGaugeUseItem->SetRatioHeight(0.0f);
}

//=============================================================================
// プレイヤーのゲームオーバー時の処理
//=============================================================================
void CPlayer::GameOver(void) {
	if (GetMotionType() != (int)MOTION_TYPE::STUN) {
		m_bStun = true;
		m_nCntStun = 0;
		SetMotion((int)MOTION_TYPE::STUN);
	}
	//移動量を重力のみにする
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bLockAct = false;

	if (m_pWeapon != nullptr) {
		//武器を攻撃していない状態にする
		m_pWeapon->SetAttack(false);
		//輪郭非表示
		m_pWeapon->SetDrawOutline(false);
	}
}

//=============================================================================
// プレイヤーの当たり判定の情報の取得
//=============================================================================
void CPlayer::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//当たり判定の数を設定
	if (pNumCol != nullptr) *pNumCol = NUM_COLLISION;
	//当たり判定の半径を設定
	if (pRadiusCol != nullptr) *pRadiusCol = COLLISION_RADIUS;
	//当たり判定の位置の配列を設定
	if (ppPosColArray != nullptr) {
		//posのポインタが空だった場合
		if (*ppPosColArray == nullptr) {
			//posの変数を当たり判定の数分配列で確保
			*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];

			D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//モデルのposからのオフセット
			int nIdxModel = 0;	//胴体

			//当たり判定の位置の取得
			for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
			{
				GetPosCollision(*ppPosColArray + nCntCol, posOffset, nIdxModel);
				posOffset += D3DXVECTOR3(0.0f, 5.0f, 0.0f);	//オフセットを加算
			}
		}
	}
}

//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void CPlayer::Damage(int nDamage, bool* pDead) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	
	CGame* pGame = nullptr;
	CSound *pSound = nullptr;
	if (pManager != nullptr) {
		//ゲームの取得
		pGame = pManager->GetGame();
		//サウンドの取得
		pSound = pManager->GetSound();
	}

	//ゲームクリア時、ステージ変更時の場合無効
	if (pGame != nullptr) {
		if (pGame->GetGameClear() || pGame->GetChangeStage()) return;
	}
	//スタン時の場合無効
	if (m_bStun) return;

	//体力の減少
	m_nLife -= nDamage;
	if (m_nLife <= 0) m_nLife = 0;
	//体力ゲージの設定
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->SetRatioWidth((float)m_nLife / (float)MAX_LIFE);
		//体力の三割を切った場合
		if (m_nLife < DANGER_LIFE) {
			m_pGaugeLife->SetColor(COLOR_LIFE_GAUGE_DANGER);
		}
	}
	//スタン
	if (m_nLife <= 0) {
		m_bStun = true;
		m_nCntStun = 0;
		SetMotion((int)MOTION_TYPE::STUN);

		//色を戻す
		if (m_pGaugeLife != nullptr) m_pGaugeLife->SetColor(COLOR_LIFE_GAUGE_SAFE);

		//移動量を重力のみにする
		m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
		m_bLockAct = false;

		if (m_pWeapon != nullptr) {
			//武器を攻撃していない状態にする
			m_pWeapon->SetAttack(false);
		}

		//アイテム使用中断
		m_bUseItem = false;
		m_nCntUseItem = 0;
		//アイテム使用ゲージの更新
		if (m_pGaugeUseItem != nullptr) m_pGaugeUseItem->SetRatioHeight(0.0f);

		//輪郭非表示
		if (m_pWeapon != nullptr) m_pWeapon->SetDrawOutline(false);
	}
}

//=============================================================================
// プレイヤーのアイテム取得時の処理
//=============================================================================
bool CPlayer::GetItem(int nTypeItem) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	switch ((CItem::ITEM_TYPE)nTypeItem)
	{
		//リンゴ
	case CItem::ITEM_TYPE::APPLE:
		if (m_pCounterItem != nullptr) {
			//アイテムの所有上限を超過していた場合
			if (m_pCounterItem->GetScore() >= MAX_ITEM_OWN) break;
		}

		if (m_pCounterItem != nullptr) {
			//アイテムを所持していなかった場合
			if (m_pCounterItem->GetScore() == 0) {
				//色の変更
				m_pCounterItem->SetColor(COLOR_ITEM_DEFAULT);
			}
			//アイテム数の加算
			m_pCounterItem->AddScore(1);
			//上限に達した場合
			if (m_pCounterItem->GetScore() == MAX_ITEM_OWN) {
				//色の変更
				m_pCounterItem->SetColor(COLOR_ITEM_MAX);
			}
		}
		//取得音の再生
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::GET_ITEM);

		return true;
		break;
	}
	return false;
}

//=============================================================================
// プレイヤーの最後の位置座標の取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetLastPos(void) {
	return m_lastPos;
}

//=============================================================================
// プレイヤーの移動量の設定
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move) { m_move = move; }

//=============================================================================
// プレイヤーの移動量の取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void) { return m_move; }

//=============================================================================
// プレイヤーの当たり判定の半径の取得
//=============================================================================
float CPlayer::GetRadius(void) {
	return COLLISION_RADIUS;
}

//=============================================================================
// スタン状態の取得
//=============================================================================
bool CPlayer::GetStun(void) {
	return m_bStun;
}

//=============================================================================
// モーションの区分の取得
//=============================================================================
CPlayer::MOTION_CATEGORY CPlayer::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//移動
	case MOTION_TYPE::WALK:
	case MOTION_TYPE::DASH:
		return MOTION_CATEGORY::MOVE;
		break;

		//攻撃
	case MOTION_TYPE::PUNCH_1:
	case MOTION_TYPE::PUNCH_2:
	case MOTION_TYPE::ATTACK_SWORD_1:
	case MOTION_TYPE::ATTACK_SWORD_2:
	case MOTION_TYPE::ATTACK_SWORD_3:
	case MOTION_TYPE::ATTACK_KATANA_1:
	case MOTION_TYPE::ATTACK_KATANA_2:
	case MOTION_TYPE::ATTACK_KATANA_3:
	case MOTION_TYPE::ATTACK_KATANA_DRAW:
	case MOTION_TYPE::SHEATH:
		return MOTION_CATEGORY::ATTACK;
			break;

	case MOTION_TYPE::DODGE_BACK:
	case MOTION_TYPE::DODGE_FRONT:
		return MOTION_CATEGORY::DODGE;
		break;
		//その他
	default:
		return MOTION_CATEGORY::NONE;
		break;
	}
}

//=============================================================================
// 武器の変更
//=============================================================================
void CPlayer::ChangeWeaponType(WEAPONTYPE typeWeapon) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	CSound *pSound = nullptr;
	//サウンドの取得
	if (pManager != nullptr) pSound = pManager->GetSound();

	//変更する武器が同じだった場合
	if (m_typeWeapon == typeWeapon) {
		//武器変更不可音の再生
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CANSEL);
		//武器変更なしで終了
		return;
	}

	switch (typeWeapon)
	{
		//パンチ
	case WEAPONTYPE::PUNCH:
		m_typeWeapon = WEAPONTYPE::PUNCH;
		//武器の破棄
		if (m_pWeapon != nullptr) {
			m_pWeapon->Uninit();
			m_pWeapon = nullptr;
		}
		//武器変更音の再生
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CHANGE_PUNCH);
		//武器のアイコンの変更
		if (m_pIconWeapon != nullptr) {
			m_pIconWeapon->SetTexType(CTexture::TEXTURE_TYPE::ICON_NONE);
		}
		break;

		//剣
	case WEAPONTYPE::SWORD:
		m_typeWeapon = WEAPONTYPE::SWORD;
		//武器の破棄
		if (m_pWeapon != nullptr) {
			m_pWeapon->Uninit();
			m_pWeapon = nullptr;
		}
		//武器の生成
		if (m_nIdxWeapon >= 0 && m_nIdxWeapon < m_nNumParts) {
			m_pWeapon = CSword::Create(D3DXVECTOR3(-10.0f, 0.0f, 5.0f), D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), GetPtrModel(m_nIdxWeapon));
		}
		//武器変更音の再生
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CHANGE_SWORD);
		//武器のアイコン
		if (m_pIconWeapon != nullptr) {
			m_pIconWeapon->SetTexType(CTexture::TEXTURE_TYPE::ICON_SWORD);
		}
		break;

		//刀
	case WEAPONTYPE::KATANA:
		m_typeWeapon = WEAPONTYPE::KATANA;
		//武器の破棄
		if (m_pWeapon != nullptr) {
			m_pWeapon->Uninit();
			m_pWeapon = nullptr;
		}
		//武器の生成
		if (m_nIdxWeapon >= 0 && m_nIdxWeapon < m_nNumParts) {
			m_pWeapon = CKatana::Create(D3DXVECTOR3(-15.0f, 0.0f, 10.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, D3DX_PI * -0.5f), GetPtrModel(m_nIdxWeapon));
		}
		//武器変更音の再生
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CHANGE_KATANA);
		//武器のアイコン
		if (m_pIconWeapon != nullptr) {
			m_pIconWeapon->SetTexType(CTexture::TEXTURE_TYPE::ICON_KATANA);
		}
		break;
	}
}