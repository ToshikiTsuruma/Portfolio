//=============================================================================
//
// テクスチャ処理 [texture.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LOAD_TEXT_TEXTURE (128)		//一度にファイルから読み込むテキストのバイト数

//*****************************************************************************
// テクスチャクラス
//*****************************************************************************
class CTexture
{
public:
	enum class TEXTURE_TYPE {
		NONE = 0,

		//背景のテクスチャ
		BG_TITLE,
		BG_RESULT,

		//UIのテクスチャ
		//タイトル
		GAME_TITLE,
		QUIT_GAME,
		TITLE_SELECT_START,
		TITLE_SELECT_TUTORIAL,
		TITLE_SELECT_CREDIT,
		TITLE_TUTORIAL_001,
		TITLE_TUTORIAL_002,
		TITLE_TUTORIAL_003,
		TITLE_CREDIT,
		//ゲーム
		PAUSE_BG,
		PAUSE_START,
		PAUSE_RETRY,
		PAUSE_QUIT,
		TIMER_FRAME,
		GAUGE_LIFE_PLAYER,
		GAUGE_LIFE_BOSS,
		COMPASS_DIAL,
		COMPASS_MARK,
		ITEM_FRAME,
		WEAPON_FRAME,
		ICON_NONE,
		ICON_APPLE,
		ICON_SWORD,
		ICON_KATANA,
		CLOCK,
		ICON_INTERACT_KEYBOARD,
		ICON_INTERACT_GAMEPAD,
		GAMEOVER,
		TEXT_ITEM,
		TEXT_WEAPON,
		TEXT_GAMECLEAR,
		TEXT_RANKING,

		//モデルのテクスチャ

		//エフェクトのテクスチャ
		EFFECT_EXPLOSION,		// 爆発エフェクト
		EFFECT_DAMAGE_PUNCH,	// パンチのダメージエフェクト
		EFFECT_SWORD_SLASH,		// 剣の斬撃エフェクト
		EFFECT_DAMAGE_ENEMY,	// 敵の攻撃によるのダメージエフェクト
		EFFECT_GET_ITEM,		// コイン入手エフェクト
		EFFECT_PARTICLE,		// パーティクル

		//ナンバーのテクスチャ
		NUMBER_001,
		NUMBER_002,
		NUMBER_003,
		NUMBER_004,

		ENUM_MAX
	};

	CTexture();		//デフォルトコンストラクタ
	~CTexture();	//デストラクタ

	static HRESULT Load(void);	//テクスチャデータの読み込み
	static void Unload(void);	//テクスチャデータの解放
	static LPDIRECT3DTEXTURE9 GetTexture(TEXTURE_TYPE type);	//テクスチャの取得
	static char* GetPathName(TEXTURE_TYPE type);				//テクスチャのパスの文字列の取得

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[(int)TEXTURE_TYPE::ENUM_MAX];	//テクスチャへのポインタ
	static char m_aFilePath[(int)TEXTURE_TYPE::ENUM_MAX][MAX_LOAD_TEXT_TEXTURE];	// ファイルのパス
};

#endif // !_TEXTURE_H_