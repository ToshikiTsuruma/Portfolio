//=============================================================================
//
// �J�������� [camera.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "wallCylinder.h"
#include "terrain.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDistance = 0.0f;
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	ZeroMemory(&m_viewport, sizeof(D3DVIEWPORT9));	//�m�F
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
//��������
//=============================================================================
CCamera* CCamera::Create(void) {
	CCamera* pCamera;
	pCamera = new CCamera;
	if (pCamera != nullptr) {
		pCamera->Init();
	}

	return pCamera;
}

//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT CCamera::Init(void) {
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

	//�r���[�|�[�g�̐ݒ�
	m_viewport.X = (DWORD)0.0f;
	m_viewport.Y = (DWORD)0.0f;
	m_viewport.Width = (DWORD)SCREEN_WIDTH;
	m_viewport.Height = (DWORD)SCREEN_HEIGHT;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	//�r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewport);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),						//��p 360�x
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//��ʔ䗦
		10.0f,										//��O�̈ʒu
		MAX_FOG_DISTANCE + 100.0f);					//���̈ʒu

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//�r���[�}�g���b�N�X�̍쐬
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);
	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CCamera::Uninit(void) {

}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CCamera::Update(void) {

}

//=============================================================================
// �J�����̃Z�b�g
//=============================================================================
void CCamera::SetCamera(void) {
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

	//���_�ƒ����_�̐ݒ�
	m_posR = m_pos;
	m_posV = D3DXVECTOR3(m_pos.x + sinf(m_rot.y) * m_fDistance * cosf(m_rot.x),
		m_pos.y + sinf(m_rot.x) * m_fDistance,
		m_pos.z + cosf(m_rot.y) * m_fDistance * cosf(m_rot.x));
	
	//�ǂƂ̓����蔻��
	D3DXVECTOR2 posCrossWall = D3DXVECTOR2(m_posV.x, m_posV.z);	//�ǂƂ̌�_
	CWallCylinder::CrossPoint(&posCrossWall, &m_pos, &m_posV, 15.0f);
	//�ʒu�̔��f
	m_posV.x = posCrossWall.x;
	m_posV.z = posCrossWall.y;

	//�n�`�Ƃ̓����蔻��
	bool bColTerrain = false;	//�Փ˂��Ă��邩�ǂ���
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//�ڐG�ʒu�A�J�n�x�N�g���A�I���x�N�g��
	vecStart = m_posR;
	vecEnd = m_posV;
	//�����蔻��
	bColTerrain = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);
	//�ڐG�ʒu�ƒ����_�̋������v�Z
	float fDistCol;	//�ڐG�ʒu�Ƃ̋���
	D3DXVECTOR3 vecColTerrain = posColTerrain - m_posR;	//�����_����ڐG�ʒu�ւ̃x�N�g��
	fDistCol = D3DXVec3Length(&vecColTerrain);
	//���_�ƒ����_�̋������v�Z
	float fDistCamera;	//�ڐG�ʒu�Ƃ̋���
	D3DXVECTOR3 vecCamera = m_posV - m_posR;	//�����_����ڐG�ʒu�ւ̃x�N�g��
	fDistCamera = D3DXVec3Length(&vecCamera);
	//�Փ˂��Ă��āA���݂̃J�����̋����ȓ��ł���Έʒu�̕ύX
	if (bColTerrain && fDistCol <= fDistCamera) m_posV = posColTerrain;

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);
	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// �ʒu���W�̐ݒ�
//=============================================================================
void CCamera::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
}

//=============================================================================
// �ʒu���W�̎擾
//=============================================================================
D3DXVECTOR3 CCamera::GetPos(void) {
	return m_pos;
}

//=============================================================================
// ���_�̈ʒu���W�̎擾
//=============================================================================
D3DXVECTOR3 CCamera::GetPosV(void) {
	return m_posV;
}

//=============================================================================
// �����_�̈ʒu���W�̎擾
//=============================================================================
D3DXVECTOR3 CCamera::GetPosR(void) {
	return m_posR;
}

//=============================================================================
//�p�x�̐ݒ�
//=============================================================================
void CCamera::SetRot(D3DXVECTOR3 rot) {
	m_rot = rot;
}

//=============================================================================
//�p�x�̎擾
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void) {
	return m_rot;
}

//=============================================================================
//�����̐ݒ�
//=============================================================================
void CCamera::SetDistance(float fDistance) {
	m_fDistance = fDistance;
}

//=============================================================================
//�����̎擾
//=============================================================================
float CCamera::GetDistance(void) {
	return m_fDistance;
}