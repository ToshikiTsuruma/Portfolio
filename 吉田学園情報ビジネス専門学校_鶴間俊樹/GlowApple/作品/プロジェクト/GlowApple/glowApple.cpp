//=============================================================================
//
// グロウアップル処理 [glowApple.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "glowApple.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "particleEffect.h"
#include "appleTree.h"
#include "effect.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGlowApple::CGlowApple()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CGlowApple::CGlowApple(D3DXVECTOR3 pos, CAppleTree* pTree) : CObjectModel(CModel::MODELTYPE::OBJ_APPLE, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
{
	m_pAppleTree = pTree;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGlowApple::~CGlowApple()
{

}

//=============================================================================
// グロウアップルの初期化処理
//=============================================================================
HRESULT CGlowApple::Init(void) {
	//生成時にエフェクトを生成
	CParticleEffect::PARTICLE_INFO particleInfo;	//パーティクル情報
	particleInfo.addMove = D3DXVECTOR3(0.0f, -0.8f, 0.0f); particleInfo.colEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); particleInfo.colStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	particleInfo.fAddSize = -0.8f; particleInfo.fSizeStart = 40.0f; particleInfo.fSpeedMove = 10.0f; particleInfo.nLife = 60;
	//数個重ねて生成
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		CParticleEffect::Create(particleInfo, GetPos(), 1, 1, 0.1f * D3DX_PI, true);
	}

	CObjectModel::Init();
	return S_OK;
}

//=============================================================================
// グロウアップルの終了処理
//=============================================================================
void CGlowApple::Uninit(void) {
	CObjectModel::Uninit();
}

//=============================================================================
// グロウアップルの更新処理
//=============================================================================
void CGlowApple::Update(void) {
	CObjectModel::Update();

	if (m_pAppleTree != nullptr) {
		//林檎の木が死んだ場合林檎破棄
		if (m_pAppleTree->GetDead()) {
			//爆発エフェクト
			CEffect::Create(GetPos(), CEffect::EFFECT_TYPE::EXPLOSION, 50.0f, 50.0f);
			//終了処理
			Uninit();
		}
	}
}

//=============================================================================
// グロウアップルの描画処理
//=============================================================================
void CGlowApple::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// 種類ごとの林檎の色を取得
//=============================================================================
D3DXCOLOR CGlowApple::GetAppleColor(APPLE_TYPE type) {
	switch (type)
	{
	case CGlowApple::APPLE_TYPE::RED:
		return D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		return D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		return D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		return D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		return D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	default:
		return D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
		break;
	}
}

//=============================================================================
//種類ごとの林檎の説明テキスト
//=============================================================================
void CGlowApple::GetAppleText(char* pStr, APPLE_TYPE type) {
	switch (type)
	{
	case CGlowApple::APPLE_TYPE::RED:
		strcpy(pStr, "Red Apple : 体力の最大値が増える");
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		strcpy(pStr, "Green Apple : 自動回復する");
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		strcpy(pStr, "Silver Apple : 衝撃波の数が増える");
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		strcpy(pStr, "Gold Apple : プレイヤーの移動速度＆攻撃力UP");
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		strcpy(pStr, "Black Apple : 多くの敵を呼び寄せる");
		break;
	}
}