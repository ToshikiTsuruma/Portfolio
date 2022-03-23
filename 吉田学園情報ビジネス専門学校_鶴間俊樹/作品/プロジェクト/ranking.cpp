//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "ranking.h"
#include "manager.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_FILE_NAME_SAVE_RANKING "data/TEXT/save_ranking.txt"
#define RANK_COLOR D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CRanking::CRanking()
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++) {
		m_apScore[nCnt] = NULL;
		m_aRankingData[nCnt] = NULL;
	}
	m_pScoreOutRank = NULL;
	m_fSpaceScore = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// ランキングの生成処理
//=============================================================================
CRanking* CRanking::Create(CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize, float fSpace, float fSpaceScore) {
	CRanking* pRanking;
	pRanking = new CRanking;
	if (pRanking != NULL) {
		pRanking->m_numberData.type = type;
		pRanking->m_numberData.pos = pos;
		pRanking->m_numberData.fSize = fSize;
		pRanking->m_numberData.fSpace = fSpace;
		pRanking->m_fSpaceScore = fSpaceScore;
		pRanking->Init();
	}

	return pRanking;
}

//=============================================================================
// ランキングの初期化処理
//=============================================================================
HRESULT CRanking::Init(void) {
	FILE *pFile = NULL;	//ファイルへのポインタ

	//---------------------------------------
	//ランキングデータのロード
	//---------------------------------------
	//ファイルの読み込み
	pFile = fopen(TEXT_FILE_NAME_SAVE_RANKING, "r");
	if (pFile != NULL) {
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++) {
			fscanf(pFile, "%d", &m_aRankingData[nCnt]);
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else {
		//ファイルの読み込み失敗時
	}

	//---------------------------------------
	//読み込んだランキングデータのソート
	//---------------------------------------
	for (int nCntSort = 0; nCntSort < MAX_RANKING - 1; nCntSort++) {
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING - nCntSort - 1; nCntRanking++) {
			//ランキングデータの入れ替え
			if (m_aRankingData[nCntRanking] < m_aRankingData[nCntRanking + 1]) {
				int nTemp = m_aRankingData[nCntRanking];	//一時保存
				m_aRankingData[nCntRanking] = m_aRankingData[nCntRanking + 1];
				m_aRankingData[nCntRanking + 1] = nTemp;
			}
		}
	}

	//---------------------------------------
	//ランキングデータの更新
	//---------------------------------------
	int nScoreResult = 0;	//ゲーム終了時のスコア
	//ゲームの最終スコアの取得
	nScoreResult = CGame::GetScoreLast();

	//今回のスコアのランク（ランク外からの開始）
	int nRank = MAX_RANKING;
	//過去のランキングと今回のスコアの比較
	for (int nCnt = MAX_RANKING - 1; nCnt >= 0; nCnt--) {
		//ランキングデータの入れ替え
		if (nScoreResult >= m_aRankingData[nCnt]) {
			nRank = nCnt;	//ランクの更新
			int nTemp = m_aRankingData[nCnt];		//ランキングデータの一時保存
			m_aRankingData[nCnt] = nScoreResult;	//今回のスコアをランキングに入れる
			if (nCnt == MAX_RANKING - 1) continue;	//ループが初回の場合
			m_aRankingData[nCnt + 1] = nTemp;		//過去のランキングのデータの順位を下げる
		}
		else {
			break;
		}
	}

	//---------------------------------------
	//ランキングデータのセーブ
	//---------------------------------------
	//ファイルの読み込み
	pFile = fopen(TEXT_FILE_NAME_SAVE_RANKING, "w");
	if (pFile != NULL) {
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++) {
			fprintf(pFile, "%d\n", m_aRankingData[nCnt]);
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else {
		//ファイルの読み込み失敗時
	}

	//---------------------------------------
	//ランキングスコアの生成
	//---------------------------------------
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++) {
		m_apScore[nCnt] = CScore::Create(
			DIGIT_SCORE_RANKING,
			m_numberData.type,
			D3DXVECTOR3(m_numberData.pos.x, m_numberData.pos.y + nCnt * m_fSpaceScore, 0.0f),
			m_numberData.fSize, m_numberData.fSpace,
			0
		);
	}

	//---------------------------------------
	//スコアの初期化
	//---------------------------------------
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++) {
		if (m_apScore[nCnt] != NULL) {
			//初期化
			m_apScore[nCnt]->Init();
			//スコアの値の設定
			m_apScore[nCnt]->SetScore(m_aRankingData[nCnt]);
		}
	}

	//---------------------------------------
	//今回のスコアのランクによる処理
	//---------------------------------------
	//今回のスコアがランク内だった場合
	if (nRank < MAX_RANKING) {
		//スコアの色の設定
		m_apScore[nRank]->SetColor(RANK_COLOR);
	}
	//今回のスコアがランク外の場合
	else {
		//ランク外スコアの生成
		m_pScoreOutRank = CScore::Create(
			DIGIT_SCORE_RANKING,
			m_numberData.type,
			D3DXVECTOR3(m_numberData.pos.x, m_numberData.pos.y + (MAX_RANKING * m_fSpaceScore) + (m_numberData.fSpace * 0.25f), 0.0f),
			m_numberData.fSize, m_numberData.fSpace,
			0
		);
		//スコアの値の設定
		m_pScoreOutRank->SetScore(nScoreResult);
		//スコアの色の設定
		if (m_pScoreOutRank != NULL) {
			m_pScoreOutRank->SetColor(RANK_COLOR);
		}
	}

	return S_OK;
}

//=============================================================================
// ランキングの終了処理
//=============================================================================
void CRanking::Uninit(void) {
	//ランキングスコアの解放
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++) {
		if (m_apScore[nCnt] != NULL) {
			m_apScore[nCnt]->Uninit();
			m_apScore[nCnt] = NULL;
		}
	}
	//ランク外スコアの解放
	if (m_pScoreOutRank != NULL) {
		m_pScoreOutRank->Uninit();
		m_pScoreOutRank = NULL;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// ランキングの更新処理
//=============================================================================
void CRanking::Update(void) {

}

//=============================================================================
// ランキングの描画処理
//=============================================================================
void CRanking::Draw(void) {

}