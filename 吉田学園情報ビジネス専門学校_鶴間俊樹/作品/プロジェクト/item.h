//=============================================================================
//
// アイテム処理 [item.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "sceneModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;

//*****************************************************************************
// アイテムクラス
//*****************************************************************************
class CItem : public CSceneModel
{
public:
	enum class ITEM_TYPE {
		APPLE = 0,
		ENUM_MAX
	};

	CItem();	//デフォルトコンストラクタ
	CItem(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine);	//オーバーロードされたコンストラクタ
	~CItem();	//デストラクタ
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理
	static void Collision(CScene* pSceneCol);	//衝突判定
	virtual void CollisionObject(CScene* pScene) = 0;	//アイテムがオブジェクトと衝突したときの処理

private:
	bool m_bDraw;	//描画するかどうか
};

#endif // !_ITEM_H_