//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "renderer.h"
#include "model.h"
#include "fade.h"
#include "scene2D.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "pause.h"
#include "texture.h"
#include "model.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CManager* CManager::m_pManager = nullptr;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CManager::CManager() {
	m_pRenderer = NULL;
	m_pInputCur = NULL;
	m_pInputKeyboard = NULL;
	m_pInputGamepadX = NULL;
	m_pCamera = NULL;
	m_apLight[NUM_LIGHT] = {};
	m_pSound = NULL;
	m_pFade = NULL;
	m_mode = MODE::TITLE;
	m_pTitle = NULL;
	m_pGame = NULL;
	m_pResult = NULL;
	m_pPause = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager() {

}

//=============================================================================
// �}�l�[�W���[�̐���
//=============================================================================
void CManager::Create(void) {
	//��������Ă��Ȃ��ꍇ����
	if (m_pManager == nullptr) m_pManager = new CManager;
}
 
//=============================================================================
// �}�l�[�W���[�̔j��
//=============================================================================
void CManager::Destroy(void) {
	if (m_pManager != nullptr) {
		delete m_pManager;
		m_pManager = nullptr;
	}
}

//=============================================================================
// �}�l�[�W���[�̎擾 
//=============================================================================
CManager* CManager::GetManager(void) {
	return m_pManager;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow) {
	//�����̎�̏�����
	srand((unsigned)time(NULL));

	//------------------------------
	//�C���X�^���X����
	//------------------------------
	//�L�[�{�[�h�̐���
	m_pInputKeyboard = new CInputKeyboard;
	if (m_pInputKeyboard != NULL) m_pInputKeyboard->Init(hInstance, hWnd);
	//�Q�[���p�b�h�̐���(Xinput)
	m_pInputGamepadX = new CInputGamepadX;
	if (m_pInputGamepadX != NULL) m_pInputGamepadX->Init();
	//���݂̓��̓f�o�C�X�̐ݒ�
	if (m_pInputGamepadX != NULL) {
		//�Q�[���p�b�h���ڑ�����Ă���ꍇ
		if (m_pInputGamepadX->GetConnectGamepad() == true) {
			m_pInputCur = m_pInputGamepadX;
		}
		else {
			m_pInputCur = m_pInputKeyboard;
		}
	}
	//�T�E���h�̐���
	m_pSound = new CSound;
	if (m_pSound != NULL)m_pSound->Init(hWnd);
	//�����_���[�̃C���X�^���X����
	m_pRenderer = new CRenderer;
	if (m_pRenderer != NULL) m_pRenderer->Init(hWnd, bWindow);

	//------------------------------
	//���[�h
	//------------------------------
	//�e�N�X�`���̃��[�h
	CTexture::Load();
	//���f���̃��[�h
	CModel::Load();

	//------------------------------
	//�C���X�^���X����
	//------------------------------
	//�J�����̐���
	m_pCamera = CCamera::Create();
	//���C�g�̐���
	m_apLight[0] = CLight::Create(D3DXVECTOR3(-0.5f, -2.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apLight[1] = CLight::Create(D3DXVECTOR3(0.8f, 0.1f, 0.8f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
	m_apLight[2] = CLight::Create(D3DXVECTOR3(0.8f, -0.5f, -0.4f), D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
	//�|�[�Y�̐���
	m_pPause = new CPause;
	if (m_pPause != NULL) m_pPause->Init();
	//�t�F�[�h�̐���
	m_pFade = new CFade;
	if (m_pFade != NULL) m_pFade->Init();

	//���[�h�̐ݒ�
	SetMode(m_mode);


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void) {
	//�I�u�W�F�N�g�̔j��
	CScene::ReleaseAll();

	//�t�F�[�h�̔j��
	if (m_pFade != NULL) {
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}
	//�|�[�Y�̔j��
	if (m_pPause != NULL) {
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}
	//���C�g�̔j��
	for (int nCnt = 0; nCnt < NUM_LIGHT; nCnt++) {
		if (m_apLight[nCnt] != NULL) {
			m_apLight[nCnt]->Uninit();
			delete m_apLight[nCnt];
			m_apLight[nCnt] = NULL;
		}
	}
	//�J�����̔j��
	if (m_pCamera != NULL) {
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//���f���̃A�����[�h
	CModel::Unload();
	//�e�N�X�`���̃A�����[�h
	CTexture::Unload();
	//�����_���[�̔j��
	if (m_pRenderer != NULL) {
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	//�T�E���h�̔j��
	if (m_pSound != NULL) {
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
	//�L�[�{�[�h�̔j��
	if (m_pInputKeyboard != NULL) {
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	//�Q�[���p�b�h�̔j��(Xinput)
	if (m_pInputGamepadX != NULL) {
		m_pInputGamepadX->Uninit();
		delete m_pInputGamepadX;
		m_pInputGamepadX = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void) {
	//�L�[�{�[�h�̍X�V����
	if (m_pInputKeyboard != NULL) {
		m_pInputKeyboard->Update();
	}
	//�Q�[���p�b�h�̍X�V����(Xinput)
	if (m_pInputGamepadX != NULL) {
		m_pInputGamepadX->Update();
	}
	//���݂̓��̓f�o�C�X�̐ݒ�
	if (m_pInputGamepadX != NULL) {
		//�Q�[���p�b�h���ڑ�����Ă���ꍇ
		if (m_pInputGamepadX->GetConnectGamepad() == true) {
			m_pInputCur = m_pInputGamepadX;
		}
		else {
			m_pInputCur = m_pInputKeyboard;
		}
	}

	if (m_pPause != NULL) {
		//�|�[�Y���ł͂Ȃ��ꍇ�̂ݍX�V
		if (m_pPause->GetPause() == false) {
			//�J�����̍X�V
			if (m_pCamera != NULL) m_pCamera->Update();

			//�����_���[�̍X�V����
			if (m_pRenderer != NULL) {
				m_pRenderer->Update();
			}

			//���݂̃��[�h�̍X�V����
			switch (m_mode)
			{
			case MODE::TITLE:
				if (m_pTitle != NULL) {
					m_pTitle->Update();
				}
				break;
			case MODE::GAME:
				if (m_pGame != NULL) {
					m_pGame->Update();
				}
				break;
			case MODE::RESULT:
				if (m_pResult != NULL) {
					m_pResult->Update();
				}
				break;
			}
		}

		//�|�[�Y�̍X�V����
		m_pPause->Update();
	}

	//�t�F�[�h�̍X�V����
	if (m_pFade != NULL) {
		m_pFade->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void) {
	if (m_pRenderer != NULL) {
		m_pRenderer->Draw();
	}
}

//=============================================================================
//���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(MODE mode) {
	//���݂̃��[�h�̃I�u�W�F�N�g��j��
	switch (m_mode)
	{
	case MODE::TITLE:
		if (m_pTitle != NULL) {
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;
	case MODE::GAME:
		if (m_pGame != NULL) {
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}		
		break;
	case MODE::RESULT:
		if (m_pResult != NULL) {
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}		
		break;
	}

	//���̃��[�h�̐ݒ�
	m_mode = mode;
	switch (mode)
	{
	case MODE::TITLE:
		m_pTitle = new CTitle;
		if (m_pTitle != NULL) {
			m_pTitle->Init();
		}
		break;
	case MODE::GAME:
		m_pGame = new CGame;
		if (m_pGame != NULL) {
			m_pGame->Init();
		}		
		break;
	case MODE::RESULT:
		m_pResult = new CResult;
		if (m_pResult != NULL) {
			m_pResult->Init();
		}
		break;
	}
}

//=============================================================================
//���[�h�̎擾
//=============================================================================
CManager::MODE CManager::GetMode(void) {
	return m_mode;
}

//=============================================================================
// �����_���[�̎擾
//=============================================================================
CRenderer* CManager::GetRenderer(void) {
	return m_pRenderer;
}

//=============================================================================
// ���݂̓��̓f�o�C�X�̎擾
//=============================================================================
CInput* CManager::GetInputCur(void) {
	return m_pInputCur;
}

//=============================================================================
//�J�����̎擾
//=============================================================================
CCamera* CManager::GetCamera(void) {
	return m_pCamera;
}

//=============================================================================
// �T�E���h�̎擾
//=============================================================================
CSound* CManager::GetSound(void) {
	return m_pSound;
}

//=============================================================================
// �t�F�[�h�̎擾
//=============================================================================
CFade* CManager::GetFade(void) {
	return m_pFade;
}

//=============================================================================
//�|�[�Y�̎擾
//=============================================================================
CPause* CManager::GetPause(void) {
	return m_pPause;
}

//=============================================================================
//�Q�[�����[�h�̎擾
//=============================================================================
CGame* CManager::GetGame(void) {
	return m_pGame;
}