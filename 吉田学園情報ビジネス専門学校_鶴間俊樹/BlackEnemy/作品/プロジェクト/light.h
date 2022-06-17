//=============================================================================
//
// ライト処理 [ligth.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// ライトクラス
//*****************************************************************************
class CLight
{
public:
	CLight();	//デフォルトコンストラクタ
	~CLight();	//デストラクタ
	static CLight* Create(D3DXVECTOR3 vecDir, D3DXCOLOR col);	//生成処理
	HRESULT Init(D3DXVECTOR3 vecDir, D3DXCOLOR col);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	static void SetIdxNext(CLight* pLight);	//次のライトのインデックスを設定

private:
	D3DLIGHT9 m_light;		//ライトの情報
	D3DXVECTOR3 m_vecDir;	//ライトの方向ベクトル
	int m_nIdx;				//ライトのインデックス

	//ライトのリスト
	static CLight* m_pTop;	//先頭のオブジェクトのポインタ
	static CLight* m_pCur;	//最後尾のオブジェクトのポインタ
	CLight* m_pPrev;		//前のオブジェクトへのポインタ
	CLight* m_pNext;		//次のオブジェクトへのポインタ
};

#endif // !_LIGHT_H_