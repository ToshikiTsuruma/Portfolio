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

		TITLE_START,	//タイトル画面のゲームスタート時
		TITLE_OPEN,		//タイトル画面のウィンドウを開いた音
		TITLE_CLOSE,	//タイトル画面のウィンドウを閉じた音

		SELECT_SWITCH,	//タイトル画面の選択切替時
		CANSEL,			//キャンセル音
		SWISH_PUNCH,	//拳を振る音
		DAMAGE_PUNCH,	//拳によるダメージ音
		ATTACK_SHOCK,	//攻撃時の衝撃音
		HEAL,			//回復音
		STUN,			//気絶音
		EXPLOSION,		//爆発音
		SPAWN_ENEMY,	//敵のスポーン
		CREATE_APPLE,	//林檎の生成音
		DEAD_TREE,		//木の死亡音
		GAMEOVER,		//ゲームオーバー
		GAMECLEAR,		//ゲームクリア

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
