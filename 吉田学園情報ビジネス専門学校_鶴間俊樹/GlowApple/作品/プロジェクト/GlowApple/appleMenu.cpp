//=============================================================================
//
// 林檎選択メニュー処理 [appleMenu.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "appleMenu.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "object2D.h"
#include "font.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define RADIUS_MODEL_DISTANCE (60.0f)	//中心からモデルが離れている半径の距離
#define MENU_CAMERA_DISTANCE (200.0f)	//メニュー用カメラの中心からの距離
#define MENU_CAMERA_HEIGHT (60.0f)		//メニュー用カメラの中心からの高さ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CAppleMenu::CAppleMenu() : CSelectMenu3D()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleMenu::CAppleMenu(D3DXVECTOR3 posCenter) : CSelectMenu3D(NUM_SELECT_APPLE_MENU, posCenter, RADIUS_MODEL_DISTANCE, CModel::MODELTYPE::OBJ_MENUAPPLE, MENU_CAMERA_DISTANCE, MENU_CAMERA_HEIGHT)
{
	ZeroMemory(m_aTypeApple, sizeof(m_aTypeApple));
}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleMenu::~CAppleMenu()
{

}

//=============================================================================
// 林檎選択メニューの生成処理
//=============================================================================
CAppleMenu* CAppleMenu::Create(D3DXVECTOR3 posCenter, CAppleTree* pAppleTree) {
	CAppleMenu* pAppleMenu;
	pAppleMenu = new CAppleMenu(posCenter);
	if (pAppleMenu == nullptr) return nullptr;

	pAppleMenu->Init();
	pAppleMenu->m_pAppleTree = pAppleTree;

	return pAppleMenu;
}

//=============================================================================
// 林檎選択メニューの初期化処理
//=============================================================================
HRESULT CAppleMenu::Init(void) {
	//背景の生成
	CreateMenuBG(CTexture::TEXTURE_TYPE::MENU_BG, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 800.0f, 550.0f);

	//Grow UP テキストの生成
	m_pGrowUpText = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 180.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GROWUP, 400.0f, 100.0f);

	//矢印UIの生成
	m_apArrow[0] = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 300.0f, SCREEN_HEIGHT / 2.0f + 200.0f, 0.0f), CTexture::TEXTURE_TYPE::ARROW_LEFT, 50.0f, 50.0f);
	m_apArrow[1] = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 300.0f, SCREEN_HEIGHT / 2.0f + 200.0f, 0.0f), CTexture::TEXTURE_TYPE::ARROW_RIGHT, 50.0f, 50.0f);

	//ランダムで林檎の種類を決定
	SetAppleTypeRandom();

	for (int nCntModel = 0; nCntModel < GetNumSelect(); nCntModel++)
	{
		//UIモデルを取得
		CObjectModelUI* pAppleUI = GetModelUI(nCntModel);
		if (pAppleUI == nullptr) continue;

		//メニュー開いたときにも更新を続けるようにする
		pAppleUI->SetUpdatePriority(UPDATE_PRIORITY::MENU_UPDATE);

		//UIモデルのモデルを取得
		CModel* pAppleModel = pAppleUI->GetPtrModel();

		if (pAppleModel != nullptr) {		
			//種類によって色を設定
			pAppleModel->SetMaterialDiffuse(CGlowApple::GetAppleColor(m_aTypeApple[nCntModel]), 0);
			//pAppleModel->SetMaterialEmissive(GLOW_APPLE_EMISSIVE, 0);
		}

		//UIモデルの回転速度を設定
		pAppleUI->SetRotSpeed(D3DXVECTOR3(0.0f, 0.02f, 0.0f));
	}

	//説明テキストのフォントを生成
	m_pFontAppleText = CFont::Create("", DT_LEFT, SCREEN_WIDTH / 2.0f - 200.0f, SCREEN_HEIGHT / 2.0f + 190.0f, 18);
	//設定テキストを設定
	SetAppleText(0);

	CSelectMenu3D::Init();

	//メニューの回転速度設定
	SetSpeedRotModel(0.09f);

	//メニューによる更新停止
	SetStopUpdateAll(true);
	//メニュー関連のものは更新させる
	SetStopUpdate(UPDATE_PRIORITY::MENU_UPDATE, false);

	return S_OK;
}

//=============================================================================
// 林檎選択メニューの終了処理
//=============================================================================
void CAppleMenu::Uninit(void) {
	//Grow upテキストの破棄
	if (m_pGrowUpText != nullptr) {
		m_pGrowUpText->Uninit();
		m_pGrowUpText = nullptr;
	}

	//矢印UIの破棄
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_apArrow[nCnt] != nullptr) {
			m_apArrow[nCnt]->Uninit();
			m_apArrow[nCnt] = nullptr;
		}
	}

	//説明テキストの破棄
	if (m_pFontAppleText != nullptr) {
		m_pFontAppleText->Uninit();
		m_pFontAppleText = nullptr;
	}

	CSelectMenu3D::Uninit();

	//メニューによる更新停止を解除
	SetStopUpdateAll(false);
}

//=============================================================================
// 林檎選択メニューの更新処理
//=============================================================================
void CAppleMenu::Update(void) {
	CSelectMenu3D::Update();
}

//=============================================================================
// 林檎の説明テキストを設定
//=============================================================================
void CAppleMenu::SetAppleText(int nSelect) {
	if (m_pFontAppleText == nullptr) return;

	char aTextApple[128];	//林檎の説明テキスト
	//説明テキストを取得
	CGlowApple::GetAppleText(&aTextApple[0], m_aTypeApple[nSelect]);
	//テキストを設定
	m_pFontAppleText->SetText(aTextApple);
}

//=============================================================================
// 林檎の種類をランダムで決定
//=============================================================================
void CAppleMenu::SetAppleTypeRandom(void) {
	//------------------------------------------------
	// 生成する林檎の数が林檎の種類を上回った場合
	//------------------------------------------------
	if (NUM_SELECT_APPLE_MENU > (int)CGlowApple::APPLE_TYPE::ENUM_MAX) {
		//決定時のループが終わらなくなるためすべて０に設定して終了
		for (int nCnt = 0; nCnt < NUM_SELECT_APPLE_MENU; nCnt++)
		{
			m_aTypeApple[nCnt] = (CGlowApple::APPLE_TYPE) 0;
		}
		return;
	}

	//------------------------------------------------
	// ランダムに種類を決定
	//------------------------------------------------
	for (int nCntType = 0; nCntType < NUM_SELECT_APPLE_MENU; nCntType++)
	{
		int nRandType;	//林檎の種類
		//まだ決まっていない種類になるまでループ
		while (true)
		{
			nRandType = rand() % (int)CGlowApple::APPLE_TYPE::ENUM_MAX;	//ランダムで林檎の種類を決定

			bool bContinueLoop = false;	//ループを続けるかどうか
			for (int nAppleType = 0; nAppleType < nCntType; nAppleType++)
			{
				//すでに生成されている場合
				if (nRandType == (int)m_aTypeApple[nAppleType]) {
					bContinueLoop = true;	//ループを続ける
					break;
				}
			}

			//ループを抜ける
			if (!bContinueLoop) break;
		}

		//種類の設定
		m_aTypeApple[nCntType] = (CGlowApple::APPLE_TYPE)nRandType;
	}

	//------------------------------------------------
	// 種類を昇順にソートする
	//------------------------------------------------
	for (int nCntSort1 = 0; nCntSort1 < NUM_SELECT_APPLE_MENU - 1; nCntSort1++)
	{
		for (int nCntSort2 = 0; nCntSort2 < NUM_SELECT_APPLE_MENU - 1 - nCntSort1; nCntSort2++)
		{
			if (m_aTypeApple[nCntSort2] > m_aTypeApple[nCntSort2 + 1]) {
				CGlowApple::APPLE_TYPE typeTemp = m_aTypeApple[nCntSort2];	//一時的に保存
				//変数の入れ替え
				m_aTypeApple[nCntSort2] = m_aTypeApple[nCntSort2 + 1];
				m_aTypeApple[nCntSort2 + 1] = typeTemp;
			}
		}
	}
}

//=============================================================================
// 現在選択中の林檎の種類を取得
//=============================================================================
CGlowApple::APPLE_TYPE CAppleMenu::GetSelectAppleType(void) {
	return m_aTypeApple[GetIdxCurSelect()];
}

//=============================================================================
// 選択の変更が終了したときの処理
//=============================================================================
void CAppleMenu::EndChangeSelect(void) {
	//テキストを変更
	SetAppleText(GetIdxCurSelect());
}