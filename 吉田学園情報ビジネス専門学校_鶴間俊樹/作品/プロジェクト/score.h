//=============================================================================
//
// スコア処理 [score.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"
#include "number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DIGIT_SCORE_RANKING (6)	//ランキング表示時のスコアの最大桁数

//*****************************************************************************
//前方宣言
//*****************************************************************************

//*****************************************************************************
// スコアクラス
//*****************************************************************************
class CScore : public CScene
{
public:
	CScore();	//デフォルトコンストラクタ
	CScore(int nNumDigit, CTexture::TEXTURE_TYPE type);	//オーバーロードされたコンストラクタ
	~CScore();	//デストラクタ
	static CScore* Create(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize, float fSpace, int nLife);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetScore(int nScore);	//スコアの設定
	void AddScore(int nValue);	//スコアの追加
	int GetScore(void);			//スコアの取得
	void SetColor(D3DXCOLOR col);	//スコアの色の設定

private:
	const int m_nNumDigit;		//スコアの桁数
	CNumber* m_pNumberArray;	//ナンバーの動的配列の先頭ポインタ
	int m_nScore;	//スコア
	int m_nLife;	//ライフ
};

#endif // !_SCORE_H_