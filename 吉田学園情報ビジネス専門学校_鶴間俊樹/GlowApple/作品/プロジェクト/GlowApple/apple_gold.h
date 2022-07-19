//=============================================================================
//
// 金林檎処理 [apple_gold.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _APPLE_GOLD_H_
#define _APPLE_GOLD_H_

#include "glowApple.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// 金林檎クラス
//*****************************************************************************
class CAppleGold : public CGlowApple
{
public:
	CAppleGold();		//デフォルトコンストラクタ
	CAppleGold(D3DXVECTOR3 pos, CAppleTree* pTree);		//オーバーロードされたコンストラクタ
	virtual ~CAppleGold();		//デストラクタ
	static CAppleGold* Create(D3DXVECTOR3 pos, CAppleTree* pTree);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

private:
	void ShootBullet(void);	//弾の発射

	int m_nCntBullet;	//弾生成カウンター
};

#endif // !_APPLE_GOLD_H_