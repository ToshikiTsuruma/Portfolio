//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "sound.h"
#include "gameScene.h"
#include "player.h"
#include "terrain.h"
#include "effect.h"
#include "billboard.h"
#include "particleEffect.h"
#include "gauge3D.h"
#include "appleTree.h"
#include "apple_black.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FALLDOWN_TIME (35)	//死亡時の倒れる時間
#define DEAD_TIME (120)	//死亡後消えるまでの時間

#define GOLD_RATE (7)	//金の敵になる確率
#define DEFAULT_GROW_VALUE (1)	//デフォルトの敵死亡時の林檎の木成長量
#define GOLD_GROW_VALUE (10)		//金の敵死亡時の林檎の木成長量

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
bool CEnemy::m_bGoldRush = false;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemy::CEnemy() : m_fMoveSpeed(0.0f), m_fRotateSpeed(0.0f), m_nMaxLife(0), m_fDistAttack(0.0f), m_fHeightLifeGauge(0.0f)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CEnemy::CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, float fMoveSpeed, float fRotSpeed, int nLife, float fDistAttack, float fHeightLifeGauge)
	: CObjectMotion(pPartsInfoArray, nNumParts, pMotionInfoArray, nNumTypeMotion, false), m_fMoveSpeed(fMoveSpeed), m_fRotateSpeed(fRotSpeed), m_nMaxLife(nLife), m_fDistAttack(fDistAttack), m_fHeightLifeGauge(fHeightLifeGauge)
{
	SetUpdatePriority(UPDATE_PRIORITY::ENEMY);	//更新順の設定
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//描画順の設定
	SetObjType(OBJTYPE_ENEMY);	//オブジェクトタイプの設定

	SetColorOutlineAll(D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f));	//輪郭の色の設定
	SetColorGlowAll(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	//輪郭の発光色の設定

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDestY = 0.0f;
	m_nLife = nLife;
	m_bDead = false;
	m_nCntDead = 0;
	m_bLand = false;
	m_bAttackDist = false;
	m_bGoldEnemy = false;

	//体力ゲージの生成
	m_pGaugeLife = CGauge3D::Create(m_nMaxLife, false, m_nLife, 600, false);
	//ゲージの背景の設定
	m_pGaugeLife->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55.0f, 7.0f);
	m_pGaugeLife->SetGaugeBGColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	//ゲージの設定
	m_pGaugeLife->CreateGauge(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, 4.0f);
	m_pGaugeLife->SetGaugeColor(D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
	//ゲージを非表示
	m_pGaugeLife->SetGaugeDraw(false);
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// エネミーの初期化処理
//=============================================================================
HRESULT CEnemy::Init(void) {
	//当たり判定あり
	SetEnableCollision(true);

	//ランダムで金色の敵
	int nRandGold = rand() % GOLD_RATE;
	if (nRandGold == 0 || m_bGoldRush) {
		m_bGoldEnemy = true;
		SetDiffuseModelAll(D3DXCOLOR(0.7f, 0.5f, 0.0f, 1.0f), 0);
		SetSpecularModelAll(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f), 0);
	}

	//モーションオブジェクトの初期化処理
	CObjectMotion::Init();
	return S_OK;
}

//=============================================================================
// エネミーの終了処理
//=============================================================================
void CEnemy::Uninit(void) {
	//体力ゲージの破棄
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}
	//終了処理
	CObjectMotion::Uninit();
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

	//----------------------------
	//死亡時
	//----------------------------
	if (m_bDead) {
		m_nCntDead++;

		//重力
		SetPos(GetPos() + D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f));

		//地形との当たり判定
		bool bLand = false;	//接地しているかどうか
		D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//接触位置、開始ベクトル、終了ベクトル
		vecStart = GetPos();	//敵の位置を取得
		vecEnd = vecStart;
		vecEnd.y += 1.0f;	//上向きのベクトル

		bLand = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);
		//接地時
		if (bLand) {
			SetPos(posColTerrain);	//位置の移動
		}

		//倒れる時間
		if (m_nCntDead == FALLDOWN_TIME) {
			//倒れる音
			if (pSound != nullptr)pSound->PlaySound(CSound::SOUND_LABEL::STUN);

			//パーティクルエフェクトの生成
			D3DXVECTOR3 posParticle;	//パーティクルの位置
			GetPosCollision(&posParticle, D3DXVECTOR3(0.0f, 20.0f, 0.0f), 0);	//胴体の位置の取得

			//パーティクルエフェクトの最初の色
			D3DXCOLOR colEffectStart = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			if (m_bGoldEnemy) colEffectStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

			//パーティクルの情報
			CParticleEffect::PARTICLE_INFO particleInfo = { 60, 25.0f, -0.25f, 2.0f, D3DXVECTOR3(0.0f, -0.01f, 0.0f), colEffectStart, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) };
			//エフェクトの生成
			CParticleEffect::Create(particleInfo, posParticle, DEAD_TIME - FALLDOWN_TIME, 10, 0.05f * D3DX_PI, true);
		}

		//エネミーの破棄
		if (m_nCntDead > DEAD_TIME) {
			//フェードアウト
			bool bEndFade = false;
			bEndFade = FadeModelAll(0.0f, -0.01f);
			//フェード終了時
			if (bEndFade) {
				//終了処理
				Uninit();
				return;
			}
		}

		//モーションの更新
		CObjectMotion::Update();
		return;
	}


	//----------------------------
	//回転
	//----------------------------
	D3DXVECTOR3 rot = GetRot();	//角度の取得
	
	//目標角度と角度が不一致の場合
	if (m_rotDestY != GetRot().y) {	//GetMotionCategory() != MOTION_CATEGORY::ATTACK && 

		//目標角度へ回転させる
		float rotY = rot.y;	//Y軸の角度

		//目標角度がマイナス、角度がプラスのときに、減算した結果が-πを超えた場合
		if (m_rotDestY - rotY < -D3DX_PI) {
			//加算
			rot.y += m_fRotateSpeed;
		}
		//目標角度がプラス、角度がマイナスのときに、減算した結果がπを超えた場合
		else if (m_rotDestY - rotY > D3DX_PI) {
			//減算
			rot.y -= m_fRotateSpeed;
		}
		//個々の正負は関係ない場合
		else if (m_rotDestY - rotY > 0) {
			//加算
			rot.y += m_fRotateSpeed;
			//目標角度超過時の調整
			if (rot.y > m_rotDestY) {
				rot.y = m_rotDestY;
			}
		}
		else if (m_rotDestY - rotY < 0) {
			//減算
			rot.y -= m_fRotateSpeed;
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

	//----------------------------
	//移動
	//----------------------------
	//移動
	if (m_bMove) {
		m_move.x = sinf(rot.y + D3DX_PI) * m_fMoveSpeed;
		m_move.z = cosf(rot.y + D3DX_PI) * m_fMoveSpeed;
	}

	//位置の設定
	SetPos(GetPos() + m_move);

	//攻撃可能な距離かどうか計算
	AttackDistance();

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
	if (bLand) {
		m_bLand = true;
		m_move.y = -POWER_GRAVITY_GROUND;
		SetPos(posColTerrain);	//位置の移動

		//木の方へ目標角度を設定
		SetRotTree();

		//魔術師敵が移動モーションなしにしているので移動不可時に接地していれば移動モーション
		if (!m_bMove && GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
			//移動モーションの設定
			SetMoveMotion();
			m_bMove = true;
		}
	}
	else {
		m_bLand = false;
		//重力を掛ける
		m_move.y -= POWER_GRAVITY;
	}

	//落下死
	if (GetPos().y < DEAD_HEIGHT) {
		Uninit();
		return;
	}

	//攻撃
	Attack();

	//モーションの更新
	CObjectMotion::Update();

	//モーション終了時
	if (GetEndMotion()) {
		MotionEnd();
	}

	//オブジェクトの全モデルのワールドマトリックスを更新
	UpdateMtxWorldAll();
	//モーション中の処理
	MotionAct();

	if (m_pGaugeLife != nullptr) {
		//体力ゲージの更新
		m_pGaugeLife->Update();
		//体力ゲージの位置を更新
		m_pGaugeLife->SetAllGaugePos(D3DXVECTOR3(GetPos().x, GetPos().y + m_fHeightLifeGauge, GetPos().z));
	}
}

//=============================================================================
// エネミーの描画処理
//=============================================================================
void CEnemy::Draw(void) {
	CObjectMotion::Draw();
}

//=============================================================================
// 移動量の設定
//=============================================================================
void CEnemy::SetMove(D3DXVECTOR3 move) {
	m_move = move;
	//弾き飛ばされたりしてジャンプするような事があればニュートラルモーションを設定（移動モーション中は移動を固定されるため）
	if (m_move.y > 0.0f) {
		SetMotion(0);
		AttackStop();
		m_bMove = false;
	}
}

//=============================================================================
// エネミーのダメージ
//=============================================================================
void CEnemy::Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead) {
	if (m_bDead) return;

	//ダメージ
	m_nLife -= nDamage;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//攻撃のタイプごとの処理
	switch (typeDamage)
	{
	case DAMAGE_TYPE::PLAYER_PUNCH:
	case DAMAGE_TYPE::SHOCKWAVE:
		//攻撃エフェクトの生成
		CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f), CEffect::EFFECT_TYPE::DAMAGE_PLAYER, 40.0f, 40.0f, false);
		//ダメージ音の再生
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		break;
	case DAMAGE_TYPE::THUNDERBOLT:
		//ダメージエフェクトの生成
		CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f), CEffect::EFFECT_TYPE::THUNDER, 50.0f, 50.0f, false);
		//ダメージ音の再生
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_THUNDER);
		break;
	}

	//死亡判定
	if (m_nLife <= 0) {
		//死亡
		if (pDead != nullptr) *pDead = true;
		Dead();
	}
	else {
		if (pDead != nullptr) *pDead = false;

		//体力ゲージの設定
		if (m_pGaugeLife != nullptr) {
			m_pGaugeLife->SetGaugeValue(m_nLife);
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
	SetDeadMotion();
	//輪郭を非表示
	SetDrawOutlineAll(false);

	//体力ゲージの破棄
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	//ゲームシーンの取得
	CGameScene* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//林檎の木の取得
	CAppleTree* pAppleTree = nullptr;
	if (pGame != nullptr) pAppleTree = pGame->GetAppleTree();
	//林檎の木を成長させる
	if (pAppleTree != nullptr) {
		int nGrowValue = DEFAULT_GROW_VALUE; //成長量
		//金の敵の場合多く成長
		if (m_bGoldEnemy) nGrowValue = GOLD_GROW_VALUE;
		//木を成長
		pAppleTree->AddGrow(nGrowValue);
	}

	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//音の再生
	//if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::EXPLOSION);
}

//=============================================================================
// 木の方向への角度に設定
//=============================================================================
void CEnemy::SetRotTree(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//ゲームシーンの取得
	CGameScene* pGame = pManager->GetGameScene();
	if (pGame == nullptr) return;
	//林檎の木の取得
	CObject* pAppleTree = pGame->GetAppleTree();
	if (pAppleTree == nullptr) return;
	D3DXVECTOR3 posAppleTree = pAppleTree->GetPos();//木の位置を取得

	//敵から林檎の木への角度
	float rotTreeY = (float)atan2(posAppleTree.x - GetPos().x, posAppleTree.z - GetPos().z);

	//目標角度を林檎の木の向きへ向ける
	m_rotDestY = rotTreeY;
	//モデルが反対なので反対向きに向ける
	if (m_rotDestY >= 0.0f) {
		m_rotDestY -= D3DX_PI;
	}
	else if (m_rotDestY < 0.0f) {
		m_rotDestY += D3DX_PI;
	}
}

//=============================================================================
// 攻撃可能な距離かどうかを計算
//=============================================================================
void CEnemy::AttackDistance(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//ゲームシーンの取得
	CGameScene* pGame = pManager->GetGameScene();
	if (pGame == nullptr) return;
	//林檎の木の取得
	CObject* pAppleTree = pGame->GetAppleTree();
	if (pAppleTree == nullptr) return;
	D3DXVECTOR3 posAppleTree = pAppleTree->GetPos();//木の位置を取得
	float fDistTree = D3DXVec2Length(&D3DXVECTOR2(posAppleTree.x - GetPos().x, posAppleTree.z - GetPos().z));	//木と敵との距離

	m_bAttackDist = fDistTree <= m_fDistAttack ? true : false;
}

//=============================================================================
//　攻撃の処理
//=============================================================================
void CEnemy::Attack(void) {
	//攻撃開始条件を満たしていない場合終了
	if (GetMotionCategory() == MOTION_CATEGORY::ATTACK || !m_bAttackDist || !m_bLand) return;

	//攻撃済みリストの初期化
	InitObjAttacked();
	//移動量を重力のみにする
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bMove = false;
	//攻撃開始
	AttackStart();
}