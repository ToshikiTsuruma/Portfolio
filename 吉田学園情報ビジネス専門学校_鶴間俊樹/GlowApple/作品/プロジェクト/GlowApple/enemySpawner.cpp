//=============================================================================
//
// 敵スポナー処理 [enemySpawner.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "enemySpawner.h"
#include "manager.h"
#include "sound.h"
#include "enemy.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemySpawner::CEnemySpawner()
{
	m_nCntSpawn = 0;
	m_nSpanSpawn = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemySpawner::~CEnemySpawner()
{

}

//=============================================================================
// 敵スポナーの生成処理
//=============================================================================
CEnemySpawner* CEnemySpawner::Create(int nSpan, int nDistMin, int nDistMax) {
	CEnemySpawner* pEnemySpawner;
	pEnemySpawner = new CEnemySpawner();
	if (pEnemySpawner == nullptr) return nullptr;

	pEnemySpawner->m_nSpanSpawn = nSpan;
	pEnemySpawner->m_nDistEnemyMin = nDistMin;
	pEnemySpawner->m_nDistEnemyMax = nDistMax;
	pEnemySpawner->Init();

	return pEnemySpawner;
}

//=============================================================================
// 敵スポナーの初期化処理
//=============================================================================
HRESULT CEnemySpawner::Init(void) {
	m_nCntSpawn = 0;

	return S_OK;
}

//=============================================================================
// 敵スポナーの終了処理
//=============================================================================
void CEnemySpawner::Uninit(void) {
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 敵スポナーの更新処理
//=============================================================================
void CEnemySpawner::Update(void) {
	//生成タイミング
	if (m_nCntSpawn > m_nSpanSpawn) {
		m_nCntSpawn = 0;

		//敵をスポーンさせる
		CEnemy::SetEnemyCircle(D3DXVECTOR3(0.0f, 1500.0f, 0.0f), 1, 1800.0f, m_nDistEnemyMin, m_nDistEnemyMax);

		//マネージャーの取得
		CManager* pManager = CManager::GetManager();
		//サウンドの取得
		CSound* pSound = nullptr;
		if (pManager != nullptr) pSound = pManager->GetSound();
		//スポーン音の再生
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::SPAWN_ENEMY);
	}
	else {
		m_nCntSpawn++;
	}
}

//敵の生成間隔を増加
void CEnemySpawner::AddDistEnemy(int nDist) {
	m_nDistEnemyMin += nDist;
	m_nDistEnemyMax += nDist;
}