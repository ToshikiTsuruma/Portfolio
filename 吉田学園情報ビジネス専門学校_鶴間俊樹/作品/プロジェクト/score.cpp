//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "score.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CScore::CScore() : m_nNumDigit(0)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CScore::CScore(int nNumDigit, CTexture::TEXTURE_TYPE type) : m_nNumDigit(nNumDigit)
{
	SetDrawPriority(DRAW_PRIORITY::UI);
	//�������i���o�[�N���X�̃C���X�^���X�𐶐�
	m_pNumberArray = CNumber::Create(type, nNumDigit);

	m_nScore = 0;
	m_nLife = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// �X�R�A�̐�������
//=============================================================================
CScore* CScore::Create(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize, float fSpace, int nLife) {
	CScore* pScore;
	pScore = new CScore(nNumDigit, type);
	if (pScore != NULL) {
		//�i���o�[�̏�����
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
// �X�R�A�̏���������
//=============================================================================
HRESULT CScore::Init(void) {
	SetDrawPriority(CScene::DRAW_PRIORITY::UI);	//�`�揇�̐ݒ�
	SetScore(0);
	return S_OK;
}

//=============================================================================
// �X�R�A�̏I������
//=============================================================================
void CScore::Uninit(void) {
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Uninit();
		}
		delete[] m_pNumberArray;
		m_pNumberArray = NULL;
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�R�A�̍X�V����
//=============================================================================
void CScore::Update(void) {
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Update();
		}
	}
	//���C�t�̌���
	if (m_nLife > 0) {
		m_nLife--;
		//���C�t���s������I��
		if (m_nLife <= 0) {
			Uninit();
			return;
		}
	}
}

//=============================================================================
// �X�R�A�̕`�揈��
//=============================================================================
void CScore::Draw(void) {
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Draw();
		}
	}
}

//=============================================================================
// �X�R�A�̐ݒ�
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
// �X�R�A�̒ǉ�
//=============================================================================
void CScore::AddScore(int nValue) {
	m_nScore += nValue;
	if (m_nScore < 0)m_nScore = 0;	//0�����̏ꍇ��0

	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetNumber((m_nScore % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
		}
	}
}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int CScore::GetScore(void) {
	return m_nScore;
}

//=============================================================================
// �X�R�A�̐F�̐ݒ�
//=============================================================================
void CScore::SetColor(D3DXCOLOR col) {
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetColor(col);
		}
	}
}