//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "scene.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_DRAW_DISTANCE (5000.0f)		//描画可能な最大の距離
#define DEFAULT_CAMERA_DISTANCE (1500.0f)	//視点と注視点の距離
#define CAMERA_ROTATE_SPEED_YAW (0.008f)	//カメラの回転速度(Y軸)
#define CAMERA_ROTATE_SPEED_PITCH (0.006f)	//カメラの回転速度(X軸)
#define MAX_ROTATE_PITCH (-0.05f)	//カメラのX軸回転の最大
#define MIN_ROTATE_PITCH (-0.18f)	//カメラのX軸回転の最低

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CCamera::CCamera()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDistance = 0.0f;
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	ZeroMemory(&m_viewport, sizeof(D3DVIEWPORT9));

	m_bLockControll = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
//生成処理
//=============================================================================
CCamera* CCamera::Create() {
	CCamera* pCamera;
	pCamera = new CCamera;
	if (pCamera == nullptr) return nullptr;

	pCamera->Init();

	return pCamera;
}

//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT CCamera::Init(void) {
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot.x = D3DX_PI * -0.15f;
	m_rot.y = D3DX_PI * 0.0f;

	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return S_OK;

	//ビューポートの設定
	m_viewport.X = (DWORD)0;
	m_viewport.Y = (DWORD)0;
	m_viewport.Width = (DWORD)SCREEN_WIDTH;
	m_viewport.Height = (DWORD)SCREEN_HEIGHT;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	//ビューポートの設定
	pDevice->SetViewport(&m_viewport);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),						//画角 360度
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//画面比率
		10.0f,										//手前の位置
		MAX_DRAW_DISTANCE);							//奥の位置

	//プロジェクションマトリックスの設定
	pRenderer->SetEffectMatrixProj(m_mtxProjection);

	//上ベクトルの設定
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//ビューマトリックスの設定
	SetCamera();

	//距離の設定
	SetDistance(DEFAULT_CAMERA_DISTANCE);

	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void CCamera::Uninit(void) {

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void CCamera::Update(void) {
	//移動処理
	//Move();

	//回転処理
	if(!m_bLockControll) Rotate();
}

//=============================================================================
// カメラのセット
//=============================================================================
void CCamera::SetCamera(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;


	//------------------------------------
	//プレイヤーとの位置の間を取得
	//------------------------------------
	CScene* pScene = pManager->GetScene();
	CPlayer* pPlayer = nullptr;
	if (pScene != nullptr) pPlayer = pScene->GetPlayer();

	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();

	D3DXVECTOR3 posCamera = m_pos + (posPlayer - m_pos) / 2.0f;
	posCamera.y = m_pos.y;

	//------------------------------------
	//視点と注視点の設定
	//------------------------------------
	m_posR = posCamera;
	m_posV = D3DXVECTOR3(posCamera.x + sinf(m_rot.y) * m_fDistance * cosf(m_rot.x + D3DX_PI),
		posCamera.y + sinf(m_rot.x + D3DX_PI) * m_fDistance,
		posCamera.z + cosf(m_rot.y) * m_fDistance * cosf(m_rot.x + D3DX_PI));	//視点が中心の場合から変更したときに移動がないようにX回転にPIを足している

	//------------------------------------
	//ビューマトリックスの作成
	//------------------------------------
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);	//ビルボードのために残す
	//シェーダにビューマトリックスを設定
	pRenderer->SetEffectMatrixView(m_mtxView);


	//------------------------------------
	//シェーダのライトの位置を更新
	//------------------------------------
	D3DXMATRIX mtxLightView;   // ライトビュー変換
	D3DXVECTOR3 posLight;
	//posLight = m_posV + D3DXVECTOR3(0.0f, 1600.0f, -800.0f);
	posLight = D3DXVECTOR3(0.0f, 2200.0f, -1200.0f);
	D3DXVECTOR3 vecLight = D3DXVECTOR3(0.0f, -1.0f, 0.4f);	//ライトのベクトル
	//ライトのビューマトリックスを生成
	D3DXMatrixLookAtLH(&mtxLightView, &posLight, &D3DXVECTOR3(posLight + vecLight), &D3DXVECTOR3(0, 1, 0));
	pRenderer->SetEffectLightMatrixView(mtxLightView);

}

//=============================================================================
// 移動
//=============================================================================
void CCamera::Move(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	CInput* pInput = nullptr;	//入力デバイスへのポインタ

	//現在の入力デバイスの取得
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//nullの場合終了
	if (pInput == nullptr) return;

	//--------------------
	//入力処理
	//--------------------
	float fMoveSpeed = 5.0f;
	if (pInput->GetPress(CInput::CODE::DASH)) {
		fMoveSpeed *= 3;
	}

	if (pInput->GetPress(CInput::CODE::MOVE_UP)) {
		//前移動
		m_pos.z += fMoveSpeed * cosf(m_rot.y);
		m_pos.x += fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_DOWN)) {
		//後ろ移動
		m_pos.z -= fMoveSpeed * cosf(m_rot.y);
		m_pos.x -= fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_LEFT)) {
		//左移動
		m_pos.x -= fMoveSpeed * cosf(m_rot.y);
		m_pos.z += fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RIGHT)) {
		//右移動
		m_pos.x += fMoveSpeed * cosf(m_rot.y);
		m_pos.z -= fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RISE)) {
		//上昇
		m_pos.y += fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_FALL)) {
		//下降
		m_pos.y -= fMoveSpeed;
	}
}

//=============================================================================
// 回転
//=============================================================================
void CCamera::Rotate(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	CInput* pInput = nullptr;	//入力デバイスへのポインタ

	//現在の入力デバイスの取得
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//nullの場合終了
	if (pInput == nullptr) return;

	//--------------------
	//入力処理
	//--------------------
	if (pInput->GetPress(CInput::CODE::ROTATE_UP)) {
		//視点を下げる
		m_rot.x += CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
		if (m_rot.x > MAX_ROTATE_PITCH * D3DX_PI) {
			m_rot.x = MAX_ROTATE_PITCH * D3DX_PI;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_DOWN)) {
		//視点を上げる
		m_rot.x -= CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
		if (m_rot.x < MIN_ROTATE_PITCH * D3DX_PI) {
			m_rot.x = MIN_ROTATE_PITCH * D3DX_PI;
		}
	}

	if (pInput->GetPress(CInput::CODE::ROTATE_LEFT)) {
		//視点を左回転
		m_rot.y -= CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
		if (m_rot.y < -D3DX_PI) {
			m_rot.y = m_rot.y + D3DX_PI * 2;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_RIGHT)) {
		//視点を右回転
		m_rot.y += CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
		if (m_rot.y > D3DX_PI) {
			m_rot.y = m_rot.y - D3DX_PI * 2;
		}
	}

}

//=============================================================================
// 位置座標の設定
//=============================================================================
void CCamera::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
}

//=============================================================================
// 位置座標の取得
//=============================================================================
D3DXVECTOR3 CCamera::GetPos(void) {
	return m_pos;
}

//=============================================================================
// 視点の位置座標の取得
//=============================================================================
D3DXVECTOR3 CCamera::GetPosV(void) {
	return m_posV;
}

//=============================================================================
// 注視点の位置座標の取得
//=============================================================================
D3DXVECTOR3 CCamera::GetPosR(void) {
	return m_posR;
}

//=============================================================================
// 角度の設定
//=============================================================================
void CCamera::SetRot(D3DXVECTOR3 rot) {
	m_rot = rot;
}

//=============================================================================
// 角度の取得
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void) {
	return m_rot;
}

//=============================================================================
// 距離の設定
//=============================================================================
void CCamera::SetDistance(float fDistance) {
	m_fDistance = fDistance;
}

//=============================================================================
// 距離の取得
//=============================================================================
float CCamera::GetDistance(void) {
	return m_fDistance;
}