//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "terrain.h"
#include "enemy_normal.h"
#include "enemy_human.h"
#include "effect.h"
#include "billboard.h"
#include "timer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MOVE_SPEED_NORMAL (1.0f)		//�ړ����x�i�ʏ펞�j
#define MOVE_SPEED_CHASE (4.0f)			//�ړ����x�i�ǐՎ��j
#define MOVE_START_TIME (100)			//�ړ��J�n����
#define MIN_MOVE_TIME (60)				//�ړ����Ԃ̍ŏ��l
#define MAX_MOVE_TIME (120)				//�ړ����Ԃ̍ő�l
#define ROTATE_SPEED (0.02f * D3DX_PI)	//��]���x
#define RADIUS_VISUAL_FIELD (400.0f)	//����̔��a
#define DISTANCE_LOSE_SIGHT (600.0f)	//�v���C���[������������
#define VIEW_ANGLE (D3DX_PI * 0.8f)		//����p�x
#define ATTACK_ANGLE (D3DX_PI * 0.1f)	//�U���p�x
#define FALLDOWN_TIME (30)	//���S���̓|��鎞��
#define DEAD_TIME (120)	//���S�������܂ł̎���
#define LIFE_GAUGE_HEIGHT (100.0f)	//�G�̈ʒu����̗̑̓Q�[�W�̍���
#define DRAW_DISTANCE (1800.0f)			//�`�悪�\�ɂȂ�n�߂�v���C���[�Ƃ̋���
#define MAX_DRAW_DISTANCE (2000.0f)		//�`��\�ȍő�̃v���C���[�Ƃ̋���

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy() : m_nMaxLife(0), m_fDistAttack(0.0f), m_nAddTime(0)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, bool bOutline, int nLife, float fDistAttack, int nAddTime) : CSceneMotion(pPartsInfoArray, nNumParts, pMotionInfoArray, nNumTypeMotion, bOutline), m_nMaxLife(nLife), m_fDistAttack(fDistAttack), m_nAddTime(nAddTime)
{
	SetUpdatePriority(UPDATE_PRIORITY::ENEMY);	//�X�V���̐ݒ�
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//�`�揇�̐ݒ�
	SetObjType(OBJ_TYPE::ENEMY);	//�I�u�W�F�N�g�^�C�v�̐ݒ�

	SetColorOutlineAll(D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f));	//�֊s�̐F�̐ݒ�

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDestY = 0.0f;
	m_bRotateAttack = false;
	m_nCntMove = 0;
	m_nMoveTimeRand = 0;
	m_bDetectPlayer = false;
	m_nLife = nLife;
	m_bDead = false;
	m_nCntDead = 0;
	m_nCntDrawGaugeLife = 0;
	m_bDraw = true;

	//�̗̓Q�[�W�̐���
	m_pGaugeLifeFrame = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 75.0f, 15.0f);
	m_pGaugeLife = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 70.0f, 10.0f);

	if (m_pGaugeLifeFrame != NULL) {
		m_pGaugeLifeFrame->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)); //�̗̓Q�[�W�̐F�̐ݒ�
		m_pGaugeLifeFrame->SetDraw(false);	//�`�悵�Ȃ��悤�ɂ���
	}
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f)); //�̗̓Q�[�W�̐F�̐ݒ�
		m_pGaugeLife->SetDraw(false);	//�`�悵�Ȃ��悤�ɂ���
	}

	m_pClock = nullptr;
	for (int nCnt = 0; nCnt < MAX_DIGIT_ADD_TIME_ENEMY; nCnt++)
	{
		m_pAddTime[nCnt] = nullptr;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// ���S�~��Ƀ����_���Ɋp�x���炵�ēG��z�u����
//=============================================================================
//�����F���S�̈ʒu�@�~�̐��@�~���Ƃ̋����i���a�j�@�i�������̈�O�̓G�Ƃ́j�ŒZ�����A�Œ�����
void CEnemy::SetEnemyCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fDistCircle, int nMinDist, int nMaxDist) {
	int nCntCircle = 0;	//�쐬�����؂̉~�̐�
	float fRotCreate = 0.0f;	//�z�u����p�x
	float fRandStartRot = (rand() % 314) * 0.01f;	//�z�u�̊J�n�p�x�̈ʒu�������_�� �O�����ߑa�C���ɂȂ邽��

	while (nCntCircle < nNumCircle)
	{
		int nRandDist = rand() % (nMaxDist + 1 - nMinDist) + nMinDist;	//��O�̖؂Ƃ̋���
		fRotCreate += nRandDist / ((nCntCircle + 1) * fDistCircle * D3DX_PI) * D3DX_PI;	//�����_���ȋ��� / �~���̒����̔��� * pi �Ń��W�A�������߂�

		//����z�u�����ꍇ
		if (fRotCreate >= D3DX_PI * 2.0f) {
			nCntCircle++;	//�J�E���g�̉��Z
			fRotCreate = 0.0f;	//�z�u����p�x�̃��Z�b�g
			fRandStartRot = (rand() % 314) * 0.01f;	//�z�u����p�x�̃����_���l���Đݒ�
		}
		else {
			//�G�𐶐�����ʒu�̐ݒ�
			D3DXVECTOR3 posCreate = posCenter;	//�z�u����ʒu
			posCreate.x = sinf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			posCreate.z = cosf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			//�G�̎�ނ������_���Ō��߂�
			int nRandType = rand() % 2;
			//�G�̐���
			switch (nRandType)
			{
				//�ʏ�G
			case 0:
				CEnemyNormal::Create(posCreate);
				break;
				//�l�`�G
			case 1:
				CEnemyHuman::Create(posCreate);
				break;
			}
		}
	}
}

//=============================================================================
// �G�l�~�[�̏���������
//=============================================================================
HRESULT CEnemy::Init(void) {
	//�p�x�������_���ɐݒ�
	int nRandRot = rand() % 21 - 10;	//-10 ~ 10
	float fRotY = D3DX_PI * (float)nRandRot * 0.1f;
	SetRot(D3DXVECTOR3(0.0f, fRotY, 0.0f));
	m_rotDestY = fRotY;

	//���[�V�����I�u�W�F�N�g�̏���������
	CSceneMotion::Init();
	return S_OK;
}

//=============================================================================
// �G�l�~�[�̏I������
//=============================================================================
void CEnemy::Uninit(void) {
	//�̗̓Q�[�W�̔j��
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = NULL;
	}
	//�̗̓Q�[�W�̘g�̔j��
	if (m_pGaugeLifeFrame != NULL) {
		m_pGaugeLifeFrame->Uninit();
		m_pGaugeLifeFrame = NULL;
	}
	//�I������
	CSceneMotion::Uninit();
}

//=============================================================================
// �G�l�~�[�̍X�V����
//=============================================================================
void CEnemy::Update(void) {
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

	//----------------------------
	//���S��
	//----------------------------
	if (m_bDead == true) {
		m_nCntDead++;

		if (m_nCntDead < 60) {
			//�ʒu���グ�Ă���
			if (m_pClock != nullptr) {
				D3DXVECTOR3 posClock = m_pClock->GetPos();
				posClock.y += 0.5f;
				m_pClock->SetPos(posClock);
			}
			if (m_pAddTime != nullptr) {
				for (int nCntTime = 0; nCntTime < MAX_DIGIT_ADD_TIME_ENEMY; nCntTime++)
				{
					D3DXVECTOR3 posAddTime = m_pAddTime[nCntTime]->GetPos();
					posAddTime.y += 0.5f;
					m_pAddTime[nCntTime]->SetPos(posAddTime);

				}
			}
		}
		if (m_nCntDead == 100) {
			//���Ԃ̒ǉ��̃r���{�[�h��j��
			if (m_pClock != nullptr) {
				m_pClock->Uninit();
				m_pClock = nullptr;
			}
			for (int nCntTime = 0; nCntTime < MAX_DIGIT_ADD_TIME_ENEMY; nCntTime++) {
				if (m_pAddTime[nCntTime] != nullptr) {
					m_pAddTime[nCntTime]->Uninit();
					m_pAddTime[nCntTime] = nullptr;
				}
			}
		}

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
	//�ڕW�p�x�ƈړ��ʂ̐ݒ�
	//----------------------------
	//�v���C���[����������Ă���Ƃ�
	if (m_bDetectPlayer == true) {
		//�����ǐ�
		D3DXVECTOR3 posPlayer;	//�v���C���[�̈ʒu
		//�v���C���[�̈ʒu�̎擾
		if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();
		//�G����v���C���[�ւ̊p�x
		float rotPlayerY = (float)atan2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);

		//�ڕW�p�x���v���C���[�̌����֌�����
		m_rotDestY = rotPlayerY;
		//���f�������΂Ȃ̂Ŕ��Ό����Ɍ�����
		if (m_rotDestY >= 0.0f) {
			m_rotDestY -= D3DX_PI;
		}
		else if(m_rotDestY < 0.0f) {
			m_rotDestY += D3DX_PI;
		}

		//�ړ��ʂ̐ݒ�
		if (this->GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
			//�G�ƃv���C���[�̋��������߂�
			D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);	//�G����v���C���[�ւ̃x�N�g��
			float fDistPlayer = D3DXVec2Length(&vecPlayer);	//�G����v���C���[�ւ̋���
			//�ڕW�p�x�ƍ��̊p�x�̍��������߂�
			float fRotY = GetRot().y;	//���݂̊p�x
			float fRotDeltaY = fabsf(fRotY - m_rotDestY);	//�p�x�̍���
			if (fRotDeltaY > D3DX_PI) fRotDeltaY = D3DX_PI * 2 - fRotDeltaY;
			//�U���\�ȋ����������ꍇ
			if (fDistPlayer < m_fDistAttack || fRotDeltaY > ATTACK_ANGLE) {
				m_move = D3DXVECTOR3(0.0f, m_move.y, 0.0f);
			}
			else {
				m_move = D3DXVECTOR3(MOVE_SPEED_CHASE * sinf(fRotY + D3DX_PI), m_move.y, MOVE_SPEED_CHASE * cosf(fRotY + D3DX_PI));
			}
		}
	}
	else {
		m_nCntMove++;
		//�ړ��J�n��
		if (m_nCntMove == MOVE_START_TIME) {
			//�ڕW�p�x�̐ݒ�
			int nRandRot = rand() % 40 + 10;	//�p�x�������_��
			float fAddRot = (float)nRandRot * 0.01f;	//�ǉ�����p�x
			m_rotDestY = m_rotDestY += D3DX_PI * fAddRot;
			if (m_rotDestY > D3DX_PI)m_rotDestY -= D3DX_PI * 2;	//�p�C���ߎ��̒���
			//�ړ����Ԃ̗����̐ݒ�
			m_nMoveTimeRand = rand() % (MAX_MOVE_TIME - MIN_MOVE_TIME) + MIN_MOVE_TIME;
			//�ړ����[�V�����̐ݒ�
			this->SetMoveMotion();
		}
		//�ړ���
		if (m_nCntMove >= MOVE_START_TIME && m_nCntMove < MOVE_START_TIME + m_nMoveTimeRand) {
			//�ړ��ʂ̐ݒ�
			if (this->GetMotionCategory() != MOTION_CATEGORY::ATTACK) {	//�ǐՎ��ȊO�ōU�����s�����Ƃ͂Ȃ����O�̈�
				float fRotY = GetRot().y;
				m_move = D3DXVECTOR3(MOVE_SPEED_NORMAL * sinf(fRotY + D3DX_PI), m_move.y, MOVE_SPEED_NORMAL * cosf(fRotY + D3DX_PI));
			}
		}
		//�ړ��I����
		if (m_nCntMove == MOVE_START_TIME + m_nMoveTimeRand) {
			m_move = D3DXVECTOR3(0.0f, m_move.y, 0.0f);	//�ړ������Ȃ�����
			m_rotDestY = GetRot().y;	//�ڕW�p�x�����݂̊p�x�ɂ���
			SetMotion(0);	//�j���[�g�������[�V������ݒ�
			m_nCntMove = 0;
		}
	}

	//----------------------------
	//��]
	//----------------------------
	//�U�����[�V�������ł͂Ȃ��Ƃ�
	if (this->GetMotionCategory() != MOTION_CATEGORY::ATTACK || m_bRotateAttack == true) {
		//�ڕW�p�x�Ɗp�x���s��v�̏ꍇ
		if (m_rotDestY != GetRot().y) {
			//�ړ����[�V�������ł͂Ȃ�������
			if (this->GetMotionCategory() != MOTION_CATEGORY::MOVE && this->GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
				//�ړ����̃��[�V������ݒ� �s�K�v�H
				this->SetMoveMotion();
			}

			//�ڕW�p�x�։�]������
			D3DXVECTOR3 rot = GetRot();	//�p�x�̎擾
			float rotY = rot.y;	//Y���̊p�x

			//�ڕW�p�x���}�C�i�X�A�p�x���v���X�̂Ƃ��ɁA���Z�������ʂ�-�΂𒴂����ꍇ
			if (m_rotDestY - rotY < -D3DX_PI) {
				//���Z
				rot.y += ROTATE_SPEED;
			}
			//�ڕW�p�x���v���X�A�p�x���}�C�i�X�̂Ƃ��ɁA���Z�������ʂ��΂𒴂����ꍇ
			else if (m_rotDestY - rotY > D3DX_PI) {
				//���Z
				rot.y -= ROTATE_SPEED;
			}
			//�X�̐����͊֌W�Ȃ��ꍇ
			else if (m_rotDestY - rotY > 0) {
				//���Z
				rot.y += ROTATE_SPEED;
				//�ڕW�p�x���ߎ��̒���
				if (rot.y > m_rotDestY) {
					rot.y = m_rotDestY;
				}
			}
			else if (m_rotDestY - rotY < 0) {
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

	//----------------------------
	//�ړ�
	//----------------------------
	//�d��
	m_move.y -= POWER_GRAVITY_GROUND;	//�W�����v���邱�Ƃ��Ȃ��̂ŏd������

	//�ʒu�̐ݒ�
	SetPos(GetPos() + m_move);

	//----------------------------
	//�����蔻��
	//----------------------------

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

	//������
	if (GetPos().y < DEAD_HEIGHT) {
		Uninit();
		return;
	}

	//----------------------------
	//�v���C���[�̔���
	//----------------------------
	D3DXVECTOR3 posPlayer;	//�v���C���[�̈ʒu

	//�Q�[���N���A�����ǂ���
	bool bGameClear = false;
	if (pGame != NULL) bGameClear = pGame->GetGameClear();

	bool bStunPlayer = false;	//�v���C���[���X�^������
	if (pPlayer != NULL) {
		//�v���C���[�̈ʒu�̎擾
		posPlayer = pPlayer->GetPos();
		//�v���C���[�̃X�^����Ԃ̎擾
		bStunPlayer = pPlayer->GetStun();
	}
	D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);	//�G����v���C���[�ւ̃x�N�g��
	float fDistPlayer = D3DXVec2Length(&vecPlayer);	//�G����v���C���[�ւ̋���

	bool bAngleAttack = false;	//�U���ł���p�x�ɂ��邩�ǂ���

	//�v���C���[�������������������ꍇ
	if (fDistPlayer > DISTANCE_LOSE_SIGHT || bStunPlayer == true || bGameClear == true) {
		if (m_bDetectPlayer == true) {
			m_move = D3DXVECTOR3(0.0f, m_move.y, 0.0f);	//�ړ������Ȃ�����
			m_rotDestY = GetRot().y;	//�ڕW�p�x�����݂̊p�x�ɂ���
			SetMotion(0);	//�j���[�g�������[�V������ݒ�
			m_nCntMove = 0;	//�ړ��J�E���^�[�����Z�b�g
		}
		m_bDetectPlayer = false;	//�v���C���[�𖢔�����Ԃɂ���
	}

	//����̔��a���Ƀv���C���[�������ꍇ
	if (fDistPlayer < RADIUS_VISUAL_FIELD && bStunPlayer == false && bGameClear == false) {
		float rotY = GetRot().y;	//�G��Y���̊p�x

		//�G�̊p�x�ƃ��f���̑O���̌������t�Ȃ̂ŏC������
		if (rotY < 0.0f) {
			rotY += D3DX_PI;
		}
		else if (rotY >= 0.0f) {
			rotY -= D3DX_PI;
		}

		//�G����v���C���[�ւ̊p�x
		float rotPlayerY = (float)atan2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);

		//�p�x�𐳂̒l�ɂ���(0����2��)
		if (rotY < 0.0f) rotY += D3DX_PI * 2;
		if (rotPlayerY < 0.0f) rotPlayerY += D3DX_PI * 2;

		//�G�̑O���̌����ƓG����v���C���[�ւ̌����̍������߁A���ꂪ���ȓ��Ȃ�v���C���[����
		if (fabsf(rotPlayerY - rotY) < VIEW_ANGLE / 2.0f || fabsf(rotPlayerY - rotY) > (D3DX_PI * 2) - VIEW_ANGLE / 2.0f) {
			m_bDetectPlayer = true;
		}
		//�v���C���[���U���ł���p�x�ɂ����ꍇ
		if (fabsf(rotPlayerY - rotY) < ATTACK_ANGLE / 2.0f || fabsf(rotPlayerY - rotY) > (D3DX_PI * 2) - ATTACK_ANGLE / 2.0f) {
			bAngleAttack = true;
		}
	}

	//�v���C���[�������ōU�����[�V����������Ȃ��ꍇ
	if (m_bDetectPlayer == true && this->GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
		//�ǐՃ��[�V�����ɕύX
		this->SetChaseMotion();
	}

	//�U��
	if (bAngleAttack == true && this->GetMotionCategory() != MOTION_CATEGORY::ATTACK && bLand == true && fDistPlayer < m_fDistAttack) {
		bool bSuccessAttackStart = false;	//�U���J�n�������������ǂ���
		//�U���J�n
		bSuccessAttackStart = this->AttackStart();	
		//�U���J�n������
		if (bSuccessAttackStart == true) {
			//�U���ς݃��X�g�̏�����
			InitObjAttacked();
			//�ړ��ʂ��d�݂͂̂ɂ���
			m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
			//�U�����̍Đ�
			if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_PUNCH);
		}
	}

	//���[�V�����̍X�V
	CSceneMotion::Update();

	//���[�V�����I����
	if (GetEndMotion() == true) {
		this->MotionEnd();
	}

	//�I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X���X�V
	UpdateMtxWorldAll();
	//���[�V�������̏���
	this->MotionAct();

	//�̗̓Q�[�W�̕`�掞�Ԃ�����
	if (m_nCntDrawGaugeLife > 0) {
		m_nCntDrawGaugeLife--;
		//�`�掞�Ԃ��Ȃ��Ȃ����ꍇ
		if (m_nCntDrawGaugeLife <= 0) {
			//�`�悵�Ȃ�����
			if (m_pGaugeLife != NULL) m_pGaugeLife->SetDraw(false);
			if (m_pGaugeLifeFrame != NULL) m_pGaugeLifeFrame->SetDraw(false);
		}
		else {
			//�ʒu���X�V
			if (m_pGaugeLife != NULL) m_pGaugeLife->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + LIFE_GAUGE_HEIGHT, GetPos().z));
			if (m_pGaugeLifeFrame != NULL) m_pGaugeLifeFrame->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + LIFE_GAUGE_HEIGHT, GetPos().z));
		}
	}
}

//=============================================================================
// �G�l�~�[�̕`�揈��
//=============================================================================
void CEnemy::Draw(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�Q�[���̎擾
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//�v���C���[�̎擾
	CPlayer* pPlayer = nullptr;
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();
	//�v���C���[�Ƃ̋��������߂�
	D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);
	float fDistPlayer = D3DXVec2Length(&vecPlayer);
	//�`��̐ݒ�
	if (m_bDraw == true) {
		//�`��O�̈ʒu�ɂ�����`�悵�Ȃ��悤�ɂ���
		if (fDistPlayer > MAX_DRAW_DISTANCE) {
			m_bDraw = false;
		}
	}
	else {
		//�`����̈ʒu�ɂ�����`��ł���悤�ɂ���
		if (fDistPlayer < DRAW_DISTANCE) {
			m_bDraw = true;
		}
	}

	//�`��
	if (m_bDraw == true) {
		CSceneMotion::Draw();
	}
	else {
		//�`�悵�Ȃ��ꍇ���[���h�}�g���b�N�X�݂̂��X�V����
		UpdateMtxWorldAll();
	}
}

//=============================================================================
// �G�l�~�[�̃_���[�W
//=============================================================================
void CEnemy::Damage(int nDamage, bool* pDead) {
	if (m_bDead == true) return;
	
	//�_���[�W
	m_nLife -= nDamage;
	//��P�������A�ǉ��_���[�W
	if (m_bDetectPlayer == false) {
		m_nLife--;
	}

	if (m_nLife <= 0) {
		//���S
		if (pDead != NULL) *pDead = true;
		Dead();
	}
	else {
		if (pDead != NULL) *pDead = false;
		//�v���C���[������Ԃɂ���
		m_bDetectPlayer = true;

		//�̗̓Q�[�W�̕`�掞�Ԃ̐ݒ�
		m_nCntDrawGaugeLife = 300;
		if (m_pGaugeLife != NULL) {
			//�̗̓Q�[�W�̕���ς���
			m_pGaugeLife->SetRatioWidth((float)m_nLife / (float)m_nMaxLife);
			//�`�悳����
			m_pGaugeLife->SetDraw(true);
		}
		if (m_pGaugeLifeFrame != NULL) {
			//�`�悳����
			m_pGaugeLifeFrame->SetDraw(true);
		}
	}
}

//=============================================================================
// �G�l�~�[�̎��S
//=============================================================================
void CEnemy::Dead(void) {
	//���S�̐ݒ�
	m_bDead = true;
	//���S�J�E���g�̏�����
	m_nCntDead = 0;
	//�����蔻��𖳌�
	SetEnableCollision(false);
	//���S���[�V�����̐ݒ�
	this->SetDeadMotion();
	//�֊s���\��
	SetDrawOutlineAll(false);

	//�̗̓Q�[�W�̔j��
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = NULL;
	}
	//�̗̓Q�[�W�̘g�̔j��
	if (m_pGaugeLifeFrame != NULL) {
		m_pGaugeLifeFrame->Uninit();
		m_pGaugeLifeFrame = NULL;
	}

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	CGame* pGame = nullptr;
	CTimer* pTimer = nullptr;
	CSound* pSound = nullptr;

	//�Q�[���̎擾
	if (pManager != nullptr) pGame = pManager->GetGame();
	//�^�C�}�[�̎擾
	if (pGame != nullptr) pTimer = pGame->GetTimer();

	//�^�C�}�[�̎��Ԃ̒ǉ�
	if (pTimer != nullptr) pTimer->AddTime(m_nAddTime);

	//�ǉ����Ԃ̃r���{�[�h�̐���
	D3DXVECTOR3 posBillboard = GetPos();
	posBillboard.y += 50.0f;
	m_pClock = CBillboard::Create(posBillboard, CTexture::TEXTURE_TYPE::CLOCK, 30.0f, 30.0f);
	for (int nCntTime = 0; nCntTime < MAX_DIGIT_ADD_TIME_ENEMY; nCntTime++) {
		m_pAddTime[nCntTime] = CBillboard::Create(posBillboard, CTexture::TEXTURE_TYPE::NUMBER_004, 30.0f, 30.0f);
	}
	//�I�t�Z�b�g�̐ݒ�
	if (m_pClock != nullptr) m_pClock->SetOffsetPos(D3DXVECTOR3(-15.0f, 0.0f, 0.0f));
	if (m_pAddTime != nullptr) {
		for (int nCntTime = 0; nCntTime < MAX_DIGIT_ADD_TIME_ENEMY; nCntTime++) {
			//�I�t�Z�b�g�̐ݒ�
			m_pAddTime[nCntTime]->SetOffsetPos(D3DXVECTOR3(30.0f*MAX_DIGIT_ADD_TIME_ENEMY - 30.0f*nCntTime - 10.0f, 0.0f, 0.0f));
			//UV���W��ݒ�
			int nTimeDigit = (m_nAddTime % (int)pow(10, nCntTime + 1)) / (int)pow(10, nCntTime);	//�����Ƃ̎��Ԃ̐���
			m_pAddTime[nCntTime]->SetTexPos((float)nTimeDigit / 10.0f, 0.0f, ((float)nTimeDigit / 10.0f) + 0.1f, 1.0f);
		}
	}

	//���Ԓǉ����̍Đ�
	if (pManager != nullptr) pSound = pManager->GetSound();
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::ADD_TIME);
}

//=============================================================================
// �U�����[�V�������ɉ�]�ł��邩�ǂ����̐ݒ�
//=============================================================================
void CEnemy::SetRotateAttack(bool bRotateAttack) {
	m_bRotateAttack = bRotateAttack;
}