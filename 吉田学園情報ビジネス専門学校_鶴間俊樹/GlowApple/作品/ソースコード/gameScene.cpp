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
#include "enemy_human.h"
#include "enemy_normal.h"
#include "enemy_sorcerer.h"

#include "appleTree.h"
#include "glowApple.h"
#include "selectMenu2D.h"
#include "enemySpawner.h"

#include "effect.h"

#include "shockWaveEmitter.h"


#include "apple_red.h"
#include "apple_green.h"
#include "apple_white.h"
#include "apple_black.h"
#include "apple_silver.h"
#include "apple_gold.h"

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

	m_nCntGameClear = 0;
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

	//���[�V�������̃��[�h
	CPlayer::LoadMotionInfo();
	CEnemyNormal::LoadMotionInfo();
	CEnemyHuman::LoadMotionInfo();
	CEnemySorcerer::LoadMotionInfo();

	//�X�e�[�W�̐���
	if(m_pStage == nullptr) m_pStage = new CStage;
	if(m_pStage != nullptr) m_pStage->CreateStage(TEXT_FILE_NAME_STAGE_GAME);

	//�S�[���h���b�V������
	CEnemy::SetGoldRush(false);

	//�ь�̖؂̐���
	m_pAppleTree = CAppleTree::Create(D3DXVECTOR3(0.0f, -190.0f, 0.0f));

	//�~���̕ǂ̐���
	CWallCylinder::Create(D3DXVECTOR3(0.0f, 60.0f, 0.0f), 1600.0f, 40.0f, CTexture::TEXTURE_TYPE::NONE, true);

	//�v���C���[�̐���
	CPlayer* pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, -300.0f, -1000.0f));
	//�V�[���̃v���C���[�̐ݒ�
	SetPlayer(pPlayer);

	//�^�C�}�[�̐���
	m_pTimer = CTimer::Create(GAME_TIME, 3, CTexture::TEXTURE_TYPE::NUMBER_003, D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 75.0f, 30.0f, 0.0f), 50.0f, 50.0f);

	//�ŏ��̓G�̔z�u
	CEnemyNormal::Create(D3DXVECTOR3(900.0f, -200.0f, 300.0f));
	CEnemyNormal::Create(D3DXVECTOR3(-800.0f, -200.0f, 500.0f));
	CEnemyNormal::Create(D3DXVECTOR3(-600.0f, -200.0f, -700.0f));

	//�G�̃X�|�i�[�̐���
	m_pEnemySpawner = CEnemySpawner::Create(900, 1800.0f, 500, 1500);

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
	CEnemySorcerer::UnloadMotionInfo();

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

	//�S�[���h���b�V������
	CEnemy::SetGoldRush(false);
}

//=============================================================================
// �Q�[���V�[���̍X�V����
//=============================================================================
void CGameScene::Update(void) {
#ifdef _DEBUG
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	if (pManager == nullptr) return;
	//���݂̓��̓f�o�C�X�̎擾
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;

	//�Q�[���N���A
	if (pInput->GetTrigger(CInput::CODE::DEBUG_0)) {
		GameClear();
	}
	//�Q�[���I�[�o�[
	if (pInput->GetTrigger(CInput::CODE::DEBUG_1)) {
		GameOver();
	}
#endif

	//�c�莞�Ԍ����ɂ��X�|�[���Ԋu�̌���
	if (m_pEnemySpawner != nullptr && m_pTimer != nullptr) {
		if (m_pTimer->GetTime() == 260 && m_pTimer->GetCountTimer() == 0) {
			//�X�|�[���Ԋu�̌���
			m_pEnemySpawner->AddSpan(-60);
			//���x���̐ݒ�
			m_pEnemySpawner->SetLevel(1);
		}

		if (m_pTimer->GetTime() == 220 && m_pTimer->GetCountTimer() == 0) {
			//�X�|�[���Ԋu�̌���
			m_pEnemySpawner->AddSpan(-120);
			//���x���̐ݒ�
			m_pEnemySpawner->SetLevel(2);
		}

		if (m_pTimer->GetTime() == 180 && m_pTimer->GetCountTimer() == 0) {
			//�X�|�[���Ԋu�̌���
			m_pEnemySpawner->AddSpan(-140);
			//���x���̐ݒ�
			m_pEnemySpawner->SetLevel(3);
			//�S�[���h���b�V���L��
			CEnemy::SetGoldRush(true);
		}

		if (m_pTimer->GetTime() == 100 && m_pTimer->GetCountTimer() == 0) {
			//�X�|�[���Ԋu�̌���
			m_pEnemySpawner->AddSpan(-160);
			//�S�[���h���b�V������
			CEnemy::SetGoldRush(false);
		}

		if (m_pTimer->GetTime() == 50 && m_pTimer->GetCountTimer() == 0) {
			//�X�|�[���Ԋu�̌���
			m_pEnemySpawner->AddSpan(-180);
			//���x���̐ݒ�
			m_pEnemySpawner->SetLevel(0);	//��Ԉړ����x�������G�݂̂ɂ���
		}

		if (m_pTimer->GetTime() == 20 && m_pTimer->GetCountTimer() == 0) {
			//���x���̐ݒ�
			m_pEnemySpawner->SetLevel(4);	//��Ԉړ����x�������G�݂̂ɂ���
			//�X�|�[���̔��a���k�߂�
			m_pEnemySpawner->SetSpawnRadius(800.0f);
		}
	}

	//�Q�[���I�����Ă��Ȃ��Ƃ��Ƀ^�C�}�[���O�ɂȂ����ꍇ
	if (m_pTimer != nullptr && !m_bGameClear && !m_bGameOver) {
		if (m_pTimer->GetTime() <= 0) {
			//�Q�[���N���A
			GameClear();
		}
	}

	//�Q�[���N���A��
	if (m_bGameClear) {
		UpdateGameClear();
	}

	//�Q�[���I�[�o�[��
	if (m_bGameOver) {
		UpdateGameOver();
	}
}

//=============================================================================
// �Q�[���N���A���̍X�V
//=============================================================================
void CGameScene::UpdateGameClear(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	if (pManager == nullptr) return;
	//���݂̓��̓f�o�C�X�̎擾
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;
	//�J�����̎擾
	CCamera* pCamera = pManager->GetCamera();	//�J�����ւ̃|�C���^
	if (pCamera == nullptr) return;
	//�t�F�[�h�̎擾
	CFade* pFade = pManager->GetFade();		//�t�F�[�h�ւ̃|�C���^
	if (pFade == nullptr) return;
	//�T�E���h�̎擾
	CSound* pSound = pManager->GetSound();	//�T�E���h�ւ̃|�C���^
	if (pSound == nullptr) return;


	if (m_nCntGameClear == 60) {
		pFade->SetFade(CScene::SCENE_TYPE(-1), 0.005f, 100);
	}

	//�^�C�g���ւ̃t�F�[�h�ȑO�́A�J�ڂȂ��t�F�[�h�؂�ւ��^�C�~���O
	if (pFade->GetChangeFade() && m_nCntGameClear < 600) {
		//�����S�̖؂��琶�����������S�̐��Ǝ�ނ��擾
		int nNumCreateApple = MAX_NUM_CREATE_APPLE;	//����N���A����ɂ͂��ׂẴ����S���Ȃ��Ă���̂łƂ肠�����ő吔
		//�ő吔�𒴂��Ȃ��悤��
		if (nNumCreateApple > MAX_NUM_CREATE_APPLE) nNumCreateApple = MAX_NUM_CREATE_APPLE;

		CGlowApple::APPLE_TYPE aTypeApple[MAX_NUM_CREATE_APPLE];

		for (int nCnt = 0; nCnt < MAX_NUM_CREATE_APPLE; nCnt++)
		{
			aTypeApple[nCnt] = m_pAppleTree->GetCreateAppleType(nCnt);
		}
		//�؂ƃ����S�̔j��
		CObject::ReleaseObjtype((CObject::OBJTYPE)(CObject::OBJTYPE_APPLE_TREE));

		//�؂𐶐�
		for (int nCntTree = 0; nCntTree < nNumCreateApple; nCntTree++)
		{
			//�؂̔z�u
			D3DXVECTOR3 posTree = D3DXVECTOR3(sinf(D3DX_PI * 2 * (nCntTree / (float)nNumCreateApple) + D3DX_PI) * 700.0f, -80.0f, cosf(D3DX_PI * 2 * (nCntTree / (float)nNumCreateApple) + D3DX_PI) * 700.0f);
			
			//�؂̐���
			CObjectModel* pTree = CObjectModel::Create(CModel::MODELTYPE::OBJ_APPLE_TREE, posTree, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);
			
			if (pTree == nullptr) continue;

			CModel* pModelTree = pTree->GetPtrModel();
			if (pModelTree == nullptr) continue;

			//�֊s�F�̐ݒ�
			pModelTree->SetColorGlow(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));

			//�؂Ƀ����S�̎�������
			for (int nCntApple = 0; nCntApple < MAX_NUM_CREATE_APPLE; nCntApple++)
			{
				D3DXVECTOR3 posApple = posTree + CAppleTree::GetOffsetPosApple(nCntApple);

				//�ь�𐶐�
				switch (aTypeApple[nCntTree])
				{
				case CGlowApple::APPLE_TYPE::RED:
					CAppleRed::Create(posApple, nullptr);
					break;
				case CGlowApple::APPLE_TYPE::GREEN:
					CAppleGreen::Create(posApple, nullptr);
					break;
				case CGlowApple::APPLE_TYPE::WHITE:
					CAppleWhite::Create(posApple, nullptr);
					break;
				case CGlowApple::APPLE_TYPE::BLACK:
					CAppleBlack::Create(posApple, nullptr);
					break;
				case CGlowApple::APPLE_TYPE::SILVER:
					CAppleSilver::Create(posApple, nullptr);
					break;
				case CGlowApple::APPLE_TYPE::GOLD:
					 CAppleGold::Create(posApple, nullptr);
					break;
				}
			}
		}

		//��������̑���


		//�J�����̈ʒu�ݒ�
		pCamera->Init();
		//�J�����̋����ݒ�
		pCamera->SetDistance(2000.0f);
		//�J�����̓����Œ�
		pCamera->SetLockControll(true);

		//�v���C���[�̎擾
		CPlayer* pPlayer = GetPlayer();
		if (pPlayer != nullptr) {
			//�v���C���[�̈ʒu�ݒ�
			pPlayer->SetPos(D3DXVECTOR3(0.0f, -800.0f, 0.0f));	//���������ɂȂ����̂Ŗ����ꂳ����
			pPlayer->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	if (m_nCntGameClear == 540) {
		//�Q�[���N���A�e�L�X�g�̕\��
		CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GAMECLEAR, 600.0f, 150.0f);
		//�N���A�����Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::GAMECLEAR);
	}
	if (m_nCntGameClear == 840) {
		//�J�����̓����̌Œ������
		pCamera->SetLockControll(false);
		//�^�C�g���փt�F�[�h
		pFade->SetFade(CScene::SCENE_TYPE::TITLE, 0.01f, 60);
	}

	//�Q�[���N���A��̃J�E���g
	m_nCntGameClear++;
}

//=============================================================================
// �Q�[���I�[�o�[���̍X�V
//=============================================================================
void CGameScene::UpdateGameOver(void) {
	if (m_pMenuGameEnd == nullptr) return;

	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	if (pManager == nullptr) return;
	//���݂̓��̓f�o�C�X�̎擾
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;
	//�t�F�[�h�̎擾
	CFade* pFade = pManager->GetFade();		//�t�F�[�h�ւ̃|�C���^
	if (pFade == nullptr) return;
	//�T�E���h�̎擾
	CSound* pSound = pManager->GetSound();	//�T�E���h�ւ̃|�C���^
	if (pSound == nullptr) return;

	//�I����
	if (pInput->GetTrigger(CInput::CODE::SELECT)) {
		switch (m_pMenuGameEnd->GetIdxCurSelect())
		{
		case 0:
			//�^�C�g���փV�[���J��
			pFade->SetFade(CScene::SCENE_TYPE::TITLE, 0.02f, 60);
			break;
		case 1:
			//�Q�[�����ăv���C
			pFade->SetFade(CScene::SCENE_TYPE::GAME, 0.02f, 60);
			break;
		}

		//�I�����艹�̍Đ�
		pSound->PlaySound(CSound::SOUND_LABEL::TITLE_START);
	}
}

//=============================================================================
// �Q�[���N���A
//=============================================================================
void CGameScene::GameClear(void) {
	if (m_bGameClear || m_bGameOver) return;

	m_bGameClear = true;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//�v���C���[�̎擾
	CPlayer* pPlayer = GetPlayer();
	if (pPlayer != nullptr) {
		//�v���C���[�̃Q�[���N���A���̏���
		pPlayer->GameClear();
	}

	//�G�����ׂĎ��S������
	CObject::DeadObjtype(CObject::OBJTYPE_ENEMY);
	//�G���ŉ����Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::ENDGAME);
	//�Ռ��g
	CShockWaveEmitter::Create(3, 6, D3DXVECTOR3(0.0f, -190.0f, 0.0f), 60.0f, 10.0f, 15.0f, 400.0f, -2.0f, 120, D3DX_PI * 0.01f);

	//�^�C�}�[��j��
	if (m_pTimer != nullptr) {
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

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


	//�v���C���[�̎擾
	CPlayer* pPlayer = GetPlayer();
	if (pPlayer != nullptr) {
		//�v���C���[�̃Q�[���I�[�o�[���̏���
		pPlayer->GameOver();
	}

	//�Q�[���I�[�o�[�e�L�X�g�̕\��
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GAMEOVER, 600.0f, 150.0f);

	//�Q�[���I�����j���[�̐���
	CreateMenuEndGame();

	//�^�C�}�[��j��
	if (m_pTimer != nullptr) {
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

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