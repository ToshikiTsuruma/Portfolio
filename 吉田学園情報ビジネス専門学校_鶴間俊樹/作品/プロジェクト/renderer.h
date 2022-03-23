//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FOG_DISTANCE (3000.0f)
#define MIN_FOG_DISTANCE (500.0f)

//*****************************************************************************
// レンダリングクラス
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();	//デフォルトコンストラクタ
	~CRenderer();	//デストラクタ
	HRESULT Init(HWND hWnd, bool bWindow);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	//デバイスを取得

private:

	LPDIRECT3D9	m_pD3D;					// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Deviceオブジェクト(描画に必要)
#ifdef _DEBUG
	void DrawFPS(void);	//FPS描画処理
	LPD3DXFONT m_pFont = NULL;			// フォントへのポインタ
#endif

};

#endif // !_RENDERER_H_