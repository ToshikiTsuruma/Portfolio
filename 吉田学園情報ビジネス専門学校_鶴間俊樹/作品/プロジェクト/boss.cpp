//=============================================================================
//
// ボス処理 [boss.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "boss.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "wallCylinder.h"
#include "terrain.h"
#include "effect.h"
#include "particle.h"
#include "scene2D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_boss.txt"
#define COLLISION_RADIUS (150.0f)	//当たり判定の半径 壁等で使用
#define MOVE_SPEED (3.0f)				//移動速度
#define MOVE_SPEED_RUSH (4.5f)			//突進攻撃時の移動速度
#define ROTATE_SPEED (0.02f * D3DX_PI)	//回転速度
#define DISTANCE_ENCOUNTER_PLAYER (700.0f)	//プレイヤーとエンカウントする距離
#define MAX_LIFE (80)					//体力の最大値
#define COLOR_LIFE_GAUGE (D3DXCOLOR(0.6f, 0.0f, 1.0f, 1.0f))	//体力バーの安全時の色
#define ANGLE_ATTACK_FRONT (D3DX_PI * 0.2f)	//前方攻撃が可能な角度
#define DISTANCE_ATTACK_FRONT (180.0f)		//前方攻撃する距離
#define ANGLE_ATTACK_UNDER (D3DX_PI * 1.1f)	//下攻撃が可能な角度
#define DISTANCE_ATTACK_UNDER (100.0f)		//下攻撃する距離
#define DISTANCE_ATTACK_UNDER_ALL (40.0f)	//角度に関係なく下攻撃できる距離
#define MIN_SPAN_RUSH_ATTACK (3)	//突進攻撃が可能になる通常攻撃の回数の最小値
#define MAX_SPAN_RUSH_ATTACK (5)	//突進攻撃が可能になる通常攻撃の回数の最大値
#define RANDOM_RUSH_ATTACK (5)		//突進攻撃が可能になる確率（分母）
#define MAX_CONTINUE_RUSH_ATTACK (3)	//突進攻撃を連続で行う最大数
#define DISTANCE_STAY (600.0f)	//待機する距離
#define FALLDOWN_TIME (100)	//死亡時の倒れる時間
#define DEAD_TIME (240)		//死亡後消えるまでの時間
#define COLOR_OUTLINE (D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f))	//モデルの輪郭の色

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CBoss::m_nNumParts = 0;
CSceneMotion::PARTS_INFO* CBoss::m_pPartsInfoArray = NULL;
CSceneMotion::MOTION_INFO CBoss::m_aMotionInfo[(int)CBoss::MOTION_TYPE::ENUM_MAX] = {};
int CBoss::m_nIdxWeapon = -1;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CBoss::CBoss() : CSceneMotion(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, true)
{
	SetObjType(OBJ_TYPE::ENEMY);	//オブジェクトタイプの設定
	SetUpdatePriority(UPDATE_PRIORITY::ENEMY);	//更新順の設定
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//描画順の設定

	SetColorOutlineAll(COLOR_OUTLINE);	//輪郭の色の設定

	m_lastPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_rotDestY = 0.0f;
	m_bEncounterPlayer = false;
	m_bStay = false;
	m_bRotateAttack = false;
	m_bValidAttack = false;
	m_nCntNormalAttack = 0;
	m_nCntAttackRush = 0;
	m_bLockAct = false;
	m_nLife = MAX_LIFE;
	m_bDead = false;
	m_nCntDead = 0;

	//体力ゲージの生成
	m_pGaugeLife = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 862.5f, 20.0f);	//ゲージ
	m_pGaugeLifeBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 862.5f, 20.0f);	//背景
	m_pGaugeLifeFrame = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::GAUGE_LIFE_BOSS, 900.0f, 25.0f);	//枠
	//体力ゲージの設定 順番厳守
	if (m_pGaugeLifeBg != NULL) {	//背景
		m_pGaugeLifeBg->SetDrawPriority(DRAW_PRIORITY::UI);
		m_pGaugeLifeBg->SetColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
	}
	if (m_pGaugeLife != NULL) {	//ゲージ
		m_pGaugeLife->SetDrawPriority(DRAW_PRIORITY::UI);
		m_pGaugeLife->SetColor(COLOR_LIFE_GAUGE);
	}
	if (m_pGaugeLifeFrame != NULL) {	//枠
		m_pGaugeLifeFrame->SetDrawPriority(DRAW_PRIORITY::UI);
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CBoss::~CBoss()
{
	//体力ゲージの背景の破棄
	if (m_pGaugeLifeBg != nullptr) {
		m_pGaugeLifeBg->Uninit();
		m_pGaugeLifeBg = nullptr;
	}
	//体力ゲージの破棄
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}
	//体力ゲージの枠の破棄
	if (m_pGaugeLifeFrame != nullptr) {
		m_pGaugeLifeFrame->Uninit();
		m_pGaugeLifeFrame = nullptr;
	}
}

//=============================================================================
// プレイヤーの生成処理
//=============================================================================
CBoss* CBoss::Create(D3DXVECTOR3 pos) {
	CBoss* pBoss;
	pBoss = new CBoss();
	if (pBoss != NULL) {
		pBoss->CSceneMotion::SetPos(pos);
		pBoss->Init();
	}

	return pBoss;
}

//=============================================================================
// モーション情報をロード
//=============================================================================
void CBoss::LoadMotionInfo(void) {
	//ロードする前に動的確保したメモリをdeleteする
	UnloadMotionInfo();

	Load(TEXT_FILE_NAME_LOAD_MOTION, &m_pPartsInfoArray, &m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, &m_nIdxWeapon);
}

//=============================================================================
// モーション情報をアンロード
//=============================================================================
void CBoss::UnloadMotionInfo(void) {
	m_nNumParts = 0;
	m_nIdxWeapon = -1;

	//パーツ情報の解放
	if (m_pPartsInfoArray != NULL) {
		delete[] m_pPartsInfoArray;
		m_pPartsInfoArray = NULL;
	}
	//モーション情報の解放
	for (int nCntMotionInfo = 0; nCntMotionInfo < (int)MOTION_TYPE::ENUM_MAX; nCntMotionInfo++)
	{
		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
		{
			if (m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray != NULL) {
				delete[] m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray;
				m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray = NULL;
			}
		}
	}
}

//=============================================================================
// ボスの初期化処理
//=============================================================================
HRESULT CBoss::Init(void) {
	//ボスの初期設定
	CSceneMotion::SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_rotDestY = 0.0f;	//前向き
	m_lastPos = GetPos();

	SetMotion((int)MOTION_TYPE::WAIT);	//モーションの初期設定
	CSceneMotion::Init();

	return S_OK;
}

//=============================================================================
// ボスの終了処理
//=============================================================================
void CBoss::Uninit(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//ゲームの取得
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//ゲームが保持しているボスのポインタをNULLにする
	if (pGame != nullptr) pGame->ReleaseBoss();
	//終了処理
	CSceneMotion::Uninit();
}

//=============================================================================
// ボスの更新処理
//=============================================================================
void CBoss::Update(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//ゲームの取得
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//プレイヤーの取得
	CPlayer* pPlayer = nullptr;
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	D3DXVECTOR3 posPlayer;	//プレイヤーの位置
	bool bStunPlayer = false;	//プレイヤーがスタン中か

	//プレイヤーの位置の取得
	if (pPlayer != nullptr) {
		posPlayer = pPlayer->GetPos();
		//プレイヤーのスタン状態の取得
		bStunPlayer = pPlayer->GetStun();
		//プレイヤーがスタンしたら通常攻撃のカウントをリセット
		if (bStunPlayer == true) m_nCntNormalAttack = 0;
	}

	//スタン終了で待機終了
	if (m_bStay == true) m_bStay = bStunPlayer;

	//敵とプレイヤーの距離を求める
	D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);	//敵からプレイヤーへのベクトル
	float fDistPlayer = D3DXVec2Length(&vecPlayer);	//敵からプレイヤーへの距離
	//敵からプレイヤーへの角度
	float fRotPlayerY = (float)atan2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);

	//----------------------------
	//死亡時
	//----------------------------
	if (m_bDead == true) {
		m_nCntDead++;

		if (m_nCntDead == FALLDOWN_TIME) {
			//倒れる音
			if (pSound != NULL)pSound->PlaySound(CSound::SOUND_LABEL::STUN);
		}

		//エネミーの破棄
		if (m_nCntDead > DEAD_TIME) {
			//フェードアウト
			bool bEndFade = false;
			bEndFade = FadeModelAll(0.0f, -0.01f);
			//フェード終了時
			if (bEndFade == true) {
				//ゲームクリア
				if (pGame != nullptr) pGame->GameClear();
				//終了処理
				Uninit();
				return;
			}
		}

		//モーションの更新
		CSceneMotion::Update();
		return;
	}

	//----------------------------
	//プレイヤー発見時
	//----------------------------
	if (m_bEncounterPlayer == false) {
		//待機モーション時にプレイヤーが一定以上近づいた場合
		if (fDistPlayer < DISTANCE_ENCOUNTER_PLAYER && GetMotionType() == (int)MOTION_TYPE::WAIT) {
			//ニュートラルモーションの設定
			SetMotion((int)MOTION_TYPE::NEUTRAL);

			if (pSound != nullptr) {
				//曲の停止
				pSound->StopSound(pSound->GetBGM());
				//ボス曲の再生
				pSound->PlaySound(CSound::SOUND_LABEL::BGM_GAME_BOSS);
				pSound->SetBGM(CSound::SOUND_LABEL::BGM_GAME_BOSS);
			}
		}
		//待機モーションではなかった場合（ニュートラルモーション）
		if (GetMotionType() != (int)MOTION_TYPE::WAIT) {
			//モーションの遷移が終わっていたら
			if (GetTransMotion() == false) {
				m_bEncounterPlayer = true;
			}
		}
	}

	//----------------------------
	//硬直中
	//----------------------------
	bool bAttackRush = false;	//突進攻撃が攻撃可能かどうか

	if (m_bLockAct == true) {
		//モーションの遷移が完了した場合
		if (GetTransMotion() == false) {
			//硬直の解除
			m_bLockAct = false;

			//通常攻撃を一定回数以上行っていた場合
			if (m_nCntNormalAttack >= MAX_SPAN_RUSH_ATTACK) {
				bAttackRush = true;
			}
			else if (m_nCntNormalAttack >= MIN_SPAN_RUSH_ATTACK) {
				bAttackRush = (rand() % RANDOM_RUSH_ATTACK) == 0;	//確率で突進攻撃
			}
		}
	}

	//----------------------------
	//攻撃
	//----------------------------
	bool bAngleAttackFront = false;	//前方攻撃できる角度にいるかどうか
	bool bAngleAttackUnder = false;	//下攻撃できる角度にいるかどうか

	//硬直中ではない場合
	if (m_bLockAct == false && bStunPlayer == false) {
		//--------------------------
		//プレイヤーとの角度を計算
		//--------------------------
		float fRotY = GetRot().y;	//敵のY軸の角度

		//敵の角度とモデルの前方の向きが逆なので調整する
		if (fRotY < 0.0f) {
			fRotY += D3DX_PI;
		}
		else if (fRotY >= 0.0f) {
			fRotY -= D3DX_PI;
		}

		//角度を正の値にする(0から2π)
		if (fRotY < 0.0f) fRotY += D3DX_PI * 2;
		if (fRotPlayerY < 0.0f) fRotPlayerY += D3DX_PI * 2;

		//プレイヤーが前方攻撃できる角度にいた場合
		if (fabsf(fRotPlayerY - fRotY) < ANGLE_ATTACK_FRONT / 2.0f || fabsf(fRotPlayerY - fRotY) > (D3DX_PI * 2) - ANGLE_ATTACK_FRONT / 2.0f) {
			bAngleAttackFront = true;
		}
		//プレイヤーが下攻撃できる角度にいた場合
		if (fabsf(fRotPlayerY - fRotY) < ANGLE_ATTACK_UNDER / 2.0f || fabsf(fRotPlayerY - fRotY) > (D3DX_PI * 2) - ANGLE_ATTACK_UNDER / 2.0f) {
			bAngleAttackUnder = true;
		}

		//--------------------------
		//攻撃中ではない場合攻撃可能
		//--------------------------
		if (GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
			//突進攻撃
			if (bAttackRush == true) {
				m_nCntAttackRush = 0;
				StartAttack(ATTACK_TYPE::RUSH);
			}
			//前方攻撃
			else if (bAngleAttackFront == true && fDistPlayer < DISTANCE_ATTACK_FRONT && fDistPlayer >= DISTANCE_ATTACK_UNDER) {
				StartAttack(ATTACK_TYPE::FRONT);
			}
			//下攻撃できる角度かつ一定の距離以内。または角度関係なく攻撃できる一定の距離以内
			else if ((bAngleAttackUnder == true && fDistPlayer < DISTANCE_ATTACK_UNDER) || fDistPlayer < DISTANCE_ATTACK_UNDER_ALL) {
				StartAttack(ATTACK_TYPE::UNDER);
			}
		}
	}

	//----------------------------
	//目標角度と移動量の設定
	//----------------------------
	if (m_bLockAct == false && m_bEncounterPlayer == true) {
		//スタン中に一定以上離れた場合待機
		if (m_bStay == false && bStunPlayer == true && fDistPlayer > DISTANCE_STAY) {
			//ニュートラルモーションモーションを設定
			SetMotion((int)MOTION_TYPE::NEUTRAL);
			//移動量の設定
			m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			//プレイヤー待機状態にする
			m_bStay = true;
		}
		//攻撃モーション中ではない場合で、待機中でなはい場合	突進攻撃時は可能
		if ((GetMotionCategory() != MOTION_CATEGORY::ATTACK || GetMotionType() == (int)MOTION_TYPE::ATTACK_RUSH) && m_bStay == false) {
			//移動モーション中ではなかったら
			if (GetMotionCategory() != MOTION_CATEGORY::MOVE && GetMotionType() != (int)MOTION_TYPE::ATTACK_RUSH) {
				//移動時のモーションを設定
				SetMotion((int)MOTION_TYPE::MOVE);
			}
			//目標角度と今の角度の差分を求める
			float fRotY = GetRot().y;	//現在の角度

			float fSpeedMove = MOVE_SPEED;	//移動速度
			//突進攻撃時
			if (GetMotionType() == (int)MOTION_TYPE::ATTACK_RUSH) fSpeedMove = MOVE_SPEED_RUSH;
			//移動量の設定
			m_move = D3DXVECTOR3(fSpeedMove * sinf(fRotY + D3DX_PI), m_move.y, fSpeedMove * cosf(fRotY + D3DX_PI));
		}

		//攻撃モーション中ではないか、攻撃中に回転できる場合
		if (GetMotionCategory() != MOTION_CATEGORY::ATTACK || m_bRotateAttack == true) {
			//目標角度をプレイヤーの向きへ向ける（逆向き）
			m_rotDestY = fRotPlayerY;
			//プレイヤーがスタン中ではない場合か、スタン中に待機が完了した場合正しい向きに治す
			if (bStunPlayer == false || (bStunPlayer == true && m_bStay == true)) {
				//モデルが反対なので反対向きに向ける
				if (m_rotDestY >= 0.0f) {
					m_rotDestY -= D3DX_PI;
				}
				else if (m_rotDestY < 0.0f) {
					m_rotDestY += D3DX_PI;
				}
			}
		}
	}

	//----------------------------
	//回転
	//----------------------------
	//硬直中ではないとき
	if (m_bLockAct == false && m_bEncounterPlayer == true) {
		//目標角度と角度が不一致の場合
		if (m_rotDestY != GetRot().y) {
			//攻撃モーション中ではない場合で、待機中でなはい場合
			if (GetMotionCategory() != MOTION_CATEGORY::ATTACK && m_bStay == false) {
				//移動モーションではなかったら
				if (GetMotionCategory() != MOTION_CATEGORY::MOVE) {
					//移動時のモーションを設定
					SetMotion((int)MOTION_TYPE::MOVE);
				}
			}

			//攻撃モーション中ではないか、攻撃モーション中に回転できる場合
			if (GetMotionCategory() != MOTION_CATEGORY::ATTACK || m_bRotateAttack == true) {
				//目標角度へ回転させる
				D3DXVECTOR3 rot = GetRot();	//角度の取得
				float fRotY = rot.y;	//Y軸の角度

				//目標角度がマイナス、角度がプラスのときに、減算した結果が-πを超えた場合
				if (m_rotDestY - fRotY < -D3DX_PI) {
					//加算
					rot.y += ROTATE_SPEED;
				}
				//目標角度がプラス、角度がマイナスのときに、減算した結果がπを超えた場合
				else if (m_rotDestY - fRotY > D3DX_PI) {
					//減算
					rot.y -= ROTATE_SPEED;
				}
				//個々の正負は関係ない場合
				else if (m_rotDestY - fRotY > 0) {
					//加算
					rot.y += ROTATE_SPEED;
					//目標角度超過時の調整
					if (rot.y > m_rotDestY) {
						rot.y = m_rotDestY;
					}
				}
				else if (m_rotDestY - fRotY < 0) {
					//減算
					rot.y -= ROTATE_SPEED;
					//目標角度超過時の調整
					if (rot.y < m_rotDestY) {
						rot.y = m_rotDestY;
					}
				}

				//π超過時の調整
				if (rot.y > D3DX_PI) {
					rot.y -= D3DX_PI * 2;
				}
				else if (rot.y < -D3DX_PI) {
					rot.y += D3DX_PI * 2;
				}

				//角度の設定
				SetRot(rot);
			}
		}
	}

	//----------------------------
	//移動
	//----------------------------

	m_lastPos = GetPos();
	//位置の設定
	SetPos(GetPos() + m_move);

	//----------------------------
	//当たり判定
	//----------------------------
	//壁との当たり判定
	CWallCylinder::Collision(GetPtrPos(), m_lastPos, COLLISION_RADIUS);

	//地面との当たり判定
	bool bLand = false;	//接地しているかどうか
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//接触位置、開始ベクトル、終了ベクトル
	vecStart = GetPos();	//敵の位置を取得
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//上向きのベクトル

	bLand = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);
	//接地時
	if (bLand == true) {
		SetPos(posColTerrain);	//位置の移動
	}

	//----------------------------
	//モーションの更新
	//----------------------------
	CSceneMotion::Update();

	//----------------------------
	//モーション終了時
	//----------------------------
	if (GetEndMotion() == true) {
		switch ((MOTION_TYPE)GetMotionType())
		{
			//前方攻撃
		case MOTION_TYPE::ATTACK_FRONT:
			m_bValidAttack = false;

			//下攻撃できる角度かつ一定の距離以内。または角度関係なく攻撃できる一定の距離以内
			if ((bAngleAttackUnder == true && fDistPlayer < DISTANCE_ATTACK_UNDER) || fDistPlayer < DISTANCE_ATTACK_UNDER_ALL) {
				//下攻撃
				StartAttack(ATTACK_TYPE::UNDER);
			}
			else {
				//攻撃の終了
				SetMotion((int)MOTION_TYPE::NEUTRAL);
				m_bLockAct = true;
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			}
			break;

			//下攻撃
		case MOTION_TYPE::ATTACK_UNDER:
			//攻撃の終了
			SetMotion((int)MOTION_TYPE::NEUTRAL);
			m_bLockAct = true;
			m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			break;

			//突進攻撃
		case MOTION_TYPE::ATTACK_RUSH:
			//一定の回数連続で攻撃する
			if (m_nCntAttackRush < MAX_CONTINUE_RUSH_ATTACK && bStunPlayer == false) {
				//突進攻撃
				StartAttack(ATTACK_TYPE::RUSH);
			}
			else {
				//攻撃の終了
				SetMotion((int)MOTION_TYPE::NEUTRAL);
				m_bLockAct = true;
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
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
		//前方攻撃
	case MOTION_TYPE::ATTACK_FRONT:
		//攻撃のタイミングだった場合
		if ((GetCurrentKey() == 1 || GetCurrentKey() == 2) && GetTransMotion() == false) {
			//攻撃開始時
			if (m_bValidAttack == false) {
				m_bRotateAttack = false;	//回転を制限
				bBeginAttack = true;	//攻撃開始
			}
			m_bValidAttack = true;
			nDamageAttack = 3;
		}
		else {
			m_bValidAttack = false;
		}
		break;

		//下攻撃
	case MOTION_TYPE::ATTACK_UNDER:
		//攻撃のタイミングだった場合
		if ((GetCurrentKey() == 1) && GetTransMotion() == false) {
			//攻撃開始時
			if (m_bValidAttack == false) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 3;
		}
		else {
			m_bValidAttack = false;
		}
		break;

		//突進攻撃
	case MOTION_TYPE::ATTACK_RUSH:
		//攻撃のタイミングだった場合
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 2) && GetTransMotion() == false) {
			//攻撃開始時
			if (m_bValidAttack == false) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 3;
		}
		else {
			m_bValidAttack = false;
		}
		break;

	default:
		m_bValidAttack = false;
		break;
	}

	//攻撃開始時
	if (bBeginAttack == true) {
		InitObjAttacked();	//攻撃済みリストの初期化
	}

	//----------------------------
	//攻撃のタイミング
	//----------------------------
	if (m_bValidAttack == true) {
		//オブジェクトの全モデルのワールドマトリックスを更新
		UpdateMtxWorldAll();	//当たり判定取得時に位置のズレが生じるため

		//モーションごとに当たり判定を取得
		switch ((MOTION_TYPE)GetMotionType())
		{
			//前方攻撃
		case MOTION_TYPE::ATTACK_FRONT:
		{
			const int nNumColArmR = 6;	//右手の当たり判定の数
			const int nNumColArmL = 6;	//左手の当たり判定の数
			const int nNumColAll = nNumColArmR + nNumColArmL;	//攻撃の当たり判定の数
			D3DXVECTOR3 aPosCol[nNumColAll] = {};	//当たり判定の位置

			//右手の当たり判定の取得
			for (int nCntCol = 0; nCntCol < nNumColArmR; nCntCol++)
			{
				//当たり判定の位置の取得
				GetPosCollision(&aPosCol[nCntCol], D3DXVECTOR3(-20.0f * nCntCol, 0.0f, 0.0f), (int)PARTS_TYPE::HAND_RIGHT);
			}
			//左手の当たり判定の取得
			for (int nCntCol = 0; nCntCol < nNumColArmL; nCntCol++)
			{
				//当たり判定の位置の取得
				GetPosCollision(&aPosCol[nCntCol + nNumColArmR], D3DXVECTOR3(20.0f * nCntCol, 0.0f, 0.0f), (int)PARTS_TYPE::HAND_LEFT);
			}

			//当たり判定の数分攻撃する
			for (int nCntAttack = 0; nCntAttack < nNumColAll; nCntAttack++)
			{
				//攻撃
				Attack(OBJ_TYPE::PLAYER, aPosCol[nCntAttack], 40.0f, nDamageAttack, DAMAGE_TYPE::ENEMY, NULL);
			}

			//攻撃開始時
			if (bBeginAttack == true) {
				//攻撃音の再生
				if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::ATTACK_BOSS);
			}
		}
			break;

			//下攻撃
		case MOTION_TYPE::ATTACK_UNDER:
		{
			const int nNumColFootR = 6;	//右足の当たり判定の数
			const int nNumColFootL = 6;	//左足の当たり判定の数
			const int nNumColAll = nNumColFootR + nNumColFootL;	//攻撃の当たり判定の数
			D3DXVECTOR3 aPosCol[nNumColAll] = {};	//当たり判定の位置

			//右足の当たり判定の取得
			for (int nCntCol = 0; nCntCol < nNumColFootR; nCntCol++)
			{
				//当たり判定の位置の取得
				GetPosCollision(&aPosCol[nCntCol], D3DXVECTOR3(0.0f, -15.0f * nCntCol, 0.0f), (int)PARTS_TYPE::FOOT_RIGHT);
			}
			//左足の当たり判定の取得
			for (int nCntCol = 0; nCntCol < nNumColFootL; nCntCol++)
			{
				//当たり判定の位置の取得
				GetPosCollision(&aPosCol[nCntCol + nNumColFootR], D3DXVECTOR3(0.0f, -15.0f * nCntCol, 0.0f), (int)PARTS_TYPE::FOOT_LEFT);
			}

			//当たり判定の数分攻撃する
			for (int nCntAttack = 0; nCntAttack < nNumColAll; nCntAttack++)
			{
				//攻撃
				Attack(OBJ_TYPE::PLAYER, aPosCol[nCntAttack], 60.0f, nDamageAttack, DAMAGE_TYPE::ENEMY, NULL);
			}

			//攻撃開始時
			if (bBeginAttack == true) {
				//攻撃音の再生
				if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::ATTACK_BOSS);
			}
		}
			break;

			//突進攻撃
		case MOTION_TYPE::ATTACK_RUSH:
		{
			const int nNumColHand = 6;	//手の当たり判定の数
			const int nNumColFoot = 6;	//足の当たり判定の数
			const int nNumColAll = nNumColHand + nNumColFoot;	//攻撃の当たり判定の数
			D3DXVECTOR3 aPosCol[nNumColAll] = {};	//当たり判定の位置

			//取得パーツと位置のオフセットの決定
			int nIdxPartsHand;	//手のモデルのインデックス
			int nIdxPartsFoot;	//足のモデルのインデックス
			D3DXVECTOR3 posOffsetHand;
			D3DXVECTOR3 posOffsetFoot;

			//右手と左足の攻撃
			if (GetCurrentKey() == 0) {
				//インデックスの決定
				nIdxPartsHand = (int)PARTS_TYPE::HAND_RIGHT;
				nIdxPartsFoot = (int)PARTS_TYPE::FOOT_LEFT;

				//オフセットの決定
				posOffsetHand = D3DXVECTOR3(-15.0f, 0.0f, 0.0f);
				posOffsetFoot = D3DXVECTOR3(0.0f, -15.0f, 0.0f);
			}
			//左手と右足の攻撃
			else if (GetCurrentKey() == 2) {
				//インデックスの決定
				nIdxPartsHand = (int)PARTS_TYPE::HAND_LEFT;
				nIdxPartsFoot = (int)PARTS_TYPE::FOOT_RIGHT;

				//オフセットの決定
				posOffsetHand = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
				posOffsetFoot = D3DXVECTOR3(0.0f, -15.0f, 0.0f);
			}

			//手の当たり判定の取得
			for (int nCntCol = 0; nCntCol < nNumColHand; nCntCol++)
			{
				//当たり判定の位置の取得
				GetPosCollision(&aPosCol[nCntCol], posOffsetHand * (float)nCntCol, nIdxPartsHand);
			}
			//足の当たり判定の取得
			for (int nCntCol = 0; nCntCol < nNumColFoot; nCntCol++)
			{
				//当たり判定の位置の取得
				GetPosCollision(&aPosCol[nCntCol + nNumColHand], posOffsetFoot * (float)nCntCol, nIdxPartsFoot);
			}

			float fSizeCol = 50.0f;	//当たり判定のサイズ
			//当たり判定の数分攻撃する
			for (int nCntAttack = 0; nCntAttack < nNumColAll; nCntAttack++)
			{
				if (nCntAttack >= nNumColHand) fSizeCol = 60.0f;	//足の当たり判定のサイズは少し大きくする
				//攻撃
				Attack(OBJ_TYPE::PLAYER, aPosCol[nCntAttack], fSizeCol, nDamageAttack, DAMAGE_TYPE::ENEMY, NULL);
			}

			//攻撃開始時
			if (bBeginAttack == true) {
				//攻撃音の再生
				if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::ATTACK_BOSS);
			}
		}
		break;
		}
	}
}

//=============================================================================
// ボスの描画処理
//=============================================================================
void CBoss::Draw(void) {
	CSceneMotion::Draw();
}

//=============================================================================
// ボスの当たり判定の情報の数の取得
//=============================================================================
int CBoss::GetNumCollisionParts(void) {
	return (int)PARTS_TYPE::ENUM_MAX;
}

//=============================================================================
// ボスの当たり判定の情報の取得
//=============================================================================
void CBoss::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	int nNumCol = 0;	//当たり判定の数
	float fRadiusCol = 0.0f;	//当たり判定の半径
	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//モデルのposからのオフセット
	D3DXVECTOR3 posOffsetAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//オフセットへの加算量
	int nIdxModel = -1;	//モデルの番号

	switch ((PARTS_TYPE)nIdxColParts)
	{
		//胴体の当たり判定
	case PARTS_TYPE::BODY:
		nNumCol = 5;
		fRadiusCol = 40.0f;
		posOffset = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(0.0f, 15.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::BODY;
		break;

		//右腕の当たり判定
	case PARTS_TYPE::ARM_RIGHT:
		nNumCol = 5;
		fRadiusCol = 20.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::ARM_RIGHT;
		break;
		//右手の当たり判定
	case PARTS_TYPE::HAND_RIGHT:
		nNumCol = 6;
		fRadiusCol = 30.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::HAND_RIGHT;
		break;

		//左腕の当たり判定
	case PARTS_TYPE::ARM_LEFT:
		nNumCol = 5;
		fRadiusCol = 20.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::ARM_LEFT;
		break;
		//左手の当たり判定
	case PARTS_TYPE::HAND_LEFT:
		nNumCol = 6;
		fRadiusCol = 30.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::HAND_LEFT;
		break;

		//右腿の当たり判定
	case PARTS_TYPE::LEG_RIGHT:
		nNumCol = 5;
		fRadiusCol = 20.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::LEG_RIGHT;
		break;
		//右足の当たり判定
	case PARTS_TYPE::FOOT_RIGHT:
		nNumCol = 7;
		fRadiusCol = 30.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::FOOT_RIGHT;
		break;

		//左腿の当たり判定
	case PARTS_TYPE::LEG_LEFT:
		nNumCol = 5;
		fRadiusCol = 20.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::LEG_LEFT;
		break;
		//左足の当たり判定
	case PARTS_TYPE::FOOT_LEFT:
		nNumCol = 7;
		fRadiusCol = 30.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::FOOT_LEFT;
		break;

		//その他
	default:
		//終了
		return;
		break;
	}

	//当たり判定の数を設定
	if (pNumCol != NULL) *pNumCol = nNumCol;
	//当たり判定の半径を設定
	if (pRadiusCol != NULL) *pRadiusCol = fRadiusCol;

	//当たり判定の位置の配列を設定
	if (ppPosColArray != NULL) {
		//posのポインタが空だった場合
		if (*ppPosColArray == NULL) {
			//posの変数を当たり判定の数分配列で確保
			*ppPosColArray = new D3DXVECTOR3[nNumCol];

			//当たり判定の位置の取得
			for (int nCntCol = 0; nCntCol < nNumCol; nCntCol++)
			{
				GetPosCollision(*ppPosColArray + nCntCol, posOffset, nIdxModel);
				posOffset += posOffsetAdd;	//オフセットを加算
			}
		}
	}
}

//=============================================================================
// ボスの攻撃開始時の処理
//=============================================================================
void CBoss::StartAttack(ATTACK_TYPE type) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	switch ((ATTACK_TYPE)type)
	{
		//前攻撃
	case ATTACK_TYPE::FRONT:
		//攻撃モーションの設定
		SetMotion((int)MOTION_TYPE::ATTACK_FRONT);
		//攻撃準備中は回転できる
		m_bRotateAttack = true;
		//振り上げ音の再生
		if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_PUNCH);
		//通常攻撃のカウンタの加算
		m_nCntNormalAttack++;
		break;

		//下攻撃
	case ATTACK_TYPE::UNDER:
		//攻撃モーションの設定
		SetMotion((int)MOTION_TYPE::ATTACK_UNDER);
		//攻撃中回転できない
		m_bRotateAttack = false;
		//振り上げ音の再生
		if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_PUNCH);
		//通常攻撃のカウンタの加算
		m_nCntNormalAttack++;
		break;

		//突進攻撃
	case ATTACK_TYPE::RUSH:
		//攻撃モーションの設定
		SetMotion((int)MOTION_TYPE::ATTACK_RUSH);
		//攻撃中回転できる
		m_bRotateAttack = true;
		//通常攻撃のカウンタのリセット
		m_nCntNormalAttack = 0;
		//突進攻撃の連続回数
		m_nCntAttackRush++;
		break;

		//その他
	default:
		return;	//何もせず終了
		break;
	}

	//以下共通の処理
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
}

//=============================================================================
// ボスのダメージ処理
//=============================================================================
void CBoss::Damage(int nDamage, bool* pDead) {
	if (m_bDead == true) return;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//ゲームの取得
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();

	//ライフの減少
	m_nLife -= nDamage;
	//体力ゲージの設定
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->SetRatioWidth((float)m_nLife / (float)MAX_LIFE);
	}
	//死亡
	if (m_nLife <= 0) {
		//死亡の設定
		m_bDead = true;
		//死亡カウントの初期化
		m_nCntDead = 0;
		//当たり判定を無効
		SetEnableCollision(false);
		//死亡モーションの設定
		SetMotion((int)MOTION_TYPE::DEAD);
		//輪郭を非表示
		SetDrawOutlineAll(false);
		//ボス死亡時の処理
		if (pGame != nullptr) pGame->DeadBoss();
	}
}

//=============================================================================
// モーションの区分の取得
//=============================================================================
CBoss::MOTION_CATEGORY CBoss::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//移動
	case MOTION_TYPE::MOVE:
		return MOTION_CATEGORY::MOVE;
		break;

		//攻撃
	case MOTION_TYPE::ATTACK_FRONT:
	case MOTION_TYPE::ATTACK_UNDER:
	case MOTION_TYPE::ATTACK_RUSH:
		return MOTION_CATEGORY::ATTACK;
		break;

		//その他
	default:
		return MOTION_CATEGORY::NONE;
		break;
	}
}