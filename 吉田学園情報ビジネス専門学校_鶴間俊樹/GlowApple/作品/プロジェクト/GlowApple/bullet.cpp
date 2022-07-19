//=============================================================================
//
// �e���� [bullet.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "bullet.h"
#include "particle.h"
#include "manager.h"
#include "sound.h"
#include "effect.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_ATTACK_DISTANCE (500.0f)	//�I�u�W�F�N�g�ɍU���\�ȍő�̋���

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CBullet::CBullet()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CBullet::CBullet(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos) : CObjectModel(typeModel, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
{
	m_bCreateParticleFirst = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// �e�̐�������
//=============================================================================
CBullet* CBullet::Create(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, int nObjtypeAttack, float fRadiusCol, int nDamage) {
	CBullet* pBullet;
	pBullet = new CBullet(typeModel, pos);
	if (pBullet == nullptr) return nullptr;

	pBullet->m_move = move;
	pBullet->m_nLife = nLife;
	pBullet->m_nObjtypeAttack = nObjtypeAttack;
	pBullet->m_fRadiusCol = fRadiusCol;
	pBullet->m_nDamage = nDamage;

	pBullet->Init();

	return pBullet;
}

//=============================================================================
// �e�̏���������
//=============================================================================
HRESULT CBullet::Init(void) {
	m_bCreateParticleFirst = false;

	CObjectModel::Init();
	return S_OK;
}

//=============================================================================
// �e�̏I������
//=============================================================================
void CBullet::Uninit(void) {

	CObjectModel::Uninit();
}

//=============================================================================
// �e�̍X�V����
//=============================================================================
void CBullet::Update(void) {
	//���C�t�̌���
	if (m_nLife <= 0) {
		//�I������
		Uninit();
		return;
	}
	else {
		m_nLife--;
	}

	//�e�̈ړ�
	SetPos(GetPos() + m_move);

	//�����蔻��
	bool bHit = AttackCollision();
	//�������Ă���ꍇ
	if (bHit) {
		//�I������
		Uninit();
		return;
	}

	//�p�[�e�B�N������
	if (m_bCreateParticle) {
		D3DXVECTOR3 posParticle = GetPos();

		//�⊮�p�p�[�e�B�N���̐���
		if (m_bCreateParticleFirst) {
			D3DXVECTOR3 posParticle2 = (m_posLastParticle - posParticle) / 2.0f + posParticle;	//�⊮�p�[�e�B�N���̈ʒu
			CParticle::Create(posParticle2, m_nLifeParticle, m_fSizeParticle + (m_fAddSizeParticle / 2.0f), m_fAddSizeParticle, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_colStartParticle, m_colEndParticle);
		}

		CParticle::Create(posParticle, m_nLifeParticle, m_fSizeParticle, m_fAddSizeParticle, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_colStartParticle, m_colEndParticle);
		//����̐����ʒu��ۑ�
		m_posLastParticle = posParticle;
		m_bCreateParticleFirst = true;
	}

	CObjectModel::Update();
}

//=============================================================================
// �e�̕`�揈��
//=============================================================================
void CBullet::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// �p�[�e�B�N������ݒ�
//=============================================================================
void CBullet::SetParticleInfo(int nLife, float fSize, float fAddSize, D3DXCOLOR colStart, D3DXCOLOR colEnd) {
	m_bCreateParticle = true;

	m_nLifeParticle = nLife;
	m_fSizeParticle = fSize;
	m_fAddSizeParticle = fAddSize;
	m_colStartParticle = colStart;
	m_colEndParticle = colEnd;
}

//=============================================================================
// �U���Ώۂւ̓����蔻��
//=============================================================================
bool CBullet::AttackCollision(void) {
	CObject* pObject = GetObjectTopAll();	//�S�I�u�W�F�N�g�̃��X�g�̐擪���擾
	D3DXVECTOR3 posBullet = GetPos();	//�e�̈ʒu

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾

		//�I�u�W�F�N�g�^�C�v�̊m�F
		bool bMatchType = false;
		if (pObject->GetObjType() & m_nObjtypeAttack) bMatchType = true;

		//���S��Ԃ̎擾
		bool bDeath = pObject->GetDeath();

		//�����蔻��\��Ԃ̎擾
		bool bEnableCollision = pObject->GetEnableCollision();

		//�U���\�Ȕ͈͂ɂ��邩�ǂ���
		D3DXVECTOR2 vecObj = D3DXVECTOR2(pObject->GetPos().x - posBullet.x, pObject->GetPos().z - posBullet.z);	//�U���ʒu�ƃI�u�W�F�N�g�̃x�N�g��
		float fDistObj = D3DXVec2Length(&vecObj);	//�U���ʒu�ƃI�u�W�F�N�g�̋���

		//���̃��[�v�ɔ�΂�
		if (!bMatchType || bDeath || !bEnableCollision || fDistObj > MAX_ATTACK_DISTANCE) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;	
		}

		//---------------------------
		//�����蔻��
		//---------------------------
		bool bCollision = false;	//�����蔻��̃t���O
		D3DXVECTOR3 posHit;	//���������ʒu

		//�����蔻�肪����I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X���X�V
		pObject->UpdateMtxWorldAll();	//�����蔻��擾���Ɉʒu�̃Y���������邽��

		//�����蔻�肪����p�[�c�̐��̎擾
		int nNumColParts = pObject->GetNumCollisionParts();

		//�p�[�c���Ƃɓ����蔻��
		for (int nCntColParts = 0; nCntColParts < nNumColParts; nCntColParts++)
		{
			int nNumCollision = 0;		//�����蔻��̐�
			D3DXVECTOR3* pPosColArray = nullptr;	//�����蔻��̈ʒu�̔z��̃|�C���^
			float fRadiusCol = 0.0f;	//�����蔻��̔��a

			//�����蔻��̏��̎擾
			pObject->GetCollisionInfo(nCntColParts, &nNumCollision, &pPosColArray, &fRadiusCol);

			//�ʒu���̃��������m�ۂ���Ă��Ȃ��ꍇ�I��
			if (pPosColArray == nullptr) continue;

			for (int nCntCol = 0; nCntCol < nNumCollision; nCntCol++)
			{
				//���Ƌ��̓����蔻��
				D3DXVECTOR3 vecCol = pPosColArray[nCntCol] - posBullet;	//�e���瓖���蔻��̋��ւ̃x�N�g��
				float fDistCol = D3DXVec3Length(&vecCol);	//�����蔻�蓯�m�̋���
				//�����Ɠ����蔻�蓯�m�̋������r
				if (fDistCol < m_fRadiusCol + fRadiusCol) {
					bCollision = true;
					posHit = pPosColArray[nCntCol];
					break;
				}
			}

			//�ʒu���̔j��
			delete[] pPosColArray;		

			//���łɓ������Ă����瓖���蔻��̃��[�v�I��
			if (bCollision) break;
		}

		//---------------------------
		//�����蔻�莞�̏���
		//---------------------------
		if (bCollision) {
			//�I�u�W�F�N�g�Ƀ_���[�W��^����
			pObject->Damage(m_nDamage, DAMAGE_TYPE::BULLET, nullptr);

			//�}�l�[�W���[�̎擾
			CManager* pManager = CManager::GetManager();
			//�T�E���h�̎擾
			CSound *pSound = nullptr;
			if (pManager != nullptr) pSound = pManager->GetSound();

			//�U���G�t�F�N�g�̐���
			CEffect::Create(posHit, CEffect::EFFECT_TYPE::DAMAGE_BULLET, 30.0f, 30.0f, false);
			//�_���[�W���̍Đ�
			if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_BULLET);
		
			//�I��
			return true;
		}

		pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}

	return false;
}