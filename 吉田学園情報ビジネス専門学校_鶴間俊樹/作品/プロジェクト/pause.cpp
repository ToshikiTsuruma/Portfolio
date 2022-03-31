//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "pause.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "scene2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DEFAULT_COLOR D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f)
#define SELECT_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define FADE_SPEED (0.02f)	//�t�F�[�h���鑬�x

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CPause::CPause()
{
	m_pPauseBg = NULL;
	for (int nCnt = 0; nCnt < NUM_CHOICE; nCnt++) {
		m_apChoice[nCnt] = NULL;
	}
	m_bPause = false;
	m_nChoice = 0;
	m_bLockInput = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// �|�[�Y�̏���������
//=============================================================================
HRESULT CPause::Init(void) {
	m_bPause = false;
	m_nChoice = 0;

	return S_OK;
}

//=============================================================================
// �|�[�Y�̏I������
//=============================================================================
void CPause::Uninit(void) {

}

//=============================================================================
// �|�[�Y�̍X�V����
//=============================================================================
void CPause::Update(void) {
	//�|�[�Y���ł͂Ȃ��ꍇ���f
	if (m_bPause == false) return;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	//�T�E���h�̎擾
	CInput* pInput = nullptr;
	CFade* pFade = nullptr;
	CSound *pSound = nullptr;

	if (pManager != nullptr) {
		//���݂̓��̓f�o�C�X�̎擾
		pInput = pManager->GetInputCur();
		//�t�F�[�h�̎擾
		pFade = pManager->GetFade();
		//�T�E���h�̎擾
		pSound = pManager->GetSound();
	}

	//���͏���
	if (pInput != NULL && pFade != NULL && m_bLockInput == false && m_apChoice[0] != NULL && m_apChoice[1] != NULL && m_apChoice[2] != NULL) {
		if (pFade->GetFade() == false) {
			//----------------------------------------
			//�I��؂�ւ�
			//----------------------------------------
			//��L�[
			if (pInput->GetTrigger(CInput::CODE::UP)) {
				//�I������O���|���S���̐F��߂�
				m_apChoice[m_nChoice]->SetColor(DEFAULT_COLOR);
				//���ʉ��̍Đ�
				if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_SWITCH);
				//�I����ύX
				if (m_nChoice <= 0) {
					m_nChoice = NUM_CHOICE - 1;
				}
				else {
					m_nChoice--;
				}
				//�I�𒆂̃|���S���̐F��ς���
				m_apChoice[m_nChoice]->SetColor(SELECT_COLOR);
			}
			//���L�[
			if (pInput->GetTrigger(CInput::CODE::DOWN)) {
				//�I������O���|���S���̐F��߂�
				m_apChoice[m_nChoice]->SetColor(DEFAULT_COLOR);
				//���ʉ��̍Đ�
				if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_SWITCH);
				//�I����ύX
				if (m_nChoice >= NUM_CHOICE - 1) {
					m_nChoice = 0;
				}
				else {
					m_nChoice++;
				}
				//�I�𒆂̃|���S���̐F��ς���
				m_apChoice[m_nChoice]->SetColor(SELECT_COLOR);
			}


			//----------------------------------------
			//�I�����莞
			//----------------------------------------
			if (pInput->GetTrigger(CInput::CODE::SELECT)) {
				switch (m_nChoice)
				{
				//�Q�[���ĊJ
				case 0:
					//���ʉ��̍Đ�
					if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::PAUSE_SELECT);
					SetPause(false);	//�|�[�Y�̏I��
					break;

				//���g���C
				case 1:
					//���ʉ��̍Đ�
					if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::PAUSE_SELECT);

					//�Q�[�����ă��[�h
					if (pFade != NULL) {
						pFade->SetFade(CManager::MODE::GAME, FADE_SPEED);
					}
					break;

				//�I��
				case 2:
					//���ʉ��̍Đ�
					if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::PAUSE_SELECT);

					//�^�C�g���ɐ؂�ւ�
					if (pFade != NULL) {
						pFade->SetFade(CManager::MODE::TITLE, FADE_SPEED);
					}
					break;
				}
			}//choice
		}//fade
	}//NULL

	 //���͏����̃��b�N������
	if (m_bLockInput == true)m_bLockInput = false;
}

//=============================================================================
// �|�[�Y��Ԃ̐ݒ�
//=============================================================================
void CPause::SetPause(bool bPause) {
	m_bPause = bPause;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	if (bPause == true) {
		m_nChoice = 0;
		m_bLockInput = true;	//���͏��������b�N
		//���ʉ��̍Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::PAUSE);

		//�w�i�|���S���̐���
		m_pPauseBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::PAUSE_BG, SCREEN_WIDTH, SCREEN_HEIGHT);
		//�w�i�|���S���̐ݒ�
		if (m_pPauseBg != NULL) {
			m_pPauseBg->SetDrawPriority(CScene2D::DRAW_PRIORITY::FRONT);	//�`�揇���őO��
		}

		//�I�����|���S���̐���
		m_apChoice[0] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f , 0.0f), CTexture::TEXTURE_TYPE::PAUSE_START, 280.0f, 50.0f);
		m_apChoice[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 400.0f , 0.0f), CTexture::TEXTURE_TYPE::PAUSE_RETRY, 280.0f, 50.0f);
		m_apChoice[2] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 500.0f , 0.0f), CTexture::TEXTURE_TYPE::PAUSE_QUIT, 280.0f, 50.0f);
		//�I�����|���S���̐ݒ�
		for (int nCnt = 0; nCnt < NUM_CHOICE; nCnt++) {
			if (m_apChoice[nCnt] != NULL) {
				m_apChoice[nCnt]->SetDrawPriority(CScene2D::DRAW_PRIORITY::FRONT);	//�`�揇���őO��
				m_apChoice[nCnt]->SetColor(DEFAULT_COLOR);	//���I�����̐F�̕ύX
			}
		}
		//�I�𒆂̃|���S���̐F��ύX
		if (m_apChoice[m_nChoice] != NULL) {
			m_apChoice[m_nChoice]->SetColor(SELECT_COLOR);
		}

		//�Ȃ̈ꎞ��~
		if (pSound != nullptr) pSound->PauseSound(CSound::GetBGM());
	}
	else if (bPause == false) {
		//�w�i�|���S���̔j��
		if (m_pPauseBg != NULL) {
			m_pPauseBg->Uninit();
			m_pPauseBg = NULL;
		}
		//�I�����|���S���̔j��
		for (int nCnt = 0; nCnt < NUM_CHOICE; nCnt++) {
			if (m_apChoice[nCnt] != NULL) {
				m_apChoice[nCnt]->Uninit();
				m_apChoice[nCnt] = NULL;
			}
		}

		//�Ȃ̍Đ�
		if (pSound != nullptr) pSound->StartSound(CSound::GetBGM());
	}
}

//=============================================================================
// �|�[�Y��Ԃ̎擾
//=============================================================================
bool CPause::GetPause(void) {
	return m_bPause;
}