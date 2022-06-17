//=============================================================================
//
// タイトル処理 [title.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CScene2D;
//*****************************************************************************
// タイトルクラス
//*****************************************************************************
class CTitle
{
public:
	CTitle();	//デフォルトコンストラクタ
	~CTitle();	//デストラクタ
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理

private:
	//タイトルの選択肢の種類
	enum class SELECT_TYPE {
		START = 0,
		TUTORIAL,
		CREDIT,
		ENUM_MAX
	};	

		//チュートリアル画面の種類
	enum class TUTORIAL_TYPE {
		CONTROLL = 0,	//操作画面
		GAME,	//ゲーム画面
		BOSS,	//ボス画面
		ENUM_MAX
	};	

	CScene2D* m_apScene2D[(int)SELECT_TYPE::ENUM_MAX];	//選択肢のポリゴン
	bool m_bFade;			//フェード開始済みかどうか
	CScene2D* m_pTutorial;	//チュートリアル画面のポリゴン
	bool m_bTutorial;		//チュートリアルを開いている状態かどうか
	TUTORIAL_TYPE m_typeTutorial;	//チュートリアル画面の種類
	CScene2D* m_pCredit;	//クレジット画面のポリゴン
	bool m_bCredit;			//クレジットを開いている状態かどうか
	int m_nSelect;			//現在の選択
};

#endif // !_TITLE_H_