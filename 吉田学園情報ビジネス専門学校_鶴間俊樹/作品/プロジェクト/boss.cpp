//=============================================================================
//
// �{�X���� [boss.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "boss.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "wallCylinder.h"
#include "terrain.h"
#include "effect.h"
#include "particle.h"
#include "scene2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_boss.txt"
#define COLLISION_RADIUS (150.0f)	//�����蔻��̔��a �Ǔ��Ŏg�p
#define MOVE_SPEED (3.0f)				//�ړ����x
#define MOVE_SPEED_RUSH (4.5f)			//�ːi�U�����̈ړ����x
#define ROTATE_SPEED (0.02f * D3DX_PI)	//��]���x
#define DISTANCE_ENCOUNTER_PLAYER (700.0f)	//�v���C���[�ƃG���J�E���g���鋗��
#define MAX_LIFE (80)					//�̗͂̍ő�l
#define COLOR_LIFE_GAUGE (D3DXCOLOR(0.6f, 0.0f, 1.0f, 1.0f))	//�̗̓o�[�̈��S���̐F
#define ANGLE_ATTACK_FRONT (D3DX_PI * 0.2f)	//�O���U�����\�Ȋp�x
#define DISTANCE_ATTACK_FRONT (180.0f)		//�O���U�����鋗��
#define ANGLE_ATTACK_UNDER (D3DX_PI * 1.1f)	//���U�����\�Ȋp�x
#define DISTANCE_ATTACK_UNDER (100.0f)		//���U�����鋗��
#define DISTANCE_ATTACK_UNDER_ALL (40.0f)	//�p�x�Ɋ֌W�Ȃ����U���ł��鋗��
#define MIN_SPAN_RUSH_ATTACK (3)	//�ːi�U�����\�ɂȂ�ʏ�U���̉񐔂̍ŏ��l
#define MAX_SPAN_RUSH_ATTACK (5)	//�ːi�U�����\�ɂȂ�ʏ�U���̉񐔂̍ő�l
#define RANDOM_RUSH_ATTACK (5)		//�ːi�U�����\�ɂȂ�m���i����j
#define MAX_CONTINUE_RUSH_ATTACK (3)	//�ːi�U����A���ōs���ő吔
#define DISTANCE_STAY (600.0f)	//�ҋ@���鋗��
#define FALLDOWN_TIME (100)	//���S���̓|��鎞��
#define DEAD_TIME (240)		//���S�������܂ł̎���
#define COLOR_OUTLINE (D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f))	//���f���̗֊s�̐F

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CBoss::m_nNumParts = 0;
CSceneMotion::PARTS_INFO* CBoss::m_pPartsInfoArray = NULL;
CSceneMotion::MOTION_INFO CBoss::m_aMotionInfo[(int)CBoss::MOTION_TYPE::ENUM_MAX] = {};
int CBoss::m_nIdxWeapon = -1;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CBoss::CBoss() : CSceneMotion(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, true)
{
	SetObjType(OBJ_TYPE::ENEMY);	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	SetUpdatePriority(UPDATE_PRIORITY::ENEMY);	//�X�V���̐ݒ�
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//�`�揇�̐ݒ�

	SetColorOutlineAll(COLOR_OUTLINE);	//�֊s�̐F�̐ݒ�

	m_lastPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_rotDestY = 0.0f;
	m_bEncounterPlayer = false;
	m_bStay = false;
	m_bRotateAttack = false;
	m_bValidAttack = false;
	m_nCntNormalAttack = 0;
	m_nCntAttackRush = 0;
	m_bLockAct = false;
	m_nLife = MAX_LIFE;
	m_bDead = false;
	m_nCntDead = 0;

	//�̗̓Q�[�W�̐���
	m_pGaugeLife = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 862.5f, 20.0f);	//�Q�[�W
	m_pGaugeLifeBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 862.5f, 20.0f);	//�w�i
	m_pGaugeLifeFrame = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::GAUGE_LIFE_BOSS, 900.0f, 25.0f);	//�g
	//�̗̓Q�[�W�̐ݒ� ���Ԍ���
	if (m_pGaugeLifeBg != NULL) {	//�w�i
		m_pGaugeLifeBg->SetDrawPriority(DRAW_PRIORITY::UI);
		m_pGaugeLifeBg->SetColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
	}
	if (m_pGaugeLife != NULL) {	//�Q�[�W
		m_pGaugeLife->SetDrawPriority(DRAW_PRIORITY::UI);
		m_pGaugeLife->SetColor(COLOR_LIFE_GAUGE);
	}
	if (m_pGaugeLifeFrame != NULL) {	//�g
		m_pGaugeLifeFrame->SetDrawPriority(DRAW_PRIORITY::UI);
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBoss::~CBoss()
{
	//�̗̓Q�[�W�̔w�i�̔j��
	if (m_pGaugeLifeBg != nullptr) {
		m_pGaugeLifeBg->Uninit();
		m_pGaugeLifeBg = nullptr;
	}
	//�̗̓Q�[�W�̔j��
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}
	//�̗̓Q�[�W�̘g�̔j��
	if (m_pGaugeLifeFrame != nullptr) {
		m_pGaugeLifeFrame->Uninit();
		m_pGaugeLifeFrame = nullptr;
	}
}

//=============================================================================
// �v���C���[�̐�������
//=============================================================================
CBoss* CBoss::Create(D3DXVECTOR3 pos) {
	CBoss* pBoss;
	pBoss = new CBoss();
	if (pBoss != NULL) {
		pBoss->CSceneMotion::SetPos(pos);
		pBoss->Init();
	}

	return pBoss;
}

//=============================================================================
// ���[�V�����������[�h
//=============================================================================
void CBoss::LoadMotionInfo(void) {
	//���[�h����O�ɓ��I�m�ۂ�����������delete����
	UnloadMotionInfo();

	Load(TEXT_FILE_NAME_LOAD_MOTION, &m_pPartsInfoArray, &m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, &m_nIdxWeapon);
}

//=============================================================================
// ���[�V���������A�����[�h
//=============================================================================
void CBoss::UnloadMotionInfo(void) {
	m_nNumParts = 0;
	m_nIdxWeapon = -1;

	//�p�[�c���̉��
	if (m_pPartsInfoArray != NULL) {
		delete[] m_pPartsInfoArray;
		m_pPartsInfoArray = NULL;
	}
	//���[�V�������̉��
	for (int nCntMotionInfo = 0; nCntMotionInfo < (int)MOTION_TYPE::ENUM_MAX; nCntMotionInfo++)
	{
		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
		{
			if (m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray != NULL) {
				delete[] m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray;
				m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray = NULL;
			}
		}
	}
}

//=============================================================================
// �{�X�̏���������
//=============================================================================
HRESULT CBoss::Init(void) {
	//�{�X�̏����ݒ�
	CSceneMotion::SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_rotDestY = 0.0f;	//�O����
	m_lastPos = GetPos();

	SetMotion((int)MOTION_TYPE::WAIT);	//���[�V�����̏����ݒ�
	CSceneMotion::Init();

	return S_OK;
}

//=============================================================================
// �{�X�̏I������
//=============================================================================
void CBoss::Uninit(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�Q�[���̎擾
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//�Q�[�����ێ����Ă���{�X�̃|�C���^��NULL�ɂ���
	if (pGame != nullptr) pGame->ReleaseBoss();
	//�I������
	CSceneMotion::Uninit();
}

//=============================================================================
// �{�X�̍X�V����
//=============================================================================
void CBoss::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�Q�[���̎擾
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//�v���C���[�̎擾
	CPlayer* pPlayer = nullptr;
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	D3DXVECTOR3 posPlayer;	//�v���C���[�̈ʒu
	bool bStunPlayer = false;	//�v���C���[���X�^������

	//�v���C���[�̈ʒu�̎擾
	if (pPlayer != nullptr) {
		posPlayer = pPlayer->GetPos();
		//�v���C���[�̃X�^����Ԃ̎擾
		bStunPlayer = pPlayer->GetStun();
		//�v���C���[���X�^��������ʏ�U���̃J�E���g�����Z�b�g
		if (bStunPlayer == true) m_nCntNormalAttack = 0;
	}

	//�X�^���I���őҋ@�I��
	if (m_bStay == true) m_bStay = bStunPlayer;

	//�G�ƃv���C���[�̋��������߂�
	D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);	//�G����v���C���[�ւ̃x�N�g��
	float fDistPlayer = D3DXVec2Length(&vecPlayer);	//�G����v���C���[�ւ̋���
	//�G����v���C���[�ւ̊p�x
	float fRotPlayerY = (float)atan2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);

	//----------------------------
	//���S��
	//----------------------------
	if (m_bDead == true) {
		m_nCntDead++;

		if (m_nCntDead == FALLDOWN_TIME) {
			//�|��鉹
			if (pSound != NULL)pSound->PlaySound(CSound::SOUND_LABEL::STUN);
		}

		//�G�l�~�[�̔j��
		if (m_nCntDead > DEAD_TIME) {
			//�t�F�[�h�A�E�g
			bool bEndFade = false;
			bEndFade = FadeModelAll(0.0f, -0.01f);
			//�t�F�[�h�I����
			if (bEndFade == true) {
				//�Q�[���N���A
				if (pGame != nullptr) pGame->GameClear();
				//�I������
				Uninit();
				return;
			}
		}

		//���[�V�����̍X�V
		CSceneMotion::Update();
		return;
	}

	//----------------------------
	//�v���C���[������
	//----------------------------
	if (m_bEncounterPlayer == false) {
		//�ҋ@���[�V�������Ƀv���C���[�����ȏ�߂Â����ꍇ
		if (fDistPlayer < DISTANCE_ENCOUNTER_PLAYER && GetMotionType() == (int)MOTION_TYPE::WAIT) {
			//�j���[�g�������[�V�����̐ݒ�
			SetMotion((int)MOTION_TYPE::NEUTRAL);

			if (pSound != nullptr) {
				//�Ȃ̒�~
				pSound->StopSound(pSound->GetBGM());
				//�{�X�Ȃ̍Đ�
				pSound->PlaySound(CSound::SOUND_LABEL::BGM_GAME_BOSS);
				pSound->SetBGM(CSound::SOUND_LABEL::BGM_GAME_BOSS);
			}
		}
		//�ҋ@���[�V�����ł͂Ȃ������ꍇ�i�j���[�g�������[�V�����j
		if (GetMotionType() != (int)MOTION_TYPE::WAIT) {
			//���[�V�����̑J�ڂ��I����Ă�����
			if (GetTransMotion() == false) {
				m_bEncounterPlayer = true;
			}
		}
	}

	//----------------------------
	//�d����
	//----------------------------
	bool bAttackRush = false;	//�ːi�U�����U���\���ǂ���

	if (m_bLockAct == true) {
		//���[�V�����̑J�ڂ����������ꍇ
		if (GetTransMotion() == false) {
			//�d���̉���
			m_bLockAct = false;

			//�ʏ�U�������񐔈ȏ�s���Ă����ꍇ
			if (m_nCntNormalAttack >= MAX_SPAN_RUSH_ATTACK) {
				bAttackRush = true;
			}
			else if (m_nCntNormalAttack >= MIN_SPAN_RUSH_ATTACK) {
				bAttackRush = (rand() % RANDOM_RUSH_ATTACK) == 0;	//�m���œːi�U��
			}
		}
	}

	//----------------------------
	//�U��
	//----------------------------
	bool bAngleAttackFront = false;	//�O���U���ł���p�x�ɂ��邩�ǂ���
	bool bAngleAttackUnder = false;	//���U���ł���p�x�ɂ��邩�ǂ���

	//�d�����ł͂Ȃ��ꍇ
	if (m_bLockAct == false && bStunPlayer == false) {
		//--------------------------
		//�v���C���[�Ƃ̊p�x���v�Z
		//--------------------------
		float fRotY = GetRot().y;	//�G��Y���̊p�x

		//�G�̊p�x�ƃ��f���̑O���̌������t�Ȃ̂Œ�������
		if (fRotY < 0.0f) {
			fRotY += D3DX_PI;
		}
		else if (fRotY >= 0.0f) {
			fRotY -= D3DX_PI;
		}

		//�p�x�𐳂̒l�ɂ���(0����2��)
		if (fRotY < 0.0f) fRotY += D3DX_PI * 2;
		if (fRotPlayerY < 0.0f) fRotPlayerY += D3DX_PI * 2;

		//�v���C���[���O���U���ł���p�x�ɂ����ꍇ
		if (fabsf(fRotPlayerY - fRotY) < ANGLE_ATTACK_FRONT / 2.0f || fabsf(fRotPlayerY - fRotY) > (D3DX_PI * 2) - ANGLE_ATTACK_FRONT / 2.0f) {
			bAngleAttackFront = true;
		}
		//�v���C���[�����U���ł���p�x�ɂ����ꍇ
		if (fabsf(fRotPlayerY - fRotY) < ANGLE_ATTACK_UNDER / 2.0f || fabsf(fRotPlayerY - fRotY) > (D3DX_PI * 2) - ANGLE_ATTACK_UNDER / 2.0f) {
			bAngleAttackUnder = true;
		}

		//--------------------------
		//�U�����ł͂Ȃ��ꍇ�U���\
		//--------------------------
		if (GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
			//�ːi�U��
			if (bAttackRush == true) {
				m_nCntAttackRush = 0;
				StartAttack(ATTACK_TYPE::RUSH);
			}
			//�O���U��
			else if (bAngleAttackFront == true && fDistPlayer < DISTANCE_ATTACK_FRONT && fDistPlayer >= DISTANCE_ATTACK_UNDER) {
				StartAttack(ATTACK_TYPE::FRONT);
			}
			//���U���ł���p�x�����̋����ȓ��B�܂��͊p�x�֌W�Ȃ��U���ł�����̋����ȓ�
			else if ((bAngleAttackUnder == true && fDistPlayer < DISTANCE_ATTACK_UNDER) || fDistPlayer < DISTANCE_ATTACK_UNDER_ALL) {
				StartAttack(ATTACK_TYPE::UNDER);
			}
		}
	}

	//----------------------------
	//�ڕW�p�x�ƈړ��ʂ̐ݒ�
	//----------------------------
	if (m_bLockAct == false && m_bEncounterPlayer == true) {
		//�X�^�����Ɉ��ȏ㗣�ꂽ�ꍇ�ҋ@
		if (m_bStay == false && bStunPlayer == true && fDistPlayer > DISTANCE_STAY) {
			//�j���[�g�������[�V�������[�V������ݒ�
			SetMotion((int)MOTION_TYPE::NEUTRAL);
			//�ړ��ʂ̐ݒ�
			m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			//�v���C���[�ҋ@��Ԃɂ���
			m_bStay = true;
		}
		//�U�����[�V�������ł͂Ȃ��ꍇ�ŁA�ҋ@���łȂ͂��ꍇ	�ːi�U�����͉\
		if ((GetMotionCategory() != MOTION_CATEGORY::ATTACK || GetMotionType() == (int)MOTION_TYPE::ATTACK_RUSH) && m_bStay == false) {
			//�ړ����[�V�������ł͂Ȃ�������
			if (GetMotionCategory() != MOTION_CATEGORY::MOVE && GetMotionType() != (int)MOTION_TYPE::ATTACK_RUSH) {
				//�ړ����̃��[�V������ݒ�
				SetMotion((int)MOTION_TYPE::MOVE);
			}
			//�ڕW�p�x�ƍ��̊p�x�̍��������߂�
			float fRotY = GetRot().y;	//���݂̊p�x

			float fSpeedMove = MOVE_SPEED;	//�ړ����x
			//�ːi�U����
			if (GetMotionType() == (int)MOTION_TYPE::ATTACK_RUSH) fSpeedMove = MOVE_SPEED_RUSH;
			//�ړ��ʂ̐ݒ�
			m_move = D3DXVECTOR3(fSpeedMove * sinf(fRotY + D3DX_PI), m_move.y, fSpeedMove * cosf(fRotY + D3DX_PI));
		}

		//�U�����[�V�������ł͂Ȃ����A�U�����ɉ�]�ł���ꍇ
		if (GetMotionCategory() != MOTION_CATEGORY::ATTACK || m_bRotateAttack == true) {
			//�ڕW�p�x���v���C���[�̌����֌�����i�t�����j
			m_rotDestY = fRotPlayerY;
			//�v���C���[���X�^�����ł͂Ȃ��ꍇ���A�X�^�����ɑҋ@�����������ꍇ�����������Ɏ���
			if (bStunPlayer == false || (bStunPlayer == true && m_bStay == true)) {
				//���f�������΂Ȃ̂Ŕ��Ό����Ɍ�����
				if (m_rotDestY >= 0.0f) {
					m_rotDestY -= D3DX_PI;
				}
				else if (m_rotDestY < 0.0f) {
					m_rotDestY += D3DX_PI;
				}
			}
		}
	}

	//----------------------------
	//��]
	//----------------------------
	//�d�����ł͂Ȃ��Ƃ�
	if (m_bLockAct == false && m_bEncounterPlayer == true) {
		//�ڕW�p�x�Ɗp�x���s��v�̏ꍇ
		if (m_rotDestY != GetRot().y) {
			//�U�����[�V�������ł͂Ȃ��ꍇ�ŁA�ҋ@���łȂ͂��ꍇ
			if (GetMotionCategory() != MOTION_CATEGORY::ATTACK && m_bStay == false) {
				//�ړ����[�V�����ł͂Ȃ�������
				if (GetMotionCategory() != MOTION_CATEGORY::MOVE) {
					//�ړ����̃��[�V������ݒ�
					SetMotion((int)MOTION_TYPE::MOVE);
				}
			}

			//�U�����[�V�������ł͂Ȃ����A�U�����[�V�������ɉ�]�ł���ꍇ
			if (GetMotionCategory() != MOTION_CATEGORY::ATTACK || m_bRotateAttack == true) {
				//�ڕW�p�x�։�]������
				D3DXVECTOR3 rot = GetRot();	//�p�x�̎擾
				float fRotY = rot.y;	//Y���̊p�x

				//�ڕW�p�x���}�C�i�X�A�p�x���v���X�̂Ƃ��ɁA���Z�������ʂ�-�΂𒴂����ꍇ
				if (m_rotDestY - fRotY < -D3DX_PI) {
					//���Z
					rot.y += ROTATE_SPEED;
				}
				//�ڕW�p�x���v���X�A�p�x���}�C�i�X�̂Ƃ��ɁA���Z�������ʂ��΂𒴂����ꍇ
				else if (m_rotDestY - fRotY > D3DX_PI) {
					//���Z
					rot.y -= ROTATE_SPEED;
				}
				//�X�̐����͊֌W�Ȃ��ꍇ
				else if (m_rotDestY - fRotY > 0) {
					//���Z
					rot.y += ROTATE_SPEED;
					//�ڕW�p�x���ߎ��̒���
					if (rot.y > m_rotDestY) {
						rot.y = m_rotDestY;
					}
				}
				else if (m_rotDestY - fRotY < 0) {
					//���Z
					rot.y -= ROTATE_SPEED;
					//�ڕW�p�x���ߎ��̒���
					if (rot.y < m_rotDestY) {
						rot.y = m_rotDestY;
					}
				}

				//�Β��ߎ��̒���
				if (rot.y > D3DX_PI) {
					rot.y -= D3DX_PI * 2;
				}
				else if (rot.y < -D3DX_PI) {
					rot.y += D3DX_PI * 2;
				}

				//�p�x�̐ݒ�
				SetRot(rot);
			}
		}
	}

	//----------------------------
	//�ړ�
	//----------------------------

	m_lastPos = GetPos();
	//�ʒu�̐ݒ�
	SetPos(GetPos() + m_move);

	//----------------------------
	//�����蔻��
	//----------------------------
	//�ǂƂ̓����蔻��
	CWallCylinder::Collision(GetPtrPos(), m_lastPos, COLLISION_RADIUS);

	//�n�ʂƂ̓����蔻��
	bool bLand = false;	//�ڒn���Ă��邩�ǂ���
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//�ڐG�ʒu�A�J�n�x�N�g���A�I���x�N�g��
	vecStart = GetPos();	//�G�̈ʒu���擾
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//������̃x�N�g��

	bLand = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);
	//�ڒn��
	if (bLand == true) {
		SetPos(posColTerrain);	//�ʒu�̈ړ�
	}

	//----------------------------
	//���[�V�����̍X�V
	//----------------------------
	CSceneMotion::Update();

	//----------------------------
	//���[�V�����I����
	//----------------------------
	if (GetEndMotion() == true) {
		switch ((MOTION_TYPE)GetMotionType())
		{
			//�O���U��
		case MOTION_TYPE::ATTACK_FRONT:
			m_bValidAttack = false;

			//���U���ł���p�x�����̋����ȓ��B�܂��͊p�x�֌W�Ȃ��U���ł�����̋����ȓ�
			if ((bAngleAttackUnder == true && fDistPlayer < DISTANCE_ATTACK_UNDER) || fDistPlayer < DISTANCE_ATTACK_UNDER_ALL) {
				//���U��
				StartAttack(ATTACK_TYPE::UNDER);
			}
			else {
				//�U���̏I��
				SetMotion((int)MOTION_TYPE::NEUTRAL);
				m_bLockAct = true;
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			}
			break;

			//���U��
		case MOTION_TYPE::ATTACK_UNDER:
			//�U���̏I��
			SetMotion((int)MOTION_TYPE::NEUTRAL);
			m_bLockAct = true;
			m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			break;

			//�ːi�U��
		case MOTION_TYPE::ATTACK_RUSH:
			//���̉񐔘A���ōU������
			if (m_nCntAttackRush < MAX_CONTINUE_RUSH_ATTACK && bStunPlayer == false) {
				//�ːi�U��
				StartAttack(ATTACK_TYPE::RUSH);
			}
			else {
				//�U���̏I��
				SetMotion((int)MOTION_TYPE::NEUTRAL);
				m_bLockAct = true;
				m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
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
		//�O���U��
	case MOTION_TYPE::ATTACK_FRONT:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurrentKey() == 1 || GetCurrentKey() == 2) && GetTransMotion() == false) {
			//�U���J�n��
			if (m_bValidAttack == false) {
				m_bRotateAttack = false;	//��]�𐧌�
				bBeginAttack = true;	//�U���J�n
			}
			m_bValidAttack = true;
			nDamageAttack = 3;
		}
		else {
			m_bValidAttack = false;
		}
		break;

		//���U��
	case MOTION_TYPE::ATTACK_UNDER:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurrentKey() == 1) && GetTransMotion() == false) {
			//�U���J�n��
			if (m_bValidAttack == false) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 3;
		}
		else {
			m_bValidAttack = false;
		}
		break;

		//�ːi�U��
	case MOTION_TYPE::ATTACK_RUSH:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurrentKey() == 0 || GetCurrentKey() == 2) && GetTransMotion() == false) {
			//�U���J�n��
			if (m_bValidAttack == false) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
			nDamageAttack = 3;
		}
		else {
			m_bValidAttack = false;
		}
		break;

	default:
		m_bValidAttack = false;
		break;
	}

	//�U���J�n��
	if (bBeginAttack == true) {
		InitObjAttacked();	//�U���ς݃��X�g�̏�����
	}

	//----------------------------
	//�U���̃^�C�~���O
	//----------------------------
	if (m_bValidAttack == true) {
		//�I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X���X�V
		UpdateMtxWorldAll();	//�����蔻��擾���Ɉʒu�̃Y���������邽��

		//���[�V�������Ƃɓ����蔻����擾
		switch ((MOTION_TYPE)GetMotionType())
		{
			//�O���U��
		case MOTION_TYPE::ATTACK_FRONT:
		{
			const int nNumColArmR = 6;	//�E��̓����蔻��̐�
			const int nNumColArmL = 6;	//����̓����蔻��̐�
			const int nNumColAll = nNumColArmR + nNumColArmL;	//�U���̓����蔻��̐�
			D3DXVECTOR3 aPosCol[nNumColAll] = {};	//�����蔻��̈ʒu

			//�E��̓����蔻��̎擾
			for (int nCntCol = 0; nCntCol < nNumColArmR; nCntCol++)
			{
				//�����蔻��̈ʒu�̎擾
				GetPosCollision(&aPosCol[nCntCol], D3DXVECTOR3(-20.0f * nCntCol, 0.0f, 0.0f), (int)PARTS_TYPE::HAND_RIGHT);
			}
			//����̓����蔻��̎擾
			for (int nCntCol = 0; nCntCol < nNumColArmL; nCntCol++)
			{
				//�����蔻��̈ʒu�̎擾
				GetPosCollision(&aPosCol[nCntCol + nNumColArmR], D3DXVECTOR3(20.0f * nCntCol, 0.0f, 0.0f), (int)PARTS_TYPE::HAND_LEFT);
			}

			//�����蔻��̐����U������
			for (int nCntAttack = 0; nCntAttack < nNumColAll; nCntAttack++)
			{
				//�U��
				Attack(OBJ_TYPE::PLAYER, aPosCol[nCntAttack], 40.0f, nDamageAttack, DAMAGE_TYPE::ENEMY, NULL);
			}

			//�U���J�n��
			if (bBeginAttack == true) {
				//�U�����̍Đ�
				if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::ATTACK_BOSS);
			}
		}
			break;

			//���U��
		case MOTION_TYPE::ATTACK_UNDER:
		{
			const int nNumColFootR = 6;	//�E���̓����蔻��̐�
			const int nNumColFootL = 6;	//�����̓����蔻��̐�
			const int nNumColAll = nNumColFootR + nNumColFootL;	//�U���̓����蔻��̐�
			D3DXVECTOR3 aPosCol[nNumColAll] = {};	//�����蔻��̈ʒu

			//�E���̓����蔻��̎擾
			for (int nCntCol = 0; nCntCol < nNumColFootR; nCntCol++)
			{
				//�����蔻��̈ʒu�̎擾
				GetPosCollision(&aPosCol[nCntCol], D3DXVECTOR3(0.0f, -15.0f * nCntCol, 0.0f), (int)PARTS_TYPE::FOOT_RIGHT);
			}
			//�����̓����蔻��̎擾
			for (int nCntCol = 0; nCntCol < nNumColFootL; nCntCol++)
			{
				//�����蔻��̈ʒu�̎擾
				GetPosCollision(&aPosCol[nCntCol + nNumColFootR], D3DXVECTOR3(0.0f, -15.0f * nCntCol, 0.0f), (int)PARTS_TYPE::FOOT_LEFT);
			}

			//�����蔻��̐����U������
			for (int nCntAttack = 0; nCntAttack < nNumColAll; nCntAttack++)
			{
				//�U��
				Attack(OBJ_TYPE::PLAYER, aPosCol[nCntAttack], 60.0f, nDamageAttack, DAMAGE_TYPE::ENEMY, NULL);
			}

			//�U���J�n��
			if (bBeginAttack == true) {
				//�U�����̍Đ�
				if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::ATTACK_BOSS);
			}
		}
			break;

			//�ːi�U��
		case MOTION_TYPE::ATTACK_RUSH:
		{
			const int nNumColHand = 6;	//��̓����蔻��̐�
			const int nNumColFoot = 6;	//���̓����蔻��̐�
			const int nNumColAll = nNumColHand + nNumColFoot;	//�U���̓����蔻��̐�
			D3DXVECTOR3 aPosCol[nNumColAll] = {};	//�����蔻��̈ʒu

			//�擾�p�[�c�ƈʒu�̃I�t�Z�b�g�̌���
			int nIdxPartsHand;	//��̃��f���̃C���f�b�N�X
			int nIdxPartsFoot;	//���̃��f���̃C���f�b�N�X
			D3DXVECTOR3 posOffsetHand;
			D3DXVECTOR3 posOffsetFoot;

			//�E��ƍ����̍U��
			if (GetCurrentKey() == 0) {
				//�C���f�b�N�X�̌���
				nIdxPartsHand = (int)PARTS_TYPE::HAND_RIGHT;
				nIdxPartsFoot = (int)PARTS_TYPE::FOOT_LEFT;

				//�I�t�Z�b�g�̌���
				posOffsetHand = D3DXVECTOR3(-15.0f, 0.0f, 0.0f);
				posOffsetFoot = D3DXVECTOR3(0.0f, -15.0f, 0.0f);
			}
			//����ƉE���̍U��
			else if (GetCurrentKey() == 2) {
				//�C���f�b�N�X�̌���
				nIdxPartsHand = (int)PARTS_TYPE::HAND_LEFT;
				nIdxPartsFoot = (int)PARTS_TYPE::FOOT_RIGHT;

				//�I�t�Z�b�g�̌���
				posOffsetHand = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
				posOffsetFoot = D3DXVECTOR3(0.0f, -15.0f, 0.0f);
			}

			//��̓����蔻��̎擾
			for (int nCntCol = 0; nCntCol < nNumColHand; nCntCol++)
			{
				//�����蔻��̈ʒu�̎擾
				GetPosCollision(&aPosCol[nCntCol], posOffsetHand * (float)nCntCol, nIdxPartsHand);
			}
			//���̓����蔻��̎擾
			for (int nCntCol = 0; nCntCol < nNumColFoot; nCntCol++)
			{
				//�����蔻��̈ʒu�̎擾
				GetPosCollision(&aPosCol[nCntCol + nNumColHand], posOffsetFoot * (float)nCntCol, nIdxPartsFoot);
			}

			float fSizeCol = 50.0f;	//�����蔻��̃T�C�Y
			//�����蔻��̐����U������
			for (int nCntAttack = 0; nCntAttack < nNumColAll; nCntAttack++)
			{
				if (nCntAttack >= nNumColHand) fSizeCol = 60.0f;	//���̓����蔻��̃T�C�Y�͏����傫������
				//�U��
				Attack(OBJ_TYPE::PLAYER, aPosCol[nCntAttack], fSizeCol, nDamageAttack, DAMAGE_TYPE::ENEMY, NULL);
			}

			//�U���J�n��
			if (bBeginAttack == true) {
				//�U�����̍Đ�
				if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::ATTACK_BOSS);
			}
		}
		break;
		}
	}
}

//=============================================================================
// �{�X�̕`�揈��
//=============================================================================
void CBoss::Draw(void) {
	CSceneMotion::Draw();
}

//=============================================================================
// �{�X�̓����蔻��̏��̐��̎擾
//=============================================================================
int CBoss::GetNumCollisionParts(void) {
	return (int)PARTS_TYPE::ENUM_MAX;
}

//=============================================================================
// �{�X�̓����蔻��̏��̎擾
//=============================================================================
void CBoss::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	int nNumCol = 0;	//�����蔻��̐�
	float fRadiusCol = 0.0f;	//�����蔻��̔��a
	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���f����pos����̃I�t�Z�b�g
	D3DXVECTOR3 posOffsetAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�I�t�Z�b�g�ւ̉��Z��
	int nIdxModel = -1;	//���f���̔ԍ�

	switch ((PARTS_TYPE)nIdxColParts)
	{
		//���̂̓����蔻��
	case PARTS_TYPE::BODY:
		nNumCol = 5;
		fRadiusCol = 40.0f;
		posOffset = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(0.0f, 15.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::BODY;
		break;

		//�E�r�̓����蔻��
	case PARTS_TYPE::ARM_RIGHT:
		nNumCol = 5;
		fRadiusCol = 20.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::ARM_RIGHT;
		break;
		//�E��̓����蔻��
	case PARTS_TYPE::HAND_RIGHT:
		nNumCol = 6;
		fRadiusCol = 30.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::HAND_RIGHT;
		break;

		//���r�̓����蔻��
	case PARTS_TYPE::ARM_LEFT:
		nNumCol = 5;
		fRadiusCol = 20.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::ARM_LEFT;
		break;
		//����̓����蔻��
	case PARTS_TYPE::HAND_LEFT:
		nNumCol = 6;
		fRadiusCol = 30.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::HAND_LEFT;
		break;

		//�E�ڂ̓����蔻��
	case PARTS_TYPE::LEG_RIGHT:
		nNumCol = 5;
		fRadiusCol = 20.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::LEG_RIGHT;
		break;
		//�E���̓����蔻��
	case PARTS_TYPE::FOOT_RIGHT:
		nNumCol = 7;
		fRadiusCol = 30.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::FOOT_RIGHT;
		break;

		//���ڂ̓����蔻��
	case PARTS_TYPE::LEG_LEFT:
		nNumCol = 5;
		fRadiusCol = 20.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::LEG_LEFT;
		break;
		//�����̓����蔻��
	case PARTS_TYPE::FOOT_LEFT:
		nNumCol = 7;
		fRadiusCol = 30.0f;
		posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		posOffsetAdd = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		nIdxModel = (int)PARTS_TYPE::FOOT_LEFT;
		break;

		//���̑�
	default:
		//�I��
		return;
		break;
	}

	//�����蔻��̐���ݒ�
	if (pNumCol != NULL) *pNumCol = nNumCol;
	//�����蔻��̔��a��ݒ�
	if (pRadiusCol != NULL) *pRadiusCol = fRadiusCol;

	//�����蔻��̈ʒu�̔z���ݒ�
	if (ppPosColArray != NULL) {
		//pos�̃|�C���^���󂾂����ꍇ
		if (*ppPosColArray == NULL) {
			//pos�̕ϐ��𓖂��蔻��̐����z��Ŋm��
			*ppPosColArray = new D3DXVECTOR3[nNumCol];

			//�����蔻��̈ʒu�̎擾
			for (int nCntCol = 0; nCntCol < nNumCol; nCntCol++)
			{
				GetPosCollision(*ppPosColArray + nCntCol, posOffset, nIdxModel);
				posOffset += posOffsetAdd;	//�I�t�Z�b�g�����Z
			}
		}
	}
}

//=============================================================================
// �{�X�̍U���J�n���̏���
//=============================================================================
void CBoss::StartAttack(ATTACK_TYPE type) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	switch ((ATTACK_TYPE)type)
	{
		//�O�U��
	case ATTACK_TYPE::FRONT:
		//�U�����[�V�����̐ݒ�
		SetMotion((int)MOTION_TYPE::ATTACK_FRONT);
		//�U���������͉�]�ł���
		m_bRotateAttack = true;
		//�U��グ���̍Đ�
		if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_PUNCH);
		//�ʏ�U���̃J�E���^�̉��Z
		m_nCntNormalAttack++;
		break;

		//���U��
	case ATTACK_TYPE::UNDER:
		//�U�����[�V�����̐ݒ�
		SetMotion((int)MOTION_TYPE::ATTACK_UNDER);
		//�U������]�ł��Ȃ�
		m_bRotateAttack = false;
		//�U��グ���̍Đ�
		if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_PUNCH);
		//�ʏ�U���̃J�E���^�̉��Z
		m_nCntNormalAttack++;
		break;

		//�ːi�U��
	case ATTACK_TYPE::RUSH:
		//�U�����[�V�����̐ݒ�
		SetMotion((int)MOTION_TYPE::ATTACK_RUSH);
		//�U������]�ł���
		m_bRotateAttack = true;
		//�ʏ�U���̃J�E���^�̃��Z�b�g
		m_nCntNormalAttack = 0;
		//�ːi�U���̘A����
		m_nCntAttackRush++;
		break;

		//���̑�
	default:
		return;	//���������I��
		break;
	}

	//�ȉ����ʂ̏���
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
}

//=============================================================================
// �{�X�̃_���[�W����
//=============================================================================
void CBoss::Damage(int nDamage, bool* pDead) {
	if (m_bDead == true) return;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�Q�[���̎擾
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();

	//���C�t�̌���
	m_nLife -= nDamage;
	//�̗̓Q�[�W�̐ݒ�
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->SetRatioWidth((float)m_nLife / (float)MAX_LIFE);
	}
	//���S
	if (m_nLife <= 0) {
		//���S�̐ݒ�
		m_bDead = true;
		//���S�J�E���g�̏�����
		m_nCntDead = 0;
		//�����蔻��𖳌�
		SetEnableCollision(false);
		//���S���[�V�����̐ݒ�
		SetMotion((int)MOTION_TYPE::DEAD);
		//�֊s���\��
		SetDrawOutlineAll(false);
		//�{�X���S���̏���
		if (pGame != nullptr) pGame->DeadBoss();
	}
}

//=============================================================================
// ���[�V�����̋敪�̎擾
//=============================================================================
CBoss::MOTION_CATEGORY CBoss::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�ړ�
	case MOTION_TYPE::MOVE:
		return MOTION_CATEGORY::MOVE;
		break;

		//�U��
	case MOTION_TYPE::ATTACK_FRONT:
	case MOTION_TYPE::ATTACK_UNDER:
	case MOTION_TYPE::ATTACK_RUSH:
		return MOTION_CATEGORY::ATTACK;
		break;

		//���̑�
	default:
		return MOTION_CATEGORY::NONE;
		break;
	}
}