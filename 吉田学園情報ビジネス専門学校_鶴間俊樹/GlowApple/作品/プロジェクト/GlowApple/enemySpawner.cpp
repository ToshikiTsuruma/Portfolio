//=============================================================================
//
// �G�X�|�i�[���� [enemySpawner.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "enemySpawner.h"
#include "manager.h"
#include "sound.h"
#include "enemy.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemySpawner::CEnemySpawner()
{
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
		CEnemy::SetEnemyCircle(D3DXVECTOR3(0.0f, 1500.0f, 0.0f), 1, 1800.0f, m_nDistEnemyMin, m_nDistEnemyMax);

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

//�G�̐����Ԋu�𑝉�
void CEnemySpawner::AddDistEnemy(int nDist) {
	m_nDistEnemyMin += nDist;
	m_nDistEnemyMax += nDist;
}