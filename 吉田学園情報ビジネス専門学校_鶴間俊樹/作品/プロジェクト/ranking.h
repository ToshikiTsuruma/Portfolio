//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING (10)

//*****************************************************************************
// ランキングクラス
//*****************************************************************************
class CRanking : public CScene
{
public:
	CRanking();	//デフォルトコンストラクタ
	~CRanking();	//デストラクタ
	static CRanking* Create(CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize, float fSpace, float fSpaceScore);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

private:
	typedef struct {
		CTexture::TEXTURE_TYPE type;
		D3DXVECTOR3 pos;
		float fSize;
		float fSpace;
	} NumberData;	//数字生成用のデータ

	NumberData m_numberData;	//数字生成用のデータ
	float m_fSpaceScore;		//スコアのY座標の間隔
	CScore* m_apScore[MAX_RANKING];		//ランキングのスコアへのポインタ 
	CScore* m_pScoreOutRank;			//今回のスコアがランク外だった時のスコアへのポインタ
	int m_aRankingData[MAX_RANKING];	//ランキングのデータ
};

#endif // !_RANKING_H_