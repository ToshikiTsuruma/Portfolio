//=============================================================================
//
// ���ь珈�� [apple_black.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "apple_black.h"
#include "appleTree.h"
#include "manager.h"
#include "gameScene.h"
#include "enemySpawner.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

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

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	CGameScene* pGame = nullptr;
	CEnemySpawner* pEnemySpawner = nullptr;
	//�Q�[���V�[���̎擾
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//�G�̃X�|�i�[�̎擾
	if (pGame != nullptr) pEnemySpawner = pGame->GetEnemySpawner();

	//�G�̊Ԋu�����炷
	if (pEnemySpawner != nullptr) {
		pEnemySpawner->AddDistEnemy(-400);
	}

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