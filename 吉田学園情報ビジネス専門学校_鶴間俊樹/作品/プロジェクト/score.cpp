//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "score.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CScore::CScore() : m_nNumDigit(0)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CScore::CScore(int nNumDigit, CTexture::TEXTURE_TYPE type) : m_nNumDigit(nNumDigit)
{
	SetDrawPriority(DRAW_PRIORITY::UI);
	//桁数分ナンバークラスのインスタンスを生成
	m_pNumberArray = CNumber::Create(type, nNumDigit);

	m_nScore = 0;
	m_nLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// スコアの生成処理
//=============================================================================
CScore* CScore::Create(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize, float fSpace, int nLife) {
	CScore* pScore;
	pScore = new CScore(nNumDigit, type);
	if (pScore != NULL) {
		//ナンバーの初期化
		if (pScore->m_pNumberArray != NULL) {
			for (int nCnt = 0; nCnt < pScore->m_nNumDigit; nCnt++) {
				pScore->m_pNumberArray[nCnt].Init(D3DXVECTOR3(pos.x - nCnt * fSpace, pos.y, pos.z), fSize);
			}
		}
		pScore->m_nLife = nLife;
		pScore->Init();
	}

	return pScore;
}

//=============================================================================
// スコアの初期化処理
//=============================================================================
HRESULT CScore::Init(void) {
	SetDrawPriority(CScene::DRAW_PRIORITY::UI);	//描画順の設定
	SetScore(0);
	return S_OK;
}

//=============================================================================
// スコアの終了処理
//=============================================================================
void CScore::Uninit(void) {
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Uninit();
		}
		delete[] m_pNumberArray;
		m_pNumberArray = NULL;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// スコアの更新処理
//=============================================================================
void CScore::Update(void) {
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Update();
		}
	}
	//ライフの減少
	if (m_nLife > 0) {
		m_nLife--;
		//ライフが尽きたら終了
		if (m_nLife <= 0) {
			Uninit();
			return;
		}
	}
}

//=============================================================================
// スコアの描画処理
//=============================================================================
void CScore::Draw(void) {
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Draw();
		}
	}
}

//=============================================================================
// スコアの設定
//=============================================================================
void CScore::SetScore(int nScore) {
	m_nScore = nScore;
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetNumber((m_nScore % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
		}
	}
}

//=============================================================================
// スコアの追加
//=============================================================================
void CScore::AddScore(int nValue) {
	m_nScore += nValue;
	if (m_nScore < 0)m_nScore = 0;	//0未満の場合は0

	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetNumber((m_nScore % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
		}
	}
}

//=============================================================================
// スコアの取得
//=============================================================================
int CScore::GetScore(void) {
	return m_nScore;
}

//=============================================================================
// スコアの色の設定
//=============================================================================
void CScore::SetColor(D3DXCOLOR col) {
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetColor(col);
		}
	}
}