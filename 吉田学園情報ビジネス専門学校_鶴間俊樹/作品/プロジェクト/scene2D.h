//=============================================================================
//
// 2Dオブジェクト処理 [scene2D.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 2Dオブジェクトクラス
//*****************************************************************************
class CScene2D : public CScene
{
public:
	CScene2D();		//デフォルトコンストラクタ
	~CScene2D();	//デストラクタ
	static CScene2D* Create(D3DXVECTOR3 pos, CTexture::TEXTURE_TYPE type, float fWidth, float fHeight);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

	void SetPos(D3DXVECTOR3 pos);	//位置座標の設定
	D3DXVECTOR3 GetPos(void);		//位置座標の取得
	void SetMove(D3DXVECTOR3 move);	//移動量の設定
	D3DXVECTOR3 GetMove(void);		//移動量の取得
	void SetSize(D3DXVECTOR3 size);	//サイズの設定
	D3DXVECTOR3 GetSize(void);		//サイズの取得
	void SetRatioWidth(float fRatio);	//幅の割合の設定
	void SetRatioHeight(float fRatio);	//高さの割合の設定
	void SetAngle(float fAngle);	//角度の設定
	float GetAngle(void);			//角度の取得
	void SetColor(D3DXCOLOR col);	//カラーの設定
	D3DXCOLOR GetColor(void);		//カラーの取得
	void SetTexPos(float startU, float startV, float endU, float endV);	//テクスチャ座標の設定

private:
	void SetVtxPos(void);	//各頂点の位置を更新

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3	m_pos;	//ポリゴンの中心座標
	D3DXVECTOR3 m_move;	//移動量
	float m_fWidth;		//ポリゴンの幅
	float m_fRatioWidth;//ポリゴンの幅の割合
	float m_fHeight;	//ポリゴンの高さ
	float m_fRatioHeight;//ポリゴンの高さの割合
	float m_fAngle;		//ポリゴンの角度
	D3DXCOLOR m_col;	//ポリゴンの色
};

#endif // !_SCENE2D_H_
