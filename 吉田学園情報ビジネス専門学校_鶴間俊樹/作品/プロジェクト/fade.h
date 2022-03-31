//=============================================================================
//
// フェード処理 [fade.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// フェードクラス
//*****************************************************************************
class CFade
{
public:
	CFade();	//デフォルトコンストラクタ
	~CFade();	//デストラクタ
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetFade(CManager::MODE mode, float fFadeSpeed);	//フェードの設定
	void SkipFade(void);	//フェードアウトをスキップさせる
	bool GetFade(void);		//フェード中かどうかを取得
	bool GetChangeFade(void);	//フェードインからフェードアウトに切り替わったタイミングかどうか

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	float m_fFadeSpeed;	//フェードする速度
	bool m_bFadein;	//暗くなっていく状態かどうか
	float m_colA;	//カラーのアルファ値
	CManager::MODE m_nextMode;	//次のモード
	bool m_bChangeFade;	//フェードの切り替わり時

};

#endif // !_FADE_H_