//=============================================================================
//
// �O���E�A�b�v������ [glowApple.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "glowApple.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "particleEffect.h"
#include "appleTree.h"
#include "effect.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGlowApple::CGlowApple()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CGlowApple::CGlowApple(D3DXVECTOR3 pos, CAppleTree* pTree) : CObjectModel(CModel::MODELTYPE::OBJ_APPLE, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
{
	m_pAppleTree = pTree;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGlowApple::~CGlowApple()
{

}

//=============================================================================
// �O���E�A�b�v���̏���������
//=============================================================================
HRESULT CGlowApple::Init(void) {
	//�������ɃG�t�F�N�g�𐶐�
	CParticleEffect::PARTICLE_INFO particleInfo;	//�p�[�e�B�N�����
	particleInfo.addMove = D3DXVECTOR3(0.0f, -0.8f, 0.0f); particleInfo.colEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); particleInfo.colStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	particleInfo.fAddSize = -0.8f; particleInfo.fSizeStart = 40.0f; particleInfo.fSpeedMove = 10.0f; particleInfo.nLife = 60;
	//���d�˂Đ���
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		CParticleEffect::Create(particleInfo, GetPos(), 1, 1, 0.1f * D3DX_PI, true);
	}

	CObjectModel::Init();
	return S_OK;
}

//=============================================================================
// �O���E�A�b�v���̏I������
//=============================================================================
void CGlowApple::Uninit(void) {
	CObjectModel::Uninit();
}

//=============================================================================
// �O���E�A�b�v���̍X�V����
//=============================================================================
void CGlowApple::Update(void) {
	CObjectModel::Update();

	if (m_pAppleTree != nullptr) {
		//�ь�̖؂����񂾏ꍇ�ь�j��
		if (m_pAppleTree->GetDead()) {
			//�����G�t�F�N�g
			CEffect::Create(GetPos(), CEffect::EFFECT_TYPE::EXPLOSION, 50.0f, 50.0f);
			//�I������
			Uninit();
		}
	}
}

//=============================================================================
// �O���E�A�b�v���̕`�揈��
//=============================================================================
void CGlowApple::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// ��ނ��Ƃ̗ь�̐F���擾
//=============================================================================
D3DXCOLOR CGlowApple::GetAppleColor(APPLE_TYPE type) {
	switch (type)
	{
	case CGlowApple::APPLE_TYPE::RED:
		return D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		return D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		return D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		return D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		return D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	default:
		return D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
		break;
	}
}

//=============================================================================
//��ނ��Ƃ̗ь�̐����e�L�X�g
//=============================================================================
void CGlowApple::GetAppleText(char* pStr, APPLE_TYPE type) {
	switch (type)
	{
	case CGlowApple::APPLE_TYPE::RED:
		strcpy(pStr, "Red Apple : �̗͂̍ő�l��������");
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		strcpy(pStr, "Green Apple : �����񕜂���");
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		strcpy(pStr, "Silver Apple : �Ռ��g�̐���������");
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		strcpy(pStr, "Gold Apple : �v���C���[�̈ړ����x���U����UP");
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		strcpy(pStr, "Black Apple : �����̓G���Ăъ񂹂�");
		break;
	}
}