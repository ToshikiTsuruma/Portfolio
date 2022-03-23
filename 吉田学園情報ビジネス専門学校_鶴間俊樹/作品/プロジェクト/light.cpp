//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CLight* CLight::m_pTop = NULL;
CLight* CLight::m_pCur = NULL;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CLight::CLight()
{
	m_nIdx = -1;
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
	m_vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���C�g�̃��X�g�̏����ݒ�
	m_pPrev = m_pCur;	//�O�̃|�C���^�̐ݒ�
	m_pNext = NULL;		//���̃|�C���^�̐ݒ�
	if (m_pTop == NULL) {
		m_pTop = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
		m_nIdx = 0;
	}
	if (m_pCur != NULL) {
		m_pCur->m_pNext = this;		//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
		m_nIdx = m_pCur->m_nIdx + 1;
	}
	m_pCur = this;		//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLight::~CLight()
{
	//���̃��C�g�ȍ~�̃��C�g�̃C���f�b�N�X��ݒ肷��
	SetIdxNext(this);
	//���C�g�̃��X�g
	if (m_pTop == this) m_pTop = m_pNext;	//���̃��C�g��top�������ꍇ�A���̃��C�g��top�ɂ���
	if (m_pCur == this) m_pCur = m_pPrev;	//���̃��C�g��cur�������ꍇ�A�O�̃��C�g��cur�ɂ���
	if (m_pPrev != NULL) m_pPrev->m_pNext = m_pNext;	//�O�̃��C�g��pNext�ɁA���̃��C�g��pNext����
	if (m_pNext != NULL) m_pNext->m_pPrev = m_pPrev;	//���̃��C�g��pPrev�ɁA���̃��C�g��pPrev����
}

//=============================================================================
// ��������
//=============================================================================
CLight* CLight::Create(D3DXVECTOR3 vecDir, D3DXCOLOR col) {
	CLight* pLight;
	pLight = new CLight;
	if (pLight != NULL) {
		pLight->Init(vecDir, col);
	}

	return pLight;
}

//=============================================================================
// ���C�g�̏���������
//=============================================================================
HRESULT CLight::Init(D3DXVECTOR3 vecDir, D3DXCOLOR col) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return S_OK;

	//�����o�ϐ��̐ݒ�
	m_vecDir = vecDir;

	//���C�g�̎�ނ�ݒ�
	m_light.Type = D3DLIGHT_DIRECTIONAL;//���s����
	//���C�g�̊g�U����ݒ�
	m_light.Diffuse = col;
	//���K������
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);
	m_light.Direction = m_vecDir;
	//���C�g��ݒ肷��
	pDevice->SetLight(m_nIdx, &m_light);
	//���C�g��L���ɂ���
	pDevice->LightEnable(m_nIdx, TRUE);

	return S_OK;
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void CLight::Uninit(void) {

}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void CLight::Update(void) {

}

//=============================================================================
//���̃��C�g�̃C���f�b�N�X��ݒ�
//=============================================================================
void CLight::SetIdxNext(CLight* pLight) {
	if (pLight != NULL) {
		CLight* pNext = pLight->m_pNext;	//���̃��C�g
		if (pNext != NULL) {
			//���̎��̃��C�g�̃��C�g�̃C���f�b�N�X��ݒ肷��
			SetIdxNext(pNext);
			//���̃��C�g�̃C���f�b�N�X��ݒ肷��
			pNext->m_nIdx = pLight->m_nIdx;
		}
	}
}