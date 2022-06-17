//=============================================================================
//
// 刀の処理 [katana.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _KATANA_H_
#define _KATANA_H_

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
class CKatana : public CWeapon
{
public:

	CKatana();	//デフォルトコンストラクタ
	CKatana(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent);	//オーバーロードされたコンストラクタ
	~CKatana();	//デストラクタ
	static CKatana* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void GetPosCollision(D3DXVECTOR3** const ppPosCollision, int* const pNumCollision);	//当たり判定の位置の取得
	void SetDrawOutline(bool bDraw);	//モデルの輪郭の表示設定

private:
	CMeshorbit* m_pMeshorbit;	//メッシュ軌道へのポインタ

};

#endif // !_KATANA_H_