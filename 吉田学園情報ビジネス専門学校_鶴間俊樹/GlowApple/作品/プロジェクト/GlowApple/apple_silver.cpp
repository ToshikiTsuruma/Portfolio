//=============================================================================
//
// ��ь珈�� [apple_silver.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "apple_silver.h"
#include "appleTree.h"
#include "manager.h"
#include "gameScene.h"
#include "player.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CAppleSilver::CAppleSilver()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CAppleSilver::CAppleSilver(D3DXVECTOR3 pos, CAppleTree* pTree) : CGlowApple(pos, pTree)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleSilver::~CAppleSilver()
{

}

//=============================================================================
// ��ь�̐�������
//=============================================================================
CAppleSilver* CAppleSilver::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	CAppleSilver* pAppleSilver;
	pAppleSilver = new CAppleSilver(pos, pTree);
	if (pAppleSilver == nullptr) return nullptr;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	CGameScene* pGame = nullptr;
	CPlayer* pPlayer = nullptr;
	//�Q�[���V�[���̎擾
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//�v���C���[�̎擾
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	//�v���C���[�̏Ռ��g���𑝂₷
	if (pPlayer != nullptr) {
		pPlayer->AddNumShockwave(1);
	}

	pAppleSilver->Init();

	return pAppleSilver;
}

//=============================================================================
// ��ь�̏���������
//=============================================================================
HRESULT CAppleSilver::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//���f���̐F��ύX
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::SILVER), 0);
	}

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// ��ь�̏I������
//=============================================================================
void CAppleSilver::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// ��ь�̍X�V����
//=============================================================================
void CAppleSilver::Update(void) {

	CGlowApple::Update();
}

//=============================================================================
// ��ь�̕`�揈��
//=============================================================================
void CAppleSilver::Draw(void) {
	CGlowApple::Draw();
}