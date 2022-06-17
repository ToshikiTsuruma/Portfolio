//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sound.h"
#include "input.h"
#include "game.h"
#include "wallCylinder.h"
#include "terrain.h"
#include "item.h"
#include "effect.h"
#include "particle.h"
#include "particleEffect.h"
#include "scene2D.h"
#include "billboard.h"
#include "sword.h"
#include "katana.h"	 
#include "score.h"
#include "timer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_player.txt"
#define COLLISION_RADIUS (15.0f)		//�����蔻��̔��a
#define NUM_COLLISION (10)				//�����蔻��̐�
#define ADD_MOVE_SPEED (0.8f)			//����
#define DEC_MOVE_SPEED_GROUND (0.3f)	//�n��ł̌���
#define DEC_MOVE_SPEED_AIR (0.05f)		//�󒆂ł̌���
#define MAX_MOVE_SPEED_WALK (3.0f)		//���s���x
#define MAX_MOVE_SPEED_DASH (6.0f)		//���s���x
#define MAX_MOVE_SPEED_AIR (2.0f)		//�󒆑��x
#define ROTATE_SPEED (0.1f)				//��]���x
#define DODGE_TIME_FRONT (10)			//�O���������
#define DODGE_TIME_BACK (6)				//����������
#define DODGE_TIME_SHEATH (4)			//�[���������
#define DODGE_TIME_DRAWN_SWORD (4)		//�����������
#define DODGE_SPEED_FRONT (18.0f)		//�O����𑬓x
#define DODGE_SPEED_BACK (14.0f)		//�����𑬓x
#define DODGE_SPEED_SHEATH (22.0f)		//�[����𑬓x
#define DODGE_SPEED_DRAWN_SWORD (20.0f)	//������𑬓x
#define INDEX_MODEL_PUNCH_1 (5)			//�p���`�U����(1)�̓����蔻�肪���郂�f���̃C���f�b�N�X
#define INDEX_MODEL_PUNCH_2 (3)			//�p���`�U����(2)�̓����蔻�肪���郂�f���̃C���f�b�N�X
#define MAX_LIFE (12)					//�̗͂̍ő�l
#define DANGER_LIFE (MAX_LIFE / 3)		//�̗͂̊댯�l
#define COLOR_LIFE_GAUGE_SAFE (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//�̗̓o�[�̈��S���̐F
#define COLOR_LIFE_GAUGE_DANGER (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//�̗̓o�[�̊댯���̐F
#define START_HEAL_STUN_TIME (60)	//�X�^���񕜊J�n����
#define END_STUN_TIME (600)			//�X�^�����Ă��鎞��
#define MAX_ITEM_OWN (10)			//�A�C�e�����L�����
#define SPAN_ITEM_USE_SOUND (40)	//�A�C�e���g�p���̉��̊Ԋu
#define TIME_ITEM_USE (120)			//�A�C�e���g�p��������
#define HEAL_VALUE_APPLE (6)		//�����S�A�C�e���̉񕜗�
#define COLOR_ITEM_DEFAULT (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//�A�C�e���J�E���^�̐F�̃f�t�H���g
#define COLOR_ITEM_NONE (D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f))		//�A�C�e���������̃J�E���^�̐F
#define COLOR_ITEM_MAX (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))		//�A�C�e��������̃J�E���^�̐F
#define COLOR_OUTLINE (D3DXCOLOR(0.2f, 0.5f, 1.0f, 1.0f))	//���f���̗֊s�̐F
#define DISTANCE_USE_RAFT (300.0f)			//�X�e�[�W�ړ��ł��锳�Ƃ̋���
#define CAMERA_DISTANCE (500.0f)			//�v���C���[����J�����̋���
#define CAMERA_ROTATE_SPEED_YAW (0.008f)	//�J�����̉�]���x(Y��)
#define CAMERA_ROTATE_SPEED_PITCH (0.006f)	//�J�����̉�]���x(X��)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CPlayer::m_nNumParts = 0;
CSceneMotion::PARTS_INFO* CPlayer::m_pPartsInfoArray = nullptr;
CSceneMotion::MOTION_INFO CPlayer::m_aMotionInfo[(int)CPlayer::MOTION_TYPE::ENUM_MAX] = {};
int CPlayer::m_nIdxWeapon = -1;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() : CSceneMotion(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, true)
{
	SetObjType(OBJ_TYPE::PLAYER);	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	SetUpdatePriority(UPDATE_PRIORITY::PLAYER);	//�X�V���̐ݒ�
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//�`�揇�̐ݒ�

	SetColorOutlineAll(COLOR_OUTLINE);	//�֊s�̐F�̐ݒ�

	m_lastPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_destRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bLand = false;
	m_bValidAttack = false;
	m_nNumKillEnemy = 0;
	m_bLockAct = false;
	m_nCntDodge = 0;
	m_nLife = MAX_LIFE;
	m_bStun = false;
	m_nCntStun = 0;

	//�̗̓Q�[�W�̐���
	m_pGaugeLife = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 550.0f, 20.0f);	//�Q�[�W
	CScene2D* pGaugeLifeBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 550.0f, 20.0f);			//�w�i
	CScene2D* pGaugeLifeFrame = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f), CTexture::TEXTURE_TYPE::GAUGE_LIFE_PLAYER, 600.0f, 30.0f);	//�g
	//�̗̓Q�[�W�̐ݒ� ���Ԍ���
	if (pGaugeLifeBg != nullptr) {	//�w�i
		pGaugeLifeBg->SetDrawPriority(DRAW_PRIORITY::UI);
		pGaugeLifeBg->SetColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
	}
	if (m_pGaugeLife != nullptr) {	//�Q�[�W
		m_pGaugeLife->SetDrawPriority(DRAW_PRIORITY::UI);
		m_pGaugeLife->SetColor(COLOR_LIFE_GAUGE_SAFE);
	}
	if (pGaugeLifeFrame != nullptr) {	//�g
		pGaugeLifeFrame->SetDrawPriority(DRAW_PRIORITY::UI);
	}

	//�A�C�e���̘g�̐���
	CScene2D* pFrameItem = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::ITEM_FRAME, 100.0f, 100.0f);	//�g
	CScene2D* pFrameItemBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 90.0f, 90.0f);		//�w�i
	CScene2D* pTextItem = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 125.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_ITEM, 100.0f, 30.0f);	//����
	CScene2D* pIconItem = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::ICON_APPLE, 75.0f, 75.0f);	//�A�C�e���̃A�C�R��

	m_bUseItem = false;
	m_nCntUseItem = 0;
	//�A�C�e���g�p�Q�[�W�̐���
	m_pGaugeUseItem = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 90.0f, 90.0f);
	//�`�揇�̐ݒ�
	//�A�C�e���̔w�i
	if (pFrameItemBg != nullptr) {
		pFrameItemBg->SetDrawPriority(DRAW_PRIORITY::UI);
		pFrameItemBg->SetColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
	}
	//�A�C�e���̃A�C�R��
	if (pIconItem != nullptr) {
		pIconItem->SetDrawPriority(DRAW_PRIORITY::UI);
	}
	//�A�C�e���g�p�Q�[�W
	if (m_pGaugeUseItem != nullptr) {
		m_pGaugeUseItem->SetDrawPriority(DRAW_PRIORITY::UI);
		m_pGaugeUseItem->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.2f, 0.5f));
		m_pGaugeUseItem->SetRatioHeight(0.0f);
	}
	//�A�C�e���̘g
	if (pFrameItem != nullptr) {
		pFrameItem->SetDrawPriority(DRAW_PRIORITY::UI);
	}
	//�A�C�e���̕���
	if (pTextItem != nullptr) {
		pTextItem->SetDrawPriority(DRAW_PRIORITY::UI);
	}

	//�A�C�e���J�E���^�[�̐���
	m_pCounterItem = CScore::Create(2, CTexture::TEXTURE_TYPE::NUMBER_004, D3DXVECTOR3(SCREEN_WIDTH - 20.0f, SCREEN_HEIGHT - 45.0f, 0.0f), 25.0f, 25.0f, 0);
	if (m_pCounterItem != nullptr) m_pCounterItem->SetColor(COLOR_ITEM_NONE);

	m_typeWeapon = WEAPONTYPE::PUNCH;
	m_pWeapon = nullptr;

	//����̘g�̐���
	CScene2D* pFrameWeapon = CScene2D::Create(D3DXVECTOR3(60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::WEAPON_FRAME, 100.0f, 100.0f);	//�g
	CScene2D* pFrameWeaponBg = CScene2D::Create(D3DXVECTOR3(60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 90.0f, 90.0f);			//�w�i
	CScene2D* pTextWeapon = CScene2D::Create(D3DXVECTOR3(60.0f, SCREEN_HEIGHT - 125.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_WEAPON, 100.0f, 30.0f);	//����
	m_pIconWeapon = CScene2D::Create(D3DXVECTOR3(60.0f, SCREEN_HEIGHT - 60.0f, 0.0f), CTexture::TEXTURE_TYPE::ICON_NONE, 75.0f, 75.0f);						//����̃A�C�R��
	//�`�揇�̐ݒ�
	//����̔w�i
	if (pFrameWeaponBg != nullptr) {
		pFrameWeaponBg->SetDrawPriority(DRAW_PRIORITY::UI);
		pFrameWeaponBg->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	}
	//����̃A�C�R��
	if (m_pIconWeapon != nullptr) {
		m_pIconWeapon->SetDrawPriority(DRAW_PRIORITY::UI);
	}
	//����̘g
	if (pFrameWeapon != nullptr) {
		pFrameWeapon->SetDrawPriority(DRAW_PRIORITY::UI);
	}
	//����̕���
	if (pTextWeapon != nullptr) {
		pTextWeapon->SetDrawPriority(DRAW_PRIORITY::UI);
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// �v���C���[�̐�������
//=============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos) {
	CPlayer* pPlayer;
	pPlayer = new CPlayer();
	if (pPlayer != nullptr) {
		pPlayer->CSceneMotion::SetPos(pos);
		pPlayer->Init();
	}

	return pPlayer;
}

//=============================================================================
// ���[�V�����������[�h
//=============================================================================
void CPlayer::LoadMotionInfo(void) {
	//���[�h����O�ɓ��I�m�ۂ�����������delete����
	UnloadMotionInfo();

	Load(TEXT_FILE_NAME_LOAD_MOTION, &m_pPartsInfoArray, &m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, &m_nIdxWeapon);
}

//=============================================================================
// ���[�V���������A�����[�h
//=============================================================================
void CPlayer::UnloadMotionInfo(void) {
	m_nNumParts = 0;
	m_nIdxWeapon = -1;

	//�p�[�c���̉��
	if (m_pPartsInfoArray != nullptr) {
		delete[] m_pPartsInfoArray;
		m_pPartsInfoArray = nullptr;
	}
	//���[�V�������̉��
	for (int nCntMotionInfo = 0; nCntMotionInfo < (int)MOTION_TYPE::ENUM_MAX; nCntMotionInfo++)
	{
		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
		{
			if (m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray != nullptr) {
				delete[] m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray;
				m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray = nullptr;
			}
		}
	}
}

//=============================================================================
// �v���C���[�̏���������
//=============================================================================
HRESULT CPlayer::Init(void) {
	//�v���C���[�̏����ݒ�
	CSceneMotion::SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	m_destRot.y =  D3DX_PI;	//������

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�J�����̎擾
	CCamera* pCamera = nullptr;
	if (pManager != nullptr) pCamera = pManager->GetCamera();

	//�J�����̏����ݒ�
	if (pCamera != nullptr) {
		pCamera->SetPos(CSceneMotion::GetPos());
		pCamera->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, D3DX_PI, 0.0f));
		pCamera->SetDistance(CAMERA_DISTANCE);
	}

	CSceneMotion::Init();
	SetMotion((int)MOTION_TYPE::NEUTRAL);	//���[�V�����̏����ݒ�

	return S_OK;
}

//=============================================================================
// �v���C���[�̏I������
//=============================================================================
void CPlayer::Uninit(void) {
	//����̔j��
	if (m_pWeapon != nullptr) {
		m_pWeapon->Uninit();
		m_pWeapon = nullptr;
	}
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�Q�[���̎擾
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//�Q�[�����ێ����Ă���v���C���[�̃|�C���^��nullptr�ɂ���
	if (pGame != nullptr) pGame->ReleasePlayer();
	//�I������
	CSceneMotion::Uninit();
}

//=============================================================================
// �v���C���[�̍X�V����
//=============================================================================
void CPlayer::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	CInput* pInput = nullptr;
	CSound *pSound = nullptr;
	CCamera* pCamera = nullptr;
	CGame* pGame = nullptr;

	if (pManager != nullptr) {
		//���݂̓��̓f�o�C�X�̎擾
		pInput = pManager->GetInputCur();
		//�T�E���h�̎擾
		pSound = pManager->GetSound();
		//�J�����̎擾
		pCamera = pManager->GetCamera();
		//�Q�[���̎擾
		pGame = pManager->GetGame();
	}

	//�ʒu���̃|�C���^�̎擾
	D3DXVECTOR3* pPosPlayer = GetPtrPos();

	//----------------------------
	//�Q�[���N���A�����X�e�[�W�ύX��
	//----------------------------
	if (pGame != nullptr) {
		if (pGame->GetGameClear() || pGame->GetChangeStage()) {
			//���[�V�����̍X�V
			CSceneMotion::Update();
			return;
		}
	}

	//----------------------------
	//�J�����̐ݒ�
	//----------------------------
	float fRotCameraY = 0.0f;	//�J�����̊p�x
	if (pCamera != nullptr) {
		D3DXVECTOR3 posCamera = CSceneMotion::GetPos();
		posCamera.y += 50.0f;
		//�J�����̈ʒu���X�V
		pCamera->SetPos(posCamera);

		D3DXVECTOR3 rotCamera = pCamera->GetRot();
		if (pInput != nullptr) {
			if (pInput->GetPress(CInput::CODE::CAMERA_UP)) {
				//���_��������
				rotCamera.x -= CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
				if (rotCamera.x < -0.05f * D3DX_PI) {
					rotCamera.x = -0.05f * D3DX_PI;
				}
			}
			if (pInput->GetPress(CInput::CODE::CAMERA_DOWN)) {
				//���_���グ��
				rotCamera.x += CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
				if (rotCamera.x > 0.25f * D3DX_PI) {
					rotCamera.x = 0.25f * D3DX_PI;
				}
			}
			if (pInput->GetPress(CInput::CODE::CAMERA_LEFT)) {
				//���_������]
				rotCamera.y -= CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
				if (rotCamera.y < -D3DX_PI) {
					rotCamera.y = rotCamera.y + D3DX_PI * 2;
				}
			}
			if (pInput->GetPress(CInput::CODE::CAMERA_RIGHT)) {
				//���_���E��]
				rotCamera.y += CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
				if (rotCamera.y > D3DX_PI) {
					rotCamera.y = rotCamera.y - D3DX_PI * 2;
				}
			}
		}
		pCamera->SetRot(rotCamera);

		fRotCameraY = pCamera->GetRot().y;	//�J�����̊p�x���擾
		fRotCameraY += D3DX_PI;	//�p�x���t�ɂ���
	}


	//----------------------------
	//�X�^���� (�Q�[���I�[�o�[���܂�)
	//----------------------------
	if (m_bStun) {
		if (pGame != nullptr) {
			//�Q�[���I�[�o�[���Ȃ玀�S���[�V�����̂܂ܓ����Ȃ�
			if (pGame->GetGameOver()) {
				//���[�V�����̍X�V
				CSceneMotion::Update();
				return;
			}
		}

		//�X�^���J�E���^�̉��Z
		m_nCntStun++;

		//�̗̓Q�[�W�̉�
		if (m_pGaugeLife != nullptr && m_nCntStun >= START_HEAL_STUN_TIME && m_nCntStun <= END_STUN_TIME) m_pGaugeLife->SetRatioWidth((float)(m_nCntStun - START_HEAL_STUN_TIME) / (float)(END_STUN_TIME - START_HEAL_STUN_TIME));

		//�X�^���񕜊J�n����
		if (m_nCntStun == START_HEAL_STUN_TIME) {
			//�C�₷�鉹
			if (pSound != nullptr)pSound->PlaySound(CSound::SOUND_LABEL::STUN);
		}

		//�X�^���񕜏I����
		if (m_nCntStun == END_STUN_TIME) {
			//�̗͂̃��Z�b�g
			m_nLife = MAX_LIFE;
			//���[�V�����̐ݒ�
			SetMotion((int)MOTION_TYPE::NEUTRAL);
		}
		//�j���[�g�������[�V�����ւ̈ڍs���I������ꍇ
		if (GetMotionType() != (int)MOTION_TYPE::STUN && !GetTransMotion()) {
			//�X�^�������A���̂܂܍X�V
			m_bStun = false;
			return;
		}
		else {
			//���[�V�����̍X�V
			CSceneMotion::Update();
			return;
		}
	}

	//----------------------------
	//�d����
	//----------------------------
	if (m_bLockAct) {
		//���[�V�����̑J�ڂ����������ꍇ
		if (!GetTransMotion()) {
			//�d���̉���
			m_bLockAct = false;
		}
	}

	//----------------------------
	//���
	//----------------------------
	if (GetMotionCategory() == MOTION_CATEGORY::DODGE) {
		m_nCntDodge--;

		//�������
		if (m_nCntDodge <= 0) {
			//�j���[�g�������[�V������ݒ�
			SetMotion((int)MOTION_TYPE::NEUTRAL);
			//�d����Ԃɂ���
			m_bLockAct = true;

			if (m_bLand) {
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);	//�傫���d�͂�������
			}
			else {
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY, 0.0f);	//�d�͂�������
			}
		}
		else {
			//�p�[�e�B�N���̐���
			D3DXVECTOR3 posParticle = *pPosPlayer;	//�p�[�e�B�N���̈ʒu
			posParticle.y += 10.0f;
			CParticle::Create(posParticle, 20, 25.0f, -0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}
	}

	//----------------------------
	//�U�����̈ړ�
	//----------------------------
	if ((GetMotionType() == (int)MOTION_TYPE::SHEATH || GetMotionType() == (int)MOTION_TYPE::ATTACK_KATANA_DRAW) && m_nCntAttackMove > 0) {
		m_nCntAttackMove--;

		//�ړ��I��
		if (m_nCntAttackMove <= 0) {
			if (m_bLand) {
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);	//�傫���d�͂�������
			}
			else {
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY, 0.0f);	//�d�͂�������
			}
		}
	}

	if (pInput != nullptr) {
		//----------------------------
		//�U��
		//----------------------------
		if (pInput->GetTrigger(CInput::CODE::ATTACK) && GetMotionCategory() != MOTION_CATEGORY::DODGE && !m_bLockAct) {
			//�܂��U�����Ă��Ȃ��ꍇ
			if (GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
				//���[�V�����̐ݒ�
				switch (m_typeWeapon)
				{
				case WEAPONTYPE::PUNCH:
					SetMotion((int)MOTION_TYPE::PUNCH_1);
					break;
				case WEAPONTYPE::SWORD:
					SetMotion((int)MOTION_TYPE::ATTACK_SWORD_1);
					break;
				case WEAPONTYPE::KATANA:
					SetMotion((int)MOTION_TYPE::ATTACK_KATANA_1);
					break;
				}
				//�ړ��ʂ��d�݂͂̂ɂ���
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			}
			//�U�����̏ꍇ
			else {
				switch (m_typeWeapon)
				{
				case WEAPONTYPE::PUNCH:
					//1��ڂ̍U���̍Ō�̃L�[�̏ꍇ
					if (GetMotionType() == (int)MOTION_TYPE::PUNCH_1 && (GetCurrentKey() == 1 || GetCurrentKey() == 2)) {
						//���[�V�����̐ݒ�
						SetMotion((int)MOTION_TYPE::PUNCH_2);
					}
					break;
				case WEAPONTYPE::SWORD:
					//1��ڂ̍U���̍Ō�̃L�[�̏ꍇ
					if (GetMotionType() == (int)MOTION_TYPE::ATTACK_SWORD_1 && GetCurrentKey() == 2) {
						//���[�V�����̐ݒ�
						SetMotion((int)MOTION_TYPE::ATTACK_SWORD_2);
					}
					//2��ڂ̍U���̍Ō�̃L�[�̏ꍇ
					else if (GetMotionType() == (int)MOTION_TYPE::ATTACK_SWORD_2 && GetCurrentKey() == 2) {
						//���[�V�����̐ݒ�
						SetMotion((int)MOTION_TYPE::ATTACK_SWORD_3);
					}
					break;
				case WEAPONTYPE::KATANA:
					//1��ڂ̍U���̍Ō�̃L�[�̏ꍇ
					if (GetMotionType() == (int)MOTION_TYPE::ATTACK_KATANA_1 && GetCurrentKey() == 3) {
						//���[�V�����̐ݒ�
						SetMotion((int)MOTION_TYPE::ATTACK_KATANA_2);
					}
					//2��ڂ̍U���̍Ō�̃L�[�̏ꍇ
					else if (GetMotionType() == (int)MOTION_TYPE::ATTACK_KATANA_2 && GetCurrentKey() == 3) {
						//���[�V�����̐ݒ�
						SetMotion((int)MOTION_TYPE::ATTACK_KATANA_3);
					}
					//�[�����̍Ō�̃L�[�̏ꍇ
					else if (GetMotionType() == (int)MOTION_TYPE::SHEATH && GetCurrentKey() == 2) {
						//���[�V�����̐ݒ�
						SetMotion((int)MOTION_TYPE::ATTACK_KATANA_DRAW);
						//������Ԃ̐ݒ�
						m_nCntAttackMove = DODGE_TIME_DRAWN_SWORD;
						//��𑬓x�̐ݒ�
						float rotY = GetRot().y;
						rotY += D3DX_PI;	//�p�x��O���ɂ���
						m_move = D3DXVECTOR3(DODGE_SPEED_DRAWN_SWORD * sinf(rotY), 0.0f, DODGE_SPEED_DRAWN_SWORD * cosf(rotY));	//�ړ��ʂ�O���ɂ���
					}
					break;
				}
			}
		}

		//----------------------------
		//�[�����̗֊s�\��
		//----------------------------
		if (GetMotionType() == (int)MOTION_TYPE::SHEATH && GetCurrentKey() == 2) {
			//�����U���\���͗֊s�\��
			if (m_pWeapon != nullptr) m_pWeapon->SetDrawOutline(true);
		}
		else {
			//�֊s��\��
			if (m_pWeapon != nullptr) m_pWeapon->SetDrawOutline(false);
		}

		//----------------------------
		//����̕ύX
		//----------------------------
		if (pInput->GetTrigger(CInput::CODE::CHANGE_WEAPON) && GetMotionCategory() != MOTION_CATEGORY::ATTACK && !m_bLockAct && GetMotionCategory() != MOTION_CATEGORY::DODGE) {
			int nTypeWeapon = (int)m_typeWeapon;	//����̎��
			nTypeWeapon++;	//��ނ����ɐi�߂�
			if (nTypeWeapon >= (int)WEAPONTYPE::ENUM_MAX) nTypeWeapon = 0;	//��ސ��̍ő�܂ōs������O�ɖ߂�
			//�����ύX
			ChangeWeaponType((WEAPONTYPE)nTypeWeapon);
		}

		//----------------------------
		//�o�b�N�X�e�b�v���
		//----------------------------
		if (pInput->GetTrigger(CInput::CODE::DODGE) && GetMotionCategory() == MOTION_CATEGORY::ATTACK) {
			bool bDodge = false;
			switch (m_typeWeapon)
			{
			case WEAPONTYPE::PUNCH:
				//1��ڂ̍U���̍Ō�̃L�[�̏ꍇ
				if (GetMotionType() == (int)MOTION_TYPE::PUNCH_1 && (GetCurrentKey() == 1 || GetCurrentKey() == 2)) {
					bDodge = true;
				}
				//1��ڂ̍U���̍Ō�̃L�[�̏ꍇ
				if (GetMotionType() == (int)MOTION_TYPE::PUNCH_2 && GetCurrentKey() == 2) {
					bDodge = true;
				}
				break;
			case WEAPONTYPE::SWORD:
				//1��ڂ̍U���̍Ō�̃L�[�̏ꍇ
				if (GetMotionType() == (int)MOTION_TYPE::ATTACK_SWORD_1 && GetCurrentKey() == 2) {
					bDodge = true;
				}
				//2��ڂ̍U���̍Ō�̃L�[�̏ꍇ
				else if (GetMotionType() == (int)MOTION_TYPE::ATTACK_SWORD_2 && GetCurrentKey() == 2) {
					bDodge = true;
				}
				break;
			}
			//���
			if (bDodge) {
				m_nCntDodge = DODGE_TIME_BACK;
				//���������[�V�����̐ݒ�
				SetMotion((int)MOTION_TYPE::DODGE_BACK);
				//�p�x�̎擾
				float rotY = GetRot().y;
				//�ړ��ʂ�����ɂ���
				m_move = D3DXVECTOR3(DODGE_SPEED_BACK * sinf(rotY), -POWER_GRAVITY_GROUND, DODGE_SPEED_BACK * cosf(rotY));
				//������̍Đ�
				if (pSound != nullptr)pSound->PlaySound(CSound::SOUND_LABEL::DODGE);
			}
		}

		//----------------------------
		//�[��
		//----------------------------
		if (pInput->GetTrigger(CInput::CODE::DODGE) && (GetMotionType() == (int)MOTION_TYPE::ATTACK_KATANA_1 || GetMotionType() == (int)MOTION_TYPE::ATTACK_KATANA_2) && GetCurrentKey() == 3) {
			m_nCntAttackMove = DODGE_TIME_SHEATH;
			//�[�����[�V�����̐ݒ�
			SetMotion((int)MOTION_TYPE::SHEATH);
			//�p�x�̎擾
			float rotY = GetRot().y;
			//�ړ��ʂ�����ɂ���
			m_move = D3DXVECTOR3(DODGE_SPEED_SHEATH * sinf(rotY), -POWER_GRAVITY_GROUND, DODGE_SPEED_SHEATH * cosf(rotY));
			//������̍Đ�
			if (pSound != nullptr)pSound->PlaySound(CSound::SOUND_LABEL::DODGE);
		}

		//----------------------------
		//���
		//----------------------------
		if (GetMotionCategory() != MOTION_CATEGORY::ATTACK  && GetMotionCategory() != MOTION_CATEGORY::DODGE && !m_bLockAct) {
			if (m_bLand && pInput->GetTrigger(CInput::CODE::DODGE)) {
				m_nCntDodge = DODGE_TIME_FRONT;
				float rotY = GetRot().y;
				rotY += D3DX_PI;	//�p�x��O���ɂ���
				m_move = D3DXVECTOR3(DODGE_SPEED_FRONT * sinf(rotY), -POWER_GRAVITY_GROUND, DODGE_SPEED_FRONT * cosf(rotY));	//�ړ��ʂ�O���ɂ���
				SetMotion((int)MOTION_TYPE::DODGE_FRONT);	//�O��������[�V����
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::DODGE);	//������̍Đ�
			}
		}

		//----------------------------
		//�A�C�e���g�p
		//----------------------------
		if (m_pCounterItem != nullptr) {
			if (pInput->GetTrigger(CInput::CODE::USE_ITME) && (m_pCounterItem->GetScore() <= 0 || m_nLife >= MAX_LIFE)) {
				//�A�C�e���g�p�s���̍Đ�
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CANSEL);
			}

			if (GetMotionCategory() != MOTION_CATEGORY::ATTACK && GetMotionCategory() != MOTION_CATEGORY::DODGE && !m_bLockAct) {
				//�A�C�e���g�p�J�n
				if (pInput->GetTrigger(CInput::CODE::USE_ITME) && m_pCounterItem->GetScore() > 0 && m_nLife < MAX_LIFE) {
					m_bUseItem = true;
					m_nCntUseItem = 0;
				}
			}

			//�A�C�e���g�p��
			if (m_bUseItem) {
				//�A�C�e���g�p�p��
				if (pInput->GetPress(CInput::CODE::USE_ITME) && GetMotionCategory() != MOTION_CATEGORY::ATTACK && GetMotionCategory() != MOTION_CATEGORY::DODGE && !m_bLockAct) {
					if (m_nCntUseItem % SPAN_ITEM_USE_SOUND == 0) {
						//�A�C�e���Q�[�W�i�s���̍Đ�
						if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::GAUGE);
					}

					//�J�E���^�̉��Z
					m_nCntUseItem++;

					//�A�C�e���g�p����
					if (m_nCntUseItem >= TIME_ITEM_USE) {
						m_bUseItem = false;
						m_nCntUseItem = 0;

						//�A�C�e���̌���
						m_nLife += HEAL_VALUE_APPLE;	//�̗͂���
						if (m_nLife > MAX_LIFE) {
							m_nLife = MAX_LIFE;
						}
						if (m_pGaugeLife != nullptr) {
							//�̗̓Q�[�W�̊����̍X�V
							m_pGaugeLife->SetRatioWidth((float)m_nLife / (float)MAX_LIFE);
							//�̗͂̐F�̕ύX
							if (m_nLife >= DANGER_LIFE) m_pGaugeLife->SetColor(COLOR_LIFE_GAUGE_SAFE);
						}

						//���L����܂ŃA�C�e���������Ă����ꍇ
						if (m_pCounterItem->GetScore() == MAX_ITEM_OWN) {
							//�F�̕ύX
							m_pCounterItem->SetColor(COLOR_ITEM_DEFAULT);
						}
						//�A�C�e���̎c�ʂ����炷
						m_pCounterItem->AddScore(-1);
						//�A�C�e�����g���؂����ꍇ
						if (m_pCounterItem->GetScore() == 0) {
							//�F�̕ύX
							m_pCounterItem->SetColor(COLOR_ITEM_NONE);
						}

						if (pSound != nullptr) {
							//�A�C�e���Q�[�W�i�s���̒�~
							pSound->StopSound(CSound::SOUND_LABEL::GAUGE);
							//�A�C�e���g�p���̍Đ�
							pSound->PlaySound(CSound::SOUND_LABEL::HEAL);
						}

						//�p�[�e�B�N���G�t�F�N�g�̐���
						D3DXVECTOR3 posParticle = *pPosPlayer;	//�p�[�e�B�N���̈ʒu
						posParticle.y += 10.0f;	//�ʒu�̒���
						//�p�[�e�B�N���̏��
						CParticleEffect::PARTICLE_INFO particleInfo = { 30, 50.0f, -1.5f, 9.0f, D3DXVECTOR3(0.0f, -0.6f, 0.0f), D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f), D3DXCOLOR(0.8f, 1.0f, 0.2f, 1.0f) };
						//�G�t�F�N�g�̐���
						CParticleEffect::EmitCircle(particleInfo, posParticle, 12, 0.1f * D3DX_PI);
					}
					//�A�C�e���g�p�Q�[�W�̍X�V
					if (m_pGaugeUseItem != nullptr) m_pGaugeUseItem->SetRatioHeight((float)m_nCntUseItem / (float)TIME_ITEM_USE);
				}
				//�A�C�e���g�p���f
				else {
					m_bUseItem = false;
					m_nCntUseItem = 0;
					//�A�C�e���g�p�Q�[�W�̍X�V
					if (m_pGaugeUseItem != nullptr) m_pGaugeUseItem->SetRatioHeight(0.0f);
					//�A�C�e���Q�[�W�i�s���̒�~
					if (pSound != nullptr) pSound->StopSound(CSound::SOUND_LABEL::GAUGE);
				}
			}
		}
	}

	//----------------------------
	//�ړ�
	//----------------------------
	//����
	if (!m_bLand && GetMotionCategory() != MOTION_CATEGORY::DODGE) {
		m_move.y -= POWER_GRAVITY;	//�d�͂����Z
	}

	if (pInput != nullptr && GetMotionCategory() != MOTION_CATEGORY::ATTACK && GetMotionCategory() != MOTION_CATEGORY::DODGE && !m_bLockAct) {
		//�㉺���E�L�[���͏�Ԃ̎擾
		const bool bPressUp = pInput->GetPress(CInput::CODE::UP);
		const bool bPressDown = pInput->GetPress(CInput::CODE::DOWN);
		const bool bPressLeft = pInput->GetPress(CInput::CODE::LEFT);
		const bool bPressRight = pInput->GetPress(CInput::CODE::RIGHT);

		bool bDiagonalMove = (bPressUp != bPressDown) && (bPressLeft != bPressRight);	//�΂߈ړ�
		bool bRotateUp, bRotateDown, bRotateLeft, bRotateRight;	//��]�������
		bRotateUp = bRotateDown = bRotateLeft = bRotateRight = false;

		D3DXVECTOR3 moveAddSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ǉ�����ړ���
		D3DXVECTOR3 moveMaxSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ǉ�����ړ��ʂ̍ő�

		//------------------------
		//�ړ����x�̍ő���v�Z
		//------------------------
		float fMaxSpeed;	//�ړ����x�̍ő�
		//�n��
		if (pInput->GetPress(CInput::CODE::DASH)) {
			fMaxSpeed = MAX_MOVE_SPEED_DASH;
		}
		else {
			fMaxSpeed = MAX_MOVE_SPEED_WALK;
		}
		//��
		if (!m_bLand) {
			fMaxSpeed = MAX_MOVE_SPEED_AIR;
		}

		//------------------------
		//�ړ��ʂ̌���
		//------------------------
		//�O��ړ��̂ǂ��炩�̂݉�����Ă���ꍇ
		if (bPressUp != bPressDown) {
			if (bPressUp) {
				moveAddSpeed.z = ADD_MOVE_SPEED;
				moveMaxSpeed.z = fMaxSpeed;
				bRotateUp = true;
			}
			else if (bPressDown) {
				moveAddSpeed.z = -ADD_MOVE_SPEED;
				moveMaxSpeed.z = -fMaxSpeed;
				bRotateDown = true;
			}
			//�΂߈ړ�
			if (bDiagonalMove) {
				moveAddSpeed.z *= 0.7f;
				moveMaxSpeed.z *= 0.7f;
			}
		}
		//���E�ړ��̂ǂ��炩�̂݉�����Ă���ꍇ
		if (bPressLeft != bPressRight) {
			if (bPressLeft) {
				moveAddSpeed.x = -ADD_MOVE_SPEED;
				moveMaxSpeed.x = -fMaxSpeed;
				bRotateLeft = true;
			}
			else if (bPressRight) {
				moveAddSpeed.x = ADD_MOVE_SPEED;
				moveMaxSpeed.x = fMaxSpeed;
				bRotateRight = true;
			}
			//�΂߈ړ�
			if (bDiagonalMove) {
				moveAddSpeed.x *= 0.7f;
				moveMaxSpeed.x *= 0.7f;
			}
		}

		//------------------------
		//�J�����̊p�x�ɍ��킹�Ĉړ��ʂ̍ő�ʂ�ݒ�
		//------------------------
		D3DXVECTOR3 moveMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ��̍ő��
		moveMax.x += moveMaxSpeed.x * sinf(fRotCameraY + 0.5f * D3DX_PI);
		moveMax.x += moveMaxSpeed.z * sinf(fRotCameraY);

		moveMax.z += moveMaxSpeed.x * -sinf(fRotCameraY);
		moveMax.z += moveMaxSpeed.z * cosf(fRotCameraY);

		//------------------------
		//����
		//------------------------
		//X�����̈ړ��ʂ��ő�ʖ����̏ꍇ
		if ((moveMax.x > 0 && m_move.x < moveMax.x) || (moveMax.x < 0 && m_move.x > moveMax.x)) {
			//�J�����̊p�x�ɍ��킹�Ĉړ��ʂ�ǉ�
			m_move.x += moveAddSpeed.x * sinf(fRotCameraY + 0.5f * D3DX_PI);
			m_move.x += moveAddSpeed.z * sinf(fRotCameraY);
			//�������Ɉړ��ʂ��ő�𒴂����ꍇ
			if (moveMax.x > 0) {
				if (m_move.x > moveMax.x) {
					m_move.x = moveMax.x;
				}
			}
			else if (moveMax.x < 0) {
				if (m_move.x < moveMax.x) {
					m_move.x = moveMax.x;
				}
			}
		}
		//Z�����̈ړ��ʂ��ő�ʖ����̏ꍇ
		if ((moveMax.z > 0 && m_move.z < moveMax.z) || (moveMax.z < 0 && m_move.z > moveMax.z)) {
			//�J�����̊p�x�ɍ��킹�Ĉړ��ʂ�ǉ�
			m_move.z += moveAddSpeed.x * -sinf(fRotCameraY);
			m_move.z += moveAddSpeed.z * cosf(fRotCameraY);
			//�������Ɉړ��ʂ��ő�𒴂����ꍇ
			if (moveMax.z > 0) {
				if (m_move.z > moveMax.z) {
					m_move.z = moveMax.z;
				}
			}
			else if (moveMax.z < 0) {
				if (m_move.z < moveMax.z) {
					m_move.z = moveMax.z;
				}
			}
		}

		//------------------------
		//�ړ��ʂ̌���
		//------------------------
		float fDecSpeed = DEC_MOVE_SPEED_GROUND;	//������
		if (!m_bLand) fDecSpeed = DEC_MOVE_SPEED_AIR;	//��
		D3DXVECTOR2 vecMoveDec = D3DXVECTOR2(-m_move.x, -m_move.z);	//�ړ��ʃx�N�g���̋t
		D3DXVec2Normalize(&vecMoveDec, &vecMoveDec);	//���K��
		vecMoveDec *= fDecSpeed;	//�ړ��ʂ̉��Z

		//����
		m_move.x += vecMoveDec.x;
		m_move.z += vecMoveDec.y;

		//X�����̈ړ��ʂ̒�~
		if (vecMoveDec.x > 0.0f) {
			if (m_move.x > 0.0f) {
				m_move.x = 0.0f;
			}
		}
		else if (vecMoveDec.x < 0.0f) {
			if (m_move.x < 0.0f) {
				m_move.x = 0.0f;
			}
		}
		//Z�����̈ړ��ʂ̒�~
		if (vecMoveDec.y > 0.0f) {
			if (m_move.z > 0.0f) {
				m_move.z = 0.0f;
			}
		}
		else if (vecMoveDec.x < 0.0f) {
			if (m_move.z < 0.0f) {
				m_move.z = 0.0f;
			}
		}

		//------------------------
		//��]�����̌���
		//------------------------		
		bool bInputMove = false;	//�ړ��L�[�������Ă��邩�ǂ���
		bInputMove = bRotateUp || bRotateDown || bRotateLeft || bRotateRight;	//�ǂꂩ��������Ă���ꍇ
		//�ړ��L�[������
		if (bInputMove) {
			//�L�[�������Ă���������ڕW�̊p�x�ɂ���
			//��
			if (bRotateUp) {
				if (bRotateRight) {
					m_destRot.y = D3DX_PI * -0.75f + fRotCameraY;
				}
				else if (bRotateLeft) {
					m_destRot.y = D3DX_PI * 0.75f + fRotCameraY;
				}
				else {
					m_destRot.y = D3DX_PI + fRotCameraY;
				}
			}
			//��
			else if (bRotateDown) {
				if (bRotateRight) {
					m_destRot.y = D3DX_PI * -0.25f + fRotCameraY;
				}
				else if (bRotateLeft) {
					m_destRot.y = D3DX_PI * 0.25f + fRotCameraY;
				}
				else {
					m_destRot.y = 0.0f + fRotCameraY;
				}
			}
			//���E
			else {
				if (bRotateRight) {
					m_destRot.y = D3DX_PI * -0.5f + fRotCameraY;
				}
				else if (bRotateLeft) {
					m_destRot.y = D3DX_PI * 0.5f + fRotCameraY;
				}
			}

			//�p�C���ߎ�
			if (m_destRot.y > D3DX_PI) {
				m_destRot.y = -D3DX_PI * 2 + m_destRot.y;
			}
			else if (m_destRot.y < -D3DX_PI) {
				m_destRot.y = D3DX_PI * 2 + m_destRot.y;
			}

			D3DXVECTOR3 rotPlayer = CSceneMotion::GetRot();//�p�x�̎擾
			float fdeltaRot;	//�p�x�̍���
			//���݂̊p�x�ƖڕW�̊p�x�̍����̌v�Z
			if (m_destRot.y >= 0.0f) {
				if (rotPlayer.y >= 0.0f) {
					fdeltaRot = m_destRot.y - rotPlayer.y;
				}
				else if (rotPlayer.y < 0.0f) {
					if (m_destRot.y - rotPlayer.y >= D3DX_PI) {
						fdeltaRot = -D3DX_PI - rotPlayer.y - D3DX_PI + m_destRot.y;
					}
					else if (m_destRot.y - rotPlayer.y < D3DX_PI) {
						fdeltaRot = m_destRot.y - rotPlayer.y;
					}
				}
			}
			else if (m_destRot.y < 0.0f) {
				if (rotPlayer.y >= 0.0f) {
					if (rotPlayer.y - m_destRot.y >= D3DX_PI) {
						fdeltaRot = D3DX_PI - rotPlayer.y + D3DX_PI + m_destRot.y;
					}
					else if (rotPlayer.y - m_destRot.y < D3DX_PI) {
						fdeltaRot = m_destRot.y - rotPlayer.y;
					}
				}
				else if (rotPlayer.y < 0.0f) {
					fdeltaRot = m_destRot.y - rotPlayer.y;
				}
			}

			//��]�̔��f
			rotPlayer.y += fdeltaRot * ROTATE_SPEED;

			//�p�C���ߎ�
			if (rotPlayer.y > D3DX_PI) {
				rotPlayer.y = -D3DX_PI;
			}
			else if (rotPlayer.y < -D3DX_PI) {
				rotPlayer.y = D3DX_PI;
			}

			//�p�x�̐ݒ�
			CSceneMotion::SetRot(rotPlayer);
		}

		//------------------------
		//�n��ɂ��鎞�̃��[�V�����؂�ւ�
		//------------------------
		if (m_bLand) {
			//�ړ��L�[�������Ă��鎞�̃��[�V�����؂�ւ�
			if (bInputMove) {
				if (GetMotionCategory() != MOTION_CATEGORY::MOVE) {
					SetMotion((int)MOTION_TYPE::WALK);
				}
				//�������[�V�����Ƒ��胂�[�V�����̐؂�ւ�
				if (GetMotionCategory() == MOTION_CATEGORY::MOVE) {
					if (GetMotionType() == (int)MOTION_TYPE::WALK && pInput->GetPress(CInput::CODE::DASH)) {
						SetMotion((int)MOTION_TYPE::DASH);
					}
					else if (GetMotionType() == (int)MOTION_TYPE::DASH && !pInput->GetPress(CInput::CODE::DASH)) {
						SetMotion((int)MOTION_TYPE::WALK);
					}
				}
			}
			//�ړ��L�[�������Ă��Ȃ��ꍇ�̃��[�V�����؂�ւ�
			if (!bInputMove) {
				//���[�V�����̐ݒ�
				if (GetMotionCategory() == MOTION_CATEGORY::MOVE) {
					SetMotion((int)MOTION_TYPE::NEUTRAL);
				}
			}
		}
	} //pInput


	//----------------------------
	//�ړ��̔��f
	//----------------------------
	//�Ō�̈ʒu���W�̕ۑ�
	m_lastPos = *pPosPlayer;
	//�ړ�
	*pPosPlayer += m_move;

	//----------------------------
	//�����蔻��
	//----------------------------
	//�A�C�e���Ƃ̓����蔻��
	CItem::Collision(this);
	//���S�t���O���������ꍇ�I��
	if (GetDeath()) return;

	//�ǂƂ̓����蔻��
	CWallCylinder::Collision(pPosPlayer, m_lastPos, COLLISION_RADIUS);

	//�n�ʂƂ̓����蔻��
	bool bLand = false;	//�ڒn���Ă��邩�ǂ���
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//�ڐG�ʒu�A�J�n�x�N�g���A�I���x�N�g��
	vecStart = *pPosPlayer;	//�v���C���[�̈ʒu���擾
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//������̃x�N�g��

	bLand = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);

	//�ڒn��
	if (bLand) {
		*pPosPlayer = posColTerrain;	//�ʒu�̈ړ�
		//���n
		if (!m_bLand) {
			m_bLand = true;
		}
	}
	//����
	else if (!bLand) {
		//�n�ォ���
		if (m_bLand) {
			m_bLand = false;
		}
	}

	//----------------------------
	//���[�V�����̍X�V
	//----------------------------
	CSceneMotion::Update();

	//----------------------------
	//���[�V�����I����
	//----------------------------
	if (GetEndMotion()) {
		switch ((MOTION_TYPE)GetMotionType())
		{
			//--------------------------
			//�U�����[�V����
			//--------------------------
			//�d������̃��[�V����
		case MOTION_TYPE::ATTACK_SWORD_1:
		case MOTION_TYPE::ATTACK_SWORD_2:
		case MOTION_TYPE::ATTACK_SWORD_3:
		case MOTION_TYPE::ATTACK_KATANA_1:
		case MOTION_TYPE::ATTACK_KATANA_2:
		case MOTION_TYPE::ATTACK_KATANA_3:
		case MOTION_TYPE::ATTACK_KATANA_DRAW:
		case MOTION_TYPE::SHEATH:
			m_bLockAct = true;	//�d���̐ݒ�

			//�d���Ȃ��̃��[�V����
		case MOTION_TYPE::PUNCH_1:
		case MOTION_TYPE::PUNCH_2:
			SetMotion((int)MOTION_TYPE::NEUTRAL);
			//�ړ��ʂ��d�݂͂̂ɂ���
			m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//������U�����Ă��Ȃ���Ԃɂ���
				m_pWeapon->SetAttack(false);
			}
			break;
		}
	}

	//----------------------------
	//���[�V�������̏���
	//----------------------------
	int nDamageAttack = 0;		//�U���̃_���[�W��
	bool bBeginAttack = false;	//�U���J�n�����ǂ���

	switch ((MOTION_TYPE)GetMotionType())
	{
		//-------------------------
		//�p���`�̍U��
		//-------------------------
		//�p���`�̍U�� 1
	case MOTION_TYPE::PUNCH_1:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 1) && !GetTransMotion()) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 1;
		}
		else {
			m_bValidAttack = false;
		}
		break;

		//�p���`�̍U�� 2
	case MOTION_TYPE::PUNCH_2:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 1) && !GetTransMotion()) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 2;
		}
		else {
			m_bValidAttack = false;
		}
		break;

		//-------------------------
		//���̍U��
		//-------------------------
		//���̍U�� 1
	case MOTION_TYPE::ATTACK_SWORD_1:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 1) && !GetTransMotion()) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 2;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//������U�����Ă��Ȃ���Ԃɂ���
				m_pWeapon->SetAttack(false);
			}
		}
		break;

		//���̍U�� 2
	case MOTION_TYPE::ATTACK_SWORD_2:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 1) && !GetTransMotion()) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 2;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//������U�����Ă��Ȃ���Ԃɂ���
				m_pWeapon->SetAttack(false);
			}
		}
		break;


		//���̍U�� 3
	case MOTION_TYPE::ATTACK_SWORD_3:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 1 || GetCurrentKey() == 2) && !GetTransMotion()) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 5;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//������U�����Ă��Ȃ���Ԃɂ���
				m_pWeapon->SetAttack(false);			
			}
		}
		break;

		//-------------------------
		//���̍U��
		//-------------------------
		//���̍U�� 1
	case MOTION_TYPE::ATTACK_KATANA_1:
		//�U���̃^�C�~���O�������ꍇ
		if (GetCurrentKey() == 1 || GetCurrentKey() == 2) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 3;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//������U�����Ă��Ȃ���Ԃɂ���
				m_pWeapon->SetAttack(false);
			}
		}
		break;
		//���̍U�� 2
	case MOTION_TYPE::ATTACK_KATANA_2:
		//�U���̃^�C�~���O�������ꍇ
		if (GetCurrentKey() == 1 || GetCurrentKey() == 2) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 3;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//������U�����Ă��Ȃ���Ԃɂ���
				m_pWeapon->SetAttack(false);
			}
		}
		break;
		//���̍U�� 3
	case MOTION_TYPE::ATTACK_KATANA_3:
		//�U���̃^�C�~���O�������ꍇ
		if (GetCurrentKey() == 1) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 6;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//������U�����Ă��Ȃ���Ԃɂ���
				m_pWeapon->SetAttack(false);
			}
		}
		break;
		//���̍U�� �����U��
	case MOTION_TYPE::ATTACK_KATANA_DRAW:
		//�U���̃^�C�~���O�������ꍇ
		if (GetCurrentKey() == 0 || GetCurrentKey() == 1 || GetCurrentKey() == 4 || GetCurrentKey() == 5) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 5;
		}
		else {
			m_bValidAttack = false;
			if (m_pWeapon != nullptr) {
				//������U�����Ă��Ȃ���Ԃɂ���
				m_pWeapon->SetAttack(false);
			}
		}
		break;

	default:
		m_bValidAttack = false;
		break;
	}

	//�U���J�n��
	if (bBeginAttack) {
		InitObjAttacked();	//�U���ς݃��X�g�̏�����
	}

	//----------------------------
	//�U���̃^�C�~���O
	//----------------------------
	if (m_bValidAttack) {
		int nNumKillEnemy = 0;	//�G��|������

		//�I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X���X�V
		UpdateMtxWorldAll();	//�����蔻��擾���Ɉʒu�̃Y���������邽��

		switch (m_typeWeapon)
		{
			//�p���`�̍U��
		case WEAPONTYPE::PUNCH:
		//�ϐ��錾�̂��߂�{}�ň͂�
		{
			const int nNumCol = 3;	//�p���`�̍U���̓����蔻��̐�
			D3DXVECTOR3 aPosCol[nNumCol] = {};	//�����蔻��̈ʒu
			const float fRadiusCol = 10.0f;	//�����蔻��̔��a
			int nIdxModel = -1;	//���f���̔ԍ�
			D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���f������̃I�t�Z�b�g

			//���[�V�����ɂ���ă��f�����擾
			if (GetMotionType() == (int)MOTION_TYPE::PUNCH_1) {
				nIdxModel = INDEX_MODEL_PUNCH_1;	//����
			}
			else if (GetMotionType() == (int)MOTION_TYPE::PUNCH_2) {
				nIdxModel = INDEX_MODEL_PUNCH_2;	//�E��
			}

			for (int nCntCol = 0; nCntCol < nNumCol; nCntCol++)
			{
				//�����蔻��̈ʒu�̎擾
				GetPosCollision(&aPosCol[nCntCol], posOffset, nIdxModel);
				//�I�t�Z�b�g�����Z
				if (nIdxModel == INDEX_MODEL_PUNCH_1) posOffset += D3DXVECTOR3(10.0f, 0.0f, 0.0f);
				if (nIdxModel == INDEX_MODEL_PUNCH_2) posOffset += D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
			}

			//�����蔻��̐����U������
			for (int nCntCollision = 0; nCntCollision < nNumCol; nCntCollision++)
			{
				//�U��
				Attack(OBJ_TYPE::ENEMY, aPosCol[nCntCollision], fRadiusCol, nDamageAttack, DAMAGE_TYPE::PUNCH, &nNumKillEnemy);	//�G
				Attack(OBJ_TYPE::TREE, aPosCol[nCntCollision], fRadiusCol, nDamageAttack, DAMAGE_TYPE::PUNCH, nullptr);		//��
			}
			//�U���̐�[�Ƀp�[�e�B�N���̐���
			CParticle::Create(aPosCol[nNumCol - 1], 20, 15.0f, -0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

			//�U���J�n��
			if (bBeginAttack) {
				//����U�鉹�̍Đ�
				if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_PUNCH);
			}
		}
		break;

		//���̍U��
		case WEAPONTYPE::SWORD:
			if (m_pWeapon != nullptr) {
				D3DXVECTOR3* pPosColArray = nullptr;	//�����蔻��̈ʒu�̔z��
				int nNumCol = 0;	//�����蔻��̐�
				const float fRadiusCol = 10.0f;	//�����蔻��̔��a

				//�����蔻��̈ʒu�̎擾
				m_pWeapon->GetPosCollision(&pPosColArray, &nNumCol);

				if (pPosColArray != nullptr && nNumCol != 0) {
					//�����蔻��̐����U������
					for (int nCntAttack = 0; nCntAttack < nNumCol; nCntAttack++)
					{
						//�U��
						Attack(OBJ_TYPE::ENEMY, pPosColArray[nCntAttack], fRadiusCol, nDamageAttack, DAMAGE_TYPE::SWORD, &nNumKillEnemy);	//�G
						Attack(OBJ_TYPE::TREE, pPosColArray[nCntAttack], fRadiusCol, nDamageAttack, DAMAGE_TYPE::SWORD, nullptr);		//��
					}
					//�U���J�n��
					if (bBeginAttack) {
						//����U�鉹�̍Đ�
						if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_SWORD);
						//������U�����Ă����Ԃɂ���
						if (!m_pWeapon->GetAttack()) {
							m_pWeapon->SetAttack(true);
						}
					}
				}

				//���I�m�ۂ����z��̔j��
				if (pPosColArray != nullptr) {
					delete pPosColArray;
					pPosColArray = nullptr;
				}
			}
			break;

			//���̍U��
		case WEAPONTYPE::KATANA:
			if (m_pWeapon != nullptr) {
				D3DXVECTOR3* pPosColArray = nullptr;	//�����蔻��̈ʒu�̔z��
				int nNumCol = 0;	//�����蔻��̐�
				const float fRadiusCol = 10.0f;	//�����蔻��̔��a

				//�����蔻��̈ʒu�̎擾
				m_pWeapon->GetPosCollision(&pPosColArray, &nNumCol);

				if (pPosColArray != nullptr && nNumCol != 0) {
					//�����蔻��̐����U������
					for (int nCntAttack = 0; nCntAttack < nNumCol; nCntAttack++)
					{
						//�U��
						Attack(OBJ_TYPE::ENEMY, pPosColArray[nCntAttack], fRadiusCol, nDamageAttack, DAMAGE_TYPE::SWORD, &nNumKillEnemy);	//�G
						Attack(OBJ_TYPE::TREE, pPosColArray[nCntAttack], fRadiusCol, nDamageAttack, DAMAGE_TYPE::SWORD, nullptr);		//��
					}
					//�U���J�n��
					if (bBeginAttack) {
						//����U�鉹�̍Đ�
						if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_KATANA);
						//������U�����Ă����Ԃɂ���
						if (!m_pWeapon->GetAttack()) {
							m_pWeapon->SetAttack(true);
						}
					}
				}

				//���I�m�ۂ����z��̔j��
				if (pPosColArray != nullptr) {
					delete pPosColArray;
					pPosColArray = nullptr;
				}
			}
			break;
		}

		if (nNumKillEnemy > 0 && m_nNumKillEnemy < 20) {
			//�G��|�������̉��Z
			m_nNumKillEnemy += nNumKillEnemy;
		}
	}

	//----------------------------
	//�X�e�[�W�ړ�
	//----------------------------
	if (pInput != nullptr && pGame != nullptr) {
		//���̎擾
		CSceneModel* pRaft = pGame->GetRaft();
		//�C���^���N�g�{�^���̃A�C�R���̎擾
		CBillboard* pIconInteract = pGame->GetIconInteract();
		if (pRaft != nullptr) {

			D3DXVECTOR3 vecRaft = *pPosPlayer - pRaft->GetPos();	//�v���C���[���申�̈ʒu�ւ̃|�C���^
			//���̋����ȓ��̏ꍇ
			if (D3DXVec3Length(&vecRaft) <= DISTANCE_USE_RAFT) {
				//�C���^���N�g�{�^���̃A�C�R����\��
				if (pIconInteract != nullptr) pIconInteract->SetDraw(true);

				//�{�^��������
				if (pInput->GetTrigger(CInput::CODE::INTERACT)) {
					//�X�e�[�W�̈ڍs
					pGame->SetFadeNextStage();
					//�j���[�g�������[�V�����̐ݒ�
					SetMotion((int)MOTION_TYPE::NEUTRAL);
					//���̈ʒu�Ɉړ�
					SetPos(pRaft->GetPos());
					//���̊p�x�ɍ��킹��
					D3DXVECTOR3 rotRaft = pRaft->GetRot();	//���̊p�x���擾
					//�v���C���[�̃��f���̕����ɏC��
					rotRaft.y += D3DX_PI;
					if (rotRaft.y > D3DX_PI * 2) rotRaft.y -= D3DX_PI * 2;
					//�p�x��ݒ�
					SetRot(rotRaft);
					//�J�����̈ʒu��ݒ�
					if (pCamera != nullptr) {
						D3DXVECTOR3 posCamera = CSceneMotion::GetPos();
						posCamera.y += 50.0f;
						//�J�����̈ʒu���X�V
						pCamera->SetPos(posCamera);
					}
					//�C���^���N�g�{�^���̃A�C�R�����\��
					if (pIconInteract != nullptr) pIconInteract->SetDraw(false);
				}
			}
			else {
				//�C���^���N�g�{�^���̃A�C�R�����\��
				if (pIconInteract != nullptr) pIconInteract->SetDraw(false);
			}
		}
	}
}

//=============================================================================
// �v���C���[�̕`�揈��
//=============================================================================
void CPlayer::Draw(void) {
	CSceneMotion::Draw();
}


//=============================================================================
// �v���C���[�̃Q�[���N���A���̏���
//=============================================================================
void CPlayer::GameClear(void) {
	//�j���[�g�������[�V�����̐ݒ�
	SetMotion((int)MOTION_TYPE::NEUTRAL);

	//�ړ��ʂ��d�݂͂̂ɂ���
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bLockAct = false;

	if (m_pWeapon != nullptr) {
		//������U�����Ă��Ȃ���Ԃɂ���
		m_pWeapon->SetAttack(false);
		//�֊s��\��
		m_pWeapon->SetDrawOutline(false);
	}

	//�A�C�e���g�p���f
	m_bUseItem = false;
	m_nCntUseItem = 0;
	//�A�C�e���g�p�Q�[�W�̍X�V
	if (m_pGaugeUseItem != nullptr) m_pGaugeUseItem->SetRatioHeight(0.0f);
}

//=============================================================================
// �v���C���[�̃Q�[���I�[�o�[���̏���
//=============================================================================
void CPlayer::GameOver(void) {
	if (GetMotionType() != (int)MOTION_TYPE::STUN) {
		m_bStun = true;
		m_nCntStun = 0;
		SetMotion((int)MOTION_TYPE::STUN);
	}
	//�ړ��ʂ��d�݂͂̂ɂ���
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bLockAct = false;

	if (m_pWeapon != nullptr) {
		//������U�����Ă��Ȃ���Ԃɂ���
		m_pWeapon->SetAttack(false);
		//�֊s��\��
		m_pWeapon->SetDrawOutline(false);
	}
}

//=============================================================================
// �v���C���[�̓����蔻��̏��̎擾
//=============================================================================
void CPlayer::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//�����蔻��̐���ݒ�
	if (pNumCol != nullptr) *pNumCol = NUM_COLLISION;
	//�����蔻��̔��a��ݒ�
	if (pRadiusCol != nullptr) *pRadiusCol = COLLISION_RADIUS;
	//�����蔻��̈ʒu�̔z���ݒ�
	if (ppPosColArray != nullptr) {
		//pos�̃|�C���^���󂾂����ꍇ
		if (*ppPosColArray == nullptr) {
			//pos�̕ϐ��𓖂��蔻��̐����z��Ŋm��
			*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];

			D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���f����pos����̃I�t�Z�b�g
			int nIdxModel = 0;	//����

			//�����蔻��̈ʒu�̎擾
			for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
			{
				GetPosCollision(*ppPosColArray + nCntCol, posOffset, nIdxModel);
				posOffset += D3DXVECTOR3(0.0f, 5.0f, 0.0f);	//�I�t�Z�b�g�����Z
			}
		}
	}
}

//=============================================================================
// �v���C���[�̃_���[�W����
//=============================================================================
void CPlayer::Damage(int nDamage, bool* pDead) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	
	CGame* pGame = nullptr;
	CSound *pSound = nullptr;
	if (pManager != nullptr) {
		//�Q�[���̎擾
		pGame = pManager->GetGame();
		//�T�E���h�̎擾
		pSound = pManager->GetSound();
	}

	//�Q�[���N���A���A�X�e�[�W�ύX���̏ꍇ����
	if (pGame != nullptr) {
		if (pGame->GetGameClear() || pGame->GetChangeStage()) return;
	}
	//�X�^�����̏ꍇ����
	if (m_bStun) return;

	//�̗͂̌���
	m_nLife -= nDamage;
	if (m_nLife <= 0) m_nLife = 0;
	//�̗̓Q�[�W�̐ݒ�
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->SetRatioWidth((float)m_nLife / (float)MAX_LIFE);
		//�̗͂̎O����؂����ꍇ
		if (m_nLife < DANGER_LIFE) {
			m_pGaugeLife->SetColor(COLOR_LIFE_GAUGE_DANGER);
		}
	}
	//�X�^��
	if (m_nLife <= 0) {
		m_bStun = true;
		m_nCntStun = 0;
		SetMotion((int)MOTION_TYPE::STUN);

		//�F��߂�
		if (m_pGaugeLife != nullptr) m_pGaugeLife->SetColor(COLOR_LIFE_GAUGE_SAFE);

		//�ړ��ʂ��d�݂͂̂ɂ���
		m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
		m_bLockAct = false;

		if (m_pWeapon != nullptr) {
			//������U�����Ă��Ȃ���Ԃɂ���
			m_pWeapon->SetAttack(false);
		}

		//�A�C�e���g�p���f
		m_bUseItem = false;
		m_nCntUseItem = 0;
		//�A�C�e���g�p�Q�[�W�̍X�V
		if (m_pGaugeUseItem != nullptr) m_pGaugeUseItem->SetRatioHeight(0.0f);

		//�֊s��\��
		if (m_pWeapon != nullptr) m_pWeapon->SetDrawOutline(false);
	}
}

//=============================================================================
// �v���C���[�̃A�C�e���擾���̏���
//=============================================================================
bool CPlayer::GetItem(int nTypeItem) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	switch ((CItem::ITEM_TYPE)nTypeItem)
	{
		//�����S
	case CItem::ITEM_TYPE::APPLE:
		if (m_pCounterItem != nullptr) {
			//�A�C�e���̏��L����𒴉߂��Ă����ꍇ
			if (m_pCounterItem->GetScore() >= MAX_ITEM_OWN) break;
		}

		if (m_pCounterItem != nullptr) {
			//�A�C�e�����������Ă��Ȃ������ꍇ
			if (m_pCounterItem->GetScore() == 0) {
				//�F�̕ύX
				m_pCounterItem->SetColor(COLOR_ITEM_DEFAULT);
			}
			//�A�C�e�����̉��Z
			m_pCounterItem->AddScore(1);
			//����ɒB�����ꍇ
			if (m_pCounterItem->GetScore() == MAX_ITEM_OWN) {
				//�F�̕ύX
				m_pCounterItem->SetColor(COLOR_ITEM_MAX);
			}
		}
		//�擾���̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::GET_ITEM);

		return true;
		break;
	}
	return false;
}

//=============================================================================
// �v���C���[�̍Ō�̈ʒu���W�̎擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetLastPos(void) {
	return m_lastPos;
}

//=============================================================================
// �v���C���[�̈ړ��ʂ̐ݒ�
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move) { m_move = move; }

//=============================================================================
// �v���C���[�̈ړ��ʂ̎擾
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void) { return m_move; }

//=============================================================================
// �v���C���[�̓����蔻��̔��a�̎擾
//=============================================================================
float CPlayer::GetRadius(void) {
	return COLLISION_RADIUS;
}

//=============================================================================
// �X�^����Ԃ̎擾
//=============================================================================
bool CPlayer::GetStun(void) {
	return m_bStun;
}

//=============================================================================
// ���[�V�����̋敪�̎擾
//=============================================================================
CPlayer::MOTION_CATEGORY CPlayer::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�ړ�
	case MOTION_TYPE::WALK:
	case MOTION_TYPE::DASH:
		return MOTION_CATEGORY::MOVE;
		break;

		//�U��
	case MOTION_TYPE::PUNCH_1:
	case MOTION_TYPE::PUNCH_2:
	case MOTION_TYPE::ATTACK_SWORD_1:
	case MOTION_TYPE::ATTACK_SWORD_2:
	case MOTION_TYPE::ATTACK_SWORD_3:
	case MOTION_TYPE::ATTACK_KATANA_1:
	case MOTION_TYPE::ATTACK_KATANA_2:
	case MOTION_TYPE::ATTACK_KATANA_3:
	case MOTION_TYPE::ATTACK_KATANA_DRAW:
	case MOTION_TYPE::SHEATH:
		return MOTION_CATEGORY::ATTACK;
			break;

	case MOTION_TYPE::DODGE_BACK:
	case MOTION_TYPE::DODGE_FRONT:
		return MOTION_CATEGORY::DODGE;
		break;
		//���̑�
	default:
		return MOTION_CATEGORY::NONE;
		break;
	}
}

//=============================================================================
// ����̕ύX
//=============================================================================
void CPlayer::ChangeWeaponType(WEAPONTYPE typeWeapon) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	CSound *pSound = nullptr;
	//�T�E���h�̎擾
	if (pManager != nullptr) pSound = pManager->GetSound();

	//�ύX���镐�킪�����������ꍇ
	if (m_typeWeapon == typeWeapon) {
		//����ύX�s���̍Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CANSEL);
		//����ύX�Ȃ��ŏI��
		return;
	}

	switch (typeWeapon)
	{
		//�p���`
	case WEAPONTYPE::PUNCH:
		m_typeWeapon = WEAPONTYPE::PUNCH;
		//����̔j��
		if (m_pWeapon != nullptr) {
			m_pWeapon->Uninit();
			m_pWeapon = nullptr;
		}
		//����ύX���̍Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CHANGE_PUNCH);
		//����̃A�C�R���̕ύX
		if (m_pIconWeapon != nullptr) {
			m_pIconWeapon->SetTexType(CTexture::TEXTURE_TYPE::ICON_NONE);
		}
		break;

		//��
	case WEAPONTYPE::SWORD:
		m_typeWeapon = WEAPONTYPE::SWORD;
		//����̔j��
		if (m_pWeapon != nullptr) {
			m_pWeapon->Uninit();
			m_pWeapon = nullptr;
		}
		//����̐���
		if (m_nIdxWeapon >= 0 && m_nIdxWeapon < m_nNumParts) {
			m_pWeapon = CSword::Create(D3DXVECTOR3(-10.0f, 0.0f, 5.0f), D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f), GetPtrModel(m_nIdxWeapon));
		}
		//����ύX���̍Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CHANGE_SWORD);
		//����̃A�C�R��
		if (m_pIconWeapon != nullptr) {
			m_pIconWeapon->SetTexType(CTexture::TEXTURE_TYPE::ICON_SWORD);
		}
		break;

		//��
	case WEAPONTYPE::KATANA:
		m_typeWeapon = WEAPONTYPE::KATANA;
		//����̔j��
		if (m_pWeapon != nullptr) {
			m_pWeapon->Uninit();
			m_pWeapon = nullptr;
		}
		//����̐���
		if (m_nIdxWeapon >= 0 && m_nIdxWeapon < m_nNumParts) {
			m_pWeapon = CKatana::Create(D3DXVECTOR3(-15.0f, 0.0f, 10.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, D3DX_PI * -0.5f), GetPtrModel(m_nIdxWeapon));
		}
		//����ύX���̍Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CHANGE_KATANA);
		//����̃A�C�R��
		if (m_pIconWeapon != nullptr) {
			m_pIconWeapon->SetTexType(CTexture::TEXTURE_TYPE::ICON_KATANA);
		}
		break;
	}
}