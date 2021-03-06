//=============================================================================
//
// タイトルシーン処理 [titleScene.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CSelectMenu2D;
class CObject2D;

//*****************************************************************************
// タイトルシーンクラス
//*****************************************************************************
class CTitleScene : public CScene
{
public:
	CTitleScene();	//デフォルトコンストラクタ
	virtual ~CTitleScene();	//デストラクタ
	void Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理

private:
	CSelectMenu2D* m_pSelectMenuTitle;	//タイトルの選択肢
	CObject2D* m_pTutorial;	//チュートリアル画面
	CObject2D* m_pCreadit;	//クレジット画面

	bool m_bBeginFade;	//シーン遷移開始したかどうか

};

#endif // !_TITLE_SCENE_H_