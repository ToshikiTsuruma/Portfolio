//=============================================================================
//
// �Ηь珈�� [apple_green.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "apple_green.h"
#include "appleTree.h"
#include "manager.h"
#include "sound.h"
#include "effect.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define HEAL_VALUE (50)	//�񕜗�
#define HEAL_SPAN (FPS * 5)	//�񕜂̃X�p��

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CAppleGreen::CAppleGreen()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CAppleGreen::CAppleGreen(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleGreen::~CAppleGreen()
{

}

//=============================================================================
// �Ηь�̐�������
//=============================================================================
CAppleGreen* CAppleGreen::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	CAppleGreen* pAppleGreen;
	pAppleGreen = new CAppleGreen(pTree);
	if (pAppleGreen == nullptr) return nullptr;

	pAppleGreen->SetPos(pos);
	pAppleGreen->Init();
	pAppleGreen->m_nCntHeal = HEAL_SPAN;

	return pAppleGreen;
}

//=============================================================================
// �Ηь�̏���������
//=============================================================================
HRESULT CAppleGreen::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//���f���̐F��ύX
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::GREEN), 0);
	}

	//�����\���̂��߂Ƀp�[�e�B�N���𐶐�
	CreateGlowParticle(APPLE_TYPE::GREEN);

	m_nCntHeal = 0;

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// �Ηь�̏I������
//=============================================================================
void CAppleGreen::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// �Ηь�̍X�V����
//=============================================================================
void CAppleGreen::Update(void) {
	if (m_nCntHeal >= HEAL_SPAN) {
		//�ь�̖؂��񕜂���
		CAppleTree* pAppleTree = GetAppleTree();
		if (pAppleTree != nullptr) {
			pAppleTree->HealLife(HEAL_VALUE);
		}
		//�ь�̈ʒu����񕜂̃G�t�F�N�g���o��
		D3DXVECTOR3 posEffect = GetPos();
		posEffect.y -= 15.0f;	//�ʒu�̒���
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::HEAL_APPLE, 80.0f, 80.0f, false);

		//�}�l�[�W���[�̎擾
		CManager* pManager = CManager::GetManager();
		CSound* pSound = nullptr;	//�T�E���h�ւ̃|�C���^
		//�T�E���h�̎擾
		if (pManager != nullptr) pSound = pManager->GetSound();
		//�񕜗ʉ��̍Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::HEAL);

		m_nCntHeal = 0;
	}
	else {
		m_nCntHeal++;
	}

	CGlowApple::Update();
}

//=============================================================================
// �Ηь�̕`�揈��
//=============================================================================
void CAppleGreen::Draw(void) {
	CGlowApple::Draw();
}