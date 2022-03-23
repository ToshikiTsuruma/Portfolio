//=============================================================================
//
// メッシュシリンダー処理 [meshcylinder.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"
#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// メッシュシリンダークラス
//*****************************************************************************
class CMeshcylinder : public CScene3D
{
public:
	CMeshcylinder();	//デフォルトコンストラクタ
	CMeshcylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadius, int nNumY, float fHeight, bool bOut, D3DXCOLOR col);	//オーバーロードされたコンストラクタ
	~CMeshcylinder();	//デストラクタ
	static CMeshcylinder* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadius, int nNumY, float fHeight, bool bOut, D3DXCOLOR col);	//生成処理
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理
	float GetRadius(void);		//半径の取得
	void SetColor(D3DXCOLOR col);	//色の設定
	D3DXCOLOR GetColor(void);		//色の取得

private:
	const int m_nNumXZ;	//XZ方向のポリゴンの数
	const int m_nNumY;	//Y方向のポリゴンの数
	float m_fHeight;	//ポリゴンの高さ
	float m_fRadius;	//円柱の半径
	bool m_bOut;		//ポリゴンが外向きかどうか
	D3DXCOLOR m_col;	//色
};

#endif // !_MESHCYLINDER_H_