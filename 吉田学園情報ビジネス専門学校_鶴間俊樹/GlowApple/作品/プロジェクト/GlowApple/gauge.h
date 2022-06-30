//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"
#include "texture.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// ゲージクラス
//*****************************************************************************
class CGauge
{
public:

	CGauge();		//デフォルトコンストラクタ
	CGauge(int nMaxValue, int nValue, int nDrawLifeMax);		//オーバーロードされたコンストラクタ
	virtual ~CGauge();		//デストラクタ
	virtual void Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void CreateGauge(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) = 0;			//ゲージを生成
	virtual void CreateGaugeBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) = 0;		//ゲージの背景を生成
	virtual void CreateGaugeFrame(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) = 0;	//ゲージの枠を生成

	void AddGaugeValue(int nAddValue);	//ゲージの値を追加
	void SetGaugeValue(int nValue);	//ゲージの値を設定
	void SetMaxValue(int nValue);	//ゲージの最大値を設定

	void SetGaugeDraw(bool bDraw);	//ゲージの描画状態の設定

	void SetAllGaugePos(D3DXVECTOR3 pos);	//ゲージのすべての位置の設定
	void SetGaugePos(D3DXVECTOR3 pos);		//ゲージの位置の設定
	void SetGaugeBGPos(D3DXVECTOR3 pos);	//ゲージの背景の位置の設定
	void SetGaugeFramePos(D3DXVECTOR3 pos);	//ゲージの枠の位置の設定

	void SetGaugeColor(D3DXCOLOR col);		//ゲージの色の設定
	void SetGaugeColorDanger(D3DXCOLOR col) { m_colGaugeDanger = col; }	//ゲージのデフォルトカラーの設定
	void SetDangerValue(int nValue) { m_nDangerValue = nValue; }	//ゲージの危険時の色になる値を設定
	void SetGaugeBGColor(D3DXCOLOR col);	//ゲージの背景の色の設定
	void SetGaugeFrameColor(D3DXCOLOR col);	//ゲージの枠の色の設定

protected:
	CObject** GetGaugePtr(void) { return &m_pGauge; }			//ゲージのダブルポインタを取得
	CObject** GetGaugeBGPtr(void) { return &m_pGaugeBG; }		//ゲージの背景のダブルポインタを取得
	CObject** GetGaugeFramePtr(void) { return &m_pGaugeFrame; }	//ゲージの枠のダブルポインタを取得

private:
	const int m_nDrawLifeMax;	//描画時間の最大
	const int m_nMaxValueDefault;	//生成時の値の最大値
	int m_nDrawLife;	//描画時間
	int m_nMaxValue;	//ゲージの値の最大
	int m_nValue;		//ゲージの値

	CObject* m_pGauge;		//2Dかビルボードのゲージのオブジェクトのポインタ
	CObject* m_pGaugeBG;	//2Dかビルボードのゲージの背景のオブジェクトのポインタ
	CObject* m_pGaugeFrame;	//2Dかビルボードのゲージの枠のオブジェクトのポインタ

	D3DXCOLOR m_colGaugeDefault;	//ゲージのデフォルト色
	D3DXCOLOR m_colGaugeDanger;		//ゲージの危険時の色
	int m_nDangerValue;	//ゲージが危険時の色になる値
};

#endif // !_GAUGE_H_