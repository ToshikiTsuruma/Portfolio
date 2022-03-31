//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "item.h"
#include "manager.h"
#include "game.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ROTATE_SPEED (0.03f)	//回転速度
#define DRAW_DISTANCE (1800.0f)			//描画が可能になり始めるプレイヤーとの距離
#define MAX_DRAW_DISTANCE (2000.0f)		//描画可能な最大のプレイヤーとの距離

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CItem::CItem()
{
	m_bDraw = true;
}

//=============================================================================
//オーバーロードされたコンストラクタ
//=============================================================================
CItem::CItem(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine) : CSceneModel(typeModel, pos, rot, NULL, bOutLine)
{
	SetObjType(OBJ_TYPE::ITEM);	//オブジェクトタイプの設定
}

//=============================================================================
// デストラクタ
//=============================================================================
CItem::~CItem()
{

}

//=============================================================================
// アイテムの初期化処理
//=============================================================================
HRESULT CItem::Init(void) {
	CSceneModel::Init();

	return S_OK;
}

//=============================================================================
// アイテムの終了処理
//=============================================================================
void CItem::Uninit(void) {
	CSceneModel::Uninit();
}

//=============================================================================
// アイテムの更新処理
//=============================================================================
void CItem::Update(void) {
	CSceneModel::Update();
	//回転
	SetRot(D3DXVECTOR3(0.0f, GetRot().y + ROTATE_SPEED, 0.0f));
}

//=============================================================================
// アイテムの描画処理
//=============================================================================
void CItem::Draw(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//ゲームの取得
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//プレイヤーの取得
	CPlayer* pPlayer = nullptr;
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	//プレイヤーの位置を取得
	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pPlayer != NULL) posPlayer = pPlayer->GetPos();
	//プレイヤーとの距離を求める
	D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);
	float fDistPlayer = D3DXVec2Length(&vecPlayer);
	//描画の設定
	if (m_bDraw == true) {
		//描画外の位置にいたら描画しないようにする
		if (fDistPlayer > MAX_DRAW_DISTANCE) {
			m_bDraw = false;
		}
	}
	else {
		//描画内の位置にいたら描画できるようにする
		if (fDistPlayer < DRAW_DISTANCE) {
			m_bDraw = true;
		}
	}

	//描画
	if (m_bDraw == true) {
		CSceneModel::Draw();
	}
}

//=============================================================================
// アイテムとの衝突判定
//=============================================================================
void CItem::Collision(CScene* pSceneCol) {
	if (pSceneCol == NULL) return;

	D3DXVECTOR3 posCol = pSceneCol->GetPos();			//当たり判定するオブジェクトの位置座標
	float radiusCol = pSceneCol->GetRadius();			//当たり判定するオブジェクトの半径

	CScene* pScene;	//オブジェクトへのポインタ
	pScene = GetSceneTopObjtype(OBJ_TYPE::ITEM);	//アイテムのオブジェクトタイプのリストの先頭を取得

	while (pScene != NULL) {
		CScene* pSceneNext = GetSceneNextObjtype(pScene);	//リストの次のオブジェクトのポインタを取得
		CItem* pItem = dynamic_cast<CItem*>(pScene);	//アイテムクラスにキャスト
		//キャスト成功時
		if (pItem != NULL) {
			//死亡フラグが立っていない場合
			if (pItem->GetDeath() == false) {
				D3DXVECTOR3 posItem = pItem->GetPos();	//アイテムの位置の取得
				float fRadiusItem = pItem->GetRadius();	//アイテムの半径の取得
				posItem.y += fRadiusItem;	//アイテムはローカル座標で底辺が0なので中央に来るように調整する

				D3DXVECTOR3 posColCenter = posCol;
				posColCenter.y += radiusCol;	//衝突オブジェクトの位置も中央に合わせる

				D3DXVECTOR3 vecItem = posItem - posColCenter;	//衝突オブジェクトからアイテムへのベクトル
				float fDistCol = D3DXVec3Length(&vecItem);	//アイテムの中心と衝突オブジェクトの中心の距離を求める
				//当たり判定
				if (fDistCol < radiusCol + fRadiusItem) {
					//アイテムが取得されたときの処理
					pItem->CollisionObject(pSceneCol);
				}
			}
		}
		pScene = pSceneNext;	//リストの次のオブジェクトを代入
	}
}