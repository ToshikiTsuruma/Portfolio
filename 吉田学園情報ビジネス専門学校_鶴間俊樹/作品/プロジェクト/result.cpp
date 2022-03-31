//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "scene2D.h"
#include "ranking.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define COUNT_FADE_ENABLE (100)		//フェード可能までのカウント
#define COUNT_FADE_FORCE (2400)		//強制的にフェードされるまでのカウント
#define RANKING_NUMBER_SIZE (40.0f)	//ランキングの数字のサイズ
#define FADE_SPEED (0.01f)	//フェードする速度

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CResult::CResult() {
	m_nCntResult = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult() {

}

//=============================================================================
// リザルトの初期化処理
//=============================================================================
HRESULT CResult::Init(void) {
	//オブジェクトのインスタンス生成
	//背景の生成
	CScene2D* pBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::BG_RESULT , SCREEN_WIDTH, SCREEN_HEIGHT);
	if (pBg != NULL) pBg->SetDrawPriority(CScene2D::DRAW_PRIORITY::BG);	//描画優先度を背景として設定

	//ランキングテキストの表示
	CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 400.0f, 80.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_RANKING, 300.0f, 75.0f);
	//ランキングの生成
	CRanking::Create(CTexture::TEXTURE_TYPE::NUMBER_004, D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 150.0f, 0.0f), 50.0f, 50.0f, 50.0f);

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//曲の再生
	if (pSound != nullptr) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_RESULT);
		pSound->SetBGM(CSound::SOUND_LABEL::BGM_RESULT);
	}

	return S_OK;
}

//=============================================================================
// リザルトの終了処理
//=============================================================================
void CResult::Uninit(void) {
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
// リザルトの更新処理
//=============================================================================
void CResult::Update(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	CInput* pInput = nullptr;
	CFade* pFade = nullptr;

	if (pManager != nullptr) {
		//現在の入力デバイスの取得
		pInput = pManager->GetInputCur();
		//フェードの取得
		pFade = pManager->GetFade();
	}


	if (pInput != NULL) {
		if ((pInput->GetTrigger(CInput::CODE::SELECT) || m_nCntResult >= COUNT_FADE_FORCE) && m_nCntResult >= COUNT_FADE_ENABLE) {
			//ゲームのモードに切り替え
			if (pFade != NULL) {
				pFade->SetFade(CManager::MODE::TITLE, FADE_SPEED);
			}
		}
	}

	m_nCntResult++;
}