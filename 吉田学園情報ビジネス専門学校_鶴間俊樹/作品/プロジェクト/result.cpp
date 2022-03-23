//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "scene2D.h"
#include "ranking.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define COUNT_FADE_ENABLE (100)		//�t�F�[�h�\�܂ł̃J�E���g
#define COUNT_FADE_FORCE (2400)		//�����I�Ƀt�F�[�h�����܂ł̃J�E���g
#define RANKING_NUMBER_SIZE (40.0f)	//�����L���O�̐����̃T�C�Y
#define FADE_SPEED (0.01f)	//�t�F�[�h���鑬�x

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CResult::CResult() {
	m_nCntResult = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult() {

}

//=============================================================================
// ���U���g�̏���������
//=============================================================================
HRESULT CResult::Init(void) {
	//�I�u�W�F�N�g�̃C���X�^���X����
	//�w�i�̐���
	CScene2D* pBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::BG_RESULT , SCREEN_WIDTH, SCREEN_HEIGHT);
	if (pBg != NULL) pBg->SetDrawPriority(CScene2D::DRAW_PRIORITY::BG);	//�`��D��x��w�i�Ƃ��Đݒ�

	//�����L���O�e�L�X�g�̕\��
	CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 400.0f, 80.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_RANKING, 300.0f, 75.0f);
	//�����L���O�̐���
	CRanking::Create(CTexture::TEXTURE_TYPE::NUMBER_004, D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 150.0f, 0.0f), 50.0f, 50.0f, 50.0f);

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�Ȃ̍Đ�
	if (pSound != nullptr) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_RESULT);
		pSound->SetBGM(CSound::SOUND_LABEL::BGM_RESULT);
	}

	return S_OK;
}

//=============================================================================
// ���U���g�̏I������
//=============================================================================
void CResult::Uninit(void) {
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
// ���U���g�̍X�V����
//=============================================================================
void CResult::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	CInput* pInput = nullptr;
	CFade* pFade = nullptr;

	if (pManager != nullptr) {
		//���݂̓��̓f�o�C�X�̎擾
		pInput = pManager->GetInputCur();
		//�t�F�[�h�̎擾
		pFade = pManager->GetFade();
	}


	if (pInput != NULL) {
		if ((pInput->GetTrigger(CInput::CODE::SELECT) || m_nCntResult >= COUNT_FADE_FORCE) && m_nCntResult >= COUNT_FADE_ENABLE) {
			//�Q�[���̃��[�h�ɐ؂�ւ�
			if (pFade != NULL) {
				pFade->SetFade(CManager::MODE::TITLE, FADE_SPEED);
			}
		}
	}

	m_nCntResult++;
}