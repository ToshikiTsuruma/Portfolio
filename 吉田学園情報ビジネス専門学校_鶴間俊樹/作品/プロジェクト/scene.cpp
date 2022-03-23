//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "scene.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CScene* CScene::m_pTopAll = NULL;
CScene* CScene::m_pCurAll = NULL;
CScene* CScene::m_apTopObjType[(int)OBJ_TYPE::ENUM_MAX] = {};
CScene* CScene::m_apCurObjType[(int)OBJ_TYPE::ENUM_MAX] = {};
CScene* CScene::m_apTopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CScene* CScene::m_apCurUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CScene* CScene::m_apTopDraw[(int)DRAW_PRIORITY::ENUM_MAX] = {};
CScene* CScene::m_apCurDraw[(int)DRAW_PRIORITY::ENUM_MAX] = {};

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CScene::CScene()
{
	m_bDeath = false;
	m_objType = OBJ_TYPE::NONE;
	m_updatePriority = UPDATE_PRIORITY::DEFAULT;
	m_drawPriority = DRAW_PRIORITY::DEFAULT;

	m_bEnableCollision = true;

	//全オブジェクトのリストの初期設定
	m_pPrevAll = m_pCurAll;	//前のポインタの設定
	m_pNextAll = NULL;		//次のポインタの設定
	if (m_pTopAll == NULL) m_pTopAll = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_pCurAll != NULL) m_pCurAll->m_pNextAll = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_pCurAll = this;		//最後尾のポインタの設定

							//オブジェクトタイプリストの初期設定
	m_pPrevObjtype = m_apCurObjType[(int)m_objType];	//前のポインタの設定
	m_pNextObjtype = NULL;						//次のポインタの設定
	if (m_apTopObjType[(int)m_objType] == NULL) m_apTopObjType[(int)m_objType] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurObjType[(int)m_objType] != NULL) m_apCurObjType[(int)m_objType]->m_pNextObjtype = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurObjType[(int)m_objType] = this;		//最後尾のポインタの設定

											//更新順のリストの初期設定
	m_pPrevUpdate = m_apCurUpdate[(int)m_updatePriority];	//前のポインタの設定
	m_pNextUpdate = NULL;								//次のポインタの設定
	if (m_apTopUpdate[(int)m_updatePriority] == NULL) m_apTopUpdate[(int)m_updatePriority] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurUpdate[(int)m_updatePriority] != NULL) m_apCurUpdate[(int)m_updatePriority]->m_pNextUpdate = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurUpdate[(int)m_updatePriority] = this;			//最後尾のポインタの設定

													//描画順のリストの初期設定
	m_pPrevDraw = m_apCurDraw[(int)m_drawPriority];	//前のポインタの設定
	m_pNextDraw = NULL;							//次のポインタの設定
	if (m_apTopDraw[(int)m_drawPriority] == NULL) m_apTopDraw[(int)m_drawPriority] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurDraw[(int)m_drawPriority] != NULL) m_apCurDraw[(int)m_drawPriority]->m_pNextDraw = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurDraw[(int)m_drawPriority] = this;			//最後尾のポインタの設定
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{
	//全リストの更新
	//全オブジェクトリスト
	if (m_pTopAll == this) m_pTopAll = m_pNextAll;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_pCurAll == this) m_pCurAll = m_pPrevAll;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevAll != NULL) m_pPrevAll->m_pNextAll = m_pNextAll;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextAll != NULL) m_pNextAll->m_pPrevAll = m_pPrevAll;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//オブジェクトタイプリスト
	if (m_apTopObjType[(int)m_objType] == this) m_apTopObjType[(int)m_objType] = m_pNextObjtype;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurObjType[(int)m_objType] == this) m_apCurObjType[(int)m_objType] = m_pPrevObjtype;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevObjtype != NULL) m_pPrevObjtype->m_pNextObjtype = m_pNextObjtype;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextObjtype != NULL) m_pNextObjtype->m_pPrevObjtype = m_pPrevObjtype;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//更新順リスト
	if (m_apTopUpdate[(int)m_updatePriority] == this) m_apTopUpdate[(int)m_updatePriority] = m_pNextUpdate;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurUpdate[(int)m_updatePriority] == this) m_apCurUpdate[(int)m_updatePriority] = m_pPrevUpdate;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevUpdate != NULL) m_pPrevUpdate->m_pNextUpdate = m_pNextUpdate;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextUpdate != NULL) m_pNextUpdate->m_pPrevUpdate = m_pPrevUpdate;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//描画順リスト
	if (m_apTopDraw[(int)m_drawPriority] == this) m_apTopDraw[(int)m_drawPriority] = m_pNextDraw;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurDraw[(int)m_drawPriority] == this) m_apCurDraw[(int)m_drawPriority] = m_pPrevDraw;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevDraw != NULL) m_pPrevDraw->m_pNextDraw = m_pNextDraw;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextDraw != NULL) m_pNextDraw->m_pPrevDraw = m_pPrevDraw;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入
}

//=============================================================================
// 全オブジェクトのリストのTopを取得
//=============================================================================
CScene* CScene::GetSceneTopAll(void) {
	return m_pTopAll;
}

//=============================================================================
// 全オブジェクトのリストのNextを取得
//=============================================================================
CScene* CScene::GetSceneNextAll(CScene* pScene) {
	if (pScene != NULL) {
		return pScene->m_pNextAll;
	}
	return NULL;
}

//=============================================================================
// オブジェクトタイプのリストのTopを取得
//=============================================================================
CScene* CScene::GetSceneTopObjtype(OBJ_TYPE objType) {
	return m_apTopObjType[(int)objType];
}

//=============================================================================
// オブジェクトタイプのリストのNextを取得
//=============================================================================
CScene* CScene::GetSceneNextObjtype(CScene* pScene) {
	if (pScene != NULL) {
		return pScene->m_pNextObjtype;
	}
	return NULL;
}

//=============================================================================
// 全オブジェクトの解放処理
//=============================================================================
void CScene::ReleaseAll(void) {
	CScene* pSceneAll = m_pTopAll;	//全オブジェクトのポインタを先頭から順に代入
	while (pSceneAll != NULL)
	{
		CScene* pSceneNext = pSceneAll->m_pNextAll;
		//死亡フラグが立っていなかった場合
		if (pSceneAll->m_bDeath == false) {
			pSceneAll->Uninit();	//オブジェクトの終了処理
		}
		delete pSceneAll;	//オブジェクトの破棄
		pSceneAll = pSceneNext;
	}

	//リストの先頭と最後尾を空にする
	m_pTopAll = NULL;
	m_pCurAll = NULL;
	for (int nCnt = 0; nCnt < (int)OBJ_TYPE::ENUM_MAX; nCnt++)
	{
		m_apTopObjType[nCnt] = NULL;
		m_apCurObjType[nCnt] = NULL;
	}
	for (int nCnt = 0; nCnt < (int)UPDATE_PRIORITY::ENUM_MAX; nCnt++)
	{
		m_apTopUpdate[nCnt] = NULL;
		m_apCurUpdate[nCnt] = NULL;
	}
	for (int nCnt = 0; nCnt < (int)DRAW_PRIORITY::ENUM_MAX; nCnt++)
	{
		m_apTopDraw[nCnt] = NULL;
		m_apCurDraw[nCnt] = NULL;
	}
}

//=============================================================================
// 指定したタイプのオブジェクトの解放処理
//=============================================================================
void CScene::ReleaseObjtype(OBJ_TYPE objtype) {
	CScene* pSceneObjtype = m_apTopObjType[(int)objtype];	//指定したタイプのオブジェクトのポインタを先頭から順に代入
	while (pSceneObjtype != NULL)
	{
		CScene* pSceneNext = pSceneObjtype->m_pNextObjtype;
		//死亡フラグが立っていなかった場合
		if (pSceneObjtype->m_bDeath == false) {
			pSceneObjtype->Uninit();	//オブジェクトの終了処理
		}
		delete pSceneObjtype;	//オブジェクトの破棄
		pSceneObjtype = pSceneNext;
	}

	//リストの先頭と最後尾を空にする
	m_apTopObjType[(int)objtype] = NULL;
	m_apCurObjType[(int)objtype] = NULL;
}

//=============================================================================
// 全オブジェクトの更新処理
//=============================================================================
void CScene::UpdateAll(void) {
	for (int nCnt = 0; nCnt < (int)UPDATE_PRIORITY::ENUM_MAX; nCnt++) {
		CScene* pSceneUpdate = m_apTopUpdate[nCnt];	//更新するオブジェクト
		while (pSceneUpdate != NULL) {
			CScene* pSceneNext = pSceneUpdate->m_pNextUpdate;
			if (pSceneUpdate->m_bDeath == false) pSceneUpdate->Update();	//死亡フラグが立っていない場合更新
			pSceneUpdate = pSceneNext;
		}
	}
	//死亡フラグが立ったオブジェクトを破棄
	CScene* pSceneAll = m_pTopAll;	//全オブジェクトのポインタを先頭から順に代入
	while (pSceneAll != NULL)
	{
		CScene* pSceneNext = pSceneAll->m_pNextAll;
		//死亡していた場合破棄
		if (pSceneAll->m_bDeath == true) {
			delete pSceneAll;	//オブジェクトの破棄
		}
		pSceneAll = pSceneNext;
	}
}

//=============================================================================
// 全オブジェクトの描画処理
//=============================================================================
void CScene::DrawAll(void) {
	for (int nCnt = 0; nCnt < (int)DRAW_PRIORITY::ENUM_MAX; nCnt++) {
		CScene* pScene = m_apTopDraw[nCnt];
		while (pScene != NULL) {
			CScene* pSceneNext = pScene->m_pNextDraw;
			pScene->Draw();
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// オブジェクトの解放処理
//=============================================================================
void CScene::Release(void) {
	m_bDeath = true;	//死亡フラグを立てる
}

//=============================================================================
// 死亡フラグの取得
//=============================================================================
bool CScene::GetDeath(void) {
	return m_bDeath;
}

//=============================================================================
// オブジェクトの種類の設定
//=============================================================================
void CScene::SetObjType(OBJ_TYPE objType) {
	//変更前のオブジェクトタイプリストの設定
	if (m_apTopObjType[(int)m_objType] == this) m_apTopObjType[(int)m_objType] = m_pNextObjtype;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurObjType[(int)m_objType] == this) m_apCurObjType[(int)m_objType] = m_pPrevObjtype;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevObjtype != NULL) m_pPrevObjtype->m_pNextObjtype = m_pNextObjtype;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextObjtype != NULL) m_pNextObjtype->m_pPrevObjtype = m_pPrevObjtype;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//オブジェクトタイプのリストを変更
	m_objType = objType;
	m_pPrevObjtype = m_apCurObjType[(int)m_objType];	//前のポインタの設定
	m_pNextObjtype = NULL;						//次のポインタの設定
	if (m_apTopObjType[(int)m_objType] == NULL) m_apTopObjType[(int)m_objType] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurObjType[(int)m_objType] != NULL) m_apCurObjType[(int)m_objType]->m_pNextObjtype = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurObjType[(int)m_objType] = this;			//最後尾のポインタの設定
}

//=============================================================================
// オブジェクトの種類の取得
//=============================================================================
CScene::OBJ_TYPE CScene::GetObjType(void) {
	return m_objType;
}

//=============================================================================
// 更新の優先順位の設定
//=============================================================================
void CScene::SetUpdatePriority(UPDATE_PRIORITY priority) {
	//変更前の更新優先順のオブジェクトのリストの設定
	if (m_apTopUpdate[(int)m_updatePriority] == this) m_apTopUpdate[(int)m_updatePriority] = m_pNextUpdate;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurUpdate[(int)m_updatePriority] == this) m_apCurUpdate[(int)m_updatePriority] = m_pPrevUpdate;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevUpdate != NULL) m_pPrevUpdate->m_pNextUpdate = m_pNextUpdate;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextUpdate != NULL) m_pNextUpdate->m_pPrevUpdate = m_pPrevUpdate;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//更新優先順のリストを変更
	m_updatePriority = priority;
	m_pPrevUpdate = m_apCurUpdate[(int)m_updatePriority];	//前のポインタの設定
	m_pNextUpdate = NULL;								//次のポインタの設定
	if (m_apTopUpdate[(int)m_updatePriority] == NULL) m_apTopUpdate[(int)m_updatePriority] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurUpdate[(int)m_updatePriority] != NULL) m_apCurUpdate[(int)m_updatePriority]->m_pNextUpdate = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurUpdate[(int)m_updatePriority] = this;			//最後尾のポインタの設定
}

//=============================================================================
// 描画の優先順位の設定
//=============================================================================
void CScene::SetDrawPriority(DRAW_PRIORITY priority) {
	//変更前の描画優先順のオブジェクトのリストの設定
	if (m_apTopDraw[(int)m_drawPriority] == this) m_apTopDraw[(int)m_drawPriority] = m_pNextDraw;	//このオブジェクトがtopだった場合、次のオブジェクトをtopにする
	if (m_apCurDraw[(int)m_drawPriority] == this) m_apCurDraw[(int)m_drawPriority] = m_pPrevDraw;	//このオブジェクトがcurだった場合、前のオブジェクトをcurにする
	if (m_pPrevDraw != NULL) m_pPrevDraw->m_pNextDraw = m_pNextDraw;	//前のオブジェクトのpNextに、このオブジェクトのpNextを代入
	if (m_pNextDraw != NULL) m_pNextDraw->m_pPrevDraw = m_pPrevDraw;	//次のオブジェクトのpPrevに、このオブジェクトのpPrevを代入

	//描画優先順のリストを変更
	m_drawPriority = priority;	//描画優先順の設定
	m_pPrevDraw = m_apCurDraw[(int)m_drawPriority];	//前のポインタの設定
	m_pNextDraw = NULL;							//次のポインタの設定
	if (m_apTopDraw[(int)m_drawPriority] == NULL) m_apTopDraw[(int)m_drawPriority] = this;				//topが存在しない場合、このオブジェクトをtopにする
	if (m_apCurDraw[(int)m_drawPriority] != NULL) m_apCurDraw[(int)m_drawPriority]->m_pNextDraw = this;	//curが存在する場合、curのpNextをこのオブジェクトのポインタにする
	m_apCurDraw[(int)m_drawPriority] = this;			//最後尾のポインタの設定
}

//当たり判定の使用状態の設定
void CScene::SetEnableCollision(bool bUse) {
	m_bEnableCollision = bUse;
}

//当たり判定の使用状態の取得
bool CScene::GetEnableCollision(void) {
	return m_bEnableCollision;
}

//テクスチャの設定
void CScene::SetTexType(CTexture::TEXTURE_TYPE type) {
	m_texType = type;
}
//テクスチャの種類の取得
CTexture::TEXTURE_TYPE CScene::GetTexType(void) {
	return m_texType;
}


//=============================================================================
//仮想関数
//=============================================================================
void CScene::SetPos(D3DXVECTOR3 pos) {}		//位置座標の設定
D3DXVECTOR3 CScene::GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//位置座標の取得

D3DXVECTOR3 CScene::GetLastPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	//最後の位置座標の取得

void CScene::SetMove(D3DXVECTOR3 move) {}	//移動量の設定
D3DXVECTOR3 CScene::GetMove(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f);}		//移動量の取得

float CScene::GetRadius(void) { return 0.0f; }			//当たり判定の半径の取得

int CScene::GetNumCollisionParts(void) { return 1; }	//当たり判定があるパーツの数の取得 基本は１つとする
void CScene::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosCol, float* const pRadiusCol) {}	//当たり判定の情報の取得
void CScene::UpdateMtxWorldAll(void) {}			//オブジェクトの全モデルのワールドマトリックスの更新

void CScene::Damage(int nDamage, bool* pDead) {}	//ダメージ
bool CScene::GetItem(int nTypeItem) { return false; }	//アイテム取得時の処理