//=============================================================================
//
// 刀の処理 [katana.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "katana.h"
#include "meshorbit.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define COLOR_ORBIT_0 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//メッシュ軌道の剣の根本側の色
#define COLOR_ORBIT_1 (D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f))	//メッシュ軌道の剣の刃先側の色
#define NUM_COLLISION (6)	//当たり判定の数

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CKatana::CKatana()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CKatana::CKatana(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent) : CWeapon(CModel::MODELTYPE::WEAPON_KATANA, pos, rot, pParent, true)
{
	m_pMeshorbit = NULL;
	SetDrawOutline(false);
}

//=============================================================================
// デストラクタ
//=============================================================================
CKatana::~CKatana()
{

}


//=============================================================================
// 刀の生成処理
//=============================================================================
CKatana* CKatana::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent) {
	CKatana* pKatana;
	pKatana = new CKatana(pos, rot, pParent);
	if (pKatana != NULL) {
		pKatana->Init();
	}

	return pKatana;
}

//=============================================================================
// 刀の初期化処理
//=============================================================================
HRESULT CKatana::Init(void) {
	//メッシュ軌道の生成
	CModel* pModel = GetPtrModel();	//武器のモデルのポインタ
	if (pModel != NULL) m_pMeshorbit = CMeshorbit::Create(pModel->GetPtrMtxWorld(), 40, 0, D3DXVECTOR3(0.0f, 25.0f, 0.0f), D3DXVECTOR3(0.0f, 80.0f, 0.0f), COLOR_ORBIT_0, COLOR_ORBIT_1);
	if (m_pMeshorbit != NULL)m_pMeshorbit->SetVisible(false);
	//武器の初期化処理
	CWeapon::Init();
	return S_OK;
}

//=============================================================================
// 刀の終了処理
//=============================================================================
void CKatana::Uninit(void) {
	//メッシュ軌道の破棄
	if (m_pMeshorbit != NULL) {
		m_pMeshorbit->Uninit();
		m_pMeshorbit = NULL;
	}
	//武器の終了処理
	CWeapon::Uninit();
}

//=============================================================================
// 刀の更新処理
//=============================================================================
void CKatana::Update(void) {
	if (m_pMeshorbit != NULL) {
		//メッシュ軌道が不可視の場合
		if (m_pMeshorbit->GetVisible() == false || m_pMeshorbit->GetBindPos() == true) {
			//攻撃中の場合
			if (GetAttack() == true) {
				m_pMeshorbit->SetVisible(true);	//メッシュ軌道を可視化
			}
		}
		else {
			//攻撃中でない場合
			if (GetAttack() == false) {
				m_pMeshorbit->SetBindPos();	//メッシュ軌道の原点の位置を固定
			}
		}
	}
	//武器の更新処理
	CWeapon::Update();
}

//=============================================================================
// 刀の描画処理
//=============================================================================
void CKatana::Draw(void) {
	CWeapon::Draw();
}

//=============================================================================
// 当たり判定の位置の取得
//=============================================================================
void CKatana::GetPosCollision(D3DXVECTOR3** ppPosCollision, int* const pNumCollision) {
	//当たり判定の数を設定
	if (pNumCollision != nullptr) *pNumCollision = NUM_COLLISION;

	//当たり判定の位置を数分生成
	if (*ppPosCollision == nullptr) {
		*ppPosCollision = new D3DXVECTOR3[NUM_COLLISION];
	}
	else {
		//すでに配列が生成されていた場合終了
		return;
	}

	//配列が生成されていない場合終了
	if (*ppPosCollision == nullptr) return;

	//武器のモデルを取得
	CModel* pModel = CSceneModel::GetPtrModel();

	//武器のモデルがない場合終了
	if (pModel == NULL) return;

	D3DXMATRIX mtxWeapon;	//武器のモデルのワールドマトリックス

	//武器のモデルのワールドマトリックスを更新
	pModel->UpdateMtxWorld();
	//武器のワールド変換行列を取得
	mtxWeapon = pModel->GetMtxWorld();

	D3DXMATRIX mtxWorld;	//攻撃位置のワールドマトリックス
	D3DXMATRIX mtxTrans;	//計算用マトリックス

	for (int nCntCollsion = 0; nCntCollsion < NUM_COLLISION; nCntCollsion++)
	{
		D3DXVECTOR3 posAttack = D3DXVECTOR3(0.0f, 25.0f + 10.0f * nCntCollsion, 0.0f);	//攻撃の当たり判定の位置のオフセット

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);
		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, posAttack.x, posAttack.y, posAttack.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		//算出したワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&mtxWorld,
			&mtxWorld,
			&mtxWeapon);

		//攻撃位置の設定
		(*ppPosCollision)[nCntCollsion].x = mtxWorld._41;
		(*ppPosCollision)[nCntCollsion].y = mtxWorld._42;
		(*ppPosCollision)[nCntCollsion].z = mtxWorld._43;
	}
}

//=============================================================================
// モデルの輪郭の表示設定
//=============================================================================
void CKatana::SetDrawOutline(bool bDraw) {
	CModel* pModel = GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetDrawOutline(bDraw);
	}
}