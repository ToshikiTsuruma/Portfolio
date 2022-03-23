//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "scene2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DEFAULT_COLOR D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f)
#define SELECT_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define FADE_SPEED (0.01f)	//�t�F�[�h���鑬�x

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTitle::CTitle() {
	for (int nCnt = 0; nCnt < (int)SELECT_TYPE::ENUM_MAX; nCnt++) {
		m_apScene2D[nCnt] = NULL;
	}
	m_bFade = false;
	m_pTutorial = NULL;
	m_bTutorial = false;
	m_typeTutorial = TUTORIAL_TYPE::CONTROLL;
	m_pCredit = NULL;
	m_bCredit = false;
	m_nSelect = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle() {

}

//=============================================================================
// �^�C�g���̏���������
//=============================================================================
HRESULT CTitle::Init(void) {
	//�I�u�W�F�N�g�̃C���X�^���X����
	//�|���S������
	CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f), CTexture::TEXTURE_TYPE::GAME_TITLE, 720.0f, 150.0f);	//�^�C�g����
	CScene2D::Create(D3DXVECTOR3(130.0f, 40.0f, 0.0f), CTexture::TEXTURE_TYPE::QUIT_GAME, 230.0f, 50.0f);					//�Q�[���I���A�C�R��
	//�w�i�̐���
	CScene2D* pBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::BG_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
	pBg->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//�`��D��x��w�i�Ƃ��Đݒ�
	//�I�����̐���
	m_apScene2D[(int)SELECT_TYPE::START] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 480.0f, 0.0f), CTexture::TEXTURE_TYPE::TITLE_SELECT_START, 400.0f, 80.0f);		//�Q�[���X�^�[�g
	m_apScene2D[(int)SELECT_TYPE::TUTORIAL] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 560.0f, 0.0f), CTexture::TEXTURE_TYPE::TITLE_SELECT_TUTORIAL, 350.0f, 50.0f);	//�`���[�g���A��
	m_apScene2D[(int)SELECT_TYPE::CREDIT] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 620.0f, 0.0f), CTexture::TEXTURE_TYPE::TITLE_SELECT_CREDIT, 350.0f, 50.0f);		//�N���W�b�g	
	//�|���S���̐F�̏�����
	m_apScene2D[(int)SELECT_TYPE::START]->SetColor(SELECT_COLOR);
	m_apScene2D[(int)SELECT_TYPE::TUTORIAL]->SetColor(DEFAULT_COLOR);
	m_apScene2D[(int)SELECT_TYPE::CREDIT]->SetColor(DEFAULT_COLOR);
	//�����o�ϐ��̏�����
	m_bFade = false;
	m_bTutorial = false;
	m_bCredit = false;
	m_nSelect = (int)SELECT_TYPE::START;

	//�T�E���h�̎擾
	CManager* pManager = nullptr;
	pManager = CManager::GetManager();
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�Ȃ̍Đ�
	if (pSound != NULL) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_TITLE);
		CSound::SetBGM(CSound::SOUND_LABEL::BGM_TITLE);
	}

	return S_OK;
}

//=============================================================================
// �^�C�g���̏I������
//=============================================================================
void CTitle::Uninit(void) {
	//�I�u�W�F�N�g�̔j��
	CScene::ReleaseAll();

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�Ȃ̒�~
	if (pSound != nullptr) {
		pSound->StopSound(CSound::GetBGM());
		pSound->SetBGM(CSound::SOUND_LABEL::NONE);
	}
}

//=============================================================================
// �^�C�g���̍X�V����
//=============================================================================
void CTitle::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

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
	if (pInput != nullptr && pFade != nullptr) {
		//�^�C�g���J�n���̃t�F�[�h���̏���
		if (pFade->GetFade() == true && m_bFade == false) {
			if (pInput->GetTrigger(CInput::CODE::SELECT)) {
				pFade->SkipFade();
			}
		}
		//�t�F�[�h���ł͂Ȃ���
		else {
			//----------------------------------------
			//�߂�{�^��������
			//----------------------------------------
			if (pInput->GetTrigger(CInput::CODE::BACK) && m_bFade == false) {
				if (m_bTutorial == true) {
					//�`���[�g���A�������
					if (m_pTutorial != NULL) {
						m_pTutorial->Uninit();
						//���ʉ��̍Đ�
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
						m_pTutorial = NULL;
					}
					m_bTutorial = false;
				}
				else if (m_bCredit == true) {
					//�N���W�b�g�����
					if (m_pCredit != NULL) {
						m_pCredit->Uninit();
						//���ʉ��̍Đ�
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
						m_pCredit = NULL;
					}
					m_bCredit = false;
				}
			}

			//----------------------------------------
			//�I��؂�ւ�
			//----------------------------------------
			if (m_bTutorial == false && m_bCredit == false && m_bFade == false) {
				//��L�[
				if (pInput->GetTrigger(CInput::CODE::UP)) {
					//�I������O���|���S���̐F��߂�
					m_apScene2D[m_nSelect]->SetColor(DEFAULT_COLOR);
					//���ʉ��̍Đ�
					if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_SWITCH);
					//�I����ύX
					if (m_nSelect <= 0) {
						m_nSelect = (int)SELECT_TYPE::ENUM_MAX - 1;
					}
					else {
						m_nSelect--;
					}
					//�I�𒆂̃|���S���̐F��ς���
					m_apScene2D[m_nSelect]->SetColor(SELECT_COLOR);
				}
				//���L�[
				if (pInput->GetTrigger(CInput::CODE::DOWN)) {
					//�I������O���|���S���̐F��߂�
					m_apScene2D[m_nSelect]->SetColor(DEFAULT_COLOR);
					//���ʉ��̍Đ�
					if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_SWITCH);
					//�I����ύX
					if (m_nSelect >= (int)SELECT_TYPE::ENUM_MAX - 1) {
						m_nSelect = 0;
					}
					else {
						m_nSelect++;
					}
					//�I�𒆂̃|���S���̐F��ς���
					m_apScene2D[m_nSelect]->SetColor(SELECT_COLOR);
				}
			}

			//�`���[�g���A��
			if (m_bTutorial == true && m_bCredit == false && m_bFade == false) {
				//���E�L�[
				if (pInput->GetTrigger(CInput::CODE::LEFT) || pInput->GetTrigger(CInput::CODE::RIGHT)) {
					if (pInput->GetTrigger(CInput::CODE::LEFT)) {
						//�O�̃`���[�g���A����ʂɂ���
						int nType = (int)m_typeTutorial - 1;
						if (nType < 0) nType = (int)TUTORIAL_TYPE::ENUM_MAX - 1;
						m_typeTutorial = (TUTORIAL_TYPE)nType;
					}
					if (pInput->GetTrigger(CInput::CODE::RIGHT)) {
						//���̃`���[�g���A����ʂɂ���
						int nType = (int)m_typeTutorial + 1;
						if (nType >= (int)TUTORIAL_TYPE::ENUM_MAX) nType = 0;
						m_typeTutorial = (TUTORIAL_TYPE)nType;
					}

					//�e�N�X�`���̌���
					CTexture::TEXTURE_TYPE typeTex = CTexture::TEXTURE_TYPE::TITLE_TUTORIAL_001;	//�e�N�X�`���̎��

					switch (m_typeTutorial)
					{
					case CTitle::TUTORIAL_TYPE::CONTROLL:
						typeTex = CTexture::TEXTURE_TYPE::TITLE_TUTORIAL_001;
						break;
					case CTitle::TUTORIAL_TYPE::GAME:
						typeTex = CTexture::TEXTURE_TYPE::TITLE_TUTORIAL_002;
						break;
					case CTitle::TUTORIAL_TYPE::BOSS:
						typeTex = CTexture::TEXTURE_TYPE::TITLE_TUTORIAL_003;
						break;
					}

					//�ʂ̃`���[�g���A����ʂ𐶐�
					if (m_pTutorial != NULL) {
						m_pTutorial->Uninit();
						m_pTutorial = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), typeTex, SCREEN_WIDTH, SCREEN_HEIGHT);
						//���ʉ��̍Đ�
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_SWITCH);
					}
				}
			}

			//----------------------------------------
			//�I�����莞
			//----------------------------------------
			if (pInput->GetTrigger(CInput::CODE::SELECT) && m_bFade == false) {
				//�`���[�g���A����ʕ\����
				if (m_bTutorial == true) {
					//�`���[�g���A�������
					if (m_pTutorial != NULL) {
						m_pTutorial->Uninit();
						//���ʉ��̍Đ�
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
						m_pTutorial = NULL;
					}
					m_bTutorial = false;
				}
				//�N���W�b�g��ʕ\����
				else if (m_bCredit == true) {
					//�N���W�b�g�����
					if (m_pCredit != NULL) {
						m_pCredit->Uninit();
						//���ʉ��̍Đ�
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
						m_pCredit = NULL;
					}
					m_bCredit = false;
				}
				//�ʏ��ʎ�
				else {
					switch (m_nSelect)
					{
						//�Q�[���X�^�[�g
					case (int)SELECT_TYPE::START:
						//���ʉ��̍Đ�
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_START);
						//�Q�[���̃��[�h�ɐ؂�ւ�
						if (pFade != NULL) {
							pFade->SetFade(CManager::MODE::GAME, FADE_SPEED);
							m_bFade = true;
						}
						break;
						//�`���[�g���A��
					case (int)SELECT_TYPE::TUTORIAL:
						//���ʉ��̍Đ�
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
						//�`���[�g���A����ʐ���
						if (m_pTutorial == NULL) {
							m_pTutorial = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::TITLE_TUTORIAL_001, SCREEN_WIDTH, SCREEN_HEIGHT);
							m_bTutorial = true;
							m_typeTutorial = TUTORIAL_TYPE::CONTROLL;
						}
						break;
						//�N���W�b�g�\�L
					case (int)SELECT_TYPE::CREDIT:
						//���ʉ��̍Đ�
						if (pSound != NULL) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
						//�N���W�b�g��ʐ���
						if (m_pCredit == NULL) {
							m_pCredit = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::TITLE_CREDIT, SCREEN_WIDTH, SCREEN_HEIGHT);
							m_bCredit = true;
						}
						break;
					}
				}
			}
		}
	}
}