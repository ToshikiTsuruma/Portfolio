//=============================================================================
//
// �ь�I�����j���[���� [appleMenu.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "appleMenu.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "object2D.h"
#include "font.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define RADIUS_MODEL_DISTANCE (60.0f)	//���S���烂�f��������Ă��锼�a�̋���
#define MENU_CAMERA_DISTANCE (200.0f)	//���j���[�p�J�����̒��S����̋���
#define MENU_CAMERA_HEIGHT (60.0f)		//���j���[�p�J�����̒��S����̍���

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CAppleMenu::CAppleMenu() : CSelectMenu3D()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CAppleMenu::CAppleMenu(D3DXVECTOR3 posCenter) : CSelectMenu3D(NUM_SELECT_APPLE_MENU, posCenter, RADIUS_MODEL_DISTANCE, CModel::MODELTYPE::OBJ_MENUAPPLE, MENU_CAMERA_DISTANCE, MENU_CAMERA_HEIGHT)
{
	ZeroMemory(m_aTypeApple, sizeof(m_aTypeApple));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleMenu::~CAppleMenu()
{

}

//=============================================================================
// �ь�I�����j���[�̐�������
//=============================================================================
CAppleMenu* CAppleMenu::Create(D3DXVECTOR3 posCenter, CAppleTree* pAppleTree) {
	CAppleMenu* pAppleMenu;
	pAppleMenu = new CAppleMenu(posCenter);
	if (pAppleMenu == nullptr) return nullptr;

	pAppleMenu->Init();
	pAppleMenu->m_pAppleTree = pAppleTree;

	return pAppleMenu;
}

//=============================================================================
// �ь�I�����j���[�̏���������
//=============================================================================
HRESULT CAppleMenu::Init(void) {
	//�w�i�̐���
	CreateMenuBG(CTexture::TEXTURE_TYPE::MENU_BG, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 800.0f, 550.0f);

	//Grow UP �e�L�X�g�̐���
	m_pGrowUpText = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 180.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_GROWUP, 400.0f, 100.0f);

	//���UI�̐���
	m_apArrow[0] = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 300.0f, SCREEN_HEIGHT / 2.0f + 200.0f, 0.0f), CTexture::TEXTURE_TYPE::ARROW_LEFT, 50.0f, 50.0f);
	m_apArrow[1] = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f + 300.0f, SCREEN_HEIGHT / 2.0f + 200.0f, 0.0f), CTexture::TEXTURE_TYPE::ARROW_RIGHT, 50.0f, 50.0f);

	//�����_���ŗь�̎�ނ�����
	SetAppleTypeRandom();

	for (int nCntModel = 0; nCntModel < GetNumSelect(); nCntModel++)
	{
		//UI���f�����擾
		CObjectModelUI* pAppleUI = GetModelUI(nCntModel);
		if (pAppleUI == nullptr) continue;

		//���j���[�J�����Ƃ��ɂ��X�V�𑱂���悤�ɂ���
		pAppleUI->SetUpdatePriority(UPDATE_PRIORITY::MENU_UPDATE);

		//UI���f���̃��f�����擾
		CModel* pAppleModel = pAppleUI->GetPtrModel();

		if (pAppleModel != nullptr) {		
			//��ނɂ���ĐF��ݒ�
			pAppleModel->SetMaterialDiffuse(CGlowApple::GetAppleColor(m_aTypeApple[nCntModel]), 0);
			//pAppleModel->SetMaterialEmissive(GLOW_APPLE_EMISSIVE, 0);
		}

		//UI���f���̉�]���x��ݒ�
		pAppleUI->SetRotSpeed(D3DXVECTOR3(0.0f, 0.02f, 0.0f));
	}

	//�����e�L�X�g�̃t�H���g�𐶐�
	m_pFontAppleText = CFont::Create("", DT_LEFT, SCREEN_WIDTH / 2.0f - 200.0f, SCREEN_HEIGHT / 2.0f + 190.0f, 18);
	//�ݒ�e�L�X�g��ݒ�
	SetAppleText(0);

	CSelectMenu3D::Init();

	//���j���[�̉�]���x�ݒ�
	SetSpeedRotModel(0.09f);

	//���j���[�ɂ��X�V��~
	SetStopUpdateAll(true);
	//���j���[�֘A�̂��͍̂X�V������
	SetStopUpdate(UPDATE_PRIORITY::MENU_UPDATE, false);

	return S_OK;
}

//=============================================================================
// �ь�I�����j���[�̏I������
//=============================================================================
void CAppleMenu::Uninit(void) {
	//Grow up�e�L�X�g�̔j��
	if (m_pGrowUpText != nullptr) {
		m_pGrowUpText->Uninit();
		m_pGrowUpText = nullptr;
	}

	//���UI�̔j��
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_apArrow[nCnt] != nullptr) {
			m_apArrow[nCnt]->Uninit();
			m_apArrow[nCnt] = nullptr;
		}
	}

	//�����e�L�X�g�̔j��
	if (m_pFontAppleText != nullptr) {
		m_pFontAppleText->Uninit();
		m_pFontAppleText = nullptr;
	}

	CSelectMenu3D::Uninit();

	//���j���[�ɂ��X�V��~������
	SetStopUpdateAll(false);
}

//=============================================================================
// �ь�I�����j���[�̍X�V����
//=============================================================================
void CAppleMenu::Update(void) {
	CSelectMenu3D::Update();
}

//=============================================================================
// �ь�̐����e�L�X�g��ݒ�
//=============================================================================
void CAppleMenu::SetAppleText(int nSelect) {
	if (m_pFontAppleText == nullptr) return;

	char aTextApple[128];	//�ь�̐����e�L�X�g
	//�����e�L�X�g���擾
	CGlowApple::GetAppleText(&aTextApple[0], m_aTypeApple[nSelect]);
	//�e�L�X�g��ݒ�
	m_pFontAppleText->SetText(aTextApple);
}

//=============================================================================
// �ь�̎�ނ������_���Ō���
//=============================================================================
void CAppleMenu::SetAppleTypeRandom(void) {
	//------------------------------------------------
	// ��������ь�̐����ь�̎�ނ��������ꍇ
	//------------------------------------------------
	if (NUM_SELECT_APPLE_MENU > (int)CGlowApple::APPLE_TYPE::ENUM_MAX) {
		//���莞�̃��[�v���I���Ȃ��Ȃ邽�߂��ׂĂO�ɐݒ肵�ďI��
		for (int nCnt = 0; nCnt < NUM_SELECT_APPLE_MENU; nCnt++)
		{
			m_aTypeApple[nCnt] = (CGlowApple::APPLE_TYPE) 0;
		}
		return;
	}

	//------------------------------------------------
	// �����_���Ɏ�ނ�����
	//------------------------------------------------
	for (int nCntType = 0; nCntType < NUM_SELECT_APPLE_MENU; nCntType++)
	{
		int nRandType;	//�ь�̎��
		//�܂����܂��Ă��Ȃ���ނɂȂ�܂Ń��[�v
		while (true)
		{
			nRandType = rand() % (int)CGlowApple::APPLE_TYPE::ENUM_MAX;	//�����_���ŗь�̎�ނ�����

			bool bContinueLoop = false;	//���[�v�𑱂��邩�ǂ���
			for (int nAppleType = 0; nAppleType < nCntType; nAppleType++)
			{
				//���łɐ�������Ă���ꍇ
				if (nRandType == (int)m_aTypeApple[nAppleType]) {
					bContinueLoop = true;	//���[�v�𑱂���
					break;
				}
			}

			//���[�v�𔲂���
			if (!bContinueLoop) break;
		}

		//��ނ̐ݒ�
		m_aTypeApple[nCntType] = (CGlowApple::APPLE_TYPE)nRandType;
	}

	//------------------------------------------------
	// ��ނ������Ƀ\�[�g����
	//------------------------------------------------
	for (int nCntSort1 = 0; nCntSort1 < NUM_SELECT_APPLE_MENU - 1; nCntSort1++)
	{
		for (int nCntSort2 = 0; nCntSort2 < NUM_SELECT_APPLE_MENU - 1 - nCntSort1; nCntSort2++)
		{
			if (m_aTypeApple[nCntSort2] > m_aTypeApple[nCntSort2 + 1]) {
				CGlowApple::APPLE_TYPE typeTemp = m_aTypeApple[nCntSort2];	//�ꎞ�I�ɕۑ�
				//�ϐ��̓���ւ�
				m_aTypeApple[nCntSort2] = m_aTypeApple[nCntSort2 + 1];
				m_aTypeApple[nCntSort2 + 1] = typeTemp;
			}
		}
	}
}

//=============================================================================
// ���ݑI�𒆂̗ь�̎�ނ��擾
//=============================================================================
CGlowApple::APPLE_TYPE CAppleMenu::GetSelectAppleType(void) {
	return m_aTypeApple[GetIdxCurSelect()];
}

//=============================================================================
// �I���̕ύX���I�������Ƃ��̏���
//=============================================================================
void CAppleMenu::EndChangeSelect(void) {
	//�e�L�X�g��ύX
	SetAppleText(GetIdxCurSelect());
}