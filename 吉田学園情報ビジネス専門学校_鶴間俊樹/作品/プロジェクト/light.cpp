//=============================================================================
//
// ライト処理 [light.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CLight* CLight::m_pTop = NULL;
CLight* CLight::m_pCur = NULL;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CLight::CLight()
{
	m_nIdx = -1;
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
	m_vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//ライトのリストの初期設定
	m_pPrev = m_pCur;	//前のポインタの設定
	m_pNext = NULL;		//次のポインタの設定
	if (m_pTop == NULL) {
		m_pTop = this;				//topが存在しない場合、このオブジェクトをtopにする
		m_nIdx = 0;
	}
	if (m_pCur != NULL) {
		m_pCur->m_pNext = this;		//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
		m_nIdx = m_pCur->m_nIdx + 1;
	}
	m_pCur = this;		//最後尾のポインタの設定
}

//=============================================================================
// デストラクタ
//=============================================================================
CLight::~CLight()
{
	//このライト以降のライトのインデックスを設定する
	SetIdxNext(this);
	//ライトのリスト
	if (m_pTop == this) m_pTop = m_pNext;	//このライトがtopだった場合、次のライトをtopにする
	if (m_pCur == this) m_pCur = m_pPrev;	//このライトがcurだった場合、前のライトをcurにする
	if (m_pPrev != NULL) m_pPrev->m_pNext = m_pNext;	//前のライトのpNextに、このライトのpNextを代入
	if (m_pNext != NULL) m_pNext->m_pPrev = m_pPrev;	//次のライトのpPrevに、このライトのpPrevを代入
}

//=============================================================================
// 生成処理
//=============================================================================
CLight* CLight::Create(D3DXVECTOR3 vecDir, D3DXCOLOR col) {
	CLight* pLight;
	pLight = new CLight;
	if (pLight != NULL) {
		pLight->Init(vecDir, col);
	}

	return pLight;
}

//=============================================================================
// ライトの初期化処理
//=============================================================================
HRESULT CLight::Init(D3DXVECTOR3 vecDir, D3DXCOLOR col) {
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

	//メンバ変数の設定
	m_vecDir = vecDir;

	//ライトの種類を設定
	m_light.Type = D3DLIGHT_DIRECTIONAL;//平行光源
	//ライトの拡散光を設定
	m_light.Diffuse = col;
	//正規化する
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);
	m_light.Direction = m_vecDir;
	//ライトを設定する
	pDevice->SetLight(m_nIdx, &m_light);
	//ライトを有効にする
	pDevice->LightEnable(m_nIdx, TRUE);

	return S_OK;
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void CLight::Uninit(void) {

}

//=============================================================================
// ライトの更新処理
//=============================================================================
void CLight::Update(void) {

}

//=============================================================================
//次のライトのインデックスを設定
//=============================================================================
void CLight::SetIdxNext(CLight* pLight) {
	if (pLight != NULL) {
		CLight* pNext = pLight->m_pNext;	//次のライト
		if (pNext != NULL) {
			//次の次のライトのライトのインデックスを設定する
			SetIdxNext(pNext);
			//次のライトのインデックスを設定する
			pNext->m_nIdx = pLight->m_nIdx;
		}
	}
}