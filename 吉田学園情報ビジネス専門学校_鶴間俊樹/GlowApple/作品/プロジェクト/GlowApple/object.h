//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define MAX_OBJECT (1024)
#define MAX_OBJECT_NAME (32)
//*****************************************************************************
//前方宣言
//*****************************************************************************

//*****************************************************************************
// オブジェクトクラス
//*****************************************************************************
class CObject
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
		APPLE_TREE,	//林檎の木
		OTHER,		//マップ切替時に破棄されるようなもの
		ENUM_MAX
	};	

	//更新順（更新停止でも必要な場合は定義）
	enum class UPDATE_PRIORITY {
		DEFAULT = 0,	//更新順が影響しないもの
		MENU_UPDATE,	//メニュークラス生成時に更新が停止されないオブジェクト
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
		BILLBOARD_UI,//UIのビルボード
		UI_BG,		//UIの背景
		UI,			//UI
		FRONT,		//最前
		ENUM_MAX
	};	//描画順

	CObject();			//デフォルトコンストラクタ
	virtual ~CObject();	//デストラクタ
	virtual HRESULT Init(void) = 0;	//初期化処理
	virtual void Uninit(void) = 0;	//終了処理
	virtual void Update(void) = 0;	//更新処理
	virtual void Draw(void) = 0;	//描画処理

	virtual void SetPos(D3DXVECTOR3 pos);	//位置座標の設定
	virtual D3DXVECTOR3 GetPos(void);		//位置座標の取得
	virtual D3DXVECTOR3 GetLastPos(void);	//最後の位置座標の取得
	virtual D3DXVECTOR3 GetRot(void);		//角度の取得
	virtual void SetMove(D3DXVECTOR3 move);	//移動量の設定
	virtual D3DXVECTOR3 GetMove(void);		//移動量の取得
	virtual float GetRadius(void);			//当たり判定の半径の取得
						
	virtual int GetNumCollisionParts(void);	//当たり判定があるパーツの数の取得
	virtual void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosCol, float* const pRadiusCol);	//当たり判定の情報の取得
	virtual void UpdateMtxWorldAll(void);			//オブジェクトの全モデルのワールドマトリックスの更新
	virtual void Damage(int nDamage, bool* pDead);	//ダメージ
	virtual void Dead(void) {}						//死亡処理
	virtual bool GetItem(int nTypeItem);	//アイテム取得時の処理
	virtual void SetRatioWidth(float fRatio) {}	//幅の割合の設定
	virtual void SetColor(D3DXCOLOR col) {};	//色の設定

	static void ReleaseAll(void);	//全オブジェクトの解放処理
	static void UpdateAll(void);	//全オブジェクトの更新処理
	static void DrawAll(void);		//全オブジェクトの描画処理
	static CObject* GetObjectTopAll(void);	//全オブジェクトのリストのTopを取得
	static CObject* GetObjectNextAll(CObject* pObject);	//全オブジェクトのリストのNextを取得
	static CObject* GetObjectTopObjtype(OBJ_TYPE objType);	//オブジェクトタイプのリストのTopを取得
	static CObject* GetObjectNextObjtype(CObject* pObject);	//オブジェクトタイプのリストの引数のNextを取得
	static void ReleaseObjtype(OBJ_TYPE objtype);	//指定したタイプのオブジェクトの解放処理
	static void DeadObjtype(OBJ_TYPE objtype);		//指定したタイプの死亡処理

	static void SetStopUpdateAll(bool bFlag);	//更新停止するすべての更新順の設定
	static void SetStopUpdate(UPDATE_PRIORITY update, bool bFlag) { m_abStopUpdate[(int)update] = bFlag; }	//更新停止する更新順の設定

	bool GetDeath(void);	//死亡フラグの取得
	void SetObjType(OBJ_TYPE objType);	//オブジェクトの種類の設定
	OBJ_TYPE GetObjType(void);			//オブジェクトの種類の取得
	void SetUpdatePriority(UPDATE_PRIORITY priority);	//更新の優先順位の設定
	void SetDrawPriority(DRAW_PRIORITY priority);		//描画の優先順位の設定

	void SetTexType(CTexture::TEXTURE_TYPE type);	//テクスチャの種類の設定
	CTexture::TEXTURE_TYPE GetTexType(void);		//テクスチャの種類の取得
	void SetEnableCollision(bool bEnable);	//当たり判定の有効状態を設定
	bool GetEnableCollision(void);			//当たり判定の有効状態を取得
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }	//描画状態の設定
	bool GetDraw(void) { return m_bDraw; }			//描画状態の取得

protected:
	void Release(void);	//オブジェクトの解放処理

private:
	bool m_bDeath;		//死亡フラグ
	OBJ_TYPE m_objType;	//オブジェクトの種類
	UPDATE_PRIORITY m_updatePriority;	//更新の優先順位
	DRAW_PRIORITY m_drawPriority;		//描画の優先順位

	CTexture::TEXTURE_TYPE m_texType;	//テクスチャのタイプ
	bool m_bEnableCollision;	//当たり判定が有効
	bool m_bDraw;	//描画するかどうか

	static bool m_abStopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//更新停止するオブジェクトタイプ

	//全オブジェクトのリスト
	static CObject* m_pTopAll;	//先頭のオブジェクトのポインタ
	static CObject* m_pCurAll;	//最後尾のオブジェクトのポインタ
	CObject* m_pPrevAll;		//前のオブジェクトへのポインタ
	CObject* m_pNextAll;		//次のオブジェクトへのポインタ

	//オブジェクトタイプのリスト
	static CObject* m_apTopObjType[(int)OBJ_TYPE::ENUM_MAX];	//先頭のオブジェクトのポインタ
	static CObject* m_apCurObjType[(int)OBJ_TYPE::ENUM_MAX];	//最後尾のオブジェクトのポインタ
	CObject* m_pPrevObjtype;		//前のオブジェクトへのポインタ
	CObject* m_pNextObjtype;		//次のオブジェクトへのポインタ
	
	//更新順のリスト
	static CObject* m_apTopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//先頭のオブジェクトのポインタ
	static CObject* m_apCurUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//最後尾のオブジェクトのポインタ
	CObject* m_pPrevUpdate;		//前のオブジェクトへのポインタ
	CObject* m_pNextUpdate;		//次のオブジェクトへのポインタ

	//描画順のリスト
	static CObject* m_apTopDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//先頭のオブジェクトのポインタ
	static CObject* m_apCurDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//最後尾のオブジェクトのポインタ
	CObject* m_pPrevDraw;		//前のオブジェクトへのポインタ
	CObject* m_pNextDraw;		//次のオブジェクトへのポインタ
};

#endif // !_OBJECT_H_