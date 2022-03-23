//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// メッシュフィールドクラス
//*****************************************************************************
class CMeshfield : public CScene3D
{
public:
	CMeshfield();		//デフォルトコンストラクタ
	~CMeshfield();	//デストラクタ
	static CMeshfield* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumX, int nNumZ, float fWidth, float fDepth);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

private:
	int m_nNumX;	//X方向のポリゴンの数
	int m_nNumZ;	//Z方向のポリゴンの数
	float m_fWidth;	//ポリゴンの幅
	float m_fDepth;//ポリゴンの高さ
};

#endif // !_MESHFIELD_H_