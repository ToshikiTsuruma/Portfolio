//=============================================================================
//
// タイトルシーン処理 [titleScene.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "titleScene.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "object2D.h"
#include "fade.h"
#include "selectMenu2D.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTitleScene::CTitleScene()
{
	m_pSelectMenuTitle = CSelectMenu2D::Create(3, false);
	if (m_pSelectMenuTitle != nullptr) {
		//縦選択
		m_pSelectMenuTitle->SetSelectType(CSelectMenu::SELECT_TYPE::VERTICAL);
		//選択肢UIの詳細設定
		m_pSelectMenuTitle->SetSelectUI(0, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 470.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_GAMESTART);
		m_pSelectMenuTitle->SetSelectUI(1, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 550.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_TUTORIAL);
		m_pSelectMenuTitle->SetSelectUI(2, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 630.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_CREDIT);
		//選択肢アイコンの生成
		m_pSelectMenuTitle->CreateSelectIcon(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), 50.0f, 50.0f, CTexture::TEXTURE_TYPE::SELECT_ICON);
		m_pSelectMenuTitle->SetIconPosOffset(1, D3DXVECTOR3(-160.0f, 0.0f, 0.0f));
		m_pSelectMenuTitle->SetIconPosOffset(2, D3DXVECTOR3(-140.0f, 0.0f, 0.0f));
	}
	m_pTutorial = nullptr;
	m_pCreadit = nullptr;
	m_bBeginFade = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitleScene::~CTitleScene()
{

}

//=============================================================================
// タイトルシーンの初期化処理
//=============================================================================
void CTitleScene::Init(void) {
	//オブジェクトのポーズが無いように設定
	CObject::SetUpdatePauseLevel(0);

	CObject2D* pTitleBG = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::BG_TITLE , SCREEN_WIDTH, SCREEN_HEIGHT);
	if (pTitleBG != nullptr) pTitleBG->SetDrawPriority(CObject::DRAW_PRIORITY::UI_BG);

	CObject2D::Create(D3DXVECTOR3(120.0f, 40.0f, 0.0f), CTexture::TEXTURE_TYPE::QUIT_GAME, 200.0f, 40.0f);
	CObject2D* pTitleName = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_TITLENAME, 800.0f, 200.0f);

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//BGMの再生
	if (pSound != nullptr) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_TITLE);
		pSound->SetBGM(CSound::SOUND_LABEL::BGM_TITLE);
	}
}

//=============================================================================
// タイトルシーンの終了処理
//=============================================================================
void CTitleScene::Uninit(void) {
	//シーンの終了処理
	CScene::Uninit();

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//音の停止
	if (pSound != nullptr) {
		pSound->StopSound();
	}
}

//=============================================================================
// タイトルシーンの更新処理
//=============================================================================
void CTitleScene::Update(void) {
	//シーン遷移開始後は更新なし
	if (m_bBeginFade) return;

	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	CInput* pInput = nullptr;	//入力デバイスへのポインタ
	CFade* pFade = nullptr;		//フェードへのポインタ
	CSound* pSound = nullptr;	//サウンドへのポインタ

	if (pManager != nullptr) {
		//現在の入力デバイスの取得
		pInput = pManager->GetInputCur();
		//フェードの取得
		pFade = pManager->GetFade();
		//サウンドの取得
		pSound = pManager->GetSound();
	}

	if (pInput == nullptr || pFade == nullptr) return;


	//決定キーを押したとき
	if (pInput->GetTrigger(CInput::CODE::SELECT) && m_pSelectMenuTitle != nullptr) {
		//フェード中だった場合
		if (pFade->GetFade()) {
			//フェードをスキップ
			pFade->SkipFade();
		}
		//チュートリアル画面が表示されているとき
		else if (m_pTutorial != nullptr) {
			//チュートリアル画面を閉じる
			m_pTutorial->Uninit();
			m_pTutorial = nullptr;
			//選択のロックを解除
			m_pSelectMenuTitle->SetLockChangeSelect(false);
			//決定音の再生
			if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
		}
		//クレジット画面が表示されているとき
		else if (m_pCreadit != nullptr) {
			//クレジット画面を閉じる
			m_pCreadit->Uninit();
			m_pCreadit = nullptr;
			//選択のロックを解除
			m_pSelectMenuTitle->SetLockChangeSelect(false);
			//決定音の再生
			if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
		}
		//他の画面が表示されていないとき
		else {
			//選択メニューの現在の選択肢を取得
			int nIdxSelect = m_pSelectMenuTitle->GetIdxCurSelect();
			switch (nIdxSelect)
			{
				//ゲームスタート
			case 0:
				//シーン遷移開始
				m_bBeginFade = true;
				if (pFade != nullptr) pFade->SetFade(CScene::SCENE_TYPE::GAME, 0.02f, 60);
				//決定音の再生
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_START);
				//メニューの選択をロック
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;

				//チュートリアル画面
			case 1:
				//チュートリアル画面の生成
				m_pTutorial = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::TUTORIAL, SCREEN_WIDTH, SCREEN_HEIGHT);
				//決定音の再生
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
				//メニューの選択をロック
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;

				//クレジット画面
			case 2:
				//クレジット画面の生成
				m_pTutorial = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::CREDIT, SCREEN_WIDTH, SCREEN_HEIGHT);
				//決定音の再生
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
				//メニューの選択をロック
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;
			}
		}
	}
}