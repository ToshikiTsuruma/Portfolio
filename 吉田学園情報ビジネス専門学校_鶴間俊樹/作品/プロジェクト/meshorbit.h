//=============================================================================
//
// メッシュ軌道処理 [meshorbit.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// メッシュ軌道クラス
//*****************************************************************************
class CMeshorbit : public CScene
{
public:
	CMeshorbit();		//デフォルトコンストラクタ
	CMeshorbit(int nNumVtx);	//オーバーロードされたコンストラクタ
	~CMeshorbit();	//デストラクタ
	static CMeshorbit* Create(D3DXMATRIX* pMtxParent, int nNumVtx,int nSpanUpdate , D3DXVECTOR3 offset0, D3DXVECTOR3 offset1, D3DXCOLOR col0, D3DXCOLOR col1);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetBindPos(void);	//メッシュ軌道の位置を固定
	bool GetBindPos(void);	//バインド状態の取得
	void SetVisible(bool bVisible);	//可視状態の設定
	bool GetVisible(void);	//可視状態の取得

private:
	const int m_nNumVtx;		//頂点数
	D3DXMATRIX* m_pMtxParent;	//親マトリックスへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファのポインタ
	D3DXVECTOR3 m_aOffset[2];	//両端のオフセット
	D3DXCOLOR m_aCol[2];		//両端の基準の色
	int m_nSpanUpdate;		//頂点位置を更新する間隔
	int m_nCntUpdate;		//頂点位置を更新するカウンタ
	bool m_bBindPos;		//位置の固定状態
	bool m_bVisible;		//可視状態
};

#endif // !_MESHORBIT_H_