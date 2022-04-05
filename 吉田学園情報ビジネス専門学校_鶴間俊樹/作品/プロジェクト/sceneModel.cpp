//=============================================================================
//
// ���f���I�u�W�F�N�g���� [sceneModel.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "sceneModel.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CSceneModel::CSceneModel()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CSceneModel::CSceneModel(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine)
{
	m_pModel = CModel::Create(typeModel, pos, rot, pParent, bOutLine);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneModel::~CSceneModel()
{

}

//=============================================================================
// ���f���I�u�W�F�N�g�̐�������
//=============================================================================
CSceneModel* CSceneModel::Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot) {
	CSceneModel* pSceneModel;
	pSceneModel = new CSceneModel(type, pos, rot, nullptr, false);
	if (pSceneModel != nullptr) {
		pSceneModel->Init();
	}

	return pSceneModel;
}

//=============================================================================
// ���f���I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CSceneModel::Init(void) {

	return S_OK;
}

//=============================================================================
// ���f���I�u�W�F�N�g�̏I������
//=============================================================================
void CSceneModel::Uninit(void) {
	if (m_pModel != nullptr) {
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// ���f���I�u�W�F�N�g�̍X�V����
//=============================================================================
void CSceneModel::Update(void) {

}

//=============================================================================
// ���f���I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CSceneModel::Draw(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return;

	//�������������[���h�}�g���b�N�X��ݒ�
	D3DXMATRIX mtxWorld;
	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//-----------------------------------
	//���f���̕`��
	//-----------------------------------
	if (m_pModel != nullptr) m_pModel->Draw();
}

//=============================================================================
// ���f���̎�ނ̎擾
//=============================================================================
CModel::MODELTYPE CSceneModel::GetModelType(void) {
	if (m_pModel != nullptr) return m_pModel->GetModelType();
	return (CModel::MODELTYPE) NULL;
}

//=============================================================================
// ���f���̃|�C���^�̎擾
//=============================================================================
CModel* CSceneModel::GetPtrModel(void) {
	return m_pModel;
}

//=============================================================================
// ���f���I�u�W�F�N�g�̈ʒu���̐ݒ�
//=============================================================================
void CSceneModel::SetPos(D3DXVECTOR3 pos) {	
	if (m_pModel != nullptr) m_pModel->SetPos(pos);
}

//=============================================================================
// ���f���I�u�W�F�N�g�̈ʒu���̎擾
//=============================================================================
D3DXVECTOR3 CSceneModel::GetPos(void) {
	if (m_pModel != nullptr) return m_pModel->GetPos();
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// ���f���I�u�W�F�N�g�̊p�x�̐ݒ�
//=============================================================================
void CSceneModel::SetRot(D3DXVECTOR3 rot) { 
	if (m_pModel != nullptr) m_pModel->SetRot(rot);
}

//=============================================================================
// ���f���I�u�W�F�N�g�̊p�x�̎擾
//=============================================================================
D3DXVECTOR3 CSceneModel::GetRot(void) {
	if (m_pModel != nullptr) return m_pModel->GetRot();
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}