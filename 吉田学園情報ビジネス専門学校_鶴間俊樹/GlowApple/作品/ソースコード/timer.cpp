//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "timer.h"
#include "manager.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTimer::CTimer() : CScore()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CTimer::CTimer(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize) : CScore(nNumDigit, type, pos, fSize)
{
	m_bStop = false;
	m_nCntTime = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
// タイマーの生成処理
//=============================================================================
CTimer* CTimer::Create(int nStartTime, int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize) {
	CTimer* pTimer = nullptr;
	pTimer = new CTimer(nNumDigit, type, pos, fSize);
	if (pTimer == nullptr) return nullptr;

	pTimer->Init();
	pTimer->SetScore(nStartTime);

	return pTimer;
}

//=============================================================================
// タイマーの初期化処理
//=============================================================================
HRESULT CTimer::Init(void) {
	//カウンタの設定
	m_nCntTime = 0;

	CScore::Init();

	return S_OK;
}

//=============================================================================
// タイマーの終了処理
//=============================================================================
void CTimer::Uninit(void) {
	CScore::Uninit();
}

//=============================================================================
// タイマーの更新処理
//=============================================================================
void CTimer::Update(void) {
	//更新条件を見たしていない場合
	if (GetScore() <= 0 || m_bStop) return;

	//１秒立った場合
	if (m_nCntTime >= FPS) {
		//カウンタの再設定
		m_nCntTime = 0;
		//タイマーの減少
		AddScore(-1);
	}
	else {
		//タイマー更新のカウント増加
		m_nCntTime++;
	}

	CScore::Update();
}