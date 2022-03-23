//=============================================================================
//
// �^�C�}�[���� [timer.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "timer.h"
#include "manager.h"
#include "sound.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
#define DEFAULT_ALERT_TIME (30)	//�f�t�H���g�̌x������
#define COLOR_ALERT_TIME (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//�^�C�����~�b�g���߂Â����Ƃ��x�����Ȃ��Ă���̐����̐F

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTimer::CTimer() : m_nNumDigit(0)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CTimer::CTimer(int nNumDigit, CTexture::TEXTURE_TYPE type) : m_nNumDigit(nNumDigit)
{
	//�������i���o�[�N���X�̃C���X�^���X�𐶐�
	m_pNumberArray = CNumber::Create(type, nNumDigit);

	m_bStop = false;
	m_nTime = 0;
	m_nCntTime = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
// �^�C�}�[�̐�������
//=============================================================================
CTimer* CTimer::Create(int nStartTime, int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize, float fSpace) {
	CTimer* pTimer;
	pTimer = new CTimer(nNumDigit, type);
	if (pTimer != NULL) {
		//�i���o�[�̏�����
		if (pTimer->m_pNumberArray != NULL) {
			for (int nCnt = 0; nCnt < pTimer->m_nNumDigit; nCnt++) {
				pTimer->m_pNumberArray[nCnt].Init(D3DXVECTOR3(pos.x - nCnt * fSpace, pos.y, pos.z), fSize);
			}
		}
		pTimer->m_nTime = nStartTime;
		pTimer->Init();
	}

	return pTimer;
}

//=============================================================================
// �^�C�}�[�̏���������
//=============================================================================
HRESULT CTimer::Init(void) {
	SetDrawPriority(CScene::DRAW_PRIORITY::UI);	//�`�揇�̐ݒ�
	//�J�E���^�̐ݒ�
	m_nCntTime = FPS;
	//�����̐ݒ�
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetNumber((m_nTime % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
		}
	}
	return S_OK;
}

//=============================================================================
// �^�C�}�[�̏I������
//=============================================================================
void CTimer::Uninit(void) {
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
// �^�C�}�[�̍X�V����
//=============================================================================
void CTimer::Update(void) {
	if (m_pNumberArray != NULL && m_nTime > 0 && m_bStop == false) {
		//�i���o�[�̍X�V����
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Update();
		}
		
		if (m_nCntTime <= 0) {
			//�J�E���^�̍Đݒ�
			m_nCntTime = FPS;	
			//�^�C�}�[�̌���
			m_nTime--;
			//�^�C�}�[�̍X�V
			for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
				m_pNumberArray[nCnt].SetNumber((m_nTime % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
			}
			//�x�����ԂɂȂ����ꍇ
			if (m_nTime == DEFAULT_ALERT_TIME) {
				//�}�l�[�W���[�̎擾
				CManager* pManager = CManager::GetManager();
				//�T�E���h�̎擾
				CSound *pSound = nullptr;
				if (pManager != nullptr) pSound = pManager->GetSound();
				//�Ȃ̕ύX
				if (pSound != nullptr) {
					//�x�����̍Đ�
					pSound->PlaySound(CSound::SOUND_LABEL::TIME_ALERT);
				}
				//�F�̕ύX
				for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
					m_pNumberArray[nCnt].SetColor(COLOR_ALERT_TIME);
				}
			}
		}
		else {
			//�^�C�}�[�̃J�E���g����
			m_nCntTime--;
		}
	}
}

//=============================================================================
// �^�C�}�[�̕`�揈��
//=============================================================================
void CTimer::Draw(void) {
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].Draw();
		}
	}
}

//=============================================================================
// �^�C�}�[�̎��Ԃ̒ǉ�
//=============================================================================
void CTimer::AddTime(int nTime) {
	//���Ԃ̒ǉ�
	m_nTime += nTime;
	//�^�C�}�[�̍X�V
	if (m_pNumberArray != NULL) {
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetNumber((m_nTime % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
		}
	}
	//�^�C�}�[�̐F��߂�
	if (m_nTime > DEFAULT_ALERT_TIME) {
		//�F�̕ύX
		for (int nCnt = 0; nCnt < m_nNumDigit; nCnt++) {
			m_pNumberArray[nCnt].SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//=============================================================================
// �^�C�}�[�̎c�莞�Ԃ��擾
//=============================================================================
int CTimer::GetTime(void) {
	return m_nTime;
}

//=============================================================================
// �^�C�}�[�̒�~��Ԃ�ݒ�
//=============================================================================
void CTimer::SetStop(bool bStop) {
	m_bStop = bStop;
}