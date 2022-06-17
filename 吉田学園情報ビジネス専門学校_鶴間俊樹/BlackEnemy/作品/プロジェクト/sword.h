//=============================================================================
//
// 剣の処理 [sword.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SWORD_H_
#define _SWORD_H_

#include "weapon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMeshorbit;

//*****************************************************************************
// 武器クラス
//*****************************************************************************
class CSword : public CWeapon
{
public:

	CSword();	//デフォルトコンストラクタ
	CSword(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent);	//オーバーロードされたコンストラクタ
	~CSword();	//デストラクタ
	static CSword* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void GetPosCollision(D3DXVECTOR3** const ppPosCollision, int* const pNumCollision);	//当たり判定の位置の取得

private:
	CMeshorbit* m_pMeshorbit;	//メッシュ軌道へのポインタ

};

#endif // !_SWORD_H_