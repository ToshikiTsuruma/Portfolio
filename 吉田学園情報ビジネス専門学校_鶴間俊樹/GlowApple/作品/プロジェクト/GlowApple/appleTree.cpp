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

#include "apple_red.h"
#include "apple_green.h"
#include "apple_silver.h"
#include "apple_gold.h"
#include "apple_black.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_NUM_APPLE (5)	//生成可能な林檎の最大数

#define NUM_COLLISION (5)			//当たり判定の数
#define COLLISION_RADIUS (50.0f)	//当たり判定の半径

#define MAX_LIFE_DEFAULT (200)			//体力の最大値
#define DANGER_LIFE ((int)(MAX_LIFE_DEFAULT * 0.2f))		//体力の危険値
#define COLOR_LIFE_GAUGE_SAFE (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//体力バーの安全時の色
#define COLOR_LIFE_GAUGE_DANGER (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//体力バーの危険時の色

#define MAX_GROW_VALUE (15)	//成長度の最大

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
	m_pMenuApple = nullptr;
	m_nMaxLife = MAX_LIFE_DEFAULT;
	m_nLife = MAX_LIFE_DEFAULT;
	m_nGrowValue = 0;
	m_bDead = false;

	m_pGaugeLife = CGauge2D::Create(MAX_LIFE_DEFAULT, MAX_LIFE_DEFAULT, 0);
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(150.0f + 20.0f, 30.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeLife->SetGaugeBGColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
		m_pGaugeLife->CreateGauge(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(150.0f + 20.0f, 30.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeLife->SetGaugeColor(COLOR_LIFE_GAUGE_SAFE);
		m_pGaugeLife->SetGaugeColorDanger(COLOR_LIFE_GAUGE_DANGER);
		m_pGaugeLife->SetDangerValue(DANGER_LIFE);
		//最大値変動に合わせられないため枠なし
		//m_pGaugeLife->CreateGaugeFrame(CTexture::TEXTURE_TYPE::CREATE_FRAME, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f), 800.0f, 20.0f);
	}
	m_pGaugeGrow = CGauge2D::Create(MAX_GROW_VALUE, MAX_GROW_VALUE, 0);
	if (m_pGaugeGrow != nullptr) {
		m_pGaugeGrow->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(150.0f + 20.0f, 55.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeGrow->SetGaugeBGColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
		m_pGaugeGrow->CreateGauge(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(150.0f + 20.0f, 55.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeGrow->SetGaugeColor(D3DXCOLOR(0.0f, 0.4f, 1.0f, 1.0f));
		m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
		//m_pGaugeGrow->CreateGaugeFrame(CTexture::TEXTURE_TYPE::CREATE_FRAME, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f), 800.0f, 20.0f);
	}
	m_pIconHP = CObject2D::Create(D3DXVECTOR3(50.0f, 30.0f, 0.0f), CTexture::TEXTURE_TYPE::ICON_HP, 20.0f, 20.0f);

	SetObjType(OBJ_TYPE::APPLE_TREE);
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
void CAppleTree::Damage(int nDamage, bool* pDead) {
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
	//当たり判定を無効
	SetEnableCollision(false);

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
	//ゲーム終了後の場合終了
	if (pGame->GetGameClear() || pGame->GetGameOver()) return;

	//メニューを生成
	m_pMenuApple = CAppleMenu::Create(D3DXVECTOR3(0.0f, 2000.0f, 3000.0f), this);
}

//=============================================================================
// 林檎生成位置の取得
//=============================================================================
D3DXVECTOR3 CAppleTree::GetPosApple(void) {
	switch (m_nNumApple)
	{
	case 0:
		return D3DXVECTOR3(169.03f, 264.21f, -43.97f);
		break;

	case 1:
		return D3DXVECTOR3(-120.97f, 477.21f, -133.97f);
		break;

	case 2:
		return D3DXVECTOR3(254.03f, 537.21f, 154.03f);
		break;

	case 3:
		return D3DXVECTOR3(-103.97f, 583.21f, 104.03f);
		break;

	case 4:
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
	D3DXVECTOR3 posCreate = GetPos() + GetPosApple();	//木の位置 + オフセット
	//林檎を生成
	switch (typeApple)
	{
	case CGlowApple::APPLE_TYPE::RED:
		CAppleRed::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		CAppleGreen::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		CAppleSilver::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		CAppleGold::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		CAppleBlack::Create(posCreate, this);
		break;
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

	if (m_nNumApple < MAX_NUM_APPLE) {
		//成長度が差し引かれる
		m_nGrowValue -= MAX_GROW_VALUE;
		if (m_pGaugeGrow != nullptr) {
			//成長ゲージの設定
			m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
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
		(*ppPosColArray)[nCntCol] = D3DXVECTOR3(GetPos().x, GetPos().y + 20.0f * nCntCol, GetPos().z);
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
	m_nMaxLife = nMaxLife;

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
	m_nGrowValue += nAddValue;

	//成長条件を満たした場合
	if (m_nGrowValue >= MAX_GROW_VALUE) {
		//林檎が生成可能な場合
		if (m_nNumApple < MAX_NUM_APPLE) {
			GrowUp();
		}
		//林檎が生成不可能の場合
		else {
			m_nGrowValue = MAX_GROW_VALUE;
		}
	}

	if (m_pGaugeGrow != nullptr) {
		//成長ゲージの設定
		m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
	}
}