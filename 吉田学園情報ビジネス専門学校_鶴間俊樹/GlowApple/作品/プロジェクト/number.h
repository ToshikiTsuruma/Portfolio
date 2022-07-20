//=============================================================================
//
// ナンバー処理 [number.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// ナンバークラス
//*****************************************************************************
class CNumber
{
public:
	CNumber();	//デフォルトコンストラクタ
	~CNumber();	//デストラクタ
	static CNumber* Create(CTexture::TEXTURE_TYPE type, int nNumDigit);	//生成処理
	HRESULT Init(D3DXVECTOR3 pos, float fSize);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetNumber(int nNumber);	//数字の設定
	void SetColor(D3DXCOLOR col);	//数字の色の設定

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	CTexture::TEXTURE_TYPE m_texType;	//テクスチャのタイプ
};

#endif // !_NUMBER_H_