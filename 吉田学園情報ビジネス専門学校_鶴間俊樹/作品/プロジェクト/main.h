//=============================================================================
//
// メイン処理 [main.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800) //ビルド時の警告対処用マクロ
#include"dinput.h"//入力処理に必要
#include "xaudio2.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <XInput.h>

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib") //描画に必要
#pragma comment(lib,"d3dx9.lib") //[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib") //DirectXコンポーネント（部品）使用に必要
#pragma comment(lib,"dinput8.lib") //入力処理に必要
#pragma comment(lib, "winmm.lib") //秒数計測に必要
#pragma comment(lib,"xinput.lib") //xinputの入力

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH (1280) //ウィンドウの幅
#define SCREEN_HEIGHT (720) //ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)					//2Dの柔軟な頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//3Dの柔軟な頂点フォーマット
#define FPS (60)		// FPSの値

//*****************************************************************************
// 構造体定義
//*****************************************************************************
//2D頂点の構造体を定義
typedef struct {
	D3DXVECTOR3 pos; //頂点座標
	float rhw; //1.0fで固定
	D3DCOLOR col; //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
} VERTEX_2D;

//頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
} VERTEX_3D;

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
int GetFPS(void);

#endif