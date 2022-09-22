//=============================================================================
//
// 生贄の処理 [scapegoat.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "scapegoat.h"
#include "manager.h"
#include "sound.h"
#include "effect.h"
#include "gauge3D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DEFAULT_MAX_LIFE (666)		//体力の最大値

#define NUM_COLLISION (8)			//当たり判定の数
#define COLLISION_RADIUS (20.0f)	//当たり判定の半径

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CScapegoat::CScapegoat() : CObjectModel(CModel::MODELTYPE::OBJ_SCAPEGOAT, false)
{
	SetObjType(OBJTYPE_SCAPEGOAT);	//オブジェクトタイプの設定
	SetEnableCollision(true);		//当たり判定有効

	m_nLife = DEFAULT_MAX_LIFE;

	//体力ゲージの生成
	m_pGaugeLife = CGauge3D::Create(DEFAULT_MAX_LIFE, false, m_nLife, FPS * 20, false);
	//ゲージの背景の設定
	m_pGaugeLife->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55.0f, 7.0f);
	m_pGaugeLife->SetGaugeBGColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	//ゲージの設定
	m_pGaugeLife->CreateGauge(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, 4.0f);
	m_pGaugeLife->SetGaugeColor(D3DXCOLOR(0.1f, 1.0f, 0.0f, 1.0f));
	//ゲージを非表示
	m_pGaugeLife->SetGaugeDraw(false);
}

//=============================================================================
// デストラクタ
//=============================================================================
CScapegoat::~CScapegoat()
{

}

//=============================================================================
// 生贄の生成処理
//=============================================================================
CScapegoat* CScapegoat::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot) {
	CScapegoat* pScapegoat;
	pScapegoat = new CScapegoat();
	if (pScapegoat == nullptr) return nullptr;

	pScapegoat->SetPos(pos);
	pScapegoat->SetRot(rot);
	if (pScapegoat->m_pGaugeLife != nullptr) {
		pScapegoat->m_pGaugeLife->SetAllGaugePos(pos + D3DXVECTOR3(0.0f, 200.0f, 0.0f));
	}
	pScapegoat->Init();

	return pScapegoat;
}

//=============================================================================
// 生贄の初期化処理
//=============================================================================
HRESULT CScapegoat::Init(void) {
	CModel* pModel = GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetColorGlow(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));
	}

	CObjectModel::Init();

	return S_OK;
}

//=============================================================================
// 生贄の終了処理
//=============================================================================
void CScapegoat::Uninit(void) {
	CObjectModel::Uninit();
}

//=============================================================================
// 生贄の更新処理
//=============================================================================
void CScapegoat::Update(void) {
	CObjectModel::Update();
}

//=============================================================================
// 生贄の描画処理
//=============================================================================
void CScapegoat::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// 生贄のダメージ処理
//=============================================================================
void CScapegoat::Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead) {
	//ダメージ
	m_nLife -= nDamage;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	D3DXVECTOR3 posEffect = GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f);	//エフェクト生成位置のオフセット
	float afRandPos[3];	//オフセットに加算するランダムな値
	//オフセットをランダムでずらす
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		int nRandPos = rand() % 30 - 15;
		afRandPos[nCnt] = (float)nRandPos;
	}
	//オフセットにランダムな値を加算
	posEffect.x += afRandPos[0];
	posEffect.y += afRandPos[1];
	posEffect.z += afRandPos[2];

	//攻撃のタイプごとの処理
	switch (typeDamage)
	{
	case DAMAGE_TYPE::ENEMY_PUNCH:
		//ダメージエフェクトの生成
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::DAMAGE_ENEMY, 40.0f, 40.0f, false);
		//ダメージ音の再生
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		break;
	case DAMAGE_TYPE::FIRE:
		//ダメージエフェクトの生成
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::FIRE, 80.0f, 80.0f, false);
		break;
	case DAMAGE_TYPE::PLAYER_PUNCH:
		//ダメージエフェクトの生成
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::DAMAGE_PLAYER, 40.0f, 40.0f, false);
		//ダメージ音の再生
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		break;
	case DAMAGE_TYPE::THUNDERBOLT:
		//ダメージエフェクトの生成
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::THUNDER, 50.0f, 50.0f, false);
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
// 生贄の死亡処理
//=============================================================================
void CScapegoat::Dead(void) {
	//当たり判定を無効
	SetEnableCollision(false);

	//体力ゲージの破棄
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//音の再生
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::EXPLOSION);

	//爆発エフェクト
	CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 80.0f, 0.0f), CEffect::EFFECT_TYPE::DEATH, 100.0f, 100.0f, false);

	//オブジェクトの破棄
	Uninit();
}

//=============================================================================
// 生贄の当たり判定の情報の取得
//=============================================================================
void CScapegoat::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
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
		(*ppPosColArray)[nCntCol] = D3DXVECTOR3(GetPos().x, GetPos().y + COLLISION_RADIUS * 0.5f * nCntCol, GetPos().z);
	}
}