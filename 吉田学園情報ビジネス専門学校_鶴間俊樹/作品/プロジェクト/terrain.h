//=============================================================================
//
// 地形処理 [terrain.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "sceneModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 地形クラス
//*****************************************************************************
class CTerrain : public CSceneModel
{
public:
	enum class TERRAIN_TYPE {
		START = 0,	//ゲーム開始時
		BOSS,		//ボス戦時
		ENUM_MAX
	};

	CTerrain();	//デフォルトコンストラクタ
	CTerrain(CModel::MODELTYPE typeModel);	//オーバーロードされたコンストラクタ
	~CTerrain();	//デストラクタ
	static CTerrain* Create(TERRAIN_TYPE typeTerrain);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static bool Collision(D3DXVECTOR3* posCollision, D3DXVECTOR3 vecStart, D3DXVECTOR3 vecEnd);	//衝突判定

private:

};

#endif // !_TERRAIN_H_