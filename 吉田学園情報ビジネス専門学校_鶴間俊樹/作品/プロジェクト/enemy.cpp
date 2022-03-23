//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "terrain.h"
#include "enemy_normal.h"
#include "enemy_human.h"
#include "effect.h"
#include "billboard.h"
#include "timer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MOVE_SPEED_NORMAL (1.0f)		//移動速度（通常時）
#define MOVE_SPEED_CHASE (4.0f)			//移動速度（追跡時）
#define MOVE_START_TIME (100)			//移動開始時間
#define MIN_MOVE_TIME (60)				//移動時間の最小値
#define MAX_MOVE_TIME (120)				//移動時間の最大値
#define ROTATE_SPEED (0.02f * D3DX_PI)	//回転速度
#define RADIUS_VISUAL_FIELD (400.0f)	//視野の半径
#define DISTANCE_LOSE_SIGHT (600.0f)	//プレイヤーを見失う距離
#define VIEW_ANGLE (D3DX_PI * 0.8f)		//視野角度
#define ATTACK_ANGLE (D3DX_PI * 0.1f)	//攻撃角度
#define FALLDOWN_TIME (30)	//死亡時の倒れる時間
#define DEAD_TIME (120)	//死亡後消えるまでの時間
#define LIFE_GAUGE_HEIGHT (100.0f)	//敵の位置からの体力ゲージの高さ
#define DRAW_DISTANCE (1800.0f)			//描画が可能になり始めるプレイヤーとの距離
#define MAX_DRAW_DISTANCE (2000.0f)		//描画可能な最大のプレイヤーとの距離

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemy::CEnemy() : m_nMaxLife(0), m_fDistAttack(0.0f), m_nAddTime(0)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CEnemy::CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, bool bOutline, int nLife, float fDistAttack, int nAddTime) : CSceneMotion(pPartsInfoArray, nNumParts, pMotionInfoArray, nNumTypeMotion, bOutline), m_nMaxLife(nLife), m_fDistAttack(fDistAttack), m_nAddTime(nAddTime)
{
	SetUpdatePriority(UPDATE_PRIORITY::ENEMY);	//更新順の設定
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//描画順の設定
	SetObjType(OBJ_TYPE::ENEMY);	//オブジェクトタイプの設定

	SetColorOutlineAll(D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f));	//輪郭の色の設定

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDestY = 0.0f;
	m_bRotateAttack = false;
	m_nCntMove = 0;
	m_nMoveTimeRand = 0;
	m_bDetectPlayer = false;
	m_nLife = nLife;
	m_bDead = false;
	m_nCntDead = 0;
	m_nCntDrawGaugeLife = 0;
	m_bDraw = true;

	//体力ゲージの生成
	m_pGaugeLifeFrame = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 75.0f, 15.0f);
	m_pGaugeLife = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 70.0f, 10.0f);

	if (m_pGaugeLifeFrame != NULL) {
		m_pGaugeLifeFrame->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)); //体力ゲージの色の設定
		m_pGaugeLifeFrame->SetDraw(false);	//描画しないようにする
	}
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f)); //体力ゲージの色の設定
		m_pGaugeLife->SetDraw(false);	//描画しないようにする
	}

	m_pClock = nullptr;
	for (int nCnt = 0; nCnt < MAX_DIGIT_ADD_TIME_ENEMY; nCnt++)
	{
		m_pAddTime[nCnt] = nullptr;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// 同心円状にランダムに角度ずらして敵を配置する
//=============================================================================
//引数：中心の位置　円の数　円ごとの距離（半径）　（同じ園の一つ前の敵との）最短距離、最長距離
void CEnemy::SetEnemyCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fDistCircle, int nMinDist, int nMaxDist) {
	int nCntCircle = 0;	//作成した木の円の数
	float fRotCreate = 0.0f;	//配置する角度
	float fRandStartRot = (rand() % 314) * 0.01f;	//配置の開始角度の位置をランダム 前方が過疎気味になるため

	while (nCntCircle < nNumCircle)
	{
		int nRandDist = rand() % (nMaxDist + 1 - nMinDist) + nMinDist;	//一つ前の木との距離
		fRotCreate += nRandDist / ((nCntCircle + 1) * fDistCircle * D3DX_PI) * D3DX_PI;	//ランダムな距離 / 円周の長さの半分 * pi でラジアンを求める

		//一周配置した場合
		if (fRotCreate >= D3DX_PI * 2.0f) {
			nCntCircle++;	//カウントの加算
			fRotCreate = 0.0f;	//配置する角度のリセット
			fRandStartRot = (rand() % 314) * 0.01f;	//配置する角度のランダム値を再設定
		}
		else {
			//敵を生成する位置の設定
			D3DXVECTOR3 posCreate = posCenter;	//配置する位置
			posCreate.x = sinf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			posCreate.z = cosf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			//敵の種類をランダムで決める
			int nRandType = rand() % 2;
			//敵の生成
			switch (nRandType)
			{
				//通常敵
			case 0:
				CEnemyNormal::Create(posCreate);
				break;
				//人形敵
			case 1:
				CEnemyHuman::Create(posCreate);
				break;
			}
		}
	}
}

//=============================================================================
// エネミーの初期化処理
//=============================================================================
HRESULT CEnemy::Init(void) {
	//角度をランダムに設定
	int nRandRot = rand() % 21 - 10;	//-10 ~ 10
	float fRotY = D3DX_PI * (float)nRandRot * 0.1f;
	SetRot(D3DXVECTOR3(0.0f, fRotY, 0.0f));
	m_rotDestY = fRotY;

	//モーションオブジェクトの初期化処理
	CSceneMotion::Init();
	return S_OK;
}

//=============================================================================
// エネミーの終了処理
//=============================================================================
void CEnemy::Uninit(void) {
	//体力ゲージの破棄
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = NULL;
	}
	//体力ゲージの枠の破棄
	if (m_pGaugeLifeFrame != NULL) {
		m_pGaugeLifeFrame->Uninit();
		m_pGaugeLifeFrame = NULL;
	}
	//終了処理
	CSceneMotion::Uninit();
}

//=============================================================================
// エネミーの更新処理
//=============================================================================
void CEnemy::Update(void) {
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

	//----------------------------
	//死亡時
	//----------------------------
	if (m_bDead == true) {
		m_nCntDead++;

		if (m_nCntDead < 60) {
			//位置を上げていく
			if (m_pClock != nullptr) {
				D3DXVECTOR3 posClock = m_pClock->GetPos();
				posClock.y += 0.5f;
				m_pClock->SetPos(posClock);
			}
			if (m_pAddTime != nullptr) {
				for (int nCntTime = 0; nCntTime < MAX_DIGIT_ADD_TIME_ENEMY; nCntTime++)
				{
					D3DXVECTOR3 posAddTime = m_pAddTime[nCntTime]->GetPos();
					posAddTime.y += 0.5f;
					m_pAddTime[nCntTime]->SetPos(posAddTime);

				}
			}
		}
		if (m_nCntDead == 100) {
			//時間の追加のビルボードを破棄
			if (m_pClock != nullptr) {
				m_pClock->Uninit();
				m_pClock = nullptr;
			}
			for (int nCntTime = 0; nCntTime < MAX_DIGIT_ADD_TIME_ENEMY; nCntTime++) {
				if (m_pAddTime[nCntTime] != nullptr) {
					m_pAddTime[nCntTime]->Uninit();
					m_pAddTime[nCntTime] = nullptr;
				}
			}
		}

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
	//目標角度と移動量の設定
	//----------------------------
	//プレイヤーが発見されているとき
	if (m_bDetectPlayer == true) {
		//自動追跡
		D3DXVECTOR3 posPlayer;	//プレイヤーの位置
		//プレイヤーの位置の取得
		if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();
		//敵からプレイヤーへの角度
		float rotPlayerY = (float)atan2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);

		//目標角度をプレイヤーの向きへ向ける
		m_rotDestY = rotPlayerY;
		//モデルが反対なので反対向きに向ける
		if (m_rotDestY >= 0.0f) {
			m_rotDestY -= D3DX_PI;
		}
		else if(m_rotDestY < 0.0f) {
			m_rotDestY += D3DX_PI;
		}

		//移動量の設定
		if (this->GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
			//敵とプレイヤーの距離を求める
			D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);	//敵からプレイヤーへのベクトル
			float fDistPlayer = D3DXVec2Length(&vecPlayer);	//敵からプレイヤーへの距離
			//目標角度と今の角度の差分を求める
			float fRotY = GetRot().y;	//現在の角度
			float fRotDeltaY = fabsf(fRotY - m_rotDestY);	//角度の差分
			if (fRotDeltaY > D3DX_PI) fRotDeltaY = D3DX_PI * 2 - fRotDeltaY;
			//攻撃可能な距離だった場合
			if (fDistPlayer < m_fDistAttack || fRotDeltaY > ATTACK_ANGLE) {
				m_move = D3DXVECTOR3(0.0f, m_move.y, 0.0f);
			}
			else {
				m_move = D3DXVECTOR3(MOVE_SPEED_CHASE * sinf(fRotY + D3DX_PI), m_move.y, MOVE_SPEED_CHASE * cosf(fRotY + D3DX_PI));
			}
		}
	}
	else {
		m_nCntMove++;
		//移動開始時
		if (m_nCntMove == MOVE_START_TIME) {
			//目標角度の設定
			int nRandRot = rand() % 40 + 10;	//角度をランダム
			float fAddRot = (float)nRandRot * 0.01f;	//追加する角度
			m_rotDestY = m_rotDestY += D3DX_PI * fAddRot;
			if (m_rotDestY > D3DX_PI)m_rotDestY -= D3DX_PI * 2;	//パイ超過時の調整
			//移動時間の乱数の設定
			m_nMoveTimeRand = rand() % (MAX_MOVE_TIME - MIN_MOVE_TIME) + MIN_MOVE_TIME;
			//移動モーションの設定
			this->SetMoveMotion();
		}
		//移動中
		if (m_nCntMove >= MOVE_START_TIME && m_nCntMove < MOVE_START_TIME + m_nMoveTimeRand) {
			//移動量の設定
			if (this->GetMotionCategory() != MOTION_CATEGORY::ATTACK) {	//追跡時以外で攻撃を行うことはないが念の為
				float fRotY = GetRot().y;
				m_move = D3DXVECTOR3(MOVE_SPEED_NORMAL * sinf(fRotY + D3DX_PI), m_move.y, MOVE_SPEED_NORMAL * cosf(fRotY + D3DX_PI));
			}
		}
		//移動終了時
		if (m_nCntMove == MOVE_START_TIME + m_nMoveTimeRand) {
			m_move = D3DXVECTOR3(0.0f, m_move.y, 0.0f);	//移動させなくする
			m_rotDestY = GetRot().y;	//目標角度を現在の角度にする
			SetMotion(0);	//ニュートラルモーションを設定
			m_nCntMove = 0;
		}
	}

	//----------------------------
	//回転
	//----------------------------
	//攻撃モーション中ではないとき
	if (this->GetMotionCategory() != MOTION_CATEGORY::ATTACK || m_bRotateAttack == true) {
		//目標角度と角度が不一致の場合
		if (m_rotDestY != GetRot().y) {
			//移動モーション中ではなかったら
			if (this->GetMotionCategory() != MOTION_CATEGORY::MOVE && this->GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
				//移動時のモーションを設定 不必要？
				this->SetMoveMotion();
			}

			//目標角度へ回転させる
			D3DXVECTOR3 rot = GetRot();	//角度の取得
			float rotY = rot.y;	//Y軸の角度

			//目標角度がマイナス、角度がプラスのときに、減算した結果が-πを超えた場合
			if (m_rotDestY - rotY < -D3DX_PI) {
				//加算
				rot.y += ROTATE_SPEED;
			}
			//目標角度がプラス、角度がマイナスのときに、減算した結果がπを超えた場合
			else if (m_rotDestY - rotY > D3DX_PI) {
				//減算
				rot.y -= ROTATE_SPEED;
			}
			//個々の正負は関係ない場合
			else if (m_rotDestY - rotY > 0) {
				//加算
				rot.y += ROTATE_SPEED;
				//目標角度超過時の調整
				if (rot.y > m_rotDestY) {
					rot.y = m_rotDestY;
				}
			}
			else if (m_rotDestY - rotY < 0) {
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

	//----------------------------
	//移動
	//----------------------------
	//重力
	m_move.y -= POWER_GRAVITY_GROUND;	//ジャンプすることがないので重くする

	//位置の設定
	SetPos(GetPos() + m_move);

	//----------------------------
	//当たり判定
	//----------------------------

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

	//落下死
	if (GetPos().y < DEAD_HEIGHT) {
		Uninit();
		return;
	}

	//----------------------------
	//プレイヤーの発見
	//----------------------------
	D3DXVECTOR3 posPlayer;	//プレイヤーの位置

	//ゲームクリア時かどうか
	bool bGameClear = false;
	if (pGame != NULL) bGameClear = pGame->GetGameClear();

	bool bStunPlayer = false;	//プレイヤーがスタン中か
	if (pPlayer != NULL) {
		//プレイヤーの位置の取得
		posPlayer = pPlayer->GetPos();
		//プレイヤーのスタン状態の取得
		bStunPlayer = pPlayer->GetStun();
	}
	D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);	//敵からプレイヤーへのベクトル
	float fDistPlayer = D3DXVec2Length(&vecPlayer);	//敵からプレイヤーへの距離

	bool bAngleAttack = false;	//攻撃できる角度にいるかどうか

	//プレイヤーを見失う距離だった場合
	if (fDistPlayer > DISTANCE_LOSE_SIGHT || bStunPlayer == true || bGameClear == true) {
		if (m_bDetectPlayer == true) {
			m_move = D3DXVECTOR3(0.0f, m_move.y, 0.0f);	//移動させなくする
			m_rotDestY = GetRot().y;	//目標角度を現在の角度にする
			SetMotion(0);	//ニュートラルモーションを設定
			m_nCntMove = 0;	//移動カウンターをリセット
		}
		m_bDetectPlayer = false;	//プレイヤーを未発見状態にする
	}

	//視野の半径内にプレイヤーがいた場合
	if (fDistPlayer < RADIUS_VISUAL_FIELD && bStunPlayer == false && bGameClear == false) {
		float rotY = GetRot().y;	//敵のY軸の角度

		//敵の角度とモデルの前方の向きが逆なので修正する
		if (rotY < 0.0f) {
			rotY += D3DX_PI;
		}
		else if (rotY >= 0.0f) {
			rotY -= D3DX_PI;
		}

		//敵からプレイヤーへの角度
		float rotPlayerY = (float)atan2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);

		//角度を正の値にする(0から2π)
		if (rotY < 0.0f) rotY += D3DX_PI * 2;
		if (rotPlayerY < 0.0f) rotPlayerY += D3DX_PI * 2;

		//敵の前方の向きと敵からプレイヤーへの向きの差を求め、それが一定以内ならプレイヤー発見
		if (fabsf(rotPlayerY - rotY) < VIEW_ANGLE / 2.0f || fabsf(rotPlayerY - rotY) > (D3DX_PI * 2) - VIEW_ANGLE / 2.0f) {
			m_bDetectPlayer = true;
		}
		//プレイヤーが攻撃できる角度にいた場合
		if (fabsf(rotPlayerY - rotY) < ATTACK_ANGLE / 2.0f || fabsf(rotPlayerY - rotY) > (D3DX_PI * 2) - ATTACK_ANGLE / 2.0f) {
			bAngleAttack = true;
		}
	}

	//プレイヤー発見中で攻撃モーション中じゃない場合
	if (m_bDetectPlayer == true && this->GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
		//追跡モーションに変更
		this->SetChaseMotion();
	}

	//攻撃
	if (bAngleAttack == true && this->GetMotionCategory() != MOTION_CATEGORY::ATTACK && bLand == true && fDistPlayer < m_fDistAttack) {
		bool bSuccessAttackStart = false;	//攻撃開始が成功したかどうか
		//攻撃開始
		bSuccessAttackStart = this->AttackStart();	
		//攻撃開始成功時
		if (bSuccessAttackStart == true) {
			//攻撃済みリストの初期化
			InitObjAttacked();
			//移動量を重力のみにする
			m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			//攻撃音の再生
			if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_PUNCH);
		}
	}

	//モーションの更新
	CSceneMotion::Update();

	//モーション終了時
	if (GetEndMotion() == true) {
		this->MotionEnd();
	}

	//オブジェクトの全モデルのワールドマトリックスを更新
	UpdateMtxWorldAll();
	//モーション中の処理
	this->MotionAct();

	//体力ゲージの描画時間を減少
	if (m_nCntDrawGaugeLife > 0) {
		m_nCntDrawGaugeLife--;
		//描画時間がなくなった場合
		if (m_nCntDrawGaugeLife <= 0) {
			//描画しなくする
			if (m_pGaugeLife != NULL) m_pGaugeLife->SetDraw(false);
			if (m_pGaugeLifeFrame != NULL) m_pGaugeLifeFrame->SetDraw(false);
		}
		else {
			//位置を更新
			if (m_pGaugeLife != NULL) m_pGaugeLife->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + LIFE_GAUGE_HEIGHT, GetPos().z));
			if (m_pGaugeLifeFrame != NULL) m_pGaugeLifeFrame->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + LIFE_GAUGE_HEIGHT, GetPos().z));
		}
	}
}

//=============================================================================
// エネミーの描画処理
//=============================================================================
void CEnemy::Draw(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//ゲームの取得
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//プレイヤーの取得
	CPlayer* pPlayer = nullptr;
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	//プレイヤーの位置を取得
	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();
	//プレイヤーとの距離を求める
	D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);
	float fDistPlayer = D3DXVec2Length(&vecPlayer);
	//描画の設定
	if (m_bDraw == true) {
		//描画外の位置にいたら描画しないようにする
		if (fDistPlayer > MAX_DRAW_DISTANCE) {
			m_bDraw = false;
		}
	}
	else {
		//描画内の位置にいたら描画できるようにする
		if (fDistPlayer < DRAW_DISTANCE) {
			m_bDraw = true;
		}
	}

	//描画
	if (m_bDraw == true) {
		CSceneMotion::Draw();
	}
	else {
		//描画しない場合ワールドマトリックスのみを更新する
		UpdateMtxWorldAll();
	}
}

//=============================================================================
// エネミーのダメージ
//=============================================================================
void CEnemy::Damage(int nDamage, bool* pDead) {
	if (m_bDead == true) return;
	
	//ダメージ
	m_nLife -= nDamage;
	//奇襲成功時、追加ダメージ
	if (m_bDetectPlayer == false) {
		m_nLife--;
	}

	if (m_nLife <= 0) {
		//死亡
		if (pDead != NULL) *pDead = true;
		Dead();
	}
	else {
		if (pDead != NULL) *pDead = false;
		//プレイヤー発見状態にする
		m_bDetectPlayer = true;

		//体力ゲージの描画時間の設定
		m_nCntDrawGaugeLife = 300;
		if (m_pGaugeLife != NULL) {
			//体力ゲージの幅を変える
			m_pGaugeLife->SetRatioWidth((float)m_nLife / (float)m_nMaxLife);
			//描画させる
			m_pGaugeLife->SetDraw(true);
		}
		if (m_pGaugeLifeFrame != NULL) {
			//描画させる
			m_pGaugeLifeFrame->SetDraw(true);
		}
	}
}

//=============================================================================
// エネミーの死亡
//=============================================================================
void CEnemy::Dead(void) {
	//死亡の設定
	m_bDead = true;
	//死亡カウントの初期化
	m_nCntDead = 0;
	//当たり判定を無効
	SetEnableCollision(false);
	//死亡モーションの設定
	this->SetDeadMotion();
	//輪郭を非表示
	SetDrawOutlineAll(false);

	//体力ゲージの破棄
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = NULL;
	}
	//体力ゲージの枠の破棄
	if (m_pGaugeLifeFrame != NULL) {
		m_pGaugeLifeFrame->Uninit();
		m_pGaugeLifeFrame = NULL;
	}

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	CGame* pGame = nullptr;
	CTimer* pTimer = nullptr;
	CSound* pSound = nullptr;

	//ゲームの取得
	if (pManager != nullptr) pGame = pManager->GetGame();
	//タイマーの取得
	if (pGame != nullptr) pTimer = pGame->GetTimer();

	//タイマーの時間の追加
	if (pTimer != nullptr) pTimer->AddTime(m_nAddTime);

	//追加時間のビルボードの生成
	D3DXVECTOR3 posBillboard = GetPos();
	posBillboard.y += 50.0f;
	m_pClock = CBillboard::Create(posBillboard, CTexture::TEXTURE_TYPE::CLOCK, 30.0f, 30.0f);
	for (int nCntTime = 0; nCntTime < MAX_DIGIT_ADD_TIME_ENEMY; nCntTime++) {
		m_pAddTime[nCntTime] = CBillboard::Create(posBillboard, CTexture::TEXTURE_TYPE::NUMBER_004, 30.0f, 30.0f);
	}
	//オフセットの設定
	if (m_pClock != nullptr) m_pClock->SetOffsetPos(D3DXVECTOR3(-15.0f, 0.0f, 0.0f));
	if (m_pAddTime != nullptr) {
		for (int nCntTime = 0; nCntTime < MAX_DIGIT_ADD_TIME_ENEMY; nCntTime++) {
			//オフセットの設定
			m_pAddTime[nCntTime]->SetOffsetPos(D3DXVECTOR3(30.0f*MAX_DIGIT_ADD_TIME_ENEMY - 30.0f*nCntTime - 10.0f, 0.0f, 0.0f));
			//UV座標を設定
			int nTimeDigit = (m_nAddTime % (int)pow(10, nCntTime + 1)) / (int)pow(10, nCntTime);	//桁ごとの時間の数字
			m_pAddTime[nCntTime]->SetTexPos((float)nTimeDigit / 10.0f, 0.0f, ((float)nTimeDigit / 10.0f) + 0.1f, 1.0f);
		}
	}

	//時間追加音の再生
	if (pManager != nullptr) pSound = pManager->GetSound();
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::ADD_TIME);
}

//=============================================================================
// 攻撃モーション中に回転できるかどうかの設定
//=============================================================================
void CEnemy::SetRotateAttack(bool bRotateAttack) {
	m_bRotateAttack = bRotateAttack;
}