//=============================================================================
//
// モデル処理 [model.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LOAD_TEXT_MODEL (128)	//一度にファイルから読み込むテキストのバイト数
#define MAX_MATERIAL (16)			//モデルごとのマテリアルの最大数

//*****************************************************************************
// モデルクラス
//*****************************************************************************
class CModel
{
public:
	enum class MODELTYPE{
		//地形
		TERRAIN_START = 0,	//地形
		TERRAIN_BOSS,	//ボスが居る地形
		//武器
		WEAPON_KATANA,	//刀
		WEAPON_SWORD,	//剣
		//アイテム
		ITEM_APPLE, //リンゴ

		//プレイヤー
		pl_0, pl_1, pl_2, pl_3, _pl_4, _pl_5, pl_6, pl_7, pl_8, pl_9,

		//敵
		eNo_0, eNo_1, eNo_2, eNo_3, eNo_4,	//通常敵
		eHu_0, eHu_1, eHu_2, eHu_3, eHu_4, eHu_5, eHu_6, eHu_7, eHu_8, eHu_9,	//人形敵
		eBo_0, eBo_1, eBo_2, eBo_3, eBo_4, eBo_5, eBo_6, eBo_7, eBo_8,	//ボス

		//木
		tr_0, tr_1,

		//その他
		RAFT,

		ENUM_MAX	//最大数
	};

	typedef struct
	{
		LPD3DXMESH pMesh;	//メッシュ（頂点情報）へのポインタ
		LPD3DXBUFFER pBuffMat;	//マテリアルへのポインタ
		DWORD nNumMat;	//マテリアルの数
	}ModelData;

	CModel();	//デフォルトコンストラクタ
	~CModel();	//デストラクタ
	static CModel* Create(MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine);	//生成処理
	static HRESULT Load(void);	//モデルデータの読み込み
	static void Unload(void);	//モデルデータの解放
	static ModelData GetModelData(MODELTYPE type);	//モデルデータの取得
	static char* GetPathName(MODELTYPE type);		//テクスチャのパスの文字列の取得
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetPos(D3DXVECTOR3 pos);	//位置座標の設定
	D3DXVECTOR3 GetPos(void);		//位置座標の取得
	void SetRot(D3DXVECTOR3 rot);	//角度の設定
	D3DXVECTOR3 GetRot(void);		//角度の取得
	void UpdateMtxWorld(void);		//ワールドマトリックスの更新
	D3DXMATRIX GetMtxWorld(void);			//ワールドマトリックスの取得
	D3DXMATRIX* GetPtrMtxWorld(void);		//ワールドマトリックスのポインタの取得
	MODELTYPE GetModelType(void);			//モデルのタイプの取得
	void SetMaterialAlpha(float fAlpha);	//マテリアルのアルファ値を設定
	float GetMaterialAlpha(void);			//マテリアルのアルファ値を取得
	void SetColorOutline(D3DXCOLOR col);	//輪郭の色の指定
	void SetDrawOutline(bool bDraw);		//輪郭の表示設定

private:
	static ModelData m_aModelData[(int)MODELTYPE::ENUM_MAX];	//全モデルの情報
	static char m_aFilePath[(int)MODELTYPE::ENUM_MAX][MAX_LOAD_TEXT_MODEL];	// モデルのテキストファイルのパス
	static D3DXMATERIAL m_aMatDefault[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL];	//モデルのマテリアルの初期値
	static CTexture::TEXTURE_TYPE m_aTexType[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL];	//マテリアルごとのテクスチャの種類

	MODELTYPE m_modelType;	//モデルの種類
	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_rot;		//向き
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	CModel* m_pParent;		//親のモデルへのポインタ
	int m_nIdxParent;		//親のインデックス
	D3DXMATERIAL m_aMat[MAX_MATERIAL];	//モデルのマテリアル

	bool m_bOutline;			//輪郭があるかどうか
	LPD3DXMESH m_pCloneMesh;	//複製したメッシュのポインタ
};

#endif // !_MODEL_H_