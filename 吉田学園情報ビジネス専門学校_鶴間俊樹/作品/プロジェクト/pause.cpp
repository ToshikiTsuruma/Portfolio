//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "pause.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "scene2D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DEFAULT_COLOR D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f)
#define SELECT_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define FADE_SPEED (0.02f)	//フェードする速度

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CPause::CPause()
{
	m_pPauseBg = NULL;
	for (int nCnt = 0; nCnt < NUM_CHOICE; nCnt++) {
		m_apChoice[nCnt] = NULL;
	}
	m_bPause = false;
	m_nChoice = 0;
	m_bLockInput = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ポーズの初期化処理
//=============================================================================
HRESULT CPause::Init(void) {
	m_bPause = false;
	m_nChoice = 0;

	return S_OK;
}

//=============================================================================
// ポーズの終了処理
//=============================================================================
void CPause::Uninit(void) {

}

//=============================================================================
// ポーズの更新処理
//=============================================================================
void CPause::Update(void) {
	//ポーズ中ではない場合中断
	if (m_bPause == false) return;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	//サウンドの取得
	CInput* pInput = nullptr;
	CFade* pFade = nullptr;
	CSound *pSound = nullptr;

	if (pManager != nullptr) {
		//現在の入力デバイスの取得
		pInput = pManager->GetInputCur();
		//フェードの取得
		pFade = pManager->GetFade();
		//サウンドの取得
		pSound = pManager->GetSound();
	}

	//入力処理
	if (pInput != NULL && pFade != NULL && m_bLockInput == false && m_apChoice[0] != NULL && m_apChoice[1] != NULL && m_apChoice[2] != NULL) {
		if (pFade->GetFade() == false) {
			//----------------------------------------
			//選択切り替え
			//----------------------------------------
			//上キー
			if (pInput->GetTrigger(CInput::CODE::UP)) {
				//選択から外れるポリゴンの色を戻す
				m_apChoice[m_nChoice]->SetColor(DEFAULT_COLOR);
				//効果音の再生
				if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_SWITCH);
				//選択を変更
				if (m_nChoice <= 0) {
					m_nChoice = NUM_CHOICE - 1;
				}
				else {
					m_nChoice--;
				}
				//選択中のポリゴンの色を変える
				m_apChoice[m_nChoice]->SetColor(SELECT_COLOR);
			}
			//下キー
			if (pInput->GetTrigger(CInput::CODE::DOWN)) {
				//選択から外れるポリゴンの色を戻す
				m_apChoice[m_nChoice]->SetColor(DEFAULT_COLOR);
				//効果音の再生
				if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_SWITCH);
				//選択を変更
				if (m_nChoice >= NUM_CHOICE - 1) {
					m_nChoice = 0;
				}
				else {
					m_nChoice++;
				}
				//選択中のポリゴンの色を変える
				m_apChoice[m_nChoice]->SetColor(SELECT_COLOR);
			}


			//----------------------------------------
			//選択決定時
			//----------------------------------------
			if (pInput->GetTrigger(CInput::CODE::SELECT)) {
				switch (m_nChoice)
				{
				//ゲーム再開
				case 0:
					//効果音の再生
					if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::PAUSE_SELECT);
					SetPause(false);	//ポーズの終了
					break;

				//リトライ
				case 1:
					//効果音の再生
					if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::PAUSE_SELECT);

					//ゲームを再ロード
					if (pFade != NULL) {
						pFade->SetFade(CManager::MODE::GAME, FADE_SPEED);
					}
					break;

				//終了
				case 2:
					//効果音の再生
					if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::PAUSE_SELECT);

					//タイトルに切り替え
					if (pFade != NULL) {
						pFade->SetFade(CManager::MODE::TITLE, FADE_SPEED);
					}
					break;
				}
			}//choice
		}//fade
	}//NULL

	 //入力処理のロックを解除
	if (m_bLockInput == true)m_bLockInput = false;
}

//=============================================================================
// ポーズ状態の設定
//=============================================================================
void CPause::SetPause(bool bPause) {
	m_bPause = bPause;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	if (bPause == true) {
		m_nChoice = 0;
		m_bLockInput = true;	//入力処理をロック
		//効果音の再生
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::PAUSE);

		//背景ポリゴンの生成
		m_pPauseBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::PAUSE_BG, SCREEN_WIDTH, SCREEN_HEIGHT);
		//背景ポリゴンの設定
		if (m_pPauseBg != NULL) {
			m_pPauseBg->SetDrawPriority(CScene2D::DRAW_PRIORITY::FRONT);	//描画順を最前面
		}

		//選択肢ポリゴンの生成
		m_apChoice[0] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f , 0.0f), CTexture::TEXTURE_TYPE::PAUSE_START, 280.0f, 50.0f);
		m_apChoice[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 400.0f , 0.0f), CTexture::TEXTURE_TYPE::PAUSE_RETRY, 280.0f, 50.0f);
		m_apChoice[2] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 500.0f , 0.0f), CTexture::TEXTURE_TYPE::PAUSE_QUIT, 280.0f, 50.0f);
		//選択肢ポリゴンの設定
		for (int nCnt = 0; nCnt < NUM_CHOICE; nCnt++) {
			if (m_apChoice[nCnt] != NULL) {
				m_apChoice[nCnt]->SetDrawPriority(CScene2D::DRAW_PRIORITY::FRONT);	//描画順を最前面
				m_apChoice[nCnt]->SetColor(DEFAULT_COLOR);	//未選択時の色の変更
			}
		}
		//選択中のポリゴンの色を変更
		if (m_apChoice[m_nChoice] != NULL) {
			m_apChoice[m_nChoice]->SetColor(SELECT_COLOR);
		}

		//曲の一時停止
		if (pSound != nullptr) pSound->PauseSound(CSound::GetBGM());
	}
	else if (bPause == false) {
		//背景ポリゴンの破棄
		if (m_pPauseBg != NULL) {
			m_pPauseBg->Uninit();
			m_pPauseBg = NULL;
		}
		//選択肢ポリゴンの破棄
		for (int nCnt = 0; nCnt < NUM_CHOICE; nCnt++) {
			if (m_apChoice[nCnt] != NULL) {
				m_apChoice[nCnt]->Uninit();
				m_apChoice[nCnt] = NULL;
			}
		}

		//曲の再生
		if (pSound != nullptr) pSound->StartSound(CSound::GetBGM());
	}
}

//=============================================================================
// ポーズ状態の取得
//=============================================================================
bool CPause::GetPause(void) {
	return m_bPause;
}