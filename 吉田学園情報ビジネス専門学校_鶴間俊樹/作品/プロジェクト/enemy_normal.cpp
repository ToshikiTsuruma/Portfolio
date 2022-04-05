//=============================================================================
//
// �ʏ�G���� [enemy_normal.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "enemy_normal.h"
#include "particle.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_enemyNormal.txt"
#define NUM_COLLISION (1)			//�����蔻��̐�
#define COLLISION_RADIUS (25.0f)	//�����蔻��̔��a
#define MAX_LIFE (6)				//�̗͂̍ő�l
#define DISTANCE_ATTACK (50.0f)		//�U�����鋗��
#define ADD_TIME (10)	//�|�����Ƃ��̎��Ԓǉ�

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CEnemyNormal::m_nNumParts = 0;
CSceneMotion::PARTS_INFO* CEnemyNormal::m_pPartsInfoArray = nullptr;
CSceneMotion::MOTION_INFO CEnemyNormal::m_aMotionInfo[(int)CEnemyNormal::MOTION_TYPE::ENUM_MAX] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemyNormal::CEnemyNormal() : CEnemy(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, true, MAX_LIFE, DISTANCE_ATTACK, ADD_TIME)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemyNormal::~CEnemyNormal()
{

}

//=============================================================================
// �ʏ�G�̐�������
//=============================================================================
CEnemyNormal* CEnemyNormal::Create(D3DXVECTOR3 pos) {
	CEnemyNormal* pEnemyNormal;
	pEnemyNormal = new CEnemyNormal();
	if (pEnemyNormal != nullptr) {
		pEnemyNormal->SetPos(pos);
		pEnemyNormal->Init();
	}

	return pEnemyNormal;
}

//=============================================================================
// �ʏ�G�̃��[�V�����������[�h
//=============================================================================
void CEnemyNormal::LoadMotionInfo(void) {
	//���[�h����O�ɓ��I�m�ۂ�����������delete����
	UnloadMotionInfo();

	Load(TEXT_FILE_NAME_LOAD_MOTION, &m_pPartsInfoArray, &m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, nullptr);
}

//=============================================================================
// �ʏ�G�̃��[�V���������A�����[�h
//=============================================================================
void CEnemyNormal::UnloadMotionInfo(void) {
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
// �ʏ�G�̏���������
//=============================================================================
HRESULT CEnemyNormal::Init(void) {
	SetMotion((int)MOTION_TYPE::NEUTRAL);	//���[�V�����̏����ݒ�

	//�G�̏���������
	CEnemy::Init();
	return S_OK;
}

//=============================================================================
// �ʏ�G�̏I������
//=============================================================================
void CEnemyNormal::Uninit(void) {
	//�I������
	CEnemy::Uninit();
}

//=============================================================================
// �ʏ�G�̍X�V����
//=============================================================================
void CEnemyNormal::Update(void) {
	//�G�̍X�V
	CEnemy::Update();

}

//=============================================================================
// �ʏ�G�̕`�揈��
//=============================================================================
void CEnemyNormal::Draw(void) {
	CEnemy::Draw();
}

//=============================================================================
// �ʏ�G�̓����蔻��̏��̎擾
//=============================================================================
void CEnemyNormal::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
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

			D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���f����pos����̃I�t�Z�b�g�̎n�_
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
// ���[�V�����̋敪�̎擾
//=============================================================================
CEnemy::MOTION_CATEGORY CEnemyNormal::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�j���[�g����
	case MOTION_TYPE::NEUTRAL:
	default:
		return MOTION_CATEGORY::NEUTRAL;

		//�ړ�
	case MOTION_TYPE::MOVE:
		return MOTION_CATEGORY::MOVE;

		//�U��
	case MOTION_TYPE::ATTACK:
		return MOTION_CATEGORY::ATTACK;
	}
}

//=============================================================================
// �ړ����̃��[�V������ݒ�
//=============================================================================
void CEnemyNormal::SetMoveMotion(void) {
	SetMotion((int)MOTION_TYPE::MOVE);
}

//=============================================================================
// �ǐՎ��̃��[�V������ݒ�
//=============================================================================
void CEnemyNormal::SetChaseMotion(void) {
	if (GetMotionType() != (int)MOTION_TYPE::MOVE) {
		SetMotion((int)MOTION_TYPE::MOVE);
	}
}

//=============================================================================
// ���S���̃��[�V������ݒ�
//=============================================================================
void CEnemyNormal::SetDeadMotion(void) {
	SetMotion((int)MOTION_TYPE::DEAD);
}

//=============================================================================
// �U���J�n��
//=============================================================================
bool CEnemyNormal::AttackStart(void) {
	//�U�����[�V�����ȊO�̂Ƃ�
	if (GetMotionType() != (int)MOTION_TYPE::ATTACK) {
		//�U�����[�V�����̐ݒ�
		SetMotion((int)MOTION_TYPE::ATTACK);
		//�U�����[�V�������ɉ�]�\
		SetRotateAttack(true);
		return true;
	}
	return false;
}

//=============================================================================
// ���[�V�����I����
//=============================================================================
void CEnemyNormal::MotionEnd(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�U�����[�V����
	case MOTION_TYPE::ATTACK:
		SetMotion((int)MOTION_TYPE::NEUTRAL);
		//�U�����[�V�������̉�]�𐧌�
		SetRotateAttack(false);
		break;
	}
}

//=============================================================================
// ���[�V�������̍s��
//=============================================================================
void CEnemyNormal::MotionAct(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�U��
	case MOTION_TYPE::ATTACK:
		//�U���̃^�C�~���O
		if ((GetCurrentKey() == 1 || GetCurrentKey() == 2) && !GetTransMotion()) {
			const int nNumCollision = 3;	//�U���̓����蔻��̐�
			D3DXVECTOR3 aPosCol[nNumCollision] = {};	//�����蔻��̈ʒu
			int nIdxModel = 2;	//�E��
			D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���f������̃I�t�Z�b�g

			for (int nCntCol = 0; nCntCol < nNumCollision; nCntCol++)
			{
				//�����蔻��̈ʒu�̎擾
				GetPosCollision(&aPosCol[nCntCol], posOffset, nIdxModel);
				//�I�t�Z�b�g�����Z
				posOffset += D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
			}

			//�����蔻��̐����U������
			for (int nCntAttack = 0; nCntAttack < nNumCollision; nCntAttack++)
			{
				//�U��
				CSceneMotion::Attack(OBJ_TYPE::PLAYER, aPosCol[nCntAttack], 10.0f, 1, DAMAGE_TYPE::ENEMY, nullptr);
			}
			//�U���̐�[�Ƀp�[�e�B�N���̐���
			//CParticle::Create(aPosCol[nNumCollision - 1], 20.0f, 1.0f, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));
		}

		if (GetCurrentKey() >= 1) {
			//�U�����[�V�������̉�]�𐧌�
			SetRotateAttack(false);
		}
		break;	//_�U��
	}
}