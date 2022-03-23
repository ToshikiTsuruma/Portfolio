//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// パーティクルクラス
//*****************************************************************************
class CParticle : public CBillboard
{
public:
	CParticle();	//デフォルトコンストラクタ
	CParticle(float fDecSize);	//オーバーロードされたコンストラクタ
	~CParticle();	//デストラクタ
	static CParticle* Create(D3DXVECTOR3 pos, float fSize, float fDecSize, D3DXCOLOR col);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetDecSize(float fSize);	//エフェクトのサイズの減少量を設定

private:
	float m_fDecSize;	//サイズの減少量
};

#endif // !_PARTICLE_H_