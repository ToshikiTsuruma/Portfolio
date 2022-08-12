//=============================================================================
//
// ���p�t�G���� [enemy_sorcerer.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "enemy_sorcerer.h"
#include "effect.h"
#include "manager.h"
#include "gameScene.h"
#include "appleTree.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_enemySorcerer.txt"
#define MOVE_SPEED (0.5f)					//�ړ����x
#define ROTATE_SPEED (0.01f * D3DX_PI)		//��]���x
#define NUM_COLLISION (5)			//�����蔻��̐�
#define COLLISION_RADIUS (30.0f)	//�����蔻��̔��a
#define MAX_LIFE (200)				//�̗͂̍ő�l
#define DISTANCE_ATTACK (500.0f)	//�U�����鋗��
#define ATTACK_EXPLOSION_DAMAGE (150)	//�����̍U����
#define LIFE_GAUGE_HEIGHT (200.0f)	//�G�̈ʒu����̗̑̓Q�[�W�̍���

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CEnemySorcerer::m_nNumParts = 0;
CObjectMotion::PARTS_INFO* CEnemySorcerer::m_pPartsInfoArray = nullptr;
CObjectMotion::MOTION_INFO CEnemySorcerer::m_aMotionInfo[(int)CEnemySorcerer::MOTION_TYPE::ENUM_MAX] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemySorcerer::CEnemySorcerer() : CEnemy(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, MOVE_SPEED, ROTATE_SPEED, MAX_LIFE, DISTANCE_ATTACK, LIFE_GAUGE_HEIGHT)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemySorcerer::~CEnemySorcerer()
{

}

//=============================================================================
// ���p�t�G�̐�������
//=============================================================================
CEnemySorcerer* CEnemySorcerer::Create(D3DXVECTOR3 pos) {
	CEnemySorcerer* pEnemySorcerer;
	pEnemySorcerer = new CEnemySorcerer();
	if (pEnemySorcerer != nullptr) {
		pEnemySorcerer->SetPos(pos);
		pEnemySorcerer->Init();
	}

	return pEnemySorcerer;
}

//=============================================================================
// ���[�V�����������[�h
//=============================================================================
void CEnemySorcerer::LoadMotionInfo(void) {
	//���[�h����O�ɓ��I�m�ۂ�����������delete����
	UnloadMotionInfo();

	LoadMotionText(TEXT_FILE_NAME_LOAD_MOTION, m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX);
}

//=============================================================================
// ���[�V���������A�����[�h
//=============================================================================
void CEnemySorcerer::UnloadMotionInfo(void) {
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
// ���p�t�G�̏���������
//=============================================================================
HRESULT CEnemySorcerer::Init(void) {
	SetMotion((int)MOTION_TYPE::NEUTRAL);	//���[�V�����̏����ݒ�

	//�G�̏���������
	CEnemy::Init();
	return S_OK;
}

//=============================================================================
// ���p�t�G�̏I������
//=============================================================================
void CEnemySorcerer::Uninit(void) {
	//�΂̋ʂ̔j��
	if (m_pFireBoll != nullptr) {
		if (m_pFireBoll != nullptr) {
			m_pFireBoll->Uninit();
			m_pFireBoll = nullptr;
		}
	}
	//�I������
	CEnemy::Uninit();
}

//=============================================================================
// ���p�t�G�̍X�V����
//=============================================================================
void CEnemySorcerer::Update(void) {
	//�G�̍X�V
	CEnemy::Update();
}

//=============================================================================
// ���p�t�G�̕`�揈��
//=============================================================================
void CEnemySorcerer::Draw(void) {
	CEnemy::Draw();
}

//=============================================================================
// ���p�t�G�̓����蔻��̏��̎擾
//=============================================================================
void CEnemySorcerer::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//�K�v�ȃ|�C���^��null�̏ꍇ�I��
	if (pNumCol == nullptr || pRadiusCol == nullptr || ppPosColArray == nullptr) return;

	//pos�̃|�C���^�����łɑ��݂��Ă���ꍇ�I��
	if (*ppPosColArray != nullptr) return;

	//�����蔻��̐���ݒ�
	*pNumCol = NUM_COLLISION;
	//�����蔻��̔��a��ݒ�
	*pRadiusCol = COLLISION_RADIUS;
	//�����蔻��̈ʒu�̔z���ݒ�
	*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];	//���I�m��

	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���f����pos����̃I�t�Z�b�g�̎n�_
	int nIdxModel = 0;	//����

	for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
	{
		//�����蔻��̈ʒu�̎擾
		GetPosCollision(*ppPosColArray + nCntCol, posOffset, nIdxModel);
		posOffset += D3DXVECTOR3(0.0f, 20.0f, 0.0f);	//�I�t�Z�b�g�����Z
	}
}

//=============================================================================
// ���[�V�����̋敪�̎擾
//=============================================================================
CEnemy::MOTION_CATEGORY CEnemySorcerer::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�j���[�g����
	case MOTION_TYPE::NEUTRAL:
	default:
		return MOTION_CATEGORY::NEUTRAL;

		//�U��
	case MOTION_TYPE::ATTACK:
		return MOTION_CATEGORY::ATTACK;

		//���S
	case MOTION_TYPE::DEAD:
		return MOTION_CATEGORY::DEAD;
	}
}

//=============================================================================
// ���S���̃��[�V������ݒ�
//=============================================================================
void CEnemySorcerer::SetDeadMotion(void) {
	//�΂̋ʂ̔j��
	if (m_pFireBoll != nullptr) {
		if (m_pFireBoll != nullptr) {
			m_pFireBoll->Uninit();
			m_pFireBoll = nullptr;
		}
	}
	//���S���[�V�����ݒ�
	SetMotion((int)MOTION_TYPE::DEAD);
}

//=============================================================================
// �U���J�n��
//=============================================================================
void CEnemySorcerer::AttackStart(void) {
	//�U�����[�V�����̐ݒ�
	SetMotion((int)MOTION_TYPE::ATTACK);
}

//=============================================================================
// �U����~
//=============================================================================
void CEnemySorcerer::AttackStop(void) {
	//�΂̋ʂ̔j��
	if (m_pFireBoll != nullptr) {
		if (m_pFireBoll != nullptr) {
			m_pFireBoll->Uninit();
			m_pFireBoll = nullptr;
		}
	}
}

//=============================================================================
// ���[�V�����I����
//=============================================================================
void CEnemySorcerer::MotionEnd(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�U�����[�V����
	case MOTION_TYPE::ATTACK:
		SetMotion((int)MOTION_TYPE::NEUTRAL);
		break;
	}
}

//=============================================================================
// ���[�V�������̍s��
//=============================================================================
void CEnemySorcerer::MotionAct(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�T�E���h�̎擾
	CSound* pSound = pManager->GetSound();
	if (pSound == nullptr) return;
	//�Q�[���V�[���̎擾
	CGameScene* pGame = pManager->GetGameScene();
	if (pGame == nullptr) return;
	//�ь�̖؂̎擾
	CObject* pAppleTree = pGame->GetAppleTree();
	if (pAppleTree == nullptr) return;
	D3DXVECTOR3 posAppleTree = pAppleTree->GetPos();//�؂̈ʒu���擾

	D3DXVECTOR3 posFireBoll = posAppleTree;
	posFireBoll.y += 650.0f;	//�؂̏�

	switch ((MOTION_TYPE)GetMotionType())
	{
		//�U��
	case MOTION_TYPE::ATTACK:

		if (GetCurKey() == 0 && !GetTransMotion()) {
			if (GetCurMotionCnt() == 0) {
				//�΂̋ʐ���
				m_pFireBoll = CEffect::Create(posFireBoll, CEffect::EFFECT_TYPE::FIRE_BOLL, 50.0f, 50.0f, true);
				//�������x��ݒ�
				if (m_pFireBoll != nullptr) m_pFireBoll->SetMove(D3DXVECTOR3(0.0f, -0.5f, 0.0f));
			}
			//�Ă������Đ�
			if (GetCurMotionCnt() == 10) pSound->PlaySound(CSound::SOUND_LABEL::DAMAGE_FIRE);
			//�΂̋ʖc��
			if (m_pFireBoll != nullptr) m_pFireBoll->AddSize(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
		}
		//�΂̋ʖc�� & �_���[�W
		if (GetCurKey() == 1) {
			if (GetCurMotionCnt() == 0) {
				//�������x��ݒ�
				if (m_pFireBoll != nullptr) m_pFireBoll->SetMove(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
			}
			//�T�C�Y�̖c��
			if (m_pFireBoll != nullptr) m_pFireBoll->AddSize(D3DXVECTOR3(2.0f, 2.0f, 0.0f));
			//���킶��_���[�W��^����
			pAppleTree->Damage(1, DAMAGE_TYPE::FIRE, nullptr);			
		}
		//����
		if (GetCurKey() == 3 && GetCurMotionCnt() == 0) {
			if (m_pFireBoll != nullptr) {
				//�����G�t�F�N�g
				CEffect::Create(m_pFireBoll->GetPos(), CEffect::EFFECT_TYPE::EXPLOSION, 300.0f, 300.0f, false);
				//�΂̋ʂ̔j��
				if (m_pFireBoll != nullptr) {
					m_pFireBoll->Uninit();
					m_pFireBoll = nullptr;
				}
			}
			//�����_���[�W
			pAppleTree->Damage(ATTACK_EXPLOSION_DAMAGE, DAMAGE_TYPE::EXPLOSION, nullptr);
			//������
			pSound->PlaySound(CSound::SOUND_LABEL::EXPLOSION);
		}
		break;	//_�U��
	}
}