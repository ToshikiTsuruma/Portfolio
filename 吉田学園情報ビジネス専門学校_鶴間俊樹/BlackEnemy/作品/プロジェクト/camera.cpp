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
#include "wallCylinder.h"
#include "terrain.h"

//=============================================================================
// マクロ定義
//=============================================================================

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
	ZeroMemory(&m_viewport, sizeof(D3DVIEWPORT9));	//確認
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
CCamera* CCamera::Create(void) {
	CCamera* pCamera;
	pCamera = new CCamera;
	if (pCamera != nullptr) {
		pCamera->Init();
	}

	return pCamera;
}

//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT CCamera::Init(void) {
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
	m_viewport.X = (DWORD)0.0f;
	m_viewport.Y = (DWORD)0.0f;
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
		MAX_FOG_DISTANCE + 100.0f);					//奥の位置

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの作成
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

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

}

//=============================================================================
// カメラのセット
//=============================================================================
void CCamera::SetCamera(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return;

	//視点と注視点の設定
	m_posR = m_pos;
	m_posV = D3DXVECTOR3(m_pos.x + sinf(m_rot.y) * m_fDistance * cosf(m_rot.x),
		m_pos.y + sinf(m_rot.x) * m_fDistance,
		m_pos.z + cosf(m_rot.y) * m_fDistance * cosf(m_rot.x));
	
	//壁との当たり判定
	D3DXVECTOR2 posCrossWall = D3DXVECTOR2(m_posV.x, m_posV.z);	//壁との交点
	CWallCylinder::CrossPoint(&posCrossWall, &m_pos, &m_posV, 15.0f);
	//位置の反映
	m_posV.x = posCrossWall.x;
	m_posV.z = posCrossWall.y;

	//地形との当たり判定
	bool bColTerrain = false;	//衝突しているかどうか
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//接触位置、開始ベクトル、終了ベクトル
	vecStart = m_posR;
	vecEnd = m_posV;
	//当たり判定
	bColTerrain = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);
	//接触位置と注視点の距離を計算
	float fDistCol;	//接触位置との距離
	D3DXVECTOR3 vecColTerrain = posColTerrain - m_posR;	//注視点から接触位置へのベクトル
	fDistCol = D3DXVec3Length(&vecColTerrain);
	//視点と注視点の距離を計算
	float fDistCamera;	//接触位置との距離
	D3DXVECTOR3 vecCamera = m_posV - m_posR;	//注視点から接触位置へのベクトル
	fDistCamera = D3DXVec3Length(&vecCamera);
	//衝突していて、現在のカメラの距離以内であれば位置の変更
	if (bColTerrain && fDistCol <= fDistCamera) m_posV = posColTerrain;

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
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
//角度の設定
//=============================================================================
void CCamera::SetRot(D3DXVECTOR3 rot) {
	m_rot = rot;
}

//=============================================================================
//角度の取得
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void) {
	return m_rot;
}

//=============================================================================
//距離の設定
//=============================================================================
void CCamera::SetDistance(float fDistance) {
	m_fDistance = fDistance;
}

//=============================================================================
//距離の取得
//=============================================================================
float CCamera::GetDistance(void) {
	return m_fDistance;
}