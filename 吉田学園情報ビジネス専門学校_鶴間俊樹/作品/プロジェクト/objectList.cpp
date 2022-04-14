//=============================================================================
//
// オブジェクトリスト処理 [objectList.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "objectList.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CObjectList::CObjectList()
{
	m_pTopNode = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjectList::~CObjectList()
{
	ListNode* pListNode = m_pTopNode;	//先頭のノードを取得

	while (pListNode != nullptr)
	{
		//次のノードを保存
		ListNode* pNextNode = pListNode->pNextNode;
		//ノードの破棄
		delete pListNode;
		//次のノードを設定
		pListNode = pNextNode;
	}

	m_pTopNode = nullptr;
}

//=============================================================================
// オブジェクトリストの初期化処理
//=============================================================================
void CObjectList::Init(void) {
	ListNode* pListNode = m_pTopNode;	//先頭のノードを取得

	while (pListNode != nullptr)
	{
		//次のノードを保存
		ListNode* pNextNode = pListNode->pNextNode;
		//ノードの破棄
		delete pListNode;
		//次のノードを設定
		pListNode = pNextNode;
	}

	m_pTopNode = nullptr;
}

//=============================================================================
// オブジェクトリストにノードを追加
//=============================================================================
void CObjectList::AppendNode(CScene* pScene) {
	ListNode* pNewNode = new ListNode;	//新しく追加するノード
	if (pNewNode != nullptr) {
		pNewNode->pScene = pScene;
		pNewNode->pNextNode = nullptr;
	}

	//先頭のノードが設定されていない場合
	if (m_pTopNode == nullptr)
	{
		//新しく追加するノードを先頭のノードとして設定
		m_pTopNode = pNewNode;
	}
	//先頭のノードがすでにある場合
	else
	{
		ListNode* pListNode = m_pTopNode;	//先頭のノードを取得

		//次のノードがnullではない場合
		while (pListNode->pNextNode != nullptr)
		{
			//次のノードを取得
			pListNode = pListNode->pNextNode;
		}

		//次のノードがnullのため、次のノードとして新しく追加するノードを設定
		if(pListNode != nullptr) pListNode->pNextNode = pNewNode;
	}
}

//=============================================================================
// オブジェクトリストのノードの削除
//=============================================================================
// ※動作未確認のため使用時は注意※
bool CObjectList::DeleteNode(CScene* pScene) {
	if (m_pTopNode == nullptr) return false;	//先頭のノードがnullの場合終了

	bool bDeleteNode = false;	//ノードを削除したかどうか

	//先頭のノードが一致している場合
	while (m_pTopNode->pScene == pScene)
	{
		//次のノードを保存
		ListNode* pNextNode = m_pTopNode->pNextNode;
		//ノード破棄フラグを設定
		bDeleteNode = true;
		//ノードを破棄
		delete m_pTopNode;
		//先頭のノードを次のノードに設定
		m_pTopNode = pNextNode;
		//先頭のノードがnullになった場合終了
		if (m_pTopNode == nullptr) return true;
	}

	ListNode* pNode = m_pTopNode->pNextNode;	//先頭のノードの次のノードを取得


	//※これだとpNodeが一致していても破棄されない

	//先頭のノードの次のノードがnullの場合終了
	if (pNode == nullptr) return bDeleteNode;
	
	//次のノードがnullではない場合
	while (pNode->pNextNode != nullptr)
	{
		//次のノードのオブジェクトと引数が一致していた場合
		if (pNode->pNextNode->pScene == pScene) {
			//次のノードの次のノードを保存
			ListNode* pNextNodeNext = pNode->pNextNode->pNextNode;	
			//ノードの破棄
			delete pNode->pNextNode;
			//ノード破棄フラグを設定
			bDeleteNode = true;
			//pNextNodeを次の次のノードにする
			pNode->pNextNode = pNextNodeNext;
		}
		//ノードを次に進める
		pNode = pNode->pNextNode;
	}

	return bDeleteNode;
}

//=============================================================================
// オブジェクトリストに一致するオブジェクトがあるかどうか
//=============================================================================
bool CObjectList::MatchObject(CScene* pScene) {
	ListNode* pListNode = m_pTopNode;	//先頭のノードを取得

	//ノードがnullではない場合
	while (pListNode != nullptr)
	{
		//オブジェクトリストに一致するオブジェクトがある場合
		if (pListNode->pScene == pScene) {
			return true;
		}
		//次のノードにする
		pListNode = pListNode->pNextNode;
	}

	return false;
}