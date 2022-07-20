//=============================================================================
//
// 黒林檎処理 [apple_black.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "apple_black.h"
#include "appleTree.h"
#include "manager.h"
#include "sound.h"
#include "gameScene.h"
#include "enemySpawner.h"
#include "effect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define HEAL_VALUE (2)	//回復量

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CAppleBlack::CAppleBlack()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleBlack::CAppleBlack(D3DXVECTOR3 pos, CAppleTree* pTree) : CGlowApple(pos, pTree)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleBlack::~CAppleBlack()
{

}

//=============================================================================
// 黒林檎の生成処理
//=============================================================================
CAppleBlack* CAppleBlack::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	CAppleBlack* pAppleBlack;
	pAppleBlack = new CAppleBlack(pos, pTree);
	if (pAppleBlack == nullptr) return nullptr;

	pAppleBlack->Init();

	return pAppleBlack;
}

//=============================================================================
// 黒林檎の初期化処理
//=============================================================================
HRESULT CAppleBlack::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//モデルの色を変更
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::BLACK), 0);
	}

	//発光表現のためにパーティクルを生成
	CreateGlowParticle(APPLE_TYPE::BLACK);

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// 黒林檎の終了処理
//=============================================================================
void CAppleBlack::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// 黒林檎の更新処理
//=============================================================================
void CAppleBlack::Update(void) {

	CGlowApple::Update();
}

//=============================================================================
// 黒林檎の描画処理
//=============================================================================
void CAppleBlack::Draw(void) {
	CGlowApple::Draw();
}

//=============================================================================
// すべての黒林檎のHP吸収
//=============================================================================
void CAppleBlack::DrainAllApple(void) {
	CObject* pObject = GetObjectTopAll();	//全オブジェクトのリストの先頭を取得

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//リストの次のオブジェクトのポインタを取得

		//オブジェクトタイプの確認
		bool bMatchType = false;
		if (pObject->GetObjType() & OBJTYPE_APPLE) bMatchType = true;

		//死亡フラグ確認
		bool bDeath = pObject->GetDeath();

		//黒林檎にダイナミックキャスト
		CAppleBlack* pAppleBlack = dynamic_cast<CAppleBlack*>(pObject);

		//次のループに飛ばす
		if (!bMatchType || bDeath || pAppleBlack == nullptr) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;
		}

		//黒林檎のHP吸収処理を行う
		pAppleBlack->Drain();

		pObject = pObjNext;	//リストの次のオブジェクトを代入
	}
}

//=============================================================================
// 体力の吸収
//=============================================================================
void CAppleBlack::Drain(void) {
	//林檎の木を回復する
	CAppleTree* pAppleTree = GetAppleTree();
	if (pAppleTree != nullptr) {
		pAppleTree->HealLife(HEAL_VALUE);
	}
	//林檎の位置からHP吸収のエフェクトを出す
	D3DXVECTOR3 posEffect = GetPos();
	posEffect.y -= 15.0f;	//位置の調整
	CEffect::Create(posEffect, CEffect::EFFECT_TYPE::DRAIN, 80.0f, 80.0f, false);

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	CSound* pSound = nullptr;	//サウンドへのポインタ
	//サウンドの取得
	if (pManager != nullptr) pSound = pManager->GetSound();
	//HP吸収音の再生
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::DRAIN);
}