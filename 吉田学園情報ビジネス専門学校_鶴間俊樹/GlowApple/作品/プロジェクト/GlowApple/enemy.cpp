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
#include "enemy_normal.h"
#include "enemy_human.h"
#include "effect.h"
#include "billboard.h"
#include "particleEffect.h"
#include "gauge3D.h"
#include "appleTree.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ROTATE_SPEED (0.05f * D3DX_PI)	//回転速度
#define FALLDOWN_TIME (35)	//死亡時の倒れる時間
#define DEAD_TIME (120)	//死亡後消えるまでの時間
#define LIFE_GAUGE_HEIGHT (100.0f)	//敵の位置からの体力ゲージの高さ

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemy::CEnemy() : m_fMoveSpeed(0.0f), m_nMaxLife(0), m_fDistAttack(0.0f)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CEnemy::CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, float fMoveSpeed, int nLife, float fDistAttack)
	: CObjectMotion(pPartsInfoArray, nNumParts, pMotionInfoArray, nNumTypeMotion, false), m_fMoveSpeed(fMoveSpeed), m_nMaxLife(nLife), m_fDistAttack(fDistAttack)
{
	SetUpdatePriority(UPDATE_PRIORITY::ENEMY);	//更新順の設定
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//描画順の設定
	SetObjType(OBJ_TYPE::ENEMY);	//オブジェクトタイプの設定

	SetColorOutlineAll(D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f));	//輪郭の色の設定

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDestY = 0.0f;
	m_nLife = nLife;
	m_bDead = false;
	m_nCntDead = 0;
	m_bAttackDist = false;
	m_bGoldEnemy = false;

	//体力ゲージの生成
	m_pGaugeLife = CGauge3D::Create(m_nMaxLife, m_nLife, 180);
	//ゲージの背景の設定
	m_pGaugeLife->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 75.0f, 15.0f);
	m_pGaugeLife->SetGaugeBGColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	//ゲージの設定
	m_pGaugeLife->CreateGauge(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 70.0f, 10.0f);
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
// 同心円状にランダムに角度ずらして敵を配置する
//=============================================================================
//引数：中心の位置　円の数　円ごとの距離（半径）　（同じ円の一つ前の敵との）最短距離、最長距離
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
			posCreate.x += sinf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			posCreate.z += cosf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
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
	//当たり判定あり
	SetEnableCollision(true);

	//ランダムで金色の敵
	int nRandGold = rand() % 3;
	if (nRandGold == 0) {
		m_bGoldEnemy = true;
		SetDiffuseModelAll(D3DXCOLOR(0.7f, 0.5f, 0.0f, 1.0f));
		SetSpecularModelAll(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
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
	
	//攻撃モーション中ではないときで目標角度と角度が不一致の場合
	if (GetMotionCategory() != MOTION_CATEGORY::ATTACK && m_rotDestY != GetRot().y) {

		//目標角度へ回転させる
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

	//----------------------------
	//移動
	//----------------------------
	//重力
	m_move.y = -POWER_GRAVITY_GROUND;	//ジャンプすることがないので重くする

	//移動
	if (GetMotionCategory() == MOTION_CATEGORY::MOVE) {
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
		m_move.y = -POWER_GRAVITY_GROUND;
		SetPos(posColTerrain);	//位置の移動

		SetRotTree();

		if (GetMotionCategory() != MOTION_CATEGORY::MOVE && GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
			SetMoveMotion();
		}
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
		m_pGaugeLife->SetAllGaugePos(D3DXVECTOR3(GetPos().x, GetPos().y + LIFE_GAUGE_HEIGHT, GetPos().z));
	}
}

//=============================================================================
// エネミーの描画処理
//=============================================================================
void CEnemy::Draw(void) {
	CObjectMotion::Draw();
}

//=============================================================================
// エネミーのダメージ
//=============================================================================
void CEnemy::Damage(int nDamage, bool* pDead) {
	if (m_bDead) return;
	
	//ダメージ
	m_nLife -= nDamage;

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
		int nGrowValue = 1; //成長量
		//金の敵の場合多く成長
		if (m_bGoldEnemy) nGrowValue = 3;
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
	if (GetMotionCategory() == MOTION_CATEGORY::ATTACK || !m_bAttackDist) return;

	//攻撃済みリストの初期化
	InitObjAttacked();
	//移動量を重力のみにする
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	//攻撃開始
	AttackStart();
}