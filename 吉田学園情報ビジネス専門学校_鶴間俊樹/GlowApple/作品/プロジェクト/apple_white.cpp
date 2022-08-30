//=============================================================================
//
// ���ь珈�� [apple_white.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "apple_white.h"
#include "appleTree.h"
#include "manager.h"
#include "gameScene.h"
#include "player.h"
#include "effect.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CAppleWhite::CAppleWhite()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CAppleWhite::CAppleWhite(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleWhite::~CAppleWhite()
{

}

//=============================================================================
// ���ь�̐�������
//=============================================================================
CAppleWhite* CAppleWhite::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	CAppleWhite* pAppleWhite;
	pAppleWhite = new CAppleWhite(pTree);
	if (pAppleWhite == nullptr) return nullptr;

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

	//���̉Q�̃G�t�F�N�g�𐶐�
	pAppleWhite->m_pEffectWind = CEffect::Create(pos + D3DXVECTOR3(0.0f, -15.0f, 0.0f), CEffect::EFFECT_TYPE::WIND, 40.0f, 40.0f, true);

	pAppleWhite->SetPos(pos);
	pAppleWhite->Init();

	return pAppleWhite;
}

//=============================================================================
// ���ь�̏���������
//=============================================================================
HRESULT CAppleWhite::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//���f���̐F��ύX
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::WHITE), 0);
	}

	//�����\���̂��߂Ƀp�[�e�B�N���𐶐�
	CreateGlowParticle(APPLE_TYPE::WHITE);

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// ���ь�̏I������
//=============================================================================
void CAppleWhite::Uninit(void) {
	//���̉Q�̃G�t�F�N�g�̔j��
	if (m_pEffectWind != nullptr) {
		m_pEffectWind->Uninit();
		m_pEffectWind = nullptr;
	}

	CGlowApple::Uninit();
}

//=============================================================================
// ���ь�̍X�V����
//=============================================================================
void CAppleWhite::Update(void) {

	CGlowApple::Update();
}

//=============================================================================
// ���ь�̕`�揈��
//=============================================================================
void CAppleWhite::Draw(void) {
	CGlowApple::Draw();
}