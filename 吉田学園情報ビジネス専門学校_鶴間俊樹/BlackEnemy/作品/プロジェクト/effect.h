//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// エフェクトクラス
//*****************************************************************************
class CEffect : public CBillboard
{
public:
	enum class EFFECT_TYPE{
		EXPLOSION = 0,	//爆発
		PUNCH_DAMAGE,	//拳によるダメージ
		SWORD_SLASH,	//剣の斬撃
		ENEMY_DAMAGE,	//敵の攻撃によるダメージ
		GET_ITEM,		//アイテム取得
		ENUM_MAX
	};	//エフェクトの種類

	CEffect();	//デフォルトコンストラクタ
	~CEffect();	//デストラクタ
	static CEffect* Create(D3DXVECTOR3 pos, EFFECT_TYPE type, float fWidth, float fHeight);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

private:
	EFFECT_TYPE m_type;			//エフェクトのタイプ
	int m_nCntAnim;			//アニメーションカウンター
	int m_nSpanAnim;		//アニメーション切り替えの間隔
	int m_nPatternAnim;		//パターン番号
	int m_nPatternU, m_nPatternV;	//UV各方向へのパターンの数
};

#endif // !_EFFECT_H_