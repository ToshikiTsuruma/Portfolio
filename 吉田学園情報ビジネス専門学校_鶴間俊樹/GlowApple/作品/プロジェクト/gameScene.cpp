//=============================================================================
//
// �Q�[���V�[������ [gameScene.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "gameScene.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"

#include "timer.h"
#include "score.h"
#include "gameScoreManager.h"
#include "pauseMenu.h"
#include "stage.h"
#include "wallCylinder.h"
#include "object2D.h"

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
#define TEXT_FILE_NAME_HISCORE "data/TEXT/save_hiscore.txt"

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
	m_pScore = nullptr;
	m_pMenuGameEnd = nullptr;
	m_nGameScore = 0;

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
	pCamera->AddRot(D3DXVECTOR3(0.0f, -0.3f, 0.0f));

	//���[�V�������̃��[�h
	CPlayer::LoadMotionInfo();
	CEnemyNormal::LoadMotionInfo();
	CEnemyHuman::LoadMotionInfo();
	CEnemySorcerer::LoadMotionInfo();

	//�I�u�W�F�N�g�̃|�[�Y�������悤�ɐݒ�
	CObject::SetUpdatePauseLevel(0);

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
	m_pTimerFrame = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 61.0f, 0.0f), CTexture::TEXTURE_TYPE::TIMER_FRAME, 220.0f, 80.0f);
	m_pTimer = CTimer::Create(GAME_TIME, 3, CTexture::TEXTURE_TYPE::NUMBER_003, D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 75.0f, 40.0f, 0.0f), 50.0f);

	//�X�R�A�̐���
	m_pScoreFrame = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 20.0f - 120.0f, 61.0f, 0.0f), CTexture::TEXTURE_TYPE::SCORE_FRAME, 280.0f, 80.0f);
	m_pScore = CScore::Create(6, CTexture::TEXTURE_TYPE::NUMBER_004, D3DXVECTOR3(SCREEN_WIDTH - 20.0f, 42.0f, 0.0f), 40.0f);
	if (m_pScore != nullptr) {
		m_pScore->SetDigitMin(6);
		m_pScore->SetScore(0);
	}

	//�Q�[���X�R�A�}�l�[�W���[�̐���
	m_pGameScoreManager = CGameScoreManager::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 150.0f, 0.0f), 80.0f);

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

#ifdef _DEBUG
	//Z�o�b�t�@�e�N�X�`���̕\��
	CObject2D* pZBuff = CObject2D::Create(D3DXVECTOR3(70.0f, 70.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 100.0f, 100.0f);
	if (pZBuff != nullptr) {
		pZBuff->SetDrawPriority(CObject::DRAW_PRIORITY::FRONT);
		pZBuff->SetUseZBuffTexture(true);
	}
#endif

	m_nGameScore = 0;
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

	//���̒�~
	if (pSound != nullptr) {
		pSound->StopSound();
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

	//�^�C���ǉ�
	if (pInput->GetTrigger(CInput::CODE::DEBUG_3)) {
		if (m_pTimer != nullptr) m_pTimer->AddScore(50);
	}
#endif

	//�Q�[���N���A��
	if (m_bGameClear) {
		UpdateGameClear();
	}
	//�Q�[���I�[�o�[��
	else if (m_bGameOver) {
		UpdateGameOver();
	}
	//�Q�[����
	else
	{
		UpdateGame();
	}
}

//=============================================================================
// �Q�[�����̍X�V
//=============================================================================
void CGameScene::UpdateGame(void) {
	//�|�[�Y���j���[������ꍇ�͍X�V�Ȃ�
	if (m_pMenuPause != nullptr) return;

	//�Q�[���I�����Ă��Ȃ��Ƃ��Ƀ^�C�}�[���O�ɂȂ����ꍇ
	if (m_pTimer != nullptr && !m_bGameClear && !m_bGameOver) {
		if (m_pTimer->GetScore() <= 0) {
			//�Q�[���N���A
			GameClear();
		}
	}

	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	if (pManager == nullptr) return;
	//���݂̓��̓f�o�C�X�̎擾
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;
	//�T�E���h�̎擾
	CSound* pSound = pManager->GetSound();	//�T�E���h�ւ̃|�C���^
	if (pSound == nullptr) return;
	//�t�F�[�h�̎擾
	CFade* pFade = pManager->GetFade();	//�t�F�[�h�̃|�C���^
	if (pFade == nullptr) return;

	//�|�[�Y
	if (pInput->GetTrigger(CInput::CODE::PAUSE) && !pFade->GetFade()) {
		//�|�[�Y���j���[�N���X�𐶐�
		m_pMenuPause = CPauseMenu::Create();
		//�T�E���h���Đ�
		pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
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


		if (m_pScore != nullptr) {
			//�Q�[���̃X�R�A���擾
			m_nGameScore = m_pScore->GetScore();
			//�E��̃X�R�A��j��
			m_pScore->Uninit();
			m_pScore = nullptr;
		}
		if (m_pScoreFrame != nullptr) {
			m_pScoreFrame->Uninit();
			m_pScoreFrame = nullptr;
		}


		//�^�C�}�[��j��
		if (m_pTimer != nullptr) {
			m_pTimer->Uninit();
			m_pTimer = nullptr;
		}
		if (m_pTimerFrame != nullptr) {
			m_pTimerFrame->Uninit();
			m_pTimerFrame = nullptr;
		}

		//�Q�[���X�R�A�}�l�[�W���[��j��
		if (m_pGameScoreManager != nullptr) {
			m_pGameScoreManager->Uninit();
			m_pGameScoreManager = nullptr;
		}

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
		pSound->PlaySound(CSound::SOUND_LABEL::GAMECLEAR);

		//�X�R�A�̔w�i�̕\��
		CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 555.0f, 0.0f), CTexture::TEXTURE_TYPE::MENU_BG, 800.0f, 150.0f);

		//Score�e�L�X�g�\��
		CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 200.0f, 525.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_SCORE, 200.0f, 50.0f);
		//�X�R�A��\��
		CScore* pScoreGame = CScore::Create(6, CTexture::TEXTURE_TYPE::NUMBER_004, D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 50.0f * 6, 500.0f, 0.0f), 50.0f);
		if (pScoreGame != nullptr) {
			pScoreGame->SetDigitMin(6);
			pScoreGame->SetScore(m_nGameScore);
		}

		int nHighScore = 0;	//�n�C�X�R�A
		//�t�@�C�����J��
		FILE* pFile = fopen(TEXT_FILE_NAME_HISCORE, "r");
		if (pFile != nullptr) {
			//�n�C�X�R�A���擾
			fscanf(pFile, "%d", &nHighScore);
			//�t�@�C�������
			fclose(pFile);
		}

		//�n�C�X�R�A�ƍ���̃X�R�A���r
		if (nHighScore < m_nGameScore) {
			//�X�R�A�̒u������
			nHighScore = m_nGameScore;
			//�t�@�C�����J��
			pFile = fopen(TEXT_FILE_NAME_HISCORE, "w");
			//�n�C�X�R�A��ۑ�
			fprintf(pFile, "%d", nHighScore);
			//�t�@�C�������
			fclose(pFile);

			//�n�C�X�R�A�X�V�̃e�L�X�g��\��
			CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 350.0f, 500.0f, 0.0f), CTexture::TEXTURE_TYPE::UPDATE_HISCORE, 200.0f, 100.0f);
		}

		//High Score�e�L�X�g�\��
		CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 200.0f, 585.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_HISCORE, 200.0f, 50.0f);
		//�n�C�X�R�A��\��
		CScore* pHighScore = CScore::Create(6, CTexture::TEXTURE_TYPE::NUMBER_005, D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 50.0f * 6, 560.0f, 0.0f), 50.0f);
		if (pHighScore != nullptr) {
			pHighScore->SetDigitMin(6);
			pHighScore->SetScore(nHighScore);
		}

	}
	if (m_nCntGameClear == 900) {
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
// �|�[�Y���j���[�̔j��
//=============================================================================
void CGameScene::DeletePauseMenu(void) {
	//�|�[�Y���j���[�̔j��
	if (m_pMenuPause != nullptr) {
		m_pMenuPause->Uninit();
		m_pMenuPause = nullptr;
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
	CShockWaveEmitter* pSWEmitter = CShockWaveEmitter::Create(5, 6, D3DXVECTOR3(0.0f, -190.0f, 0.0f), 60.0f, 10.0f, 15.0f, 400.0f, -2.0f, 120, D3DX_PI * 0.01f);
	//�������ƂɐF��ύX
	if (pSWEmitter != nullptr) {
		pSWEmitter->SetColAddCreate(D3DXCOLOR(D3DXCOLOR(0.0f, -0.1f, -0.2f, 0.0f)));
	}

	//�^�C�}�[��j��
	if (m_pTimer != nullptr) {
		m_pTimer->SetStop(true);
	}

	//�G�̃X�|�i�[�̔j��
	if (m_pEnemySpawner != nullptr) {
		m_pEnemySpawner->Uninit();
		m_pEnemySpawner = nullptr;
	}

	//�I�u�W�F�N�g�̃|�[�Y�������悤�ɐݒ�i�O�̂��߁j
	CObject::SetUpdatePauseLevel(0);
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

	//�^�C�}�[���~
	if (m_pTimer != nullptr) {
		m_pTimer->SetStop(true);
	}

	//�G�̃X�|�i�[�̔j��
	if (m_pEnemySpawner != nullptr) {
		m_pEnemySpawner->Uninit();
		m_pEnemySpawner = nullptr;
	}

	//�Q�[���X�R�A�}�l�[�W���[��j��
	if (m_pGameScoreManager != nullptr) {
		m_pGameScoreManager->Uninit();
		m_pGameScoreManager = nullptr;
	}

	//�I�u�W�F�N�g�̃|�[�Y�������悤�ɐݒ�i�O�̂��߁j
	CObject::SetUpdatePauseLevel(0);
}

//=============================================================================
// �Q�[���I�����̑I�����j���[�̐���
//=============================================================================
void CGameScene::CreateMenuEndGame(void) {
	//���łɐ�������Ă���ꍇ�I��
	if (m_pMenuGameEnd != nullptr) return;

	//���j���[�̐���
	m_pMenuGameEnd = CSelectMenu2D::Create(2, true);
	if (m_pMenuGameEnd == nullptr) return;

	//�w�i�̐ݒ�
	m_pMenuGameEnd->SetMenuBG(CTexture::TEXTURE_TYPE::MENU_BG, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 600.0f, 0.0f), 600.0f, 80.0f);
	//���I��
	m_pMenuGameEnd->SetSelectType(CSelectMenu::SELECT_TYPE::HORIZONTAL);
	//�I����UI�̏ڍאݒ�
	m_pMenuGameEnd->SetSelectUI(0, D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 130.0f, 600.0f, 0.0f), 220.0f, 40.0f, CTexture::TEXTURE_TYPE::TEXT_QUIT);
	m_pMenuGameEnd->SetSelectUI(1, D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 130.0f, 600.0f, 0.0f), 220.0f, 40.0f, CTexture::TEXTURE_TYPE::TEXT_RETRY);
	//�I�����A�C�R���̐���
	m_pMenuGameEnd->CreateSelectIcon(D3DXVECTOR3(-80.0f, 0.0f, 0.0f), 50.0f, 50.0f, CTexture::TEXTURE_TYPE::SELECT_ICON);
	m_pMenuGameEnd->SetIconPosOffset(1, D3DXVECTOR3(-105.0f, 0.0f, 0.0f));
}

//=============================================================================
// �Q�[���̃X�R�A�����Z
//=============================================================================
void CGameScene::AddGameScore(int nScore) {
	if (m_pScore == nullptr || m_pGameScoreManager == nullptr) return;
	int nAddScore = nScore;

	if (m_pGameScoreManager != nullptr) {
		//�R���{�̉��Z
		m_pGameScoreManager->AddKillCombo();
		nAddScore = (int)(nAddScore * m_pGameScoreManager->GetScoreRate());
	}

	//�X�R�A�̉��Z
	m_pScore->AddScore(nAddScore);
}