//=============================================================================
//
// 武器の処理 [weapon.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "sceneModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// 武器クラス
//*****************************************************************************
class CWeapon : public CSceneModel
{
public:

	CWeapon();	//デフォルトコンストラクタ
	CWeapon(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine);	//オーバーロードされたコンストラクタ
	~CWeapon();	//デストラクタ
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理
	virtual void GetPosCollision(D3DXVECTOR3** const ppPosCollision, int* const pNumCollision) = 0;	//当たり判定の位置の取得
	virtual void SetDrawOutline(bool bDraw);	//モデルの輪郭の表示設定
	void SetAttack(bool bAttack);	//攻撃中かどうかの設定
	bool GetAttack(void);	//攻撃中かどうかの取得

private:
	bool m_bAttack;	//攻撃中

};

#endif // !_WEAPON_H_