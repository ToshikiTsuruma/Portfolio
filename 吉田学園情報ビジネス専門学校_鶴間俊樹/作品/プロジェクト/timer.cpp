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
// 静的メンバ変数宣言
//=============================================================================
#define DEFAULT_ALERT_TIME (30)	//デフォルトの警告時間
#define COLOR_ALERT_TIME (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//タイムリミットが近づいたとき警告がなってからの数字の色

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
	if (pTimer != NULL) {
		//ナンバーの初期化
		if (pTimer->m_pNumberArray != NULL) {
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
	SetDrawPriority(CScene::DRAW_PRIORITY::UI);	//描画順の設定
	//カウンタの設定
	m_nCntTime = FPS;
	//数字の設定
	if (m_pNumberArray != NULL) {
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
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Uninit();
		}
		delete[] m_pNumberArray;
		m_pNumberArray = NULL;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// タイマーの更新処理
//=============================================================================
void CTimer::Update(void) {
	if (m_pNumberArray != NULL && m_nTime > 0 && m_bStop == false) {
		//ナンバーの更新処理
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Update();
		}
		
		if (m_nCntTime <= 0) {
			//カウンタの再設定
			m_nCntTime = FPS;	
			//タイマーの減少
			m_nTime--;
			//タイマーの更新
			for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
				m_pNumberArray[nCnt].SetNumber((m_nTime % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
			}
			//警告時間になった場合
			if (m_nTime == DEFAULT_ALERT_TIME) {
				//マネージャーの取得
				CManager* pManager = CManager::GetManager();
				//サウンドの取得
				CSound *pSound = nullptr;
				if (pManager != nullptr) pSound = pManager->GetSound();
				//曲の変更
				if (pSound != nullptr) {
					//警告音の再生
					pSound->PlaySound(CSound::SOUND_LABEL::TIME_ALERT);
				}
				//色の変更
				for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
					m_pNumberArray[nCnt].SetColor(COLOR_ALERT_TIME);
				}
			}
		}
		else {
			//タイマーのカウント減少
			m_nCntTime--;
		}
	}
}

//=============================================================================
// タイマーの描画処理
//=============================================================================
void CTimer::Draw(void) {
	if (m_pNumberArray != NULL) {
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
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetNumber((m_nTime % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
		}
	}
	//タイマーの色を戻す
	if (m_nTime > DEFAULT_ALERT_TIME) {
		//色の変更
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
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