//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_LIGHT (3)

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CRenderer;
class CInput;
class CInputKeyboard;
class CInputGamepadX;
class CCamera;
class CLight;
class CSound;
class CFade;
class CTitle;
class CGame;
class CResult;
class CPause;

//*****************************************************************************
// マネージャークラス
//*****************************************************************************
class CManager
{
public:
	enum class MODE {
		TITLE = 0,
		GAME,
		RESULT,
		ENUM_MAX
	};

	static void Create(void);		//マネージャーの生成
	static void Destroy(void);		//マネージャーの破棄
	static CManager* GetManager(void);	//マネージャーの取得

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetMode(MODE mode);	//モードの設定
	MODE GetMode(void);			//モードの取得

	CRenderer *GetRenderer(void);	//レンダラーの取得
	CInput *GetInputCur(void);	//現在の入力デバイスの取得
	CCamera* GetCamera(void);	//カメラの取得
	CSound *GetSound(void);		//サウンドの取得
	CFade *GetFade(void);		//フェードの取得
	CPause *GetPause(void);		//ポーズの取得
	CGame *GetGame(void);		//ゲームモードの取得

private:
	CManager();		//デフォルトコンストラクタ
	~CManager();	//デストラクタ
	static CManager* m_pManager;	//マネージャーのインスタンス

	CRenderer *m_pRenderer;				//レンダラーへのポインタ
	CInput *m_pInputCur;				//現在の入力デバイスへのポインタ
	CInputKeyboard *m_pInputKeyboard;	//キーボードへのポインタ
	CInputGamepadX *m_pInputGamepadX;	//ゲームパッドへのポインタ(Xinput)
	CCamera* m_pCamera;					//カメラへのポインタ
	CLight* m_apLight[NUM_LIGHT];		//ライトへのポインタ
	CSound *m_pSound;	//サウンドへのポインタ
	CFade *m_pFade;		//フェードへのポインタ
	MODE m_mode;		//モード
	CTitle *m_pTitle;	//タイトルモード
	CGame *m_pGame;		//ゲームモード
	CResult *m_pResult;	//リザルトモード
	CPause *m_pPause;	//ポーズ

};

#endif // !_MANAGER_H_
