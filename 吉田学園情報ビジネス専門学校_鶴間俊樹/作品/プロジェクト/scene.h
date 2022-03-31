//=============================================================================
//
// オブジェクト処理 [scene.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SCENE (1024)

//*****************************************************************************
//前方宣言
//*****************************************************************************

//*****************************************************************************
// オブジェクトクラス
//*****************************************************************************
class CScene
{
public:
	//オブジェクトの種類 (当たり判定や全消去で必要な場合のもの)
	enum class OBJ_TYPE {
		NONE = 0,	
		TERRAIN,	//地形
		WALL,		//壁	
		PLAYER,		//プレイヤー
		ENEMY,		//敵
		ITEM,		//アイテム
		TREE,		//木
		OTHER,		//マップ切替時に破棄されるようなもの
		ENUM_MAX
	};	

	//更新順
	enum class UPDATE_PRIORITY {
		DEFAULT = 0,	//更新順が影響しないもの
		PLAYER,			//プレイヤー
		ENEMY,			//敵
		ENUM_MAX
	};	

	enum class DRAW_PRIORITY {
		NONE = 0,	
		BG,			//背景
		DEFAULT,	//デフォルト
		CHARA,		//プレイヤー等のキャラクターに関連するもの
		CLEAR,		//透過するオブジェクト
		EFFECT,		//エフェクト
		UI,			//UI
		FRONT,		//最前
		ENUM_MAX
	};	//描画順

	CScene();			//デフォルトコンストラクタ
	virtual ~CScene();	//デストラクタ
	virtual HRESULT Init(void) = 0;	//初期化処理
	virtual void Uninit(void) = 0;	//終了処理
	virtual void Update(void) = 0;	//更新処理
	virtual void Draw(void) = 0;	//描画処理

	virtual void SetPos(D3DXVECTOR3 pos);	//位置座標の設定
	virtual D3DXVECTOR3 GetPos(void);		//位置座標の取得
	virtual D3DXVECTOR3 GetLastPos(void);	//最後の位置座標の取得
	virtual void SetMove(D3DXVECTOR3 move);	//移動量の設定
	virtual D3DXVECTOR3 GetMove(void);		//移動量の取得
	virtual float GetRadius(void);			//当たり判定の半径の取得
						
	virtual int GetNumCollisionParts(void);	//当たり判定があるパーツの数の取得
	virtual void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosCol, float* const pRadiusCol);	//当たり判定の情報の取得
	virtual void UpdateMtxWorldAll(void);			//オブジェクトの全モデルのワールドマトリックスの更新
	virtual void Damage(int nDamage, bool* pDead);	//ダメージ
	virtual bool GetItem(int nTypeItem);	//アイテム取得時の処理

	static void ReleaseAll(void);	//全オブジェクトの解放処理
	static void UpdateAll(void);	//全オブジェクトの更新処理
	static void DrawAll(void);		//全オブジェクトの描画処理
	static CScene* GetSceneTopAll(void);	//全オブジェクトのリストのTopを取得
	static CScene* GetSceneNextAll(CScene* pScene);	//全オブジェクトのリストのNextを取得
	static CScene* GetSceneTopObjtype(OBJ_TYPE objType);	//オブジェクトタイプのリストのTopを取得
	static CScene* GetSceneNextObjtype(CScene* pScene);	//オブジェクトタイプのリストの引数のNextを取得
	static void ReleaseObjtype(OBJ_TYPE objtype);	//指定したタイプのオブジェクトの解放処理

	bool GetDeath(void);	//死亡フラグの取得
	void SetObjType(OBJ_TYPE objType);	//オブジェクトの種類の設定
	OBJ_TYPE GetObjType(void);			//オブジェクトの種類の取得
	void SetUpdatePriority(UPDATE_PRIORITY priority);	//更新の優先順位の設定
	void SetDrawPriority(DRAW_PRIORITY priority);		//描画の優先順位の設定

	void SetEnableCollision(bool bUse);		//当たり判定の使用状態の設定
	bool GetEnableCollision(void);			//当たり判定の使用状態の取得
	void SetTexType(CTexture::TEXTURE_TYPE type);	//テクスチャの種類の設定
	CTexture::TEXTURE_TYPE GetTexType(void);			//テクスチャの種類の取得

protected:
	void Release(void);	//オブジェクトの解放処理

private:
	bool m_bDeath;		//死亡フラグ
	OBJ_TYPE m_objType;	//オブジェクトの種類
	UPDATE_PRIORITY m_updatePriority;	//更新の優先順位
	DRAW_PRIORITY m_drawPriority;		//描画の優先順位

	bool m_bEnableCollision;	//当たり判定が有効かどうか
	CTexture::TEXTURE_TYPE m_texType;	//テクスチャのタイプ

	//全オブジェクトのリスト
	static CScene* m_pTopAll;	//先頭のオブジェクトのポインタ
	static CScene* m_pCurAll;	//最後尾のオブジェクトのポインタ
	CScene* m_pPrevAll;		//前のオブジェクトへのポインタ
	CScene* m_pNextAll;		//次のオブジェクトへのポインタ

	//オブジェクトタイプのリスト
	static CScene* m_apTopObjType[(int)OBJ_TYPE::ENUM_MAX];	//先頭のオブジェクトのポインタ
	static CScene* m_apCurObjType[(int)OBJ_TYPE::ENUM_MAX];	//最後尾のオブジェクトのポインタ
	CScene* m_pPrevObjtype;		//前のオブジェクトへのポインタ
	CScene* m_pNextObjtype;		//次のオブジェクトへのポインタ
	
	//更新順のリスト
	static CScene* m_apTopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//先頭のオブジェクトのポインタ
	static CScene* m_apCurUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//最後尾のオブジェクトのポインタ
	CScene* m_pPrevUpdate;		//前のオブジェクトへのポインタ
	CScene* m_pNextUpdate;		//次のオブジェクトへのポインタ

	//描画順のリスト
	static CScene* m_apTopDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//先頭のオブジェクトのポインタ
	static CScene* m_apCurDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//最後尾のオブジェクトのポインタ
	CScene* m_pPrevDraw;		//前のオブジェクトへのポインタ
	CScene* m_pNextDraw;		//次のオブジェクトへのポインタ
};

#endif // !_SCENE_H_