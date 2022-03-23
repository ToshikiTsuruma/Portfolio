//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "scene2D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DEFAULT_COLOR D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f)
#define SELECT_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define FADE_SPEED (0.01f)	//フェードする速度

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTitle::CTitle() {
	for (int nCnt = 0; nCnt < (int)SELECT_TYPE::ENUM_MAX; nCnt++) {
		m_apScene2D[nCnt] = NULL;
	}
	m_bFade = false;
	m_pTutorial = NULL;
	m_bTutorial = false;
	m_typeTutorial = TUTORIAL_TYPE::CONTROLL;
	m_pCredit = NULL;
	m_bCredit = false;
	m_nSelect = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle() {

}

//=============================================================================
// タイトルの初期化処理
//=============================================================================
HRESULT CTitle::Init(void) {
	//オブジェクトのインスタンス生成
	//ポリゴン生成
	CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f), CTexture::TEXTURE_TYPE::GAME_TITLE, 720.0f, 150.0f);	//タイトル名
	CScene2D::Create(D3DXVECTOR3(130.0f, 40.0f, 0.0f), CTexture::TEXTURE_TYPE::QUIT_GAME, 230.0f, 50.0f);					//ゲーム終了アイコン
	//背景の生成
	CScene2D* pBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::BG_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
	pBg->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//描画優先度を背景として設定
	//選択肢の生成
	m_apScene2D[(int)SELECT_TYPE::START] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 480.0f, 0.0f), CTexture::TEXTURE_TYPE::TITLE_SELECT_START, 400.0f, 80.0f);		//ゲームスタート
	m_apScene2D[(int)SELECT_TYPE::TUTORIAL] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 560.0f, 0.0f), CTexture::TEXTURE_TYPE::TITLE_SELECT_TUTORIAL, 350.0f, 50.0f);	//チュートリアル
	m_apScene2D[(int)SELECT_TYPE::CREDIT] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 620.0f, 0.0f), CTexture::TEXTURE_TYPE::TITLE_SELECT_CREDIT, 350.0f, 50.0f);		//クレジット	
	//ポリゴンの色の初期化
	m_apScene2D[(int)SELECT_TYPE::START]->SetColor(SELECT_COLOR);
	m_apScene2D[(int)SELECT_TYPE::TUTORIAL]->SetColor(DEFAULT_COLOR);
	m_apScene2D[(int)SELECT_TYPE::CREDIT]->SetColor(DEFAULT_COLOR);
	//メンバ変数の初期化
	m_bFade = false;
	m_bTutorial = false;
	m_bCredit = false;
	m_nSelect = (int)SELECT_TYPE::START;

	//サウンドの取得
	CManager* pManager = nullptr;
	pManager = CManager::GetManager();
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//曲の再生
	if (pSound != NULL) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_TITLE);
		CSound::SetBGM(CSound::SOUND_LABEL::BGM_TITLE);
	}

	return S_OK;
}

//=============================================================================
// タイトルの終了処理
//=============================================================================
void CTitle::Uninit(void) {
	//オブジェクトの破棄
	CScene::ReleaseAll();

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//曲の停止
	if (pSound != nullptr) {
		pSound->StopSound(CSound::GetBGM());
		pSound->SetBGM(CSound::SOUND_LABEL::NONE);
	}
}

//=============================================================================
// タイトルの更新処理
//=============================================================================
void CTitle::Update(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

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
	if (pInput != nullptr && pFade != nullptr) {
		//タイトル開始時のフェード中の処理
		if (pFade->GetFade() == true && m_bFade == false) {
			if (pInput->GetTrigger(CInput::CODE::SELECT)) {
				pFade->SkipFade();
			}
		}
		//フェード中ではない時
		else {
			//----------------------------------------
			//戻るボタン押下時
			//----------------------------------------
			if (pInput->GetTrigger(CInput::CODE::BACK) && m_bFade == false) {
				if (m_bTutorial == true) {
					//チュートリアルを閉じる
					if (m_pTutorial != NULL) {
						m_pTutorial->Uninit();
						//効果音の再生
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
						m_pTutorial = NULL;
					}
					m_bTutorial = false;
				}
				else if (m_bCredit == true) {
					//クレジットを閉じる
					if (m_pCredit != NULL) {
						m_pCredit->Uninit();
						//効果音の再生
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
						m_pCredit = NULL;
					}
					m_bCredit = false;
				}
			}

			//----------------------------------------
			//選択切り替え
			//----------------------------------------
			if (m_bTutorial == false && m_bCredit == false && m_bFade == false) {
				//上キー
				if (pInput->GetTrigger(CInput::CODE::UP)) {
					//選択から外れるポリゴンの色を戻す
					m_apScene2D[m_nSelect]->SetColor(DEFAULT_COLOR);
					//効果音の再生
					if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_SWITCH);
					//選択を変更
					if (m_nSelect <= 0) {
						m_nSelect = (int)SELECT_TYPE::ENUM_MAX - 1;
					}
					else {
						m_nSelect--;
					}
					//選択中のポリゴンの色を変える
					m_apScene2D[m_nSelect]->SetColor(SELECT_COLOR);
				}
				//下キー
				if (pInput->GetTrigger(CInput::CODE::DOWN)) {
					//選択から外れるポリゴンの色を戻す
					m_apScene2D[m_nSelect]->SetColor(DEFAULT_COLOR);
					//効果音の再生
					if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_SWITCH);
					//選択を変更
					if (m_nSelect >= (int)SELECT_TYPE::ENUM_MAX - 1) {
						m_nSelect = 0;
					}
					else {
						m_nSelect++;
					}
					//選択中のポリゴンの色を変える
					m_apScene2D[m_nSelect]->SetColor(SELECT_COLOR);
				}
			}

			//チュートリアル
			if (m_bTutorial == true && m_bCredit == false && m_bFade == false) {
				//左右キー
				if (pInput->GetTrigger(CInput::CODE::LEFT) || pInput->GetTrigger(CInput::CODE::RIGHT)) {
					if (pInput->GetTrigger(CInput::CODE::LEFT)) {
						//前のチュートリアル画面にする
						int nType = (int)m_typeTutorial - 1;
						if (nType < 0) nType = (int)TUTORIAL_TYPE::ENUM_MAX - 1;
						m_typeTutorial = (TUTORIAL_TYPE)nType;
					}
					if (pInput->GetTrigger(CInput::CODE::RIGHT)) {
						//次のチュートリアル画面にする
						int nType = (int)m_typeTutorial + 1;
						if (nType >= (int)TUTORIAL_TYPE::ENUM_MAX) nType = 0;
						m_typeTutorial = (TUTORIAL_TYPE)nType;
					}

					//テクスチャの決定
					CTexture::TEXTURE_TYPE typeTex = CTexture::TEXTURE_TYPE::TITLE_TUTORIAL_001;	//テクスチャの種類

					switch (m_typeTutorial)
					{
					case CTitle::TUTORIAL_TYPE::CONTROLL:
						typeTex = CTexture::TEXTURE_TYPE::TITLE_TUTORIAL_001;
						break;
					case CTitle::TUTORIAL_TYPE::GAME:
						typeTex = CTexture::TEXTURE_TYPE::TITLE_TUTORIAL_002;
						break;
					case CTitle::TUTORIAL_TYPE::BOSS:
						typeTex = CTexture::TEXTURE_TYPE::TITLE_TUTORIAL_003;
						break;
					}

					//別のチュートリアル画面を生成
					if (m_pTutorial != NULL) {
						m_pTutorial->Uninit();
						m_pTutorial = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), typeTex, SCREEN_WIDTH, SCREEN_HEIGHT);
						//効果音の再生
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_SWITCH);
					}
				}
			}

			//----------------------------------------
			//選択決定時
			//----------------------------------------
			if (pInput->GetTrigger(CInput::CODE::SELECT) && m_bFade == false) {
				//チュートリアル画面表示時
				if (m_bTutorial == true) {
					//チュートリアルを閉じる
					if (m_pTutorial != NULL) {
						m_pTutorial->Uninit();
						//効果音の再生
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
						m_pTutorial = NULL;
					}
					m_bTutorial = false;
				}
				//クレジット画面表示時
				else if (m_bCredit == true) {
					//クレジットを閉じる
					if (m_pCredit != NULL) {
						m_pCredit->Uninit();
						//効果音の再生
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
						m_pCredit = NULL;
					}
					m_bCredit = false;
				}
				//通常画面時
				else {
					switch (m_nSelect)
					{
						//ゲームスタート
					case (int)SELECT_TYPE::START:
						//効果音の再生
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_START);
						//ゲームのモードに切り替え
						if (pFade != NULL) {
							pFade->SetFade(CManager::MODE::GAME, FADE_SPEED);
							m_bFade = true;
						}
						break;
						//チュートリアル
					case (int)SELECT_TYPE::TUTORIAL:
						//効果音の再生
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
						//チュートリアル画面生成
						if (m_pTutorial == NULL) {
							m_pTutorial = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::TITLE_TUTORIAL_001, SCREEN_WIDTH, SCREEN_HEIGHT);
							m_bTutorial = true;
							m_typeTutorial = TUTORIAL_TYPE::CONTROLL;
						}
						break;
						//クレジット表記
					case (int)SELECT_TYPE::CREDIT:
						//効果音の再生
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
						//クレジット画面生成
						if (m_pCredit == NULL) {
							m_pCredit = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::TITLE_CREDIT, SCREEN_WIDTH, SCREEN_HEIGHT);
							m_bCredit = true;
						}
						break;
					}
				}
			}
		}
	}
}