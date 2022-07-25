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
#include "gameScene.h"

#include "wallCylinder.h"
#include "terrain.h"
#include "effect.h"
#include "particle.h"
#include "particleEffect.h"
#include "object2D.h"
#include "billboard.h"

#include "shockWaveEmitter.h"
#include "thunderEmitter.h"

//=============================================================================
// �}�N����`
//=============================================================================
//--------------------------------
//�t�@�C����
//--------------------------------
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_player.txt"

//--------------------------------
//�ړ�
//--------------------------------
#define ADD_MOVE_SPEED (1.2f)			//����
#define DEC_MOVE_SPEED_GROUND (1.0f)	//�n��ł̌���
#define DEC_MOVE_SPEED_AIR (0.05f)		//�󒆂ł̌���
#define MAX_MOVE_SPEED_WALK (6.0f)		//���s���x
#define MAX_MOVE_SPEED_DASH (12.0f)		//���s���x
#define MAX_MOVE_SPEED_AIR (2.0f)		//�󒆑��x
#define ROTATE_SPEED (0.1f)				//��]���x

//--------------------------------
//�����蔻��
//--------------------------------
#define COLLISION_RADIUS (70.0f)		//�����蔻��̔��a	�ǂƂ��Ɏg��
#define NUM_COLLISION (10)				//�����蔻��̐�

//--------------------------------
//�U��
//--------------------------------
#define ATTACK_DAMAGE_PUNCH (20)
#define ATTACK_DAMAGE_STANP (30)

//--------------------------------
//���̑�
//--------------------------------
#define COLOR_OUTLINE (D3DXCOLOR(0.2f, 0.5f, 1.0f, 1.0f))	//���f���̗֊s�̐F

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CPlayer::m_nNumParts = 0;
CObjectMotion::PARTS_INFO* CPlayer::m_pPartsInfoArray = nullptr;
CObjectMotion::MOTION_INFO CPlayer::m_aMotionInfo[(int)CPlayer::MOTION_TYPE::ENUM_MAX] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() : CObjectMotion(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, false)
{
	SetObjType(OBJTYPE_PLAYER);	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	SetUpdatePriority(UPDATE_PRIORITY::PLAYER);	//�X�V���̐ݒ�
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//�`�揇�̐ݒ�

	SetColorOutlineAll(COLOR_OUTLINE);	//�֊s�̐F�̐ݒ�
	SetDiffuseModelAll(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0);	//���f���̐F�̐ݒ�
	SetColorGlowAll(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));	//�֊s�̔����F�̐ݒ�

	m_lastPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_destRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAddMoveSpeed = 0.0f;
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bLand = false;
	m_nAddDamage = 0;
	m_bValidAttack = false;
	m_nNumKillEnemy = 0;
	m_bLockAct = false;
	m_nNumShockWave = 0;
	m_nNumThunder = 0;

	m_bEndFadeColor = false;
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
	if (pPlayer == nullptr) return nullptr;

	pPlayer->CObjectMotion::SetPos(pos);
	pPlayer->Init();

	return pPlayer;
}

//=============================================================================
// ���[�V�����������[�h
//=============================================================================
void CPlayer::LoadMotionInfo(void) {
	//���[�h����O�ɓ��I�m�ۂ�����������delete����
	UnloadMotionInfo();

	LoadMotionText(TEXT_FILE_NAME_LOAD_MOTION, m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX);
}

//=============================================================================
// ���[�V���������A�����[�h
//=============================================================================
void CPlayer::UnloadMotionInfo(void) {
	m_nNumParts = 0;

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
	CObjectMotion::SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	m_destRot.y =  D3DX_PI;	//������

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�J�����̎擾
	CCamera* pCamera = nullptr;
	if (pManager != nullptr) pCamera = pManager->GetCamera();

	//�J�����̏����ݒ�
	if (pCamera != nullptr) {
		//pCamera->SetDistance(CAMERA_DISTANCE);
	}

	CObjectMotion::Init();
	SetMotion((int)MOTION_TYPE::NEUTRAL);	//���[�V�����̏����ݒ�

	return S_OK;
}

//=============================================================================
// �v���C���[�̏I������
//=============================================================================
void CPlayer::Uninit(void) {
	//�I������
	CObjectMotion::Uninit();
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
	CGameScene* pGame = nullptr;

	if (pManager != nullptr) {
		//���݂̓��̓f�o�C�X�̎擾
		pInput = pManager->GetInputCur();
		//�T�E���h�̎擾
		pSound = pManager->GetSound();
		//�J�����̎擾
		pCamera = pManager->GetCamera();
		//�Q�[���V�[���̎擾
		pGame = pManager->GetGameScene();
	}

	//�ʒu���̃|�C���^�̎擾
	D3DXVECTOR3* pPosPlayer = GetPtrPos();

	
	//----------------------------
	//�Q�[���I�[�o�[��
	//----------------------------
	if (pGame != nullptr) {
		//�Q�[���N���A��
		if (pGame->GetGameClear()) {
			//���[�V�����̃A�b�v�f�[�g
			CObjectMotion::Update();
			return;
		}
		//�Q�[���I�[�o�[��
		if (pGame->GetGameOver()) {
			//���łɃt�F�[�h�������Ă���ꍇ�I��
			if (m_bEndFadeColor) return;
			
			//�p�[�e�B�N���G�t�F�N�g�̐���
			CParticleEffect::PARTICLE_INFO particleInfo;	//�p�[�e�B�N�����
			particleInfo.addMove = D3DXVECTOR3(0.0f, 0.08f, 0.0f); particleInfo.colEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); particleInfo.colStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			particleInfo.fAddSize = -1.0f; particleInfo.fSizeStart = 60.0f; particleInfo.fSpeedMove = 3.5f; particleInfo.nLife = 60;
			//�P�񕪂̃G�t�F�N�g�𓯎��ɏd�˂ĕ\��
			CParticleEffect::Create(particleInfo, GetPos(), 1, 1, 0.45f * D3DX_PI, true);

			//�t�F�[�h�A�E�g
			m_bEndFadeColor = FadeModelAll(0.0f, -0.01f);
			return;
		}
	}

	//----------------------------
	//�J�����̐ݒ�
	//----------------------------
	float fRotCameraY = 0.0f;	//�J�����̊p�x
	if (pCamera != nullptr) {
		fRotCameraY = pCamera->GetRot().y;	//�J�����̊p�x���擾
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
	//�U��
	//----------------------------
	if (pInput != nullptr) {
		if (pInput->GetTrigger(CInput::CODE::ATTACK) && !m_bLockAct) {
			StartAttack();
		}
	}

	//----------------------------
	//�ړ�
	//----------------------------
	//����
	if (!m_bLand) {
		m_move.y -= POWER_GRAVITY;	//�d�͂����Z
	}

	//�ړ��Ɖ�]
	if (pInput != nullptr && GetMotionCategory() != MOTION_CATEGORY::ATTACK && !m_bLockAct) {
		Move(pInput, fRotCameraY);
	}

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
	Collision(pPosPlayer);

	//----------------------------
	//���[�V�����̍X�V
	//----------------------------
	CObjectMotion::Update();

	//----------------------------
	//���[�V�����I����
	//----------------------------
	if (GetEndMotion()) {
		EndMotion();
	}

	//----------------------------
	//���[�V�������̏���
	//----------------------------
	MotionAction();

	//----------------------------
	//�U���̃^�C�~���O
	//----------------------------
	if (m_bValidAttack) {
		//�v���C���[�̑S���f���̃��[���h�}�g���b�N�X���X�V
		UpdateMtxWorldAll();	//�����蔻��擾���Ɉʒu�̃Y���������邽��

		switch ((MOTION_TYPE)GetMotionType())
		{
		case MOTION_TYPE::PUNCH:
			AttackCollision(2, 4, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-50.0f, 0.0f, 0.0f), ATTACK_DAMAGE_PUNCH + m_nAddDamage);
			AttackCollision(4, 4, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 0.0f), ATTACK_DAMAGE_PUNCH + m_nAddDamage);
			break;

		case MOTION_TYPE::STAMP:
			AttackCollision(6, 3, 50.0f, D3DXVECTOR3(0.0f, 0.0f, -30.0f), D3DXVECTOR3(0.0f, -30.0f, 0.0f), ATTACK_DAMAGE_STANP + m_nAddDamage);
			AttackCollision(8, 3, 50.0f, D3DXVECTOR3(0.0f, 0.0f, -30.0f), D3DXVECTOR3(0.0f, -30.0f, 0.0f), ATTACK_DAMAGE_STANP + m_nAddDamage);
			break;
		}
	}
}

//=============================================================================
// �v���C���[�̕`�揈��
//=============================================================================
void CPlayer::Draw(void) {
	if (m_bEndFadeColor) return;	//�t�F�[�h�������͕`�悵�Ȃ�

	CObjectMotion::Draw();
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
}

//=============================================================================
// �v���C���[�̃Q�[���I�[�o�[���̏���
//=============================================================================
void CPlayer::GameOver(void) {
	//�ړ��ʂ��d�݂͂̂ɂ���
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bLockAct = false;
}

//=============================================================================
// �v���C���[�̓����蔻��̏��̎擾
//=============================================================================
void CPlayer::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//�����蔻��̐���ݒ�
	if (pNumCol != nullptr) *pNumCol = NUM_COLLISION;
	//�����蔻��̔��a��ݒ�
	if (pRadiusCol != nullptr) *pRadiusCol = COLLISION_RADIUS;
	//�����蔻��̈ʒu�̔z��̃|�C���^��null�̏ꍇ�I��
	if (ppPosColArray == nullptr) {
		return;
	}
	//pos�̃|�C���^����ł͂Ȃ��ꍇ�I��
	if (*ppPosColArray != nullptr) {
		return;
	}

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
// �U���J�n
//=============================================================================
void CPlayer::StartAttack(void) {
	//�܂��U�����Ă��Ȃ��ꍇ
	if (GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
		//���[�V�����̐ݒ�
		SetMotion((int)MOTION_TYPE::PUNCH);

		//�ړ��ʂ��d�݂͂̂ɂ���
		m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	}
	//�U�����̏ꍇ
	else {
		//1��ڂ̍U���̍Ō�̃L�[�̏ꍇ
		if (GetMotionType() == (int)MOTION_TYPE::PUNCH && (GetCurKey() == 2 && GetCurMotionCnt() > 8)) {
			//���[�V�����̐ݒ�
			SetMotion((int)MOTION_TYPE::STAMP);
		}
	}
}

//=============================================================================
// �ړ�
//=============================================================================
void CPlayer::Move(CInput* pInput, float fRotCameraY) {
	//�㉺���E�L�[���͏�Ԃ̎擾
	const bool bPressUp = pInput->GetPress(CInput::CODE::MOVE_UP);
	const bool bPressDown = pInput->GetPress(CInput::CODE::MOVE_DOWN);
	const bool bPressLeft = pInput->GetPress(CInput::CODE::MOVE_LEFT);
	const bool bPressRight = pInput->GetPress(CInput::CODE::MOVE_RIGHT);

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
	//if (pInput->GetPress(CInput::CODE::DASH)) {
	//	fMaxSpeed = MAX_MOVE_SPEED_DASH;
	//}
	//else {
		fMaxSpeed = MAX_MOVE_SPEED_WALK;
	//}
	//��
	if (!m_bLand) {
		fMaxSpeed = MAX_MOVE_SPEED_AIR;
	}
	//�ړ����x�̉��Z
	fMaxSpeed += m_fAddMoveSpeed;

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

		D3DXVECTOR3 rotPlayer = CObjectMotion::GetRot();//�p�x�̎擾
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
		CObjectMotion::SetRot(rotPlayer);
	}

	//------------------------
	//�n��ɂ��鎞�̃��[�V�����؂�ւ�
	//------------------------
	if (m_bLand && GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
		//�ړ��L�[�������Ă��鎞�̃��[�V�����؂�ւ�
		if (bInputMove) {
			if (GetMotionCategory() != MOTION_CATEGORY::MOVE) {
				SetMotion((int)MOTION_TYPE::WALK);
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
}

//=============================================================================
// �����蔻��
//=============================================================================
void CPlayer::Collision(D3DXVECTOR3* pPos) {
	//�ǂƂ̓����蔻��
	CWallCylinder::Collision(pPos, m_lastPos, COLLISION_RADIUS);

	//�n�ʂƂ̓����蔻��
	bool bLand = false;	//�ڒn���Ă��邩�ǂ���
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//�ڐG�ʒu�A�J�n�x�N�g���A�I���x�N�g��
	vecStart = *pPos;	//�v���C���[�̈ʒu���擾
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//������̃x�N�g��

	bLand = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);

	//�ڒn��
	if (bLand) {
		*pPos = posColTerrain;	//�ʒu�̈ړ�
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
}

//=============================================================================
// ���[�V�����I����
//=============================================================================
void CPlayer::EndMotion(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//--------------------------
		//�U�����[�V����
		//--------------------------
		//�d������̃��[�V����
	case MOTION_TYPE::PUNCH:
	case MOTION_TYPE::STAMP:
		m_bLockAct = true;	//�d���̐ݒ�

		//�d���Ȃ��̃��[�V����
	//case MOTION_TYPE::ENUM_MAX:
		SetMotion((int)MOTION_TYPE::NEUTRAL);
		//�ړ��ʂ��d�݂͂̂ɂ���
		m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);

		m_bValidAttack = false;
		break;
	}
}

//=============================================================================
// ���[�V�������̏���
//=============================================================================
void CPlayer::MotionAction(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	bool bBeginAttack = false;	//�U���J�n�����ǂ���

	switch ((MOTION_TYPE)GetMotionType())
	{

		//-------------------------
		//�ړ�
		//-------------------------
	case MOTION_TYPE::WALK:
		//�����n�ʂɐG�ꂽ�^�C�~���O�������ꍇ
		if ((GetCurKey() == 0 && GetCurMotionCnt() == 14 && !GetTransMotion()) || (GetCurKey() == 2 && GetCurMotionCnt() == 14)) {
			//���s���̉�
			if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::FOOTSTEP);

			//�������ɃG�t�F�N�g�𐶐�
			CParticleEffect::PARTICLE_INFO particleInfo;	//�p�[�e�B�N�����
			particleInfo.addMove = D3DXVECTOR3(0.0f, -0.5f, 0.0f); particleInfo.colEnd = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f); particleInfo.colStart = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
			particleInfo.fAddSize = -1.0f; particleInfo.fSizeStart = 30.0f; particleInfo.fSpeedMove = 6.5f; particleInfo.nLife = 30;

			D3DXVECTOR3 posFoot;//�������Ă�����̑��̃G�t�F�N�g�̈ʒu
			int nIdxModel = 0;	//���̃C���f�b�N�X

			if (GetCurKey() == 0) {
				nIdxModel = 6;	//�E��
			}
			else if (GetCurKey() == 2) {
				nIdxModel = 8;	//����
			}
			//���̈ʒu�̎擾
			GetPosCollision(&posFoot, D3DXVECTOR3(0.0f, -85.0f, 5.0f), nIdxModel);
			//�P�񕪂̃G�t�F�N�g�𓯎��ɏd�˂ĕ\��
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				CParticleEffect::Create(particleInfo, posFoot, 2, 1, 0.08f * D3DX_PI, true);
			}

			//�Ռ��g����
			//CShockWaveEmitter::Create(m_nNumShockWave, 4, posFoot, 30.0f, 2.0f, 4.0f, 40.0f, -1.0f, 15, D3DX_PI * 0.02f);
		
		}
		break;

		//-------------------------
		//�p���`�̍U��
		//-------------------------
		//�p���`
	case MOTION_TYPE::PUNCH:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurKey() == 1 || (GetCurKey() == 2 && GetCurMotionCnt() == 0)) && !GetTransMotion()) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
		}
		else {
			m_bValidAttack = false;
		}
		//�U�����n�ʂɓ��������^�C�~���O�������ꍇ
		if (GetCurKey() == 1 && GetCurMotionCnt() == 5) {
			//�Ռ���
			if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SHOCK_PUNCH);
			//�Ռ��g����
			D3DXVECTOR3 posWave = GetPos() + D3DXVECTOR3(sinf(GetRot().y + D3DX_PI), 0.0f, cosf(GetRot().y + D3DX_PI)) * 200.0f;	//�v���C���[�̑O��
			CShockWaveEmitter::Create(m_nNumShockWave + 1, 4, posWave, 40.0f, 10.0f, 15.0f, 60.0f, -1.0f, 18, D3DX_PI * 0.02f);

			//���̐���
			CThunderEmitter::CreateStraight(m_nNumThunder, 5, posWave, posWave - GetPos(), 100.0f);
		}
		break;

		//���݂�
	case MOTION_TYPE::STAMP:
		//�U���̃^�C�~���O�������ꍇ
		if ((GetCurKey() == 1 || (GetCurKey() == 2 && GetCurMotionCnt() == 0)) && !GetTransMotion()) {
			//�U���J�n��
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
		}
		else {
			m_bValidAttack = false;
		}
		//�U�����n�ʂɓ��������^�C�~���O�������ꍇ
		if (GetCurKey() == 1 && GetCurMotionCnt() == 3) {
			//�Ռ���
			if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SHOCK_STAMP);
			//�Ռ��g����
			D3DXVECTOR3 posWave = GetPos();
			CShockWaveEmitter::Create(m_nNumShockWave + 2, 4, posWave, 40.0f, 10.0f, 15.0f, 80.0f, -1.0f, 22, D3DX_PI * 0.02f);
			//���̐���
			if(m_nNumThunder > 0) CThunderEmitter::CreateRound(m_nNumThunder + 1, 3, posWave, GetRot().y + D3DX_PI, 300.0f);
		}
		break;

	default:
		m_bValidAttack = false;
		break;
	}

	//�U���J�n��
	if (bBeginAttack) {
		AttackBegin();
	}
}

//=============================================================================
// �U���̓����蔻��
//=============================================================================
void CPlayer::AttackCollision(int nIdxModel, const int nNumCol, float fRadiusCol, D3DXVECTOR3 posOffsetStart, D3DXVECTOR3 posOffsetAdd, int nDamage) {
	D3DXVECTOR3* pPosColArray = new D3DXVECTOR3[nNumCol];	//�����蔻��̈ʒu�𐶐�
	int nNumKillEnemy = 0;	//�G��|������

	D3DXVECTOR3 posOffset = posOffsetStart;	//���f������̃I�t�Z�b�g

	for (int nCntCol = 0; nCntCol < nNumCol; nCntCol++)
	{
		//�����蔻��̈ʒu�̎擾
		GetPosCollision(&pPosColArray[nCntCol], posOffset, nIdxModel);
		//�I�t�Z�b�g�����Z
		posOffset += posOffsetAdd;
	}

	for (int nCntCol = 0; nCntCol < nNumCol; nCntCol++)
	{
		//�U��
		CObjectMotion::Attack(OBJTYPE_ENEMY | OBJTYPE_APPLE_TREE, pPosColArray[nCntCol], fRadiusCol, nDamage, DAMAGE_TYPE::PLAYER_PUNCH, &nNumKillEnemy);	//�G
	}

	//�U���̐�[�Ƀp�[�e�B�N���̐���
	CParticle::Create(pPosColArray[nNumCol - 1], 20, 55.0f, -1.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	delete[] pPosColArray;	//���I�m�ۂ��������蔻��̈ʒu�̃����������

	//�G��|�������̉��Z
	m_nNumKillEnemy += nNumKillEnemy;
}

//=============================================================================
// �U���J�n��
//=============================================================================
void CPlayer::AttackBegin(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//�U���ς݃��X�g�̏�����
	InitObjAttacked();	

	switch ((MOTION_TYPE)GetMotionType())
	{
	case MOTION_TYPE::PUNCH:
	case MOTION_TYPE::STAMP:
		//����U�鉹�̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_PUNCH);
		break;
	}
}

//=============================================================================
// ���[�V�����̋敪�̎擾
//=============================================================================
CPlayer::MOTION_CATEGORY CPlayer::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�ړ�
	case MOTION_TYPE::WALK:
		return MOTION_CATEGORY::MOVE;
		break;

		//�U��
	case MOTION_TYPE::PUNCH:
	case MOTION_TYPE::STAMP:
		return MOTION_CATEGORY::ATTACK;
			break;

	//case MOTION_TYPE::DODGE_BACK:
	//case MOTION_TYPE::DODGE_FRONT:
		//return MOTION_CATEGORY::DODGE;
		//break;
		//���̑�
	default:
		return MOTION_CATEGORY::NONE;
		break;
	}
}