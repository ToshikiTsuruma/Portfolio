//=============================================================================
//
// ���f���I�u�W�F�N�g���� [objectModel.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "objectModel.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CObjectModel::CObjectModel()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CObjectModel::CObjectModel(CModel::MODELTYPE typeModel, bool bOutLine)
{
	m_pModel = CModel::Create(typeModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), nullptr, bOutLine);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObjectModel::~CObjectModel()
{

}

//=============================================================================
// ���f���I�u�W�F�N�g�̐�������
//=============================================================================
CObjectModel* CObjectModel::Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine) {
	CObjectModel* pObjectModel;
	pObjectModel = new CObjectModel(type, bOutLine);
	if (pObjectModel == nullptr) return nullptr;

	pObjectModel->Init();
	pObjectModel->SetPos(pos);
	pObjectModel->SetRot(rot);

	return pObjectModel;
}

//=============================================================================
// ���f���I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CObjectModel::Init(void) {

	return S_OK;
}

//=============================================================================
// ���f���I�u�W�F�N�g�̏I������
//=============================================================================
void CObjectModel::Uninit(void) {
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
void CObjectModel::Update(void) {
	//�ړ�����
	//Move();
	//��]����
	//Rotate();
}

//=============================================================================
// ���f���I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CObjectModel::Draw(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	if (pManager == nullptr) return;
	//�����_���[�̎擾
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-----------------------------------
	//���f���̕`��
	//-----------------------------------
	if (m_pModel != nullptr) m_pModel->Draw();
}

//=============================================================================
// �ړ�
//=============================================================================
void CObjectModel::Move(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	CInput* pInput = nullptr;	//���̓f�o�C�X�ւ̃|�C���^

	//���݂̓��̓f�o�C�X�̎擾
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//null�̏ꍇ�I��
	if (pInput == nullptr) return;

	//--------------------
	//���͏���
	//--------------------
	float fMoveSpeed = 1.0f;
	if (pInput->GetPress(CInput::CODE::DASH)) {
		fMoveSpeed *= 5;
	}

	//�ʒu�̎擾
	D3DXVECTOR3 pos = m_pos;

	if (pInput->GetPress(CInput::CODE::MOVE_UP)) {
		//�O�ړ�
		pos.z += fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_DOWN)) {
		//���ړ�
		pos.z -= fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_LEFT)) {
		//���ړ�
		pos.x -= fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RIGHT)) {
		//�E�ړ�
		pos.x += fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RISE)) {
		//�㏸
		pos.y += fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_FALL)) {
		//���~
		pos.y -= fMoveSpeed;
	}

	//�ʒu�̈ړ�
	m_pos = pos;
}

//=============================================================================
// ��]
//=============================================================================
void CObjectModel::Rotate(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	CInput* pInput = nullptr;	//���̓f�o�C�X�ւ̃|�C���^

	//���݂̓��̓f�o�C�X�̎擾
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//null�̏ꍇ�I��
	if (pInput == nullptr) return;
	//�V�t�g��������Ă���ꍇ�I���i�J�����̎��_�ړ����̂��߁j
	if (pInput->GetPress(CInput::CODE::DASH)) return;

	//--------------------
	//���͏���
	//--------------------
	D3DXVECTOR3 rot = m_rot;
	float fRotateSpeed = 0.01f * D3DX_PI;

	//X����]
	if (pInput->GetPress(CInput::CODE::ROTATE_UP)) {
		rot.x += fRotateSpeed;
		if (rot.x > D3DX_PI) {
			rot.x = -D3DX_PI * 2 + rot.x;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_DOWN)) {
		rot.x -= fRotateSpeed;
		if (rot.x < -D3DX_PI) {
			rot.x = D3DX_PI * 2 + rot.x;
		}
	}
	//Y����]
	if (pInput->GetPress(CInput::CODE::ROTATE_LEFT)) {
		rot.y -= fRotateSpeed;
		if (rot.y < -D3DX_PI) {
			rot.y = D3DX_PI * 2 + rot.y;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_RIGHT)) {
		rot.y += fRotateSpeed;
		if (rot.y > D3DX_PI) {
			rot.y = -D3DX_PI * 2 + rot.y;
		}
	}

	m_rot = rot;
}

//=============================================================================
// ���f���̎�ނ̎擾
//=============================================================================
CModel::MODELTYPE CObjectModel::GetModelType(void) {
	if (m_pModel != nullptr) return m_pModel->GetModelType();
	return (CModel::MODELTYPE) NULL;
}

//=============================================================================
// ���f���̃|�C���^�̎擾
//=============================================================================
CModel* CObjectModel::GetPtrModel(void) {
	return m_pModel;
}

//=============================================================================
// ���f���I�u�W�F�N�g�̈ʒu���̐ݒ�
//=============================================================================
void CObjectModel::SetPos(D3DXVECTOR3 pos) {	
	m_pos = pos;
}

//=============================================================================
// ���f���I�u�W�F�N�g�̈ʒu���̎擾
//=============================================================================
D3DXVECTOR3 CObjectModel::GetPos(void) {
	return m_pos;
}

//=============================================================================
// ���f���I�u�W�F�N�g�̊p�x�̐ݒ�
//=============================================================================
void CObjectModel::SetRot(D3DXVECTOR3 rot) { 
	m_rot = rot;
}

//=============================================================================
// ���f���I�u�W�F�N�g�̊p�x�̎擾
//=============================================================================
D3DXVECTOR3 CObjectModel::GetRot(void) {
	return m_rot;
}