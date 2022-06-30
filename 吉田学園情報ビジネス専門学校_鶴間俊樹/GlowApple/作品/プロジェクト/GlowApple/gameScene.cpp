//=============================================================================
//
// �Q�[���V�[������ [gameScene.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "gameScene.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "timer.h"
#include "stage.h"
#include "wallCylinder.h"
#include "player.h"
#include "appleTree.h"
#include "selectMenu2D.h"
#include "enemy_human.h"
#include "enemy_normal.h"
#include "enemySpawner.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define GAME_TIME (300)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGameScene::CGameScene()
{
	m_pStage = nullptr;
	m_pAppleTree = nullptr;
	m_pTimer = nullptr;
	m_pMenuGameEnd = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGameScene::~CGameScene()
{

}

//=============================================================================
// �Q�[���V�[���̏���������
//=============================================================================
void CGameScene::Init(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�J�����̎擾
	CCamera* pCamera = pManager->GetCamera();
	if (pCamera == nullptr) return;
	//�T�E���h�̎擾
	CSound* pSound = pManager->GetSound();

	//�J�����̏����ݒ�
	pCamera->Init();

	//�X�e�[�W�̐���
	if(m_pStage == nullptr) m_pStage = new CStage;
	if(m_pStage != nullptr) m_pStage->CreateStage(TEXT_FILE_NAME_STAGE_GAME);

	//���[�V�������̃��[�h
	CPlayer::LoadMotionInfo();
	CEnemyNormal::LoadMotionInfo();
	CEnemyHuman::LoadMotionInfo();

	//�ь�̖؂̐���
	m_pAppleTree = CAppleTree::Create(D3DXVECTOR3(0.0f, -190.0f, 0.0f));

	//�~���̕ǂ̐���
	CWallCylinder::Create(D3DXVECTOR3(0.0f, 60.0f, 0.0f), 1500.0f, 40.0f, CTexture::TEXTURE_TYPE::NONE, true);

	//�v���C���[�̐���
	CPlayer* pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, -300.0f, -1000.0f));
	//�V�[���̃v���C���[�̐ݒ�
	SetPlayer(pPlayer);

	//�^�C�}�[�̐���
	m_pTimer = CTimer::Create(GAME_TIME, 3, CTexture::TEXTURE_TYPE::NUMBER_003, D3DXVECTOR3(SCREEN_WIDTH - 10.0f, 10.0f, 0.0f), 50.0f, 50.0f);

	//�ŏ��̓G�̔z�u
	CEnemy::SetEnemyCircle(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1000.0f, 2000, 2000);

	//�G�̃X�|�i�[�̐���
	m_pEnemySpawner = CEnemySpawner::Create(750, 2500, 3500);

	//BGM�̍Đ�
	if (pSound != nullptr) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_GAME);
		pSound->SetBGM(CSound::SOUND_LABEL::BGM_GAME);
	}
}

//=============================================================================
// �Q�[���V�[���̏I������
//=============================================================================
void CGameScene::Uninit(void) {
	//�X�e�[�W�̔j��
	if (m_pStage != nullptr) {
		delete m_pStage;
		m_pStage = nullptr;
	}

	//���[�V�������̃A�����[�h
	CPlayer::UnloadMotionInfo();
	CEnemyNormal::UnloadMotionInfo();
	CEnemyHuman::UnloadMotionInfo();

	//�V�[���̃v���C���[�̐ݒ�
	SetPlayer(nullptr);

	//�V�[���̏I������
	CScene::Uninit();

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//BGM�̒�~
	if (pSound != nullptr) {
		pSound->StopSound(pSound->GetBGM());
	}
}

//=============================================================================
// �Q�[���V�[���̍X�V����
//=============================================================================
void CGameScene::Update(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	CInput* pInput = nullptr;	//���̓f�o�C�X�ւ̃|�C���^
	CFade* pFade = nullptr;		//�t�F�[�h�ւ̃|�C���^

	if (pManager != nullptr) {
		//���݂̓��̓f�o�C�X�̎擾
		pInput = pManager->GetInputCur();
		//�t�F�[�h�̎擾
		pFade = pManager->GetFade();
	}

	//�c�莞�Ԍ����ɂ��X�|�[���Ԋu�̌���
	if (m_pEnemySpawner != nullptr) {
		//�c��200�b�ɂȂ����u��
		if (m_pTimer->GetTime() == 200 && m_pTimer->GetCountTimer() == 0) {
			m_pEnemySpawner->AddSpan(-60);
		}
		//�c��100�b�ɂȂ����u��
		if (m_pTimer->GetTime() == 100 && m_pTimer->GetCountTimer() == 0) {
			m_pEnemySpawner->AddSpan(-60);
		}
		//�c��60�b�ɂȂ����u��
		if (m_pTimer->GetTime() == 60 && m_pTimer->GetCountTimer() == 0) {
			m_pEnemySpawner->AddSpan(-60);
		}
		//�c��30�b�ɂȂ����u��
		if (m_pTimer->GetTime() == 30 && m_pTimer->GetCountTimer() == 0) {
			m_pEnemySpawner->AddSpan(-60);
		}
	}

	//�Q�[���I�����Ă��Ȃ��Ƃ��Ƀ^�C�}�[���O�ɂȂ����ꍇ
	if (m_pTimer != nullptr && !m_bGameClear && !m_bGameOver) {
		if (m_pTimer->GetTime() <= 0) {
			//�Q�[���N���A
			GameClear();
		}
	}

	//�Q�[���I����
	if ((m_bGameClear || m_bGameOver) && m_pMenuGameEnd != nullptr && pInput != nullptr) {
		//�I����
		if (pInput->GetTrigger(CInput::CODE::SELECT)) {
			switch (m_pMenuGameEnd->GetIdxCurSelect())
			{
			case 0:
				//�^�C�g���փV�[���J��
				if (pFade != nullptr) pFade->SetFade(CScene::SCENE_TYPE::TITLE, 0.02f);
				break;
			case 1:
				//�Q�[�����ăv���C
				if (pFade != nullptr) pFade->SetFade(CScene::SCENE_TYPE::GAME, 0.02f);
				break;
			}
		}
	}
}

//=============================================================================
// �Q�[���N���A
//=============================================================================
void CGameScene::GameClear(void) {
	if (m_bGameClear || m_bGameOver) return;

	m_bGameClear = true;

	//�G�����ׂĎ��S������
	CObject::DeadObjtype(CObject::OBJ_TYPE::ENEMY);

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�N���A�����Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::GAMECLEAR);

	//�Q�[���N���A�e�L�X�g�̕\��
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GAMECLEAR, 600.0f, 150.0f);

	//�Q�[���I�����j���[�̐���
	CreateMenuEndGame();

	//�G�̃X�|�i�[�̔j��
	if (m_pEnemySpawner != nullptr) {
		m_pEnemySpawner->Uninit();
		m_pEnemySpawner = nullptr;
	}
}

//=============================================================================
// �Q�[���I�[�o�[
//=============================================================================
void CGameScene::GameOver(void) {
	if (m_bGameClear || m_bGameOver) return;

	m_bGameOver = true;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�Q�[���I�[�o�[�����Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::GAMEOVER);

	//�Q�[���I�[�o�[�e�L�X�g�̕\��
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GAMEOVER, 600.0f, 150.0f);

	//�Q�[���I�����j���[�̐���
	CreateMenuEndGame();

	//�^�C�}�[���~�߂�
	if (m_pTimer != nullptr) m_pTimer->SetStop(true);

	//�G�̃X�|�i�[�̔j��
	if (m_pEnemySpawner != nullptr) {
		m_pEnemySpawner->Uninit();
		m_pEnemySpawner = nullptr;
	}
}

//=============================================================================
// �Q�[���I�����̑I�����j���[�̐���
//=============================================================================
void CGameScene::CreateMenuEndGame(void) {
	//���łɐ�������Ă���ꍇ�I��
	if (m_pMenuGameEnd != nullptr) return;

	//���j���[�̐���
	m_pMenuGameEnd = CSelectMenu2D::Create(2);
	if (m_pMenuGameEnd == nullptr) return;

	//���I��
	m_pMenuGameEnd->SetSelectType(CSelectMenu::SELECT_TYPE::HORIZONTAL);
	//�I����UI�̏ڍאݒ�
	m_pMenuGameEnd->SetSelectUI(0, D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 130.0f, 600.0f, 0.0f), 220.0f, 40.0f, CTexture::TEXTURE_TYPE::TEXT_QUIT);
	m_pMenuGameEnd->SetSelectUI(1, D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 130.0f, 600.0f, 0.0f), 220.0f, 40.0f, CTexture::TEXTURE_TYPE::TEXT_RETRY);
	//�I�����A�C�R���̐���
	m_pMenuGameEnd->CreateSelectIcon(D3DXVECTOR3(-110.0f, 0.0f, 0.0f), 50.0f, 50.0f, CTexture::TEXTURE_TYPE::SELECT_ICON);
	//�w�i�̐���
	m_pMenuGameEnd->CreateMenuBG(CTexture::TEXTURE_TYPE::MENU_BG , D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 600.0f, 0.0f), 600.0f, 80.0f);

}