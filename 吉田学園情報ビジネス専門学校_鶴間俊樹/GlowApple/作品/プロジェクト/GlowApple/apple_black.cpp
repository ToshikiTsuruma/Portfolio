//=============================================================================
//
// ���ь珈�� [apple_black.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "apple_black.h"
#include "appleTree.h"
#include "manager.h"
#include "sound.h"
#include "gameScene.h"
#include "enemySpawner.h"
#include "effect.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define HEAL_VALUE (2)	//�񕜗�

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CAppleBlack::CAppleBlack()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CAppleBlack::CAppleBlack(D3DXVECTOR3 pos, CAppleTree* pTree) : CGlowApple(pos, pTree)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleBlack::~CAppleBlack()
{

}

//=============================================================================
// ���ь�̐�������
//=============================================================================
CAppleBlack* CAppleBlack::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	CAppleBlack* pAppleBlack;
	pAppleBlack = new CAppleBlack(pos, pTree);
	if (pAppleBlack == nullptr) return nullptr;

	pAppleBlack->Init();

	return pAppleBlack;
}

//=============================================================================
// ���ь�̏���������
//=============================================================================
HRESULT CAppleBlack::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//���f���̐F��ύX
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::BLACK), 0);
	}

	//�����\���̂��߂Ƀp�[�e�B�N���𐶐�
	CreateGlowParticle(APPLE_TYPE::BLACK);

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// ���ь�̏I������
//=============================================================================
void CAppleBlack::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// ���ь�̍X�V����
//=============================================================================
void CAppleBlack::Update(void) {

	CGlowApple::Update();
}

//=============================================================================
// ���ь�̕`�揈��
//=============================================================================
void CAppleBlack::Draw(void) {
	CGlowApple::Draw();
}

//=============================================================================
// ���ׂĂ̍��ь��HP�z��
//=============================================================================
void CAppleBlack::DrainAllApple(void) {
	CObject* pObject = GetObjectTopAll();	//�S�I�u�W�F�N�g�̃��X�g�̐擪���擾

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾

		//�I�u�W�F�N�g�^�C�v�̊m�F
		bool bMatchType = false;
		if (pObject->GetObjType() & OBJTYPE_APPLE) bMatchType = true;

		//���S�t���O�m�F
		bool bDeath = pObject->GetDeath();

		//���ь�Ƀ_�C�i�~�b�N�L���X�g
		CAppleBlack* pAppleBlack = dynamic_cast<CAppleBlack*>(pObject);

		//���̃��[�v�ɔ�΂�
		if (!bMatchType || bDeath || pAppleBlack == nullptr) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;
		}

		//���ь��HP�z���������s��
		pAppleBlack->Drain();

		pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}
}

//=============================================================================
// �̗͂̋z��
//=============================================================================
void CAppleBlack::Drain(void) {
	//�ь�̖؂��񕜂���
	CAppleTree* pAppleTree = GetAppleTree();
	if (pAppleTree != nullptr) {
		pAppleTree->HealLife(HEAL_VALUE);
	}
	//�ь�̈ʒu����HP�z���̃G�t�F�N�g���o��
	D3DXVECTOR3 posEffect = GetPos();
	posEffect.y -= 15.0f;	//�ʒu�̒���
	CEffect::Create(posEffect, CEffect::EFFECT_TYPE::DRAIN, 80.0f, 80.0f, false);

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	CSound* pSound = nullptr;	//�T�E���h�ւ̃|�C���^
	//�T�E���h�̎擾
	if (pManager != nullptr) pSound = pManager->GetSound();
	//HP�z�����̍Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::DRAIN);
}