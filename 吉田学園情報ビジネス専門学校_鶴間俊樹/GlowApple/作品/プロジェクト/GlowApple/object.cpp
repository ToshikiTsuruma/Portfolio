//=============================================================================
//
// オブジェクト処理 [object.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "object.h"
#include "objectList.h"

#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
bool CObject::m_abStopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CObject* CObject::m_pTopAll = nullptr;
CObject* CObject::m_pCurAll = nullptr;
CObject* CObject::m_apTopObjType[(int)OBJ_TYPE::ENUM_MAX] = {};
CObject* CObject::m_apCurObjType[(int)OBJ_TYPE::ENUM_MAX] = {};
CObject* CObject::m_apTopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CObject* CObject::m_apCurUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CObject* CObject::m_apTopDraw[(int)DRAW_PRIORITY::ENUM_MAX] = {};
CObject* CObject::m_apCurDraw[(int)DRAW_PRIORITY::ENUM_MAX] = {};

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CObject::CObject()
{
	m_bDeath = false;
	m_objType = OBJ_TYPE::NONE;
	m_updatePriority = UPDATE_PRIORITY::DEFAULT;
	m_drawPriority = DRAW_PRIORITY::DEFAULT;
	m_texType = CTexture::TEXTURE_TYPE::NONE;
	m_bDraw = true;

	//全オブジェクトのリストの初期設定
	m_pPrevAll = m_pCurAll;	//前のポインタの設定
	m_pNextAll = nullptr;		//次のポインタの設定
	if (m_pTopAll == nullptr) m_pTopAll = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_pCurAll != nullptr) m_pCurAll->m_pNextAll = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_pCurAll = this;		//最後尾のポインタの設定

	//オブジェクトタイプリストの初期設定
	m_pPrevObjtype = m_apCurObjType[(int)m_objType];	//前のポインタの設定
	m_pNextObjtype = nullptr;						//次のポインタの設定
	if (m_apTopObjType[(int)m_objType] == nullptr) m_apTopObjType[(int)m_objType] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurObjType[(int)m_objType] != nullptr) m_apCurObjType[(int)m_objType]->m_pNextObjtype = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurObjType[(int)m_objType] = this;		//最後尾のポインタの設定

	//更新順のリストの初期設定
	m_pPrevUpdate = m_apCurUpdate[(int)m_updatePriority];	//前のポインタの設定
	m_pNextUpdate = nullptr;								//次のポインタの設定
	if (m_apTopUpdate[(int)m_updatePriority] == nullptr) m_apTopUpdate[(int)m_updatePriority] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurUpdate[(int)m_updatePriority] != nullptr) m_apCurUpdate[(int)m_updatePriority]->m_pNextUpdate = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurUpdate[(int)m_updatePriority] = this;			//最後尾のポインタの設定

	//描画順のリストの初期設定
	m_pPrevDraw = m_apCurDraw[(int)m_drawPriority];	//前のポインタの設定
	m_pNextDraw = nullptr;							//次のポインタの設定
	if (m_apTopDraw[(int)m_drawPriority] == nullptr) m_apTopDraw[(int)m_drawPriority] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurDraw[(int)m_drawPriority] != nullptr) m_apCurDraw[(int)m_drawPriority]->m_pNextDraw = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurDraw[(int)m_drawPriority] = this;			//最後尾のポインタの設定
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{
	//全リストの更新
	//全オブジェクトリスト
	if (m_pTopAll == this) m_pTopAll = m_pNextAll;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_pCurAll == this) m_pCurAll = m_pPrevAll;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevAll != nullptr) m_pPrevAll->m_pNextAll = m_pNextAll;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextAll != nullptr) m_pNextAll->m_pPrevAll = m_pPrevAll;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//オブジェクトタイプリスト
	if (m_apTopObjType[(int)m_objType] == this) m_apTopObjType[(int)m_objType] = m_pNextObjtype;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurObjType[(int)m_objType] == this) m_apCurObjType[(int)m_objType] = m_pPrevObjtype;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevObjtype != nullptr) m_pPrevObjtype->m_pNextObjtype = m_pNextObjtype;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextObjtype != nullptr) m_pNextObjtype->m_pPrevObjtype = m_pPrevObjtype;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//更新順リスト
	if (m_apTopUpdate[(int)m_updatePriority] == this) m_apTopUpdate[(int)m_updatePriority] = m_pNextUpdate;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurUpdate[(int)m_updatePriority] == this) m_apCurUpdate[(int)m_updatePriority] = m_pPrevUpdate;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevUpdate != nullptr) m_pPrevUpdate->m_pNextUpdate = m_pNextUpdate;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextUpdate != nullptr) m_pNextUpdate->m_pPrevUpdate = m_pPrevUpdate;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//描画順リスト
	if (m_apTopDraw[(int)m_drawPriority] == this) m_apTopDraw[(int)m_drawPriority] = m_pNextDraw;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurDraw[(int)m_drawPriority] == this) m_apCurDraw[(int)m_drawPriority] = m_pPrevDraw;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevDraw != nullptr) m_pPrevDraw->m_pNextDraw = m_pNextDraw;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextDraw != nullptr) m_pNextDraw->m_pPrevDraw = m_pPrevDraw;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入
}

//=============================================================================
// 全オブジェクトのリストのTopを取得
//=============================================================================
CObject* CObject::GetObjectTopAll(void) {
	return m_pTopAll;
}

//=============================================================================
// 全オブジェクトのリストのNextを取得
//=============================================================================
CObject* CObject::GetObjectNextAll(CObject* pObject) {
	if (pObject != nullptr) {
		return pObject->m_pNextAll;
	}
	return nullptr;
}

//=============================================================================
// オブジェクトタイプのリストのTopを取得
//=============================================================================
CObject* CObject::GetObjectTopObjtype(OBJ_TYPE objType) {
	return m_apTopObjType[(int)objType];
}

//=============================================================================
// オブジェクトタイプのリストのNextを取得
//=============================================================================
CObject* CObject::GetObjectNextObjtype(CObject* pObject) {
	if (pObject != nullptr) {
		return pObject->m_pNextObjtype;
	}
	return nullptr;
}

//=============================================================================
// 全オブジェクトの解放処理
//=============================================================================
//※破棄したオブジェクトを別のクラスでポインタを保持していた場合エラーが起きるため注意
void CObject::ReleaseAll(void) {
	CObject* pObjectAll = m_pTopAll;	//全オブジェクトのポインタを先頭から順に代入
	while (pObjectAll != nullptr)
	{
		CObject* pObjectNext = pObjectAll->m_pNextAll;
		//終了処理を行う
		if (!pObjectAll->m_bDeath) pObjectAll->Uninit();
		//次のオブジェクトを代入
		pObjectAll = pObjectNext;
	}
}

//=============================================================================
// 指定したタイプのオブジェクトの解放処理
//=============================================================================
//※破棄したオブジェクトを別のクラスでポインタを保持していた場合エラーが起きるため注意
void CObject::ReleaseObjtype(OBJ_TYPE objtype) {
	CObject* pObjectObjtype = m_apTopObjType[(int)objtype];	//指定したタイプのオブジェクトのポインタを先頭から順に代入
	while (pObjectObjtype != nullptr)
	{
		CObject* pObjectNext = pObjectObjtype->m_pNextObjtype;
		//終了処理を行う
		pObjectObjtype->Uninit();
		//次のオブジェクトを代入
		pObjectObjtype = pObjectNext;
	}
}

//=============================================================================
// 全オブジェクトの更新処理
//=============================================================================
void CObject::UpdateAll(void) {
	for (int nCnt = 0; nCnt < (int)UPDATE_PRIORITY::ENUM_MAX; nCnt++) {
		//このオブジェクトタイプが停止状態の場合ループを飛ばす
		if (m_abStopUpdate[nCnt]) continue;

		CObject* pObjectUpdate = m_apTopUpdate[nCnt];	//更新するオブジェクト
		while (pObjectUpdate != nullptr) {
			CObject* pObjectNext = pObjectUpdate->m_pNextUpdate;
			//死亡フラグが立っていない場合更新
			if (!pObjectUpdate->m_bDeath) pObjectUpdate->Update();	
			pObjectUpdate = pObjectNext;
		}
	}
	//死亡フラグが立ったオブジェクトを破棄
	CObject* pObjectAll = m_pTopAll;	//全オブジェクトのポインタを先頭から順に代入
	while (pObjectAll != nullptr)
	{
		CObject* pObjectNext = pObjectAll->m_pNextAll;
		//死亡していた場合破棄
		if (pObjectAll->m_bDeath) {
			//オブジェクトリストクラスのリストのノードに破棄したオブジェクトが含まれている場合、そのノードを破棄する
			CObjectList::ReleaseDeleteNode(pObjectAll);
			//オブジェクトの破棄
			delete pObjectAll;	
		}
		pObjectAll = pObjectNext;
	}
}

//=============================================================================
// 全オブジェクトの描画処理
//=============================================================================
void CObject::DrawAll(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer= pManager->GetRenderer();
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	for (int nCnt = 0; nCnt < (int)DRAW_PRIORITY::ENUM_MAX; nCnt++) {
		//UIの描画に切り替わった場合、Zバッファを1.0fでクリアする
		if(nCnt == (int)DRAW_PRIORITY::UI_BG && pDevice != nullptr) pDevice->Clear(0, nullptr, (D3DCLEAR_ZBUFFER), D3DXCOLOR(), 1.0f, 0);

		CObject* pObject = m_apTopDraw[nCnt];
		while (pObject != nullptr) {
			CObject* pObjectNext = pObject->m_pNextDraw;	//次のオブジェクト取得
			//描画可能時の場合描画
			if(pObject->m_bDraw && !pObject->m_bDeath) pObject->Draw();
			//次のオブジェクトを代入
			pObject = pObjectNext;
		}
	}
}

//=============================================================================
// 指定したタイプの死亡処理
//=============================================================================
void CObject::DeadObjtype(OBJ_TYPE objtype) {
	CObject* pObjectObjtype = m_apTopObjType[(int)objtype];	//指定したタイプのオブジェクトのポインタを先頭から順に代入
	while (pObjectObjtype != nullptr)
	{
		CObject* pObjectNext = pObjectObjtype->m_pNextObjtype;
		//死亡処理を行う
		if (!pObjectObjtype->m_bDeath) pObjectObjtype->Dead();
		//次のオブジェクトを代入
		pObjectObjtype = pObjectNext;
	}
}

//=============================================================================
// 更新停止するすべての更新順の設定
//=============================================================================
void CObject::SetStopUpdateAll(bool bFlag) {
	for (int nCnt = 0; nCnt < (int)UPDATE_PRIORITY::ENUM_MAX; nCnt++)
	{
		m_abStopUpdate[nCnt] = bFlag;
	}
}

//=============================================================================
// オブジェクトの解放処理
//=============================================================================
void CObject::Release(void) {
	m_bDeath = true;	//死亡フラグを立てる
}

//=============================================================================
// 死亡フラグの取得
//=============================================================================
bool CObject::GetDeath(void) {
	return m_bDeath;
}

//=============================================================================
// オブジェクトの種類の設定
//=============================================================================
void CObject::SetObjType(OBJ_TYPE objType) {
	//変更前のオブジェクトタイプリストの設定
	if (m_apTopObjType[(int)m_objType] == this) m_apTopObjType[(int)m_objType] = m_pNextObjtype;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurObjType[(int)m_objType] == this) m_apCurObjType[(int)m_objType] = m_pPrevObjtype;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevObjtype != nullptr) m_pPrevObjtype->m_pNextObjtype = m_pNextObjtype;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextObjtype != nullptr) m_pNextObjtype->m_pPrevObjtype = m_pPrevObjtype;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//オブジェクトタイプのリストを変更
	m_objType = objType;
	m_pPrevObjtype = m_apCurObjType[(int)m_objType];	//前のポインタの設定
	m_pNextObjtype = nullptr;						//次のポインタの設定
	if (m_apTopObjType[(int)m_objType] == nullptr) m_apTopObjType[(int)m_objType] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurObjType[(int)m_objType] != nullptr) m_apCurObjType[(int)m_objType]->m_pNextObjtype = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurObjType[(int)m_objType] = this;			//最後尾のポインタの設定
}

//=============================================================================
// オブジェクトの種類の取得
//=============================================================================
CObject::OBJ_TYPE CObject::GetObjType(void) {
	return m_objType;
}

//=============================================================================
// 更新の優先順位の設定
//=============================================================================
void CObject::SetUpdatePriority(UPDATE_PRIORITY priority) {
	//変更前の更新優先順のオブジェクトのリストの設定
	if (m_apTopUpdate[(int)m_updatePriority] == this) m_apTopUpdate[(int)m_updatePriority] = m_pNextUpdate;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurUpdate[(int)m_updatePriority] == this) m_apCurUpdate[(int)m_updatePriority] = m_pPrevUpdate;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevUpdate != nullptr) m_pPrevUpdate->m_pNextUpdate = m_pNextUpdate;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextUpdate != nullptr) m_pNextUpdate->m_pPrevUpdate = m_pPrevUpdate;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//更新優先順のリストを変更
	m_updatePriority = priority;
	m_pPrevUpdate = m_apCurUpdate[(int)m_updatePriority];	//前のポインタの設定
	m_pNextUpdate = nullptr;								//次のポインタの設定
	if (m_apTopUpdate[(int)m_updatePriority] == nullptr) m_apTopUpdate[(int)m_updatePriority] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurUpdate[(int)m_updatePriority] != nullptr) m_apCurUpdate[(int)m_updatePriority]->m_pNextUpdate = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurUpdate[(int)m_updatePriority] = this;			//最後尾のポインタの設定
}

//=============================================================================
// 描画の優先順位の設定
//=============================================================================
void CObject::SetDrawPriority(DRAW_PRIORITY priority) {
	//変更前の描画優先順のオブジェクトのリストの設定
	if (m_apTopDraw[(int)m_drawPriority] == this) m_apTopDraw[(int)m_drawPriority] = m_pNextDraw;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurDraw[(int)m_drawPriority] == this) m_apCurDraw[(int)m_drawPriority] = m_pPrevDraw;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevDraw != nullptr) m_pPrevDraw->m_pNextDraw = m_pNextDraw;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextDraw != nullptr) m_pNextDraw->m_pPrevDraw = m_pPrevDraw;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//描画優先順のリストを変更
	m_drawPriority = priority;	//描画優先順の設定
	m_pPrevDraw = m_apCurDraw[(int)m_drawPriority];	//前のポインタの設定
	m_pNextDraw = nullptr;							//次のポインタの設定
	if (m_apTopDraw[(int)m_drawPriority] == nullptr) m_apTopDraw[(int)m_drawPriority] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurDraw[(int)m_drawPriority] != nullptr) m_apCurDraw[(int)m_drawPriority]->m_pNextDraw = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurDraw[(int)m_drawPriority] = this;			//最後尾のポインタの設定
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void CObject::SetTexType(CTexture::TEXTURE_TYPE type) {
	m_texType = type;
}

//=============================================================================
// テクスチャの種類の取得
//=============================================================================
CTexture::TEXTURE_TYPE CObject::GetTexType(void) {
	return m_texType;
}

//=============================================================================
// 当たり判定の有効状態を設定
//=============================================================================
void CObject::SetEnableCollision(bool bEnable) {
	m_bEnableCollision = bEnable;
}

//=============================================================================
// 当たり判定の有効状態を取得
//=============================================================================
bool CObject::GetEnableCollision(void) {
	return m_bEnableCollision;
}

//=============================================================================
//仮想関数
//=============================================================================
void CObject::SetPos(D3DXVECTOR3 pos) {}		//位置座標の設定
D3DXVECTOR3 CObject::GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//位置座標の取得

D3DXVECTOR3 CObject::GetLastPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	//最後の位置座標の取得

D3DXVECTOR3 CObject::GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); };	//角度の取得

void CObject::SetMove(D3DXVECTOR3 move) {}	//移動量の設定
D3DXVECTOR3 CObject::GetMove(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f);}		//移動量の取得

float CObject::GetRadius(void) { return 0.0f; }			//当たり判定の半径の取得

int CObject::GetNumCollisionParts(void) { return 1; }	//当たり判定があるパーツの数の取得 基本は１つとする
void CObject::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosCol, float* const pRadiusCol) {}	//当たり判定の情報の取得
void CObject::UpdateMtxWorldAll(void) {}			//オブジェクトの全モデルのワールドマトリックスの更新

void CObject::Damage(int nDamage, bool* pDead) {}	//ダメージ
bool CObject::GetItem(int nTypeItem) { return false; }	//アイテム取得時の処理