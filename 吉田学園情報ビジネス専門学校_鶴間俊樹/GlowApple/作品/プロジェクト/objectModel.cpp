//=============================================================================
//
// モデルオブジェクト処理 [objectModel.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "objectModel.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CObjectModel::CObjectModel()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CObjectModel::CObjectModel(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine)
{
	m_pModel = CModel::Create(typeModel, pos, rot, nullptr, bOutLine);
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjectModel::~CObjectModel()
{

}

//=============================================================================
// モデルオブジェクトの生成処理
//=============================================================================
CObjectModel* CObjectModel::Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine) {
	CObjectModel* pObjectModel;
	pObjectModel = new CObjectModel(type, pos, rot, bOutLine);
	if (pObjectModel == nullptr) return nullptr;

	pObjectModel->Init();

	return pObjectModel;
}

//=============================================================================
// モデルオブジェクトの初期化処理
//=============================================================================
HRESULT CObjectModel::Init(void) {

	return S_OK;
}

//=============================================================================
// モデルオブジェクトの終了処理
//=============================================================================
void CObjectModel::Uninit(void) {
	if (m_pModel != nullptr) {
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// モデルオブジェクトの更新処理
//=============================================================================
void CObjectModel::Update(void) {
	//移動処理
	//Move();
	//回転処理
	//Rotate();
}

//=============================================================================
// モデルオブジェクトの描画処理
//=============================================================================
void CObjectModel::Draw(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	if (pManager == nullptr) return;
	//レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;

	//初期化したワールドマトリックスを設定
	D3DXMATRIX mtxWorld;
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//-----------------------------------
	//モデルの描画
	//-----------------------------------
	if (m_pModel != nullptr) m_pModel->Draw();
}

//=============================================================================
// 移動
//=============================================================================
void CObjectModel::Move(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	CInput* pInput = nullptr;	//入力デバイスへのポインタ

	//現在の入力デバイスの取得
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//nullの場合終了
	if (pInput == nullptr) return;

	//--------------------
	//入力処理
	//--------------------
	float fMoveSpeed = 1.0f;
	if (pInput->GetPress(CInput::CODE::DASH)) {
		fMoveSpeed *= 5;
	}

	//位置の取得
	D3DXVECTOR3 pos = GetPos();

	if (pInput->GetPress(CInput::CODE::MOVE_UP)) {
		//前移動
		pos.z += fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_DOWN)) {
		//後ろ移動
		pos.z -= fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_LEFT)) {
		//左移動
		pos.x -= fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RIGHT)) {
		//右移動
		pos.x += fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RISE)) {
		//上昇
		pos.y += fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_FALL)) {
		//下降
		pos.y -= fMoveSpeed;
	}

	//位置の移動
	SetPos(pos);
}

//=============================================================================
// 回転
//=============================================================================
void CObjectModel::Rotate(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	CInput* pInput = nullptr;	//入力デバイスへのポインタ

	//現在の入力デバイスの取得
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//nullの場合終了
	if (pInput == nullptr) return;
	//シフトが押されている場合終了（カメラの視点移動中のため）
	if (pInput->GetPress(CInput::CODE::DASH)) return;

	//--------------------
	//入力処理
	//--------------------
	D3DXVECTOR3 rot = GetRot();
	float fRotateSpeed = 0.01f * D3DX_PI;

	//X軸回転
	if (pInput->GetPress(CInput::CODE::ROTATE_UP)) {
		rot.x += fRotateSpeed;
		if (rot.x > D3DX_PI) {
			rot.x = -D3DX_PI * 2 + rot.x;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_DOWN)) {
		rot.x -= fRotateSpeed;
		if (rot.x < -D3DX_PI) {
			rot.x = D3DX_PI * 2 + rot.x;
		}
	}
	//Y軸回転
	if (pInput->GetPress(CInput::CODE::ROTATE_LEFT)) {
		rot.y -= fRotateSpeed;
		if (rot.y < -D3DX_PI) {
			rot.y = D3DX_PI * 2 + rot.y;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_RIGHT)) {
		rot.y += fRotateSpeed;
		if (rot.y > D3DX_PI) {
			rot.y = -D3DX_PI * 2 + rot.y;
		}
	}

	SetRot(rot);
}

//=============================================================================
// モデルの種類の取得
//=============================================================================
CModel::MODELTYPE CObjectModel::GetModelType(void) {
	if (m_pModel != nullptr) return m_pModel->GetModelType();
	return (CModel::MODELTYPE) NULL;
}

//=============================================================================
// モデルのポインタの取得
//=============================================================================
CModel* CObjectModel::GetPtrModel(void) {
	return m_pModel;
}

//=============================================================================
// モデルオブジェクトの位置情報の設定
//=============================================================================
void CObjectModel::SetPos(D3DXVECTOR3 pos) {	
	if (m_pModel != nullptr) m_pModel->SetPos(pos);
}

//=============================================================================
// モデルオブジェクトの位置情報の取得
//=============================================================================
D3DXVECTOR3 CObjectModel::GetPos(void) {
	if (m_pModel != nullptr) return m_pModel->GetPos();
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// モデルオブジェクトの角度の設定
//=============================================================================
void CObjectModel::SetRot(D3DXVECTOR3 rot) { 
	if (m_pModel != nullptr) m_pModel->SetRot(rot);
}

//=============================================================================
// モデルオブジェクトの角度の取得
//=============================================================================
D3DXVECTOR3 CObjectModel::GetRot(void) {
	if (m_pModel != nullptr) return m_pModel->GetRot();
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}