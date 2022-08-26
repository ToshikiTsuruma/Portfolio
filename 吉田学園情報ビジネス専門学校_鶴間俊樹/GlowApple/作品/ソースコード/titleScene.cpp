//=============================================================================
//
// �^�C�g���V�[������ [titleScene.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "titleScene.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "object2D.h"
#include "fade.h"
#include "selectMenu2D.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTitleScene::CTitleScene()
{
	m_pSelectMenuTitle = CSelectMenu2D::Create(3, false);
	if (m_pSelectMenuTitle != nullptr) {
		//�c�I��
		m_pSelectMenuTitle->SetSelectType(CSelectMenu::SELECT_TYPE::VERTICAL);
		//�I����UI�̏ڍאݒ�
		m_pSelectMenuTitle->SetSelectUI(0, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 470.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_GAMESTART);
		m_pSelectMenuTitle->SetSelectUI(1, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 550.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_TUTORIAL);
		m_pSelectMenuTitle->SetSelectUI(2, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 630.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_CREDIT);
		//�I�����A�C�R���̐���
		m_pSelectMenuTitle->CreateSelectIcon(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), 50.0f, 50.0f, CTexture::TEXTURE_TYPE::SELECT_ICON);
		m_pSelectMenuTitle->SetIconPosOffset(1, D3DXVECTOR3(-160.0f, 0.0f, 0.0f));
		m_pSelectMenuTitle->SetIconPosOffset(2, D3DXVECTOR3(-140.0f, 0.0f, 0.0f));
	}
	m_pTutorial = nullptr;
	m_pCreadit = nullptr;
	m_bBeginFade = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitleScene::~CTitleScene()
{

}

//=============================================================================
// �^�C�g���V�[���̏���������
//=============================================================================
void CTitleScene::Init(void) {
	//�I�u�W�F�N�g�̃|�[�Y�������悤�ɐݒ�
	CObject::SetUpdatePauseLevel(0);

	CObject2D* pTitleBG = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::BG_TITLE , SCREEN_WIDTH, SCREEN_HEIGHT);
	if (pTitleBG != nullptr) pTitleBG->SetDrawPriority(CObject::DRAW_PRIORITY::UI_BG);

	CObject2D::Create(D3DXVECTOR3(120.0f, 40.0f, 0.0f), CTexture::TEXTURE_TYPE::QUIT_GAME, 200.0f, 40.0f);
	CObject2D* pTitleName = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_TITLENAME, 800.0f, 200.0f);

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//BGM�̍Đ�
	if (pSound != nullptr) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_TITLE);
		pSound->SetBGM(CSound::SOUND_LABEL::BGM_TITLE);
	}
}

//=============================================================================
// �^�C�g���V�[���̏I������
//=============================================================================
void CTitleScene::Uninit(void) {
	//�V�[���̏I������
	CScene::Uninit();

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//���̒�~
	if (pSound != nullptr) {
		pSound->StopSound();
	}
}

//=============================================================================
// �^�C�g���V�[���̍X�V����
//=============================================================================
void CTitleScene::Update(void) {
	//�V�[���J�ڊJ�n��͍X�V�Ȃ�
	if (m_bBeginFade) return;

	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	CInput* pInput = nullptr;	//���̓f�o�C�X�ւ̃|�C���^
	CFade* pFade = nullptr;		//�t�F�[�h�ւ̃|�C���^
	CSound* pSound = nullptr;	//�T�E���h�ւ̃|�C���^

	if (pManager != nullptr) {
		//���݂̓��̓f�o�C�X�̎擾
		pInput = pManager->GetInputCur();
		//�t�F�[�h�̎擾
		pFade = pManager->GetFade();
		//�T�E���h�̎擾
		pSound = pManager->GetSound();
	}

	if (pInput == nullptr || pFade == nullptr) return;


	//����L�[���������Ƃ�
	if (pInput->GetTrigger(CInput::CODE::SELECT) && m_pSelectMenuTitle != nullptr) {
		//�t�F�[�h���������ꍇ
		if (pFade->GetFade()) {
			//�t�F�[�h���X�L�b�v
			pFade->SkipFade();
		}
		//�`���[�g���A����ʂ��\������Ă���Ƃ�
		else if (m_pTutorial != nullptr) {
			//�`���[�g���A����ʂ����
			m_pTutorial->Uninit();
			m_pTutorial = nullptr;
			//�I���̃��b�N������
			m_pSelectMenuTitle->SetLockChangeSelect(false);
			//���艹�̍Đ�
			if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
		}
		//�N���W�b�g��ʂ��\������Ă���Ƃ�
		else if (m_pCreadit != nullptr) {
			//�N���W�b�g��ʂ����
			m_pCreadit->Uninit();
			m_pCreadit = nullptr;
			//�I���̃��b�N������
			m_pSelectMenuTitle->SetLockChangeSelect(false);
			//���艹�̍Đ�
			if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
		}
		//���̉�ʂ��\������Ă��Ȃ��Ƃ�
		else {
			//�I�����j���[�̌��݂̑I�������擾
			int nIdxSelect = m_pSelectMenuTitle->GetIdxCurSelect();
			switch (nIdxSelect)
			{
				//�Q�[���X�^�[�g
			case 0:
				//�V�[���J�ڊJ�n
				m_bBeginFade = true;
				if (pFade != nullptr) pFade->SetFade(CScene::SCENE_TYPE::GAME, 0.02f, 60);
				//���艹�̍Đ�
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_START);
				//���j���[�̑I�������b�N
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;

				//�`���[�g���A�����
			case 1:
				//�`���[�g���A����ʂ̐���
				m_pTutorial = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::TUTORIAL, SCREEN_WIDTH, SCREEN_HEIGHT);
				//���艹�̍Đ�
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
				//���j���[�̑I�������b�N
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;

				//�N���W�b�g���
			case 2:
				//�N���W�b�g��ʂ̐���
				m_pTutorial = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::CREDIT, SCREEN_WIDTH, SCREEN_HEIGHT);
				//���艹�̍Đ�
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
				//���j���[�̑I�������b�N
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;
			}
		}
	}
}