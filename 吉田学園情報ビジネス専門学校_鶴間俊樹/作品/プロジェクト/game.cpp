//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "game.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sound.h"
#include "pause.h"
#include "fade.h"
#include "timer.h"
#include "score.h"
#include "player.h"
#include "scene2D.h"
#include "billboard.h"
#include "terrain.h"
#include "meshfield.h"
#include "wallCylinder.h"
#include "boss.h"
#include "enemy_normal.h"
#include "enemy_human.h"
#include "tree.h"
#include "compass.h"

#include "particleEffect.h"
//=============================================================================
// �}�N����`
//=============================================================================
#define GAME_TIME (120)			//�Q�[���̐�������
#define COUNT_FADE_GAMEOVER (180)		//�Q�[���I�[�o�[���̃t�F�[�h�܂ł̃J�E���g
#define COUNT_FADE_GAMECLEAR (180)		//�Q�[���N���A���̃t�F�[�h�܂ł̃J�E���g
#define FADE_SPEED (0.02f)		//�t�F�[�h���鑬�x
#define GAMECLEAR_START (60)	//�Q�[���N���A���ɕ\���������܂ł̃J�E���g
#define GAMEOVER_START (120)	//�Q�[���I�[�o�[���ɕ\���������܂ł̃J�E���g
#define GAMEOVER_TIME (120)		//�Q�[���I�[�o�[�̕\������������s�����ɂȂ�܂ł̎���
#define WALL_RADIUS_STAGE_START (5000.0f)	//�X�^�[�g���̃X�e�[�W�̕ǂ̔��a
#define WALL_RADIUS_STAGE_BOSS (2500.0f)	//�{�X�펞�̃X�e�[�W�̕ǂ̔��a

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CGame::m_nScoreLate = 0;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGame::CGame() {
	m_bChangeStage = false;
	m_pTimer = nullptr;
	m_pPlayer = nullptr;
	m_pRaft = nullptr;
	m_pIconInteract = nullptr;
	m_pBoss = nullptr;
	m_pGameClear = nullptr;
	m_pGameOver = nullptr;
	m_bGameOver = false;
	m_bGameClear = false;
	m_nCntFinish = 0;
	m_pFogBillboard = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame() {

}

//=============================================================================
// �Q�[���I�����̃X�R�A�̐ݒ�
//=============================================================================
void CGame::SetScoreLate(int nScore) {
	m_nScoreLate = nScore;
}

//=============================================================================
// �Q�[���I�����̃X�R�A�̎擾
//=============================================================================
int CGame::GetScoreLast(void) {
	return m_nScoreLate;
}

//=============================================================================
// �Q�[���̏���������
//=============================================================================
HRESULT CGame::Init(void) {
	//----------------------------------
	//���[�V�����̃��[�h
	//----------------------------------
	//�v���C���[���[�V�����̃��[�h
	CPlayer::LoadMotionInfo();
	//�G�̃��[�V�����̃��[�h
	CEnemyNormal::LoadMotionInfo();
	CEnemyHuman::LoadMotionInfo();
	//�{�X�̃��[�V�����̃��[�h
	CBoss::LoadMotionInfo();
	//�؂̃��[�V�����̃��[�h
	CTree::LoadMotionInfo();

	//----------------------------------
	//�I�u�W�F�N�g�̃C���X�^���X����
	//----------------------------------
	//�^�C�}�[�̘g�̐���
	CScene2D* pTimerFrame = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH -20.0f - 75.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::TIMER_FRAME, 200.0f, 80.0f);
	if (pTimerFrame != nullptr) {
		pTimerFrame->SetDrawPriority(CScene::DRAW_PRIORITY::UI);
	}
	//�^�C�}�[�̐���
	m_pTimer = CTimer::Create(GAME_TIME, 3, CTexture::TEXTURE_TYPE::NUMBER_004, D3DXVECTOR3(SCREEN_WIDTH - 20.0f, 25.0f, 0.0f), 50.0f, 50.0f);
	//�n�`�̐���
	CTerrain* pTerrain = CTerrain::Create(CTerrain::TERRAIN_TYPE::START);
	if (pTerrain != nullptr) pTerrain->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//�`�揇�̐ݒ�
	//�C�̐���
	CMeshfield* pMeshField = CMeshfield::Create(D3DXVECTOR3(0.0f, HEIGHT_SEA, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, 30000.0f, 30000.0f);
	if (pMeshField != nullptr) {
		pMeshField->SetColor(D3DXCOLOR(0.0f, 0.2f, 0.6f, 1.0f));
		pMeshField->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//�`�揇�̐ݒ�
	}

	//�t�H�O��`�悳���邽�߃r���{�[�h
	m_pFogBillboard = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 15000.0f, 12000.0f);
	//�t�H�O��`�悳����r���{�[�h�̐ݒ�
	if (m_pFogBillboard != nullptr) {
		m_pFogBillboard->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//�`�揇�̐ݒ�
		m_pFogBillboard->SetEnableFog(true);		//�t�H�O���L��
		m_pFogBillboard->SetZtestAlways(false);		//�[�x���e������
		m_pFogBillboard->SetDrawAllDistance(true);	//�����Ɋ֌W�Ȃ��`��ł���
	}

	//�~���̕ǂ̐���
	CWallCylinder* pWall = CWallCylinder::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), WALL_RADIUS_STAGE_START);
	//�~���̕ǂ̐ݒ�
	if (pWall != nullptr) pWall->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//�`�揇�̐ݒ�

	//�v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, HEIGHT_SEA, -3750.0f));
	//�؂̐���
	CTree::SetTreeCircle(D3DXVECTOR3(0.0f, HEIGHT_SEA, 0.0f), 8, 500.0f, 800, 1200);
	//�G�̐���
	CEnemy::SetEnemyCircle(D3DXVECTOR3(0.0f, HEIGHT_SEA, 0.0f), 3, 1000.0f, 500, 800);
	//���𐶐�
	m_pRaft = CSceneModel::Create(CModel::MODELTYPE::RAFT, D3DXVECTOR3(0.0f, HEIGHT_SEA, 4700.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.1f, 0.0f));
	if (m_pRaft != nullptr) m_pRaft->SetObjType(CScene::OBJ_TYPE::OTHER);
	//���̈ʒu�̃p�[�e�B�N���G�t�F�N�g�̐���
	CParticleEffect::PARTICLE_INFO particleInfo = { 120, 50.0f, -0.5f, 2.0f, D3DXVECTOR3(0.0f, -0.0f, 0.0f), D3DXCOLOR(0.2f, 0.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.2f, 1.0f) };
	CParticleEffect::Create(particleInfo, D3DXVECTOR3(0.0f, HEIGHT_SEA, 4700.0f), 0, 10, 0.04f * D3DX_PI);
	//�C���^���N�g�L�[�L�����̃A�C�R���̐���	���̏�̈ʒu
	m_pIconInteract = CBillboard::Create(D3DXVECTOR3(0.0f, 100.0f, 4700.0f), CTexture::TEXTURE_TYPE::NONE, 50.0f, 50.0f);
	if (m_pIconInteract != nullptr) {
		m_pIconInteract->SetObjType(CScene::OBJ_TYPE::OTHER);
		m_pIconInteract->SetDrawPriority(CScene::DRAW_PRIORITY::UI);
		m_pIconInteract->SetDraw(false);
	}
	//�R���p�X�̐���
	CCompass* pCompass = CCompass::Create();
	if (pCompass != nullptr) pCompass->SetObjType(CScene::OBJ_TYPE::OTHER);

	//----------------------------------
	//�����o�ϐ��̏�����
	//----------------------------------
	m_nScoreLate = 0;
	m_bGameOver = false;
	m_bGameClear = false;
	m_nCntFinish = 0;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�Ȃ̍Đ�
	if (pSound != nullptr) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_GAME);
		pSound->SetBGM(CSound::SOUND_LABEL::BGM_GAME);
	}

	return S_OK;
}

//=============================================================================
// �Q�[���̏I������
//=============================================================================
void CGame::Uninit(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	CSound *pSound = nullptr;
	CPause* pPause = nullptr;

	if (pManager != nullptr) {
		//�T�E���h�̎擾
		pSound = pManager->GetSound();
		//�|�[�Y�̎擾
		pPause = pManager->GetPause();
	}

	//�|�[�Y�Ȃ�
	if (pPause != nullptr) {
		pPause->SetPause(false);
	}

	//�Ȃ̒�~
	if (pSound != nullptr) {
		pSound->StopSound(CSound::GetBGM());
		pSound->SetBGM(CSound::SOUND_LABEL::NONE);
	}

	//�I�u�W�F�N�g�̔j��
	CScene::ReleaseAll();

	//----------------------------------
	//���[�V�����̃A�����[�h
	//----------------------------------
	//�v���C���[���[�V�����̃A�����[�h
	CPlayer::UnloadMotionInfo();
	//�{�X�̃��[�V�����̃A�����[�h
	CBoss::UnloadMotionInfo();
	//�G�̃��[�V�����̃A�����[�h
	CEnemyNormal::UnloadMotionInfo();
	CEnemyHuman::UnloadMotionInfo();
	//�؂̃��[�V�����̃A�����[�h
	CTree::UnloadMotionInfo();
}

//=============================================================================
// �Q�[���̍X�V����
//=============================================================================
void CGame::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	CPause* pPause = nullptr;
	CInput* pInput = nullptr;
	CFade* pFade = nullptr;
	CSound* pSound = nullptr;
	CCamera* pCamera = nullptr;

	if (pManager != nullptr) {
		//�|�[�Y�̎擾
		pPause = pManager->GetPause();
		//���݂̓��̓f�o�C�X�̎擾
		pInput = pManager->GetInputCur();
		//�t�F�[�h�̎擾
		pFade = pManager->GetFade();
		//�T�E���h�̎擾
		pSound = pManager->GetSound();
		//�J�����̎擾
		pCamera = pManager->GetCamera();
	}

	if (pPause != nullptr) {
		//�|�[�Y���������ꍇ�X�V���Ȃ�
		if (pPause->GetPause()) return;
	}

	if (pFade != nullptr) {
		//�X�e�[�W�̐؂�ւ����̃t�F�[�h��
		if (m_bChangeStage) {
			//�X�e�[�W�̐؂�ւ�
			if (pFade->GetChangeFade()) SetNextStage();
		}
	}

	//���p�̃r���{�[�h�̈ʒu��ݒ�
	if (pCamera != nullptr && m_pFogBillboard != nullptr) {
		D3DXVECTOR3 posCameraV = pCamera->GetPosV();
		D3DXVECTOR3 posCameraR = pCamera->GetPosR();
		D3DXVECTOR3 vecCamera = posCameraR - posCameraV;	//�J�����̎��_���璍���_�ւ̃x�N�g��
		D3DXVec3Normalize(&vecCamera, &vecCamera);	//���K��
		D3DXVECTOR3 posFogBillboard = posCameraV + vecCamera * MAX_FOG_DISTANCE;

		m_pFogBillboard->SetPos(posFogBillboard);
	}

	//�C���^���N�g�L�[�L�����̃A�C�R���̃e�N�X�`���̍X�V
	if (m_pIconInteract != nullptr && pInput != nullptr) {
		CInputKeyboard* pInputKeyboard = dynamic_cast<CInputKeyboard*>(pInput);
		//���݂̓��̓f�o�C�X���L�[�{�[�h�������ꍇ
		if (pInputKeyboard != nullptr) {
			m_pIconInteract->SetTexType(CTexture::TEXTURE_TYPE::ICON_INTERACT_KEYBOARD);
		}
		//�Q�[���p�b�h
		else {
			m_pIconInteract->SetTexType(CTexture::TEXTURE_TYPE::ICON_INTERACT_GAMEPAD);
		}
	}

	//�^�C�}�[���O�ȉ��ŃQ�[���I�[�o�[
	if (!m_bGameOver && m_pTimer != nullptr) {
		if (m_pTimer->GetTime() <= 0) {
			GameOver();
		}
	}

	//�Q�[���I�[�o�[���A�Q�[���N���A���ł͂Ȃ��ꍇ
	if (!m_bGameOver && !m_bGameClear) {
		//�|�[�Y
		if (pFade != nullptr) {
			if (!pFade->GetFade() && pInput->GetTrigger(CInput::CODE::PAUSE)) {
				pPause->SetPause(true);
			}
		}
	}

	//�Q�[���N���A��
	if (m_bGameClear) {
		//�Q�[���N���A���琔�t���[����
		if (m_nCntFinish == GAMECLEAR_START) {

		}
	}

	//�Q�[���I�[�o�[��
	if (m_bGameOver) {
		if (m_nCntFinish == GAMEOVER_START) {
			//���ʉ��̍Đ�
			if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::GAMEOVER);
			//�Q�[���I�[�o�[��ʂ̐���
			m_pGameOver = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::GAMEOVER, SCREEN_WIDTH, SCREEN_HEIGHT);
			if (m_pGameOver != nullptr) {
				m_pGameOver->SetDrawPriority(CScene2D::DRAW_PRIORITY::FRONT);	//�`�揇���őO
				m_pGameOver->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));		//����
			}
		}
		//�����x���グ�Ă���
		if (m_pGameOver != nullptr && m_nCntFinish <= (GAMEOVER_START + GAMEOVER_TIME)) {
			m_pGameOver->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(m_nCntFinish - GAMEOVER_START) / (float)GAMEOVER_TIME));
		}
	}

	//�Q�[���N���A�����Q�[���I�[�o�[���̃t�F�[�h�܂ł̃J�E���g
	if (pFade != nullptr && (m_bGameClear || m_bGameOver)) {
		//�Q�[���N���A��
		if (m_bGameClear) {
			//�t�F�[�h�J�n
			if (m_nCntFinish == COUNT_FADE_GAMECLEAR) {
				pFade->SetFade(CManager::MODE::RESULT, FADE_SPEED);
			}
		}
		//�Q�[���I�[�o�[��
		else if (m_bGameOver) {
			//�t�F�[�h�J�n
			if (m_nCntFinish == COUNT_FADE_GAMEOVER + GAMEOVER_TIME) {
				pFade->SetFade(CManager::MODE::TITLE, FADE_SPEED);
			}
		}
		m_nCntFinish++;
	}
}

//=============================================================================
// �X�e�[�W�ύX�����ǂ����̎擾
//=============================================================================
bool CGame::GetChangeStage(void) {
	return m_bChangeStage;
}

//=============================================================================
// ���̃X�e�[�W�ֈڍs���邽�߂̃t�F�[�h��ݒ�
//=============================================================================
void CGame::SetFadeNextStage(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�t�F�[�h�̎擾
	CFade* pFade = nullptr;
	if (pManager != nullptr) pFade = pManager->GetFade();
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	if (pFade != nullptr) {
		//�t�F�[�h�̐ݒ�
		pFade->SetFade((CManager::MODE) -1, 0.01f);
		m_bChangeStage = true;
		//�t�F�[�h�����Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CHANGE_STAGE);
		//�ǂ̔j��
		CScene::ReleaseObjtype(CScene::OBJ_TYPE::WALL);
	}
}

//=============================================================================
// ���̃X�e�[�W��ݒ�
//=============================================================================
void CGame::SetNextStage(void) {
	m_bChangeStage = false;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�J�����̎擾
	CCamera* pCamera = nullptr;
	if (pManager != nullptr) pCamera = pManager->GetCamera();

	//���y�̒�~
	if (pSound != nullptr) {
		pSound->StopSound(CSound::GetBGM());
		pSound->SetBGM(CSound::SOUND_LABEL::NONE);
	}
	//�S�G�̔j��
	CScene::ReleaseObjtype(CScene::OBJ_TYPE::ENEMY);
	//�S�؂̔j��
	CScene::ReleaseObjtype(CScene::OBJ_TYPE::TREE);
	//�S�؂̔j��
	CScene::ReleaseObjtype(CScene::OBJ_TYPE::ITEM);
	//�n�`�̔j��
	CScene::ReleaseObjtype(CScene::OBJ_TYPE::TERRAIN);
	//���̑��I�u�W�F�N�g�̔j��
	CScene::ReleaseObjtype(CScene::OBJ_TYPE::OTHER);
	//���̃|�C���^��null�ɂ��� ���̑��̃I�u�W�F�N�g�Ƃ��Ĕj�������̂�
	m_pRaft = nullptr;
	m_pIconInteract = nullptr;

	//�n�`�̐���
	CTerrain* pTerrain = CTerrain::Create(CTerrain::TERRAIN_TYPE::BOSS);
	if (pTerrain != nullptr) pTerrain->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//�`�揇�̐ݒ�
	//�ǂ̐���
	CWallCylinder* pWall = CWallCylinder::Create(D3DXVECTOR3(0.0f, -300.0f, 0.0f), WALL_RADIUS_STAGE_BOSS);
	if (pWall != nullptr) pWall->SetDrawPriority(CScene::DRAW_PRIORITY::BG);	//�`�揇�̐ݒ�
	//�{�X�̐���
	CBoss::Create(D3DXVECTOR3(0.0f, HEIGHT_SEA, 400.0f));

	//�v���C���[�̐ݒ�
	if (m_pPlayer != nullptr) {
		//�v���C���[�̈ʒu��ݒ�
		m_pPlayer->SetPos(D3DXVECTOR3(0.0f, HEIGHT_SEA, -1500.0f));
		//�v���C���[�̊p�x��ݒ�
		m_pPlayer->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		//�J�����̐ݒ�
		if (pCamera != nullptr) {
			//�J�����̈ʒu�̐ݒ�
			pCamera->SetPos(m_pPlayer->GetPos());
			//�J�����̊p�x�̐ݒ�
			pCamera->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, D3DX_PI, 0.0f));
		}
	}
}

//=============================================================================
// �{�X���S���̏���
//=============================================================================
void CGame::DeadBoss(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	if (m_pTimer != nullptr) {
		//�^�C�}�[�̒�~
		m_pTimer->SetStop(true);
	}

	if (pSound != nullptr) {
		//�Ȃ̒�~
		pSound->StopSound(CSound::GetBGM());
	}
}

//=============================================================================
// �Q�[���I�[�o�[���̏���
//=============================================================================
void CGame::GameOver(void) {
	//�^�C�}�[�̒�~
	if (m_pTimer != nullptr) m_pTimer->SetStop(true);

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//�Ȃ̒�~
	if (pSound != nullptr) {
		pSound->StopSound(CSound::GetBGM());	//BGM�̒�~
		pSound->SetBGM(CSound::SOUND_LABEL::NONE);
		pSound->PlaySound(CSound::SOUND_LABEL::STUN);	//�Q�[���I�[�o�[���ʉ��̍Đ�
	}

	//�v���C���[�ɃQ�[���I�[�o�[���̏������s�킹��
	if (m_pPlayer != nullptr) {
		m_pPlayer->GameOver();
	}

	m_bGameOver = true;	//�Q�[���I�[�o�[�̐ݒ�
}

//=============================================================================
//�Q�[���I�[�o�[���ǂ����̎擾
//=============================================================================
bool CGame::GetGameOver(void) { return m_bGameOver; }		

//=============================================================================
// �Q�[���N���A���̏���
//=============================================================================
void CGame::GameClear(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	int nTimeClear = 0;	//�N���A���̃^�C��
	if (m_pTimer != nullptr) {
		//�N���A���̃^�C���̎擾
		nTimeClear = m_pTimer->GetTime();
	}
	//�Ō�̃Q�[���̃X�R�A��ݒ�
	SetScoreLate(nTimeClear);

	if (pSound != nullptr) {
		//�Q�[���N���A���ʉ��̍Đ�
		pSound->PlaySound(CSound::SOUND_LABEL::GAMECLEAR);
	}

	//�Q�[���N���A�\���̐���
	m_pGameClear = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GAMECLEAR, 800.0f, 200.0f);
	//�`�揇���őO
	if (m_pGameClear != nullptr) m_pGameClear->SetDrawPriority(CScene2D::DRAW_PRIORITY::FRONT);
	//�v���C���[�ɃQ�[���N���A���̏������s�킹��
	if (m_pPlayer != nullptr) {
		m_pPlayer->GameClear();
	}

	//�Q�[���N���A�̐ݒ�
	m_bGameClear = true;		
}

//=============================================================================
//�Q�[���I�[�o�[���ǂ����̎擾
//=============================================================================
bool CGame::GetGameClear(void) { return m_bGameClear; }

//=============================================================================
// �v���C���[�̎擾
//=============================================================================
CPlayer* CGame::GetPlayer(void) {
	return m_pPlayer;
}

//=============================================================================
// �Q�[�������v���C���[�ւ̃|�C���^��NULL�ɂ���
//=============================================================================
void CGame::ReleasePlayer(void) {
	m_pPlayer = nullptr;
}

//=============================================================================
// �{�X�̎擾
//=============================================================================
CBoss* CGame::GetBoss(void) {
	return m_pBoss;
}

//=============================================================================
// �Q�[�������{�X�ւ̃|�C���^��NULL�ɂ���
//=============================================================================
void CGame::ReleaseBoss(void) {
	m_pBoss = nullptr;
}

//=============================================================================
// �^�C�}�[�̎擾
//=============================================================================
CTimer* CGame::GetTimer(void) {
	return m_pTimer;
}

//=============================================================================
// ���̎擾
//=============================================================================
CSceneModel* CGame::GetRaft(void) {
	return m_pRaft;
}

//=============================================================================
// �C���^���N�g�L�[�L�����̃A�C�R���̎擾
//=============================================================================
CBillboard* CGame::GetIconInteract(void) {
	return m_pIconInteract;
}