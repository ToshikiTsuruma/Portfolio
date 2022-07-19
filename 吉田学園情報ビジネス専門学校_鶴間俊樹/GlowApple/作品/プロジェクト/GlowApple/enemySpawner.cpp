//=============================================================================
//
// �G�X�|�i�[���� [enemySpawner.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "enemySpawner.h"
#include "manager.h"
#include "sound.h"
#include "enemy_normal.h"
#include "enemy_human.h"
#include "enemy_sorcerer.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemySpawner::CEnemySpawner()
{
	m_nLevel = 0;
	m_nCntSpawn = 0;
	m_nSpanSpawn = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemySpawner::~CEnemySpawner()
{

}

//=============================================================================
// �G�X�|�i�[�̐�������
//=============================================================================
CEnemySpawner* CEnemySpawner::Create(int nSpan, int nDistMin, int nDistMax) {
	CEnemySpawner* pEnemySpawner;
	pEnemySpawner = new CEnemySpawner();
	if (pEnemySpawner == nullptr) return nullptr;

	pEnemySpawner->m_nSpanSpawn = nSpan;
	pEnemySpawner->m_nDistEnemyMin = nDistMin;
	pEnemySpawner->m_nDistEnemyMax = nDistMax;
	pEnemySpawner->Init();

	return pEnemySpawner;
}

//=============================================================================
// �G�X�|�i�[�̏���������
//=============================================================================
HRESULT CEnemySpawner::Init(void) {
	m_nCntSpawn = 0;

	return S_OK;
}

//=============================================================================
// �G�X�|�i�[�̏I������
//=============================================================================
void CEnemySpawner::Uninit(void) {
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �G�X�|�i�[�̍X�V����
//=============================================================================
void CEnemySpawner::Update(void) {
	//�����^�C�~���O
	if (m_nCntSpawn > m_nSpanSpawn) {
		m_nCntSpawn = 0;

		//�G���X�|�[��������
		SetEnemyCircle(D3DXVECTOR3(0.0f, 1500.0f, 0.0f), 1, 1800.0f, m_nDistEnemyMin, m_nDistEnemyMax);

		//�}�l�[�W���[�̎擾
		CManager* pManager = CManager::GetManager();
		//�T�E���h�̎擾
		CSound* pSound = nullptr;
		if (pManager != nullptr) pSound = pManager->GetSound();
		//�X�|�[�����̍Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::SPAWN_ENEMY);
	}
	else {
		m_nCntSpawn++;
	}
}


//=============================================================================
// ���S�~��Ƀ����_���Ɋp�x���炵�ēG��z�u����
//=============================================================================
//�����F���S�̈ʒu�@�~�̐��@�~���Ƃ̋����i���a�j�@�i�����~�̈�O�̓G�Ƃ́j�ŒZ�����A�Œ�����
void CEnemySpawner::SetEnemyCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fDistCircle, int nMinDist, int nMaxDist) {
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
			posCreate.x += sinf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			posCreate.z += cosf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);

			int aRateType[(int)ENEMYTYPE::ENUM_MAX] = {};	//��ނ��Ƃ̐����m��
			//�����m�����擾
			GetSpawnRate(m_nLevel, aRateType);
			//���ꂼ��̃^�C�v�ȑO�̃^�C�v�̊����̍��v�𑫂�
			for (int nCnt = 1; nCnt < (int)ENEMYTYPE::ENUM_MAX; nCnt++)
			{
				aRateType[nCnt] += aRateType[nCnt - 1];
			}

			//�G�̎�ނ������_���Ō��߂�
			int nRandType = rand() % 100;
			//�G�̐���
			if (nRandType < aRateType[(int)ENEMYTYPE::NORMAL]) {
				//�ʏ�G
				CEnemyNormal::Create(posCreate);
			}
			else if (nRandType < aRateType[(int)ENEMYTYPE::HUMAN]) {
				//�l�`�G
				CEnemyHuman::Create(posCreate);
			}
			else if (nRandType < aRateType[(int)ENEMYTYPE::SORCERER]) {
				//���p�t�G
				CEnemySorcerer::Create(posCreate);
			}
		}
	}
}

//=============================================================================
// �G�̐����Ԋu�𑝉�
//=============================================================================
void CEnemySpawner::AddDistEnemy(int nDist) {
	m_nDistEnemyMin += nDist;
	m_nDistEnemyMax += nDist;
}

//=============================================================================
// ���x�����Ƃ̐����m���̎擾
//=============================================================================
void CEnemySpawner::GetSpawnRate(int nLevel, int* pRateArray) {
	if (pRateArray == nullptr) return;
		 
	switch (nLevel)
	{
	case 0:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 100;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 0;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 0;
		break;

	case 1:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 60;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 40;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 0;
		break;

	case 2:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 30;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 65;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 5;
		break;

	case 3:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 10;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 80;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 10;
		break;

	default:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 100;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 0;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 0;
		break;
	}
}