//=============================================================================
//
// 黒林檎処理 [apple_black.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _APPLE_BLACK_H_
#define _APPLE_BLACK_H_

#include "glowApple.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// 黒林檎クラス
//*****************************************************************************
class CAppleBlack : public CGlowApple
{
public:
	CAppleBlack();		//デフォルトコンストラクタ
	CAppleBlack(D3DXVECTOR3 pos, CAppleTree* pTree);		//オーバーロードされたコンストラクタ
	virtual ~CAppleBlack();		//デストラクタ
	static CAppleBlack* Create(D3DXVECTOR3 pos, CAppleTree* pTree);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

	static void DrainAllApple(void);	//すべての黒林檎のHP吸収
	void Drain(void);	//体力の吸収

private:

};

#endif // !_APPLE_BLACK_H_