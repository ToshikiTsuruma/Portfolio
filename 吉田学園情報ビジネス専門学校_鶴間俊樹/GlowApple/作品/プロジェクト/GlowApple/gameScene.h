//=============================================================================
//
// ゲームシーン処理 [gameScene.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CStage;
class CAppleTree;
class CTimer;
class CSelectMenu2D;
class CEnemySpawner;

//*****************************************************************************
// ゲームシーンクラス
//*****************************************************************************
class CGameScene : public CScene
{
public:
	CGameScene();	//デフォルトコンストラクタ
	virtual ~CGameScene();	//デストラクタ
	void Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void GameClear(void);	//ゲームクリア
	void GameOver(void);	//ゲームオーバー

	bool GetGameClear(void) { return m_bGameClear; }	//ゲームクリア状態のフラグ
	bool GetGameOver(void) { return m_bGameOver; }		//ゲームオーバー状態のフラグ

	CAppleTree* GetAppleTree(void) { return m_pAppleTree; }	//林檎の木の取得
	CEnemySpawner* GetEnemySpawner(void) { return m_pEnemySpawner; }	//敵のスポナーを取得

private:
	void CreateMenuEndGame(void);	//ゲーム終了時の選択メニューの生成

	bool m_bGameClear;	//ゲームオーバー
	bool m_bGameOver;	//ゲームオーバー

	CStage* m_pStage;	//ステージへのポインタ
	CAppleTree* m_pAppleTree;	//林檎の木のポインタ
	CTimer* m_pTimer;	//ゲームのタイマー
	CSelectMenu2D* m_pMenuGameEnd;	//ゲーム終了時の選択メニュー
	CEnemySpawner* m_pEnemySpawner;	//敵のスポナー
};

#endif // !_GAME_SCENE_H_