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
CTimer::CTimer() : m_nNumDigit(0)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CTimer::CTimer(int nNumDigit, CTexture::TEXTURE_TYPE type) : m_nNumDigit(nNumDigit)
{
	//桁数分ナンバークラスのインスタンスを生成
	m_pNumberArray = CNumber::Create(type, nNumDigit);

	m_bStop = false;
	m_nTime = 0;
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
CTimer* CTimer::Create(int nStartTime, int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize, float fSpace) {
	CTimer* pTimer;
	pTimer = new CTimer(nNumDigit, type);
	if (pTimer != nullptr) {
		//ナンバーの初期化
		if (pTimer->m_pNumberArray != nullptr) {
			for (int nCnt = 0; nCnt < pTimer->m_nNumDigit; nCnt++) {
				pTimer->m_pNumberArray[nCnt].Init(D3DXVECTOR3(pos.x - nCnt * fSpace, pos.y, pos.z), fSize);
			}
		}
		pTimer->m_nTime = nStartTime;
		pTimer->Init();
	}

	return pTimer;
}

//=============================================================================
// タイマーの初期化処理
//=============================================================================
HRESULT CTimer::Init(void) {
	SetDrawPriority(DRAW_PRIORITY::UI);	//描画順の設定
	//カウンタの設定
	m_nCntTime = 0;
	//数字の設定
	if (m_pNumberArray != nullptr) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetNumber((m_nTime % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
		}
	}
	return S_OK;
}

//=============================================================================
// タイマーの終了処理
//=============================================================================
void CTimer::Uninit(void) {
	if (m_pNumberArray != nullptr) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Uninit();
		}
		delete[] m_pNumberArray;
		m_pNumberArray = nullptr;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// タイマーの更新処理
//=============================================================================
void CTimer::Update(void) {
	//更新条件を見たしていない場合
	if (m_pNumberArray == nullptr || m_nTime <= 0 || m_bStop) return;

	//１秒立った場合
	if (m_nCntTime >= FPS) {
		//カウンタの再設定
		m_nCntTime = 0;
		//タイマーの減少
		m_nTime--;
		//タイマーの更新
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetNumber((m_nTime % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
		}
	}
	else {
		//タイマー更新のカウント増加
		m_nCntTime++;
	}

	//ナンバーの更新処理
	for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
		m_pNumberArray[nCnt].Update();
	}

}

//=============================================================================
// タイマーの描画処理
//=============================================================================
void CTimer::Draw(void) {
	if (m_pNumberArray != nullptr) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Draw();
		}
	}
}

//=============================================================================
// タイマーの時間の追加
//=============================================================================
void CTimer::AddTime(int nTime) {
	//時間の追加
	m_nTime += nTime;
	//タイマーの更新
	if (m_pNumberArray != nullptr) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetNumber((m_nTime % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
		}
	}
}

//=============================================================================
// タイマーの残り時間を取得
//=============================================================================
int CTimer::GetTime(void) {
	return m_nTime;
}

//=============================================================================
// タイマーの停止状態を設定
//=============================================================================
void CTimer::SetStop(bool bStop) {
	m_bStop = bStop;
}