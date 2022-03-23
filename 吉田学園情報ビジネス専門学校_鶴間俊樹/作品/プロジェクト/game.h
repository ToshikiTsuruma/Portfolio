//=============================================================================
//
// ゲーム処理 [game.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define HEIGHT_SEA (-10.0f)	//海の高さ

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CTimer;
class CPlayer;
class CBoss;
class CScene2D;
class CSceneModel;
class CBillboard;
class CWallCylinder;

//*****************************************************************************
// ゲームクラス
//*****************************************************************************
class CGame
{
public:
	CGame();	//デフォルトコンストラクタ
	~CGame();	//デストラクタ
	static void SetScoreLate(int nScore);	//ゲーム終了時のスコアの設定
	static int GetScoreLast(void);			//ゲーム終了時のスコアの取得
	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理

	bool GetChangeStage(void);	//ステージ変更中かどうかの取得
	void SetFadeNextStage(void);//次のステージへ移行するためのフェードを設定
	void SetNextStage(void);	//次のステージを設定	現在はボスエリアのみ　今後ステージが複数出た場合は引数にタイプを追加
	void DeadBoss(void);		//ボス死亡時の処理
	void GameOver(void);		//ゲームオーバー時の処理
	bool GetGameOver(void);		//ゲームオーバーかどうかの取得
	void GameClear(void);		//ゲームクリア時の処理
	bool GetGameClear(void);	//ゲームクリアかどうかの取得
	CPlayer* GetPlayer(void);	//プレイヤーの取得
	void ReleasePlayer(void);	//ゲームが持つプレイヤーへのポインタをNULLにする
	CBoss* GetBoss(void);		//ボスの取得
	void ReleaseBoss(void);		//ゲームが持つボスへのポインタをNULLにする
	CTimer* GetTimer(void);		//タイマーの取得
	CSceneModel* GetRaft(void);			//筏の取得
	CBillboard* GetIconInteract(void);	//インタラクトキー有効時のアイコンの取得

private:
	static int m_nScoreLate;//最新のゲームクリア時のスコア

	bool m_bChangeStage;	//ステージ変更のフェード中かどうか
	CTimer* m_pTimer;		//タイマーへのポインタ
	CPlayer* m_pPlayer;		//プレイヤーへのポインタ
	CSceneModel* m_pRaft;			//筏のポインタ	ステージ移動の役割
	CBillboard* m_pIconInteract;	//インタラクトキー有効時のアイコンのポインタ
	CBoss* m_pBoss;			//ボスへのポインタ
	CScene2D* m_pGameOver;	//ゲームオーバー画面へのポインタ
	CScene2D* m_pGameClear;	//ゲームクリア表示へのポインタ
	bool m_bGameOver;		//ゲームオーバー判定
	bool m_bGameClear;		//ゲームクリア判定
	int m_nCntFinish;		//ゲーム終了後のカウント
	CBillboard* m_pFogBillboard;	//霧の最大距離に配置するビルボード
};

#endif // !_GAME_H_