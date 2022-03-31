//=============================================================================
//
// コンパス処理 [compass.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "compass.h"
#include "manager.h"
#include "camera.h"
#include "game.h"
#include "sceneModel.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define COMPASS_WIDTH (800.0f)	//コンパスの幅

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CCompass::CCompass()
{
	//目盛りの生成
	m_pDial = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::COMPASS_DIAL, COMPASS_WIDTH, 30.0f);
	if (m_pDial != nullptr) {
		m_pDial->SetDrawPriority(CScene::DRAW_PRIORITY::UI);
	}
	m_pMark = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::COMPASS_MARK, 50.0f, 50.0f);
	if (m_pMark != nullptr) {
		m_pMark->SetDrawPriority(CScene::DRAW_PRIORITY::UI);
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CCompass::~CCompass()
{

}

//=============================================================================
// コンパスの生成処理
//=============================================================================
CCompass* CCompass::Create() {
	CCompass* pCompass;
	pCompass = new CCompass();
	if (pCompass != nullptr) {
		pCompass->Init();
	}

	return pCompass;
}

//=============================================================================
// コンパスの初期化処理
//=============================================================================
HRESULT CCompass::Init(void) {
	return S_OK;
}

//=============================================================================
// コンパスの終了処理
//=============================================================================
void CCompass::Uninit(void) {
	//目盛りの破棄
	if (m_pDial != nullptr) {
		m_pDial->Uninit();
		m_pDial = nullptr;
	}
	//マークの破棄
	if (m_pMark != nullptr) {
		m_pMark->Uninit();
		m_pMark = nullptr;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// コンパスの更新処理
//=============================================================================
void CCompass::Update(void) {
	CManager* pManager = CManager::GetManager();

	CCamera* pCamera = nullptr;
	CGame* pGame = nullptr;
	if (pManager != nullptr) {
		//カメラの取得
		pCamera = pManager->GetCamera();
		//ゲームの取得
		pGame = pManager->GetGame();
	}

	CSceneModel* pRaft = nullptr;
	if (pGame != nullptr) pRaft = pGame->GetRaft();

	if (m_pMark != nullptr) {
		if (pRaft != nullptr && pCamera != nullptr) {
			D3DXVECTOR2 vecCameraR = D3DXVECTOR2(pCamera->GetPosR().x - pCamera->GetPosV().x, pCamera->GetPosR().z - pCamera->GetPosV().z);	//カメラの視点から注視点への上から見たベクトル
			D3DXVECTOR2 vecRaft = D3DXVECTOR2(pRaft->GetPos().x - pCamera->GetPosV().x, pRaft->GetPos().z - pCamera->GetPosV().z);			//カメラの視点から筏への上から見たベクトル			
			float fRotCameraR = (float)atan2(vecCameraR.x, vecCameraR.y);	//視点から注視点への角度
			float fRotRaft = (float)atan2(vecRaft.x, vecRaft.y);			//視点から筏への角度

			//角度を正の値にする(0から2π)
			if (fRotCameraR < 0.0f) fRotCameraR += D3DX_PI * 2;
			if (fRotRaft < 0.0f) fRotRaft += D3DX_PI * 2;

			float fRotDelta = fRotRaft - fRotCameraR;	//２つの角度の差分

			//パイ超過時の調整
			if (fRotDelta > D3DX_PI) {
				fRotDelta -= D3DX_PI * 2;
			}
			if (fRotDelta < -D3DX_PI) {
				fRotDelta += D3DX_PI * 2;
			}

			//範囲を　-π/2 ～　π/2　に調整
			if (fRotDelta > D3DX_PI / 2.0f) fRotDelta = D3DX_PI / 2.0f;
			if (fRotDelta < -D3DX_PI / 2.0f) fRotDelta = -D3DX_PI / 2.0f;

			//マークの位置の設定
			D3DXVECTOR3 posMark = m_pMark->GetPos();
			posMark.x = (SCREEN_WIDTH / 2.0f) + (fRotDelta / (D3DX_PI / 2.0f)) * (COMPASS_WIDTH / 2.0f);
			m_pMark->SetPos(posMark);
		}
		//筏が存在しない場合
		else {
			//常に中央
			D3DXVECTOR3 posMark = m_pMark->GetPos();
			posMark.x = SCREEN_WIDTH / 2.0f;
			m_pMark->SetPos(posMark);
		}
	}
}

//=============================================================================
// コンパスの描画処理
//=============================================================================
void CCompass::Draw(void) {}