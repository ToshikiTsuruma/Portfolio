//=============================================================================
//
// タイマー処理 [timer.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "object.h"
#include "number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************

//*****************************************************************************
// タイマークラス
//*****************************************************************************
class CTimer : public CObject
{
public:
	CTimer();	//デフォルトコンストラクタ
	CTimer(int nNumDigit, CTexture::TEXTURE_TYPE type);	//オーバーロードされたコンストラクタ
	~CTimer();	//デストラクタ
	static CTimer* Create(int nStartTime, int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize, float fSpace);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void AddTime(int nTime);	//タイマーの時間の追加
	int GetTime(void);	//タイマーの残り時間を取得
	int GetCountTimer(void) { return m_nCntTime; }	//タイマーのカウントを取得
	void SetStop(bool bStop);	//タイマーの停止状態を設定

private:
	const int m_nNumDigit;		//スコアの桁数
	CNumber* m_pNumberArray;	//ナンバーの動的配列の先頭ポインタ
	bool m_bStop;		//タイマーが停止中かどうか
	int m_nTime;		//タイマーの残り時間
	int m_nCntTime;		//タイマーの残り時間が減るまでのカウント
};

#endif // !_TIMER_H_