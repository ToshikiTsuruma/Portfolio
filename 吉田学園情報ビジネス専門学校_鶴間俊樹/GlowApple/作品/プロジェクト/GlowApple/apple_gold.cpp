//=============================================================================
//
// ���ь珈�� [apple_gold.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "apple_gold.h"
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
CAppleGold::CAppleGold()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CAppleGold::CAppleGold(D3DXVECTOR3 pos, CAppleTree* pTree) : CGlowApple(pos, pTree)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleGold::~CAppleGold()
{

}

//=============================================================================
// ���ь�̐�������
//=============================================================================
CAppleGold* CAppleGold::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	CAppleGold* pAppleGold;
	pAppleGold = new CAppleGold(pos, pTree);
	if (pAppleGold == nullptr) return nullptr;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	CGameScene* pGame = nullptr;
	CPlayer* pPlayer = nullptr;
	//�Q�[���V�[���̎擾
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//�v���C���[�̎擾
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	//�v���C���[�̔\�͂��㏸
	if (pPlayer != nullptr) {
		//�ړ����xUP
		pPlayer->AddMoveSpeed(3.0f);
		//�U����UP
		pPlayer->AddAttackDamage(5);
	}

	pAppleGold->Init();

	return pAppleGold;
}

//=============================================================================
// ���ь�̏���������
//=============================================================================
HRESULT CAppleGold::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//���f���̐F��ύX
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::GOLD), 0);
	}

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// ���ь�̏I������
//=============================================================================
void CAppleGold::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// ���ь�̍X�V����
//=============================================================================
void CAppleGold::Update(void) {

	CGlowApple::Update();
}

//=============================================================================
// ���ь�̕`�揈��
//=============================================================================
void CAppleGold::Draw(void) {
	CGlowApple::Draw();
}