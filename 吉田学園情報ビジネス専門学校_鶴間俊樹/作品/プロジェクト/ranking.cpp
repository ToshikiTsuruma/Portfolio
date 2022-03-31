//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "ranking.h"
#include "manager.h"
#include "game.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_FILE_NAME_SAVE_RANKING "data/TEXT/save_ranking.txt"
#define RANK_COLOR D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)

//=============================================================================
// �f�t�H���g�R���X�g���N�^
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
// �f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// �����L���O�̐�������
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
// �����L���O�̏���������
//=============================================================================
HRESULT CRanking::Init(void) {
	FILE *pFile = NULL;	//�t�@�C���ւ̃|�C���^

	//---------------------------------------
	//�����L���O�f�[�^�̃��[�h
	//---------------------------------------
	//�t�@�C���̓ǂݍ���
	pFile = fopen(TEXT_FILE_NAME_SAVE_RANKING, "r");
	if (pFile != NULL) {
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++) {
			fscanf(pFile, "%d", &m_aRankingData[nCnt]);
		}
		//�t�@�C�������
		fclose(pFile);
	}
	else {
		//�t�@�C���̓ǂݍ��ݎ��s��
	}

	//---------------------------------------
	//�ǂݍ��񂾃����L���O�f�[�^�̃\�[�g
	//---------------------------------------
	for (int nCntSort = 0; nCntSort < MAX_RANKING - 1; nCntSort++) {
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING - nCntSort - 1; nCntRanking++) {
			//�����L���O�f�[�^�̓���ւ�
			if (m_aRankingData[nCntRanking] < m_aRankingData[nCntRanking + 1]) {
				int nTemp = m_aRankingData[nCntRanking];	//�ꎞ�ۑ�
				m_aRankingData[nCntRanking] = m_aRankingData[nCntRanking + 1];
				m_aRankingData[nCntRanking + 1] = nTemp;
			}
		}
	}

	//---------------------------------------
	//�����L���O�f�[�^�̍X�V
	//---------------------------------------
	int nScoreResult = 0;	//�Q�[���I�����̃X�R�A
	//�Q�[���̍ŏI�X�R�A�̎擾
	nScoreResult = CGame::GetScoreLast();

	//����̃X�R�A�̃����N�i�����N�O����̊J�n�j
	int nRank = MAX_RANKING;
	//�ߋ��̃����L���O�ƍ���̃X�R�A�̔�r
	for (int nCnt = MAX_RANKING - 1; nCnt >= 0; nCnt--) {
		//�����L���O�f�[�^�̓���ւ�
		if (nScoreResult >= m_aRankingData[nCnt]) {
			nRank = nCnt;	//�����N�̍X�V
			int nTemp = m_aRankingData[nCnt];		//�����L���O�f�[�^�̈ꎞ�ۑ�
			m_aRankingData[nCnt] = nScoreResult;	//����̃X�R�A�������L���O�ɓ����
			if (nCnt == MAX_RANKING - 1) continue;	//���[�v������̏ꍇ
			m_aRankingData[nCnt + 1] = nTemp;		//�ߋ��̃����L���O�̃f�[�^�̏��ʂ�������
		}
		else {
			break;
		}
	}

	//---------------------------------------
	//�����L���O�f�[�^�̃Z�[�u
	//---------------------------------------
	//�t�@�C���̓ǂݍ���
	pFile = fopen(TEXT_FILE_NAME_SAVE_RANKING, "w");
	if (pFile != NULL) {
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++) {
			fprintf(pFile, "%d\n", m_aRankingData[nCnt]);
		}
		//�t�@�C�������
		fclose(pFile);
	}
	else {
		//�t�@�C���̓ǂݍ��ݎ��s��
	}

	//---------------------------------------
	//�����L���O�X�R�A�̐���
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
	//�X�R�A�̏�����
	//---------------------------------------
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++) {
		if (m_apScore[nCnt] != NULL) {
			//������
			m_apScore[nCnt]->Init();
			//�X�R�A�̒l�̐ݒ�
			m_apScore[nCnt]->SetScore(m_aRankingData[nCnt]);
		}
	}

	//---------------------------------------
	//����̃X�R�A�̃����N�ɂ�鏈��
	//---------------------------------------
	//����̃X�R�A�������N���������ꍇ
	if (nRank < MAX_RANKING) {
		//�X�R�A�̐F�̐ݒ�
		m_apScore[nRank]->SetColor(RANK_COLOR);
	}
	//����̃X�R�A�������N�O�̏ꍇ
	else {
		//�����N�O�X�R�A�̐���
		m_pScoreOutRank = CScore::Create(
			DIGIT_SCORE_RANKING,
			m_numberData.type,
			D3DXVECTOR3(m_numberData.pos.x, m_numberData.pos.y + (MAX_RANKING * m_fSpaceScore) + (m_numberData.fSpace * 0.25f), 0.0f),
			m_numberData.fSize, m_numberData.fSpace,
			0
		);
		//�X�R�A�̒l�̐ݒ�
		m_pScoreOutRank->SetScore(nScoreResult);
		//�X�R�A�̐F�̐ݒ�
		if (m_pScoreOutRank != NULL) {
			m_pScoreOutRank->SetColor(RANK_COLOR);
		}
	}

	return S_OK;
}

//=============================================================================
// �����L���O�̏I������
//=============================================================================
void CRanking::Uninit(void) {
	//�����L���O�X�R�A�̉��
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++) {
		if (m_apScore[nCnt] != NULL) {
			m_apScore[nCnt]->Uninit();
			m_apScore[nCnt] = NULL;
		}
	}
	//�����N�O�X�R�A�̉��
	if (m_pScoreOutRank != NULL) {
		m_pScoreOutRank->Uninit();
		m_pScoreOutRank = NULL;
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �����L���O�̍X�V����
//=============================================================================
void CRanking::Update(void) {

}

//=============================================================================
// �����L���O�̕`�揈��
//=============================================================================
void CRanking::Draw(void) {

}