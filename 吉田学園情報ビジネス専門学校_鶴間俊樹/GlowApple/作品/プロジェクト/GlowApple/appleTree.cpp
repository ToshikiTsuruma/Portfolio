//=============================================================================
//
// �ь�̖؂̏��� [appleTree.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "appleTree.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "appleMenu.h"
#include "gauge2D.h"
#include "gameScene.h"
#include "object2D.h"

#include "apple_red.h"
#include "apple_green.h"
#include "apple_silver.h"
#include "apple_gold.h"
#include "apple_black.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_NUM_APPLE (5)	//�����\�ȗь�̍ő吔

#define NUM_COLLISION (5)			//�����蔻��̐�
#define COLLISION_RADIUS (50.0f)	//�����蔻��̔��a

#define MAX_LIFE_DEFAULT (200)			//�̗͂̍ő�l
#define DANGER_LIFE ((int)(MAX_LIFE_DEFAULT * 0.2f))		//�̗͂̊댯�l
#define COLOR_LIFE_GAUGE_SAFE (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//�̗̓o�[�̈��S���̐F
#define COLOR_LIFE_GAUGE_DANGER (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//�̗̓o�[�̊댯���̐F

#define MAX_GROW_VALUE (15)	//�����x�̍ő�

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CAppleTree::CAppleTree()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CAppleTree::CAppleTree(D3DXVECTOR3 pos) : CObjectModel(CModel::MODELTYPE::OBJ_APPLE_TREE, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
{
	m_nNumApple = 0;
	m_pMenuApple = nullptr;
	m_nMaxLife = MAX_LIFE_DEFAULT;
	m_nLife = MAX_LIFE_DEFAULT;
	m_nGrowValue = 0;
	m_bDead = false;

	m_pGaugeLife = CGauge2D::Create(MAX_LIFE_DEFAULT, MAX_LIFE_DEFAULT, 0);
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(150.0f + 20.0f, 30.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeLife->SetGaugeBGColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
		m_pGaugeLife->CreateGauge(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(150.0f + 20.0f, 30.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeLife->SetGaugeColor(COLOR_LIFE_GAUGE_SAFE);
		m_pGaugeLife->SetGaugeColorDanger(COLOR_LIFE_GAUGE_DANGER);
		m_pGaugeLife->SetDangerValue(DANGER_LIFE);
		//�ő�l�ϓ��ɍ��킹���Ȃ����ߘg�Ȃ�
		//m_pGaugeLife->CreateGaugeFrame(CTexture::TEXTURE_TYPE::CREATE_FRAME, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f), 800.0f, 20.0f);
	}
	m_pGaugeGrow = CGauge2D::Create(MAX_GROW_VALUE, MAX_GROW_VALUE, 0);
	if (m_pGaugeGrow != nullptr) {
		m_pGaugeGrow->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(150.0f + 20.0f, 55.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeGrow->SetGaugeBGColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
		m_pGaugeGrow->CreateGauge(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(150.0f + 20.0f, 55.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeGrow->SetGaugeColor(D3DXCOLOR(0.0f, 0.4f, 1.0f, 1.0f));
		m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
		//m_pGaugeGrow->CreateGaugeFrame(CTexture::TEXTURE_TYPE::CREATE_FRAME, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f), 800.0f, 20.0f);
	}
	m_pIconHP = CObject2D::Create(D3DXVECTOR3(50.0f, 30.0f, 0.0f), CTexture::TEXTURE_TYPE::ICON_HP, 20.0f, 20.0f);

	SetObjType(OBJ_TYPE::APPLE_TREE);
	SetEnableCollision(true);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleTree::~CAppleTree()
{

}

//=============================================================================
// �ь�̖؂̐�������
//=============================================================================
CAppleTree* CAppleTree::Create(D3DXVECTOR3 pos) {
	CAppleTree* pAppleTree;
	pAppleTree = new CAppleTree(pos);
	if (pAppleTree == nullptr) return nullptr;

	pAppleTree->Init();

	return pAppleTree;
}

//=============================================================================
// �ь�̖؂̏���������
//=============================================================================
HRESULT CAppleTree::Init(void) {
	//���j���[�J�����Ƃ��ɂ��X�V�𑱂���悤�ɂ���
	SetUpdatePriority(UPDATE_PRIORITY::MENU_UPDATE);

	CObjectModel::Init();
	return S_OK;
}

//=============================================================================
// �ь�̖؂̏I������
//=============================================================================
void CAppleTree::Uninit(void) {
	//�ь�̃��j���[�̔j��
	if (m_pMenuApple != nullptr) {
		m_pMenuApple->Uninit();
		m_pMenuApple = nullptr;
	}
	//�̗̓Q�[�W�̔j��
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}
	//�����Q�[�W�̔j��
	if (m_pGaugeGrow != nullptr) {
		m_pGaugeGrow->Uninit();
		m_pGaugeGrow = nullptr;
	}

	CObjectModel::Uninit();
}

//=============================================================================
// �ь�̖؂̍X�V����
//=============================================================================
void CAppleTree::Update(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	CInput* pInput = nullptr;	//���݂̓��̓f�o�C�X�ւ̃|�C���^
	if (pManager != nullptr) pInput = pManager->GetInputCur();

	if (pInput != nullptr) {
		//���j���[������ꍇ
		if (m_pMenuApple != nullptr) {
			//�ь�̐���
			if (pInput->GetTrigger(CInput::CODE::SELECT) && !m_pMenuApple->GetLockChangeSelect()) CreateApple();
		}
	}

	CObjectModel::Update();
}

//=============================================================================
// �ь�̖؂̕`�揈��
//=============================================================================
void CAppleTree::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// �ь�̖؂̃_���[�W
//=============================================================================
void CAppleTree::Damage(int nDamage, bool* pDead) {
	if (m_bDead) return;

	//�_���[�W
	m_nLife -= nDamage;

	if (m_nLife <= 0) {
		m_nLife = 0;
		//���S
		if (pDead != nullptr) *pDead = true;
		Dead();
	}
	else {
		if (pDead != nullptr) *pDead = false;
	}
	//�̗̓Q�[�W�̐ݒ�
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->SetGaugeValue(m_nLife);
	}
}

//=============================================================================
// ���S
//=============================================================================
void CAppleTree::Dead(void) {
	//���S�̐ݒ�
	m_bDead = true;
	//���S�J�E���g�̏�����
	m_nCntDead = 0;
	//�����蔻��𖳌�
	SetEnableCollision(false);

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	CSound* pSound = nullptr;
	CGameScene* pGame = nullptr;

	//�T�E���h�̎擾
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�Q�[���̎擾
	if (pManager != nullptr) pGame = pManager->GetGameScene();

	//���S���̍Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::DEAD_TREE);
	//�Q�[���I�[�o�[�ݒ�
	if (pGame != nullptr) pGame->GameOver();
}

//=============================================================================
// ����
//=============================================================================
void CAppleTree::GrowUp(void) {
	//���łɐ����ς݂̏ꍇ�����S���Ă���ꍇ�I��
	if (m_pMenuApple != nullptr || m_bDead) return;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�Q�[���̎擾
	CGameScene* pGame = pManager->GetGameScene();
	if (pGame == nullptr) return;
	//�Q�[���I����̏ꍇ�I��
	if (pGame->GetGameClear() || pGame->GetGameOver()) return;

	//���j���[�𐶐�
	m_pMenuApple = CAppleMenu::Create(D3DXVECTOR3(0.0f, 2000.0f, 3000.0f), this);
}

//=============================================================================
// �ь琶���ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CAppleTree::GetPosApple(void) {
	switch (m_nNumApple)
	{
	case 0:
		return D3DXVECTOR3(169.03f, 264.21f, -43.97f);
		break;

	case 1:
		return D3DXVECTOR3(-120.97f, 477.21f, -133.97f);
		break;

	case 2:
		return D3DXVECTOR3(254.03f, 537.21f, 154.03f);
		break;

	case 3:
		return D3DXVECTOR3(-103.97f, 583.21f, 104.03f);
		break;

	case 4:
		return D3DXVECTOR3(-227.97f, 479.21f, -51.97f);
		break;

	default:
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;
	}
}

//=============================================================================
// �ь�̐���
//=============================================================================
void CAppleTree::CreateApple(void) {
	//�ь�̎�ނ��擾
	CGlowApple::APPLE_TYPE typeApple = m_pMenuApple->GetSelectAppleType();
	//�ь�̈ʒu���擾
	D3DXVECTOR3 posCreate = GetPos() + GetPosApple();	//�؂̈ʒu + �I�t�Z�b�g
	//�ь�𐶐�
	switch (typeApple)
	{
	case CGlowApple::APPLE_TYPE::RED:
		CAppleRed::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		CAppleGreen::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		CAppleSilver::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		CAppleGold::Create(posCreate, this);
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		CAppleBlack::Create(posCreate, this);
		break;
	}
	//�ь�̐����������Z
	m_nNumApple++;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	CSound* pSound = nullptr;	//�T�E���h�ւ̃|�C���^
	//�T�E���h�̎擾
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�������̍Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CREATE_APPLE);

	//���j���[�����
	if (m_pMenuApple != nullptr) {
		m_pMenuApple->Uninit();
		m_pMenuApple = nullptr;
	}

	if (m_nNumApple < MAX_NUM_APPLE) {
		//�����x�������������
		m_nGrowValue -= MAX_GROW_VALUE;
		if (m_pGaugeGrow != nullptr) {
			//�����Q�[�W�̐ݒ�
			m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
		}
	}
}


//=============================================================================
// �ь�̖؂̓����蔻��̏��̎擾
//=============================================================================
void CAppleTree::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//�K�v�ȃ|�C���^��null�̏ꍇ�I��
	if (pNumCol == nullptr || pRadiusCol == nullptr || ppPosColArray == nullptr) return;

	//pos�̃|�C���^�����łɑ��݂��Ă���ꍇ�I��
	if (*ppPosColArray != nullptr) return;

	//�����蔻��̐���ݒ�
	*pNumCol = NUM_COLLISION;
	//�����蔻��̔��a��ݒ�
	*pRadiusCol = COLLISION_RADIUS;
	//�����蔻��̈ʒu�̔z���ݒ�
	*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];	//���I�m��

	for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
	{
		(*ppPosColArray)[nCntCol] = D3DXVECTOR3(GetPos().x, GetPos().y + 20.0f * nCntCol, GetPos().z);
	}
}

//=============================================================================
// �̗͂̍ő�l�̐ݒ�
//=============================================================================
void CAppleTree::SetMaxLife(int nMaxLife) {
	int nDeltaLife = nMaxLife - m_nMaxLife;	//�̗͂̍���
	if (nDeltaLife < 0) nDeltaLife = 0;	//�}�C�i�X�̏ꍇ�ύX�Ȃ�

	//�ő�l��ݒ�
	m_nMaxLife = nMaxLife;
	
	//�Q�[�W�ɍ����𑫂��Đݒ�
	m_nLife += nDeltaLife;
	m_nMaxLife = nMaxLife;

	if (m_pGaugeLife != nullptr) {
		//�̗̓Q�[�W�̍ő�l�̐ݒ�
		m_pGaugeLife->SetMaxValue(nMaxLife);
		//�̗̓Q�[�W�̐ݒ�
		m_pGaugeLife->SetGaugeValue(m_nLife);
	}
}

//=============================================================================
// �̗͂̍ő�l�̑���
//=============================================================================
void CAppleTree::AddMaxLife(int nAddLife) {
	SetMaxLife(m_nMaxLife + nAddLife);
}

//=============================================================================
// �̗͂��񕜂���
//=============================================================================
void CAppleTree::HealLife(int nHeal) {
	m_nLife += nHeal;
	if (m_nLife > m_nMaxLife) m_nLife = m_nMaxLife;

	if (m_pGaugeLife != nullptr) {
		//�̗̓Q�[�W�̐ݒ�
		m_pGaugeLife->SetGaugeValue(m_nLife);
	}
}

//=============================================================================
// �����x�𑝂₷
//=============================================================================
void CAppleTree::AddGrow(int nAddValue) {
	m_nGrowValue += nAddValue;

	//���������𖞂������ꍇ
	if (m_nGrowValue >= MAX_GROW_VALUE) {
		//�ь炪�����\�ȏꍇ
		if (m_nNumApple < MAX_NUM_APPLE) {
			GrowUp();
		}
		//�ь炪�����s�\�̏ꍇ
		else {
			m_nGrowValue = MAX_GROW_VALUE;
		}
	}

	if (m_pGaugeGrow != nullptr) {
		//�����Q�[�W�̐ݒ�
		m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
	}
}