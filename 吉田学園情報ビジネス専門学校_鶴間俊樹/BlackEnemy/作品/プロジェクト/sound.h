//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドクラス
//*****************************************************************************
class CSound
{
public:
	enum class SOUND_LABEL {
		NONE = 0,
		BGM_TITLE,		//タイトルBGM
		BGM_GAME,		//ゲームBGM
		BGM_GAME_BOSS,	//ボスと対峙時のゲームBGM
		BGM_RESULT,		//リザルトBGM

		TITLE_START,	//タイトル画面のゲームスタート時
		TITLE_SWITCH,	//タイトル画面の選択切替時
		TITLE_OPEN,		//タイトル画面のウィンドウを開いた音
		TITLE_CLOSE,	//タイトル画面のウィンドウを閉じた音

		CANSEL,			//キャンセル音
		DODGE,			//回避音
		SWISH_PUNCH,	//拳を振る音
		SWISH_SWORD,	//剣を振る音
		SWISH_KATANA,	//刀を振る音
		DAMAGE_PUNCH,	//拳によるダメージ音
		DAMAGE_SWORD,	//剣によるダメージ音
		DAMAGE_SWORD_CRITICAL,	//剣によるクリティカルダメージ音
		CHANGE_PUNCH,	//拳に武器を変更する音
		CHANGE_SWORD,	//剣に武器を変更する音
		CHANGE_KATANA,	//刀に武器を変更する音
		ATTACK_BOSS,	//ボスの攻撃時の衝撃音
		HEAL,			//回復音
		STUN,			//気絶音
		EXPLOSION,		//爆発音
		GET_ITEM,		//アイテム取得音
		ADD_TIME,		//時間追加音
		TIME_ALERT,		//時間の警告音
		BREAK_TREE,		//木が壊れる音
		GAUGE,			//ゲージ進行音
		CHANGE_STAGE,	//ステージ変更時
		GAMEOVER,		//ゲームオーバー
		GAMECLEAR,		//ゲームクリア

		PAUSE,			//ポーズ
		PAUSE_SELECT,	//ポーズ画面での選択

		ENUM_MAX
	};

	CSound();	//デフォルトコンストラクタ
	~CSound();	//デストラクタ
	HRESULT Init(HWND hWnd);	//初期化処理
	void Uninit(void);			//終了処理
	HRESULT PlaySound(SOUND_LABEL label);	//音の再生
	void StopSound(SOUND_LABEL label);		//音の停止
	void StopSound(void);					//音の停止
	void PauseSound(SOUND_LABEL label);		//音の一時停止
	void StartSound(SOUND_LABEL label);		//音の再開

	static void SetBGM(SOUND_LABEL label);			//再生中BGMの設定
	static SOUND_LABEL GetBGM(void);				//再生中BGMの取得

private:
	// パラメータ構造体定義
	typedef struct
	{
		char *filename;		// ファイル名
		int nCntLoop;		// ループカウント
	} PARAM;

	//メンバ関数
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	
	//メンバ変数
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[(int)SOUND_LABEL::ENUM_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[(int)SOUND_LABEL::ENUM_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[(int)SOUND_LABEL::ENUM_MAX] = {};					// オーディオデータサイズ
	static PARAM m_aParam[(int)SOUND_LABEL::ENUM_MAX];						//ファイル名とループ

	static SOUND_LABEL m_playBGM;	//再生中のBGM
};

#endif // !_SOUND_H_
