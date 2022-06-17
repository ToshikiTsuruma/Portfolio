//=============================================================================
//
// 木の処理 [tree.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _TREE_H_
#define _TREE_H_

#include "sceneMotion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CBillboard;

//*****************************************************************************
// 木クラス
//*****************************************************************************
class CTree : public CSceneMotion
{
public:
	//モーションの種類
	enum class MOTION_TYPE {
		BREAK = 0,	//破壊モーション
		ENUM_MAX
	};

	CTree();	//デフォルトコンストラクタ
	~CTree();	//デストラクタ

	static void SetTreeCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fDistCircle, int nMinDist, int nMaxDist);	//同心円状にランダムに角度ずらして木を配置する
	static CTree* Create(D3DXVECTOR3 pos);	//生成処理
	static void LoadMotionInfo(void);	//モーション情報をロード
	static void UnloadMotionInfo(void);	//モーション情報をアンロード

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理
	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//当たり判定の情報の取得
	void Damage(int nDamage, bool* pDead);		//ダメージ処理

private:
	static int m_nNumParts;	//パーツ数
	static PARTS_INFO* m_pPartsInfoArray;	//パーツ情報の配列のポインタ
	static MOTION_INFO m_aMotionInfo[(int)MOTION_TYPE::ENUM_MAX];	//モーション情報

	bool m_bDead;	//死亡
	int m_nCntDead;	//死亡後破棄されるまでのカウント
	int m_nLife;	//体力
	CBillboard* m_pGaugeLife;	//体力ゲージのビルボードのポインタ
	CBillboard* m_pGaugeLifeFrame;	//体力ゲージのビルボードの枠のポインタ
	int m_nCntDrawGaugeLife;	//体力ゲージの描画時間
	bool m_bDraw;	//描画するかどうか
};

#endif // !_TREE_H_