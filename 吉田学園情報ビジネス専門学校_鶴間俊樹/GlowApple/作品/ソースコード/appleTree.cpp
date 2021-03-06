//=============================================================================
//
// 林檎の木の処理 [appleTree.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "appleTree.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "appleMenu.h"
#include "gauge2D.h"
#include "gameScene.h"
#include "object2D.h"
#include "effect.h"
#include "particleEffect.h"

#include "apple_red.h"
#include "apple_green.h"
#include "apple_white.h"
#include "apple_black.h"
#include "apple_silver.h"
#include "apple_gold.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUM_COLLISION (8)			//当たり判定の数
#define COLLISION_RADIUS (30.0f)	//当たり判定の半径

#define MAX_LIFE_DEFAULT (800)			//体力の最大値
#define DANGER_LIFE ((int)(MAX_LIFE_DEFAULT * 0.35f))		//体力の危険値
#define COLOR_LIFE_GAUGE_SAFE (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//体力バーの安全時の色
#define COLOR_LIFE_GAUGE_DANGER (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//体力バーの危険時の色
#define FOLD_PLAYER_DAMAGE (2)		//プレイヤーのダメージに乗算する量

#define MAX_GROW_VALUE_START (3)	//ゲーム開始時の成長度の最大
#define FOLD_GROW_VALUE (3)			//成長度を乗算する量

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CAppleTree::CAppleTree()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleTree::CAppleTree(D3DXVECTOR3 pos) : CObjectModel(CModel::MODELTYPE::OBJ_APPLE_TREE, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
{
	m_nNumApple = 0;
	for (int nCnt = 0; nCnt < MAX_NUM_CREATE_APPLE; nCnt++)
	{
		m_apCreateApple[nCnt] = nullptr;
		m_aTypeCreateApple[nCnt] = CGlowApple::APPLE_TYPE::RED;
	}

	m_pMenuApple = nullptr;
	m_nMaxLife = MAX_LIFE_DEFAULT;
	m_nLife = MAX_LIFE_DEFAULT;
	m_bDead = false;

	//体力ゲージの生成
	m_pGaugeLife = CGauge2D::Create(MAX_LIFE_DEFAULT, false, MAX_LIFE_DEFAULT, 0, true);
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(150.0f + 20.0f, SCREEN_HEIGHT - 30.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeLife->SetGaugeBGColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
		m_pGaugeLife->CreateGauge(CTexture::TEXTURE_TYPE::GAUGE_HORIZONTAL, D3DXVECTOR3(150.0f + 20.0f, SCREEN_HEIGHT - 30.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeLife->SetGaugeColor(COLOR_LIFE_GAUGE_SAFE);
		m_pGaugeLife->SetGaugeColorDanger(COLOR_LIFE_GAUGE_DANGER);
		m_pGaugeLife->SetDangerValue(DANGER_LIFE);
		m_pGaugeLife->CreateGaugeFrame(CTexture::TEXTURE_TYPE::HP_GAUGE_FRAME, D3DXVECTOR3(150.0f + 20.0f, SCREEN_HEIGHT - 30.0f, 0.0f), 200.0f, 20.0f);
	}
	//HPアイコンの生成
	m_pIconHP = CObject2D::Create(D3DXVECTOR3(55.0f, SCREEN_HEIGHT - 30.0f, 0.0f), CTexture::TEXTURE_TYPE::ICON_HP, 40.0f, 40.0f);

	//成長ゲージの生成
	m_nGrowValue = 0;
	m_nGrowValueMax = MAX_GROW_VALUE_START;
	m_pGaugeGrow = CGauge2D::Create(m_nGrowValueMax, false, m_nGrowValue, 0, false);
	if (m_pGaugeGrow != nullptr) {
		m_pGaugeGrow->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 10.0f, 0.0f), SCREEN_WIDTH, 20.0f);
		m_pGaugeGrow->SetGaugeBGColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
		m_pGaugeGrow->CreateGauge(CTexture::TEXTURE_TYPE::GAUGE_HORIZONTAL, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 10.0f, 0.0f), SCREEN_WIDTH - 6.0f, 20.0f - 6.0f);
		m_pGaugeGrow->SetGaugeColor(D3DXCOLOR(0.0f, 0.4f, 1.0f, 1.0f));
		m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
		m_pGaugeGrow->CreateGaugeFrame(CTexture::TEXTURE_TYPE::GROW_GAUGE_FRAME, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 10.0f, 0.0f), SCREEN_WIDTH, 20.0f);
	}

	SetObjType(OBJTYPE_APPLE_TREE);
	SetEnableCollision(true);
}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleTree::~CAppleTree()
{

}

//=============================================================================
// 林檎の木の生成処理
//=============================================================================
CAppleTree* CAppleTree::Create(D3DXVECTOR3 pos) {
	CAppleTree* pAppleTree;
	pAppleTree = new CAppleTree(pos);
	if (pAppleTree == nullptr) return nullptr;

	pAppleTree->Init();

	return pAppleTree;
}

//=============================================================================
// 林檎の木の初期化処理
//=============================================================================
HRESULT CAppleTree::Init(void) {
	//メニュー開いたときにも更新を続けるようにする
	SetUpdatePriority(UPDATE_PRIORITY::MENU_UPDATE);

	CModel* pModel = GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetColorGlow(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));
	}

	CObjectModel::Init();
	return S_OK;
}

//=============================================================================
// 林檎の木の終了処理
//=============================================================================
void CAppleTree::Uninit(void) {
	//林檎のメニューの破棄
	if (m_pMenuApple != nullptr) {
		m_pMenuApple->Uninit();
		m_pMenuApple = nullptr;
	}
	//体力ゲージの破棄
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}
	//成長ゲージの破棄
	if (m_pGaugeGrow != nullptr) {
		m_pGaugeGrow->Uninit();
		m_pGaugeGrow = nullptr;
	}
	//HPアイコンの破棄
	if (m_pIconHP != nullptr) {
		m_pIconHP->Uninit();
		m_pIconHP = nullptr;
	}

	//リンゴの破棄
	for (int nCntApple = 0; nCntApple < MAX_NUM_CREATE_APPLE; nCntApple++)
	{
		if (m_apCreateApple[nCntApple] == nullptr) continue;

		//生成したリンゴを死亡させる
		m_apCreateApple[nCntApple]->Dead();
		m_apCreateApple[nCntApple] = nullptr;
	}


	CObjectModel::Uninit();
}

//=============================================================================
// 林檎の木の更新処理
//=============================================================================
void CAppleTree::Update(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	CInput* pInput = nullptr;	//現在の入力デバイスへのポインタ
	if (pManager != nullptr) pInput = pManager->GetInputCur();

	if (pInput != nullptr) {
		//メニューがある場合
		if (m_pMenuApple != nullptr) {
			//林檎の生成
			if (pInput->GetTrigger(CInput::CODE::SELECT) && !m_pMenuApple->GetLockChangeSelect()) CreateApple();
		}
	}

	CObjectModel::Update();
}

//=============================================================================
// 林檎の木の描画処理
//=============================================================================
void CAppleTree::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// 林檎の木のダメージ
//=============================================================================
void CAppleTree::Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	D3DXVECTOR3 posEffectOffset = D3DXVECTOR3(0.0f, 100.0f, 0.0f);	//エフェクト生成位置のオフセット
	float afRandPos[3];	//オフセットに加算するランダムな値
	//オフセットをランダムでずらす
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		int nRandPos = rand() % 40 - 20;
		afRandPos[nCnt] = (float)nRandPos;
	}
	//オフセットにランダムな値を加算
	posEffectOffset.x += afRandPos[0];
	posEffectOffset.y += afRandPos[1];
	posEffectOffset.z += afRandPos[2];

	//攻撃のタイプごとの処理
	switch (typeDamage)
	{
	case DAMAGE_TYPE::PLAYER_PUNCH:
		//攻撃エフェクトの生成
		CEffect::Create(GetPos() + posEffectOffset, CEffect::EFFECT_TYPE::DAMAGE_PLAYER, 60.0f, 60.0f, false);
		//ダメージ音の再生
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		//ダメージ量を倍にする
		nDamage *= FOLD_PLAYER_DAMAGE;
		break;
	case DAMAGE_TYPE::ENEMY_PUNCH:
		//攻撃エフェクトの生成
		CEffect::Create(GetPos() + posEffectOffset, CEffect::EFFECT_TYPE::DAMAGE_ENEMY, 60.0f, 60.0f, false);
		//ダメージ音の再生
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		break;
	case DAMAGE_TYPE::FIRE:
		//ダメージエフェクトの生成
		CEffect::Create(GetPos() + posEffectOffset, CEffect::EFFECT_TYPE::FIRE, 100.0f, 100.0f, false);
		break;
	case DAMAGE_TYPE::THUNDERBOLT:
		//ダメージエフェクトの生成
		CEffect::Create(GetPos() + posEffectOffset, CEffect::EFFECT_TYPE::THUNDER, 100.0f, 100.0f, false);
		//ダメージ音の再生
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_THUNDER);
		//ダメージ量を倍にする
		nDamage *= FOLD_PLAYER_DAMAGE;
		break;
	}

	//死亡していた場合終了
	if (m_bDead) return;

	//ダメージ
	m_nLife -= nDamage;

	if (m_nLife <= 0) {
		m_nLife = 0;
		//死亡
		if (pDead != nullptr) *pDead = true;
		Dead();
	}
	else {
		if (pDead != nullptr) *pDead = false;
	}
	//体力ゲージの設定
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->SetGaugeValue(m_nLife);
	}
}

//=============================================================================
// 死亡
//=============================================================================
void CAppleTree::Dead(void) {
	//死亡の設定
	m_bDead = true;
	//死亡カウントの初期化
	m_nCntDead = 0;

	CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 400.0f, 0.0f), CEffect::EFFECT_TYPE::DEATH, 666.0f, 666.0f, false);

	//リンゴの破棄
	for (int nCntApple = 0; nCntApple < MAX_NUM_CREATE_APPLE; nCntApple++)
	{
		if (m_apCreateApple[nCntApple] == nullptr) continue;

		//リンゴの位置に死亡エフェクト
		CEffect::Create(m_apCreateApple[nCntApple]->GetPos(), CEffect::EFFECT_TYPE::DEATH, 66.6f, 66.6f, false);

		//生成したリンゴを死亡させる
		m_apCreateApple[nCntApple]->Dead();
		m_apCreateApple[nCntApple] = nullptr;
	}

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	CSound* pSound = nullptr;
	CGameScene* pGame = nullptr;

	//サウンドの取得
	if (pManager != nullptr) pSound = pManager->GetSound();
	//ゲームの取得
	if (pManager != nullptr) pGame = pManager->GetGameScene();

	//死亡音の再生
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::DEAD_TREE);
	//ゲームオーバー設定
	if (pGame != nullptr) pGame->GameOver();
}

//=============================================================================
// 成長
//=============================================================================
void CAppleTree::GrowUp(void) {
	//すでに生成済みの場合か死亡している場合終了
	if (m_pMenuApple != nullptr || m_bDead) return;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//ゲームの取得
	CGameScene* pGame = pManager->GetGameScene();
	if (pGame == nullptr) return;
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//ゲーム終了後の場合終了
	if (pGame->GetGameClear() || pGame->GetGameOver()) return;

	//メニューを生成
	m_pMenuApple = CAppleMenu::Create(D3DXVECTOR3(0.0f, 2000.0f, 3000.0f), this);
	//成長音
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::GROW_UP);
}

//=============================================================================
// 林檎生成位置の取得
//=============================================================================
D3DXVECTOR3 CAppleTree::GetOffsetPosApple(int nIdxApple) {
	switch (nIdxApple)
	{

	case 0:
		return D3DXVECTOR3(-60.97f, 181.21f, 26.97f);
		break;

	case 1:
		return D3DXVECTOR3(169.03f, 264.21f, -43.97f);
		break;

	case 2:
		return D3DXVECTOR3(-120.97f, 477.21f, -133.97f);
		break;

	case 3:
		return D3DXVECTOR3(254.03f, 537.21f, 154.03f);
		break;

	case 4:
		return D3DXVECTOR3(-103.97f, 583.21f, 104.03f);
		break;

	case 5:
		return D3DXVECTOR3(-227.97f, 479.21f, -51.97f);
		break;


	default:
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;
	}
}

//=============================================================================
// 林檎の生成
//=============================================================================
void CAppleTree::CreateApple(void) {
	//林檎の種類を取得
	CGlowApple::APPLE_TYPE typeApple = m_pMenuApple->GetSelectAppleType();
	//林檎の位置を取得
	D3DXVECTOR3 posCreate = GetPos() + GetOffsetPosApple(m_nNumApple);	//木の位置 + オフセット

	CGlowApple* pCreateApple = nullptr;	//生成したリンゴ
	//林檎を生成
	switch (typeApple)
	{
	case CGlowApple::APPLE_TYPE::RED:
		pCreateApple = CAppleRed::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		pCreateApple = CAppleGreen::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::WHITE:
		pCreateApple = CAppleWhite::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		pCreateApple = CAppleBlack::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		pCreateApple = CAppleSilver::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		pCreateApple = CAppleGold::Create(posCreate, this);
		break;
	}

	//生成時にエフェクトを生成
	CParticleEffect::PARTICLE_INFO particleInfo;	//パーティクル情報
	particleInfo.addMove = D3DXVECTOR3(0.0f, -0.8f, 0.0f); particleInfo.colEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); particleInfo.colStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	particleInfo.fAddSize = -0.8f; particleInfo.fSizeStart = 40.0f; particleInfo.fSpeedMove = 10.0f; particleInfo.nLife = 60;
	//数個重ねて生成
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		CParticleEffect::Create(particleInfo, posCreate, 1, 1, 0.1f * D3DX_PI, true);
	}

	//生成したリンゴを配列に登録
	if (m_nNumApple >= 0 && m_nNumApple < MAX_NUM_CREATE_APPLE) {
		m_apCreateApple[m_nNumApple] = pCreateApple;
		m_aTypeCreateApple[m_nNumApple] = typeApple;
	}
	//林檎の生成数を加算
	m_nNumApple++;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	CSound* pSound = nullptr;	//サウンドへのポインタ
	//サウンドの取得
	if (pManager != nullptr) pSound = pManager->GetSound();
	//生成音の再生
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CREATE_APPLE);

	//メニューを閉じる
	if (m_pMenuApple != nullptr) {
		m_pMenuApple->Uninit();
		m_pMenuApple = nullptr;
	}

	if (m_nNumApple < MAX_NUM_CREATE_APPLE) {
		//使用した成長度が差し引かれる
		m_nGrowValue -= m_nGrowValueMax;
		//成長の必要量を増やす
		m_nGrowValueMax *= FOLD_GROW_VALUE;
		if (m_pGaugeGrow != nullptr) {
			//成長ゲージの設定
			m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
			m_pGaugeGrow->SetMaxValue(m_nGrowValueMax);
		}
		//差し引かれた成長量が必要量を超えていた場合、連続で成長させるために0の成長量を足す
		AddGrow(0);
	}
	else {
		//成長ゲージのの色を変更
		if (m_pGaugeGrow != nullptr) {
			m_pGaugeGrow->SetGaugeColor(D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
		}
	}
}


//=============================================================================
// 林檎の木の当たり判定の情報の取得
//=============================================================================
void CAppleTree::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//必要なポインタがnullの場合終了
	if (pNumCol == nullptr || pRadiusCol == nullptr || ppPosColArray == nullptr) return;

	//posのポインタがすでに存在している場合終了
	if (*ppPosColArray != nullptr) return;

	//当たり判定の数を設定
	*pNumCol = NUM_COLLISION;
	//当たり判定の半径を設定
	*pRadiusCol = COLLISION_RADIUS;
	//当たり判定の位置の配列を設定
	*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];	//動的確保

	for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
	{
		(*ppPosColArray)[nCntCol] = D3DXVECTOR3(GetPos().x, GetPos().y + 10.0f * nCntCol, GetPos().z);
	}
}

//=============================================================================
// 体力の最大値の設定
//=============================================================================
void CAppleTree::SetMaxLife(int nMaxLife) {
	int nDeltaLife = nMaxLife - m_nMaxLife;	//体力の差分
	if (nDeltaLife < 0) nDeltaLife = 0;	//マイナスの場合変更なし

	//最大値を設定
	m_nMaxLife = nMaxLife;	
	//ゲージに差分を足して設定
	m_nLife += nDeltaLife;

	if (m_pGaugeLife != nullptr) {
		//体力ゲージの最大値の設定
		m_pGaugeLife->SetMaxValue(nMaxLife);
		//体力ゲージの設定
		m_pGaugeLife->SetGaugeValue(m_nLife);
	}
}

//=============================================================================
// 体力の最大値の増加
//=============================================================================
void CAppleTree::AddMaxLife(int nAddLife) {
	SetMaxLife(m_nMaxLife + nAddLife);
}

//=============================================================================
// 体力を回復する
//=============================================================================
void CAppleTree::HealLife(int nHeal) {
	m_nLife += nHeal;
	if (m_nLife > m_nMaxLife) m_nLife = m_nMaxLife;

	if (m_pGaugeLife != nullptr) {
		//体力ゲージの設定
		m_pGaugeLife->SetGaugeValue(m_nLife);
	}
}

//=============================================================================
// 成長度を増やす
//=============================================================================
void CAppleTree::AddGrow(int nAddValue) {
	//生成上限に達している場合終了
	if (m_nNumApple >= MAX_NUM_CREATE_APPLE) return;

	m_nGrowValue += nAddValue;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//経験値取得音の再生
	if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::GETEXP);

	//成長条件を満たした場合
	if (m_nGrowValue >= m_nGrowValueMax) {
		//成長
		GrowUp();
	}

	if (m_pGaugeGrow != nullptr) {
		//成長ゲージの設定
		m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
	}
}

//=============================================================================
// リンゴの種類の取得
//=============================================================================
CGlowApple::APPLE_TYPE CAppleTree::GetCreateAppleType(int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_NUM_CREATE_APPLE) return CGlowApple::APPLE_TYPE::RED;

	return m_aTypeCreateApple[nIdx];
}