//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "renderer.h"
#include "model.h"
#include "fade.h"
#include "scene2D.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "pause.h"
#include "texture.h"
#include "model.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CManager* CManager::m_pManager = nullptr;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CManager::CManager() {
	m_pRenderer = nullptr;
	m_pInputCur = nullptr;
	m_pInputKeyboard = nullptr;
	m_pInputGamepadX = nullptr;
	m_pCamera = nullptr;
	m_apLight[NUM_LIGHT] = {};
	m_pSound = nullptr;
	m_pFade = nullptr;
	m_mode = MODE::TITLE;
	m_pTitle = nullptr;
	m_pGame = nullptr;
	m_pResult = nullptr;
	m_pPause = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager() {

}

//=============================================================================
// マネージャーの生成
//=============================================================================
void CManager::Create(void) {
	//生成されていない場合生成
	if (m_pManager == nullptr) m_pManager = new CManager;
}
 
//=============================================================================
// マネージャーの破棄
//=============================================================================
void CManager::Destroy(void) {
	if (m_pManager != nullptr) {
		delete m_pManager;
		m_pManager = nullptr;
	}
}

//=============================================================================
// マネージャーの取得 
//=============================================================================
CManager* CManager::GetManager(void) {
	return m_pManager;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow) {
	//乱数の種の初期化
	srand((unsigned)time(nullptr));

	//------------------------------
	//インスタンス生成
	//------------------------------
	//キーボードの生成
	m_pInputKeyboard = new CInputKeyboard;
	if (m_pInputKeyboard != nullptr) m_pInputKeyboard->Init(hInstance, hWnd);
	//ゲームパッドの生成(Xinput)
	m_pInputGamepadX = new CInputGamepadX;
	if (m_pInputGamepadX != nullptr) m_pInputGamepadX->Init();
	//現在の入力デバイスの設定
	if (m_pInputGamepadX != nullptr) {
		//ゲームパッドが接続されている場合
		if (m_pInputGamepadX->GetConnectGamepad()) {
			m_pInputCur = m_pInputGamepadX;
		}
		else {
			m_pInputCur = m_pInputKeyboard;
		}
	}
	//サウンドの生成
	m_pSound = new CSound;
	if (m_pSound != nullptr)m_pSound->Init(hWnd);
	//レンダラーのインスタンス生成
	m_pRenderer = new CRenderer;
	if (m_pRenderer != nullptr) m_pRenderer->Init(hWnd, bWindow);

	//------------------------------
	//ロード
	//------------------------------
	//テクスチャのロード
	CTexture::Load();
	//モデルのロード
	CModel::Load();

	//------------------------------
	//インスタンス生成
	//------------------------------
	//カメラの生成
	m_pCamera = CCamera::Create();
	//ライトの生成
	m_apLight[0] = CLight::Create(D3DXVECTOR3(-0.5f, -2.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apLight[1] = CLight::Create(D3DXVECTOR3(0.8f, 0.1f, 0.8f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
	m_apLight[2] = CLight::Create(D3DXVECTOR3(0.8f, -0.5f, -0.4f), D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
	//ポーズの生成
	m_pPause = new CPause;
	if (m_pPause != nullptr) m_pPause->Init();
	//フェードの生成
	m_pFade = new CFade;
	if (m_pFade != nullptr) m_pFade->Init();

	//モードの設定
	SetMode(m_mode);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void) {
	//オブジェクトの破棄
	CScene::ReleaseAll();

	//フェードの破棄
	if (m_pFade != nullptr) {
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}
	//ポーズの破棄
	if (m_pPause != nullptr) {
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}
	//ライトの破棄
	for (int nCnt = 0; nCnt < NUM_LIGHT; nCnt++) {
		if (m_apLight[nCnt] != nullptr) {
			m_apLight[nCnt]->Uninit();
			delete m_apLight[nCnt];
			m_apLight[nCnt] = nullptr;
		}
	}
	//カメラの破棄
	if (m_pCamera != nullptr) {
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	//モデルのアンロード
	CModel::Unload();
	//テクスチャのアンロード
	CTexture::Unload();
	//レンダラーの破棄
	if (m_pRenderer != nullptr) {
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
	//サウンドの破棄
	if (m_pSound != nullptr) {
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
	//キーボードの破棄
	if (m_pInputKeyboard != nullptr) {
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}
	//ゲームパッドの破棄(Xinput)
	if (m_pInputGamepadX != nullptr) {
		m_pInputGamepadX->Uninit();
		delete m_pInputGamepadX;
		m_pInputGamepadX = nullptr;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void) {
	//キーボードの更新処理
	if (m_pInputKeyboard != nullptr) {
		m_pInputKeyboard->Update();
	}
	//ゲームパッドの更新処理(Xinput)
	if (m_pInputGamepadX != nullptr) {
		m_pInputGamepadX->Update();
	}
	//現在の入力デバイスの設定
	if (m_pInputGamepadX != nullptr) {
		//ゲームパッドが接続されている場合
		if (m_pInputGamepadX->GetConnectGamepad()) {
			m_pInputCur = m_pInputGamepadX;
		}
		else {
			m_pInputCur = m_pInputKeyboard;
		}
	}

	if (m_pPause != nullptr) {
		//ポーズ中ではない場合のみ更新
		if (!m_pPause->GetPause()) {
			//カメラの更新
			if (m_pCamera != nullptr) m_pCamera->Update();

			//レンダラーの更新処理
			if (m_pRenderer != nullptr) {
				m_pRenderer->Update();
			}

			//現在のモードの更新処理
			switch (m_mode)
			{
			case MODE::TITLE:
				if (m_pTitle != nullptr) {
					m_pTitle->Update();
				}
				break;
			case MODE::GAME:
				if (m_pGame != nullptr) {
					m_pGame->Update();
				}
				break;
			case MODE::RESULT:
				if (m_pResult != nullptr) {
					m_pResult->Update();
				}
				break;
			}
		}

		//ポーズの更新処理
		m_pPause->Update();
	}

	//フェードの更新処理
	if (m_pFade != nullptr) {
		m_pFade->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void) {
	if (m_pRenderer != nullptr) {
		m_pRenderer->Draw();
	}
}

//=============================================================================
//モードの設定
//=============================================================================
void CManager::SetMode(MODE mode) {
	//現在のモードのオブジェクトを破棄
	switch (m_mode)
	{
	case MODE::TITLE:
		if (m_pTitle != nullptr) {
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = nullptr;
		}
		break;
	case MODE::GAME:
		if (m_pGame != nullptr) {
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = nullptr;
		}		
		break;
	case MODE::RESULT:
		if (m_pResult != nullptr) {
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = nullptr;
		}		
		break;
	}

	//次のモードの設定
	m_mode = mode;
	switch (mode)
	{
	case MODE::TITLE:
		m_pTitle = new CTitle;
		if (m_pTitle != nullptr) {
			m_pTitle->Init();
		}
		break;
	case MODE::GAME:
		m_pGame = new CGame;
		if (m_pGame != nullptr) {
			m_pGame->Init();
		}		
		break;
	case MODE::RESULT:
		m_pResult = new CResult;
		if (m_pResult != nullptr) {
			m_pResult->Init();
		}
		break;
	}
}

//=============================================================================
//モードの取得
//=============================================================================
CManager::MODE CManager::GetMode(void) {
	return m_mode;
}

//=============================================================================
// レンダラーの取得
//=============================================================================
CRenderer* CManager::GetRenderer(void) {
	return m_pRenderer;
}

//=============================================================================
// 現在の入力デバイスの取得
//=============================================================================
CInput* CManager::GetInputCur(void) {
	return m_pInputCur;
}

//=============================================================================
//カメラの取得
//=============================================================================
CCamera* CManager::GetCamera(void) {
	return m_pCamera;
}

//=============================================================================
// サウンドの取得
//=============================================================================
CSound* CManager::GetSound(void) {
	return m_pSound;
}

//=============================================================================
// フェードの取得
//=============================================================================
CFade* CManager::GetFade(void) {
	return m_pFade;
}

//=============================================================================
//ポーズの取得
//=============================================================================
CPause* CManager::GetPause(void) {
	return m_pPause;
}

//=============================================================================
//ゲームモードの取得
//=============================================================================
CGame* CManager::GetGame(void) {
	return m_pGame;
}