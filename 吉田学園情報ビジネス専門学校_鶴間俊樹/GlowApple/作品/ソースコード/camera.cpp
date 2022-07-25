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

#include "scene.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_DRAW_DISTANCE (5000.0f)		//�`��\�ȍő�̋���
#define DEFAULT_CAMERA_DISTANCE (1500.0f)	//���_�ƒ����_�̋���
#define CAMERA_ROTATE_SPEED_YAW (0.008f)	//�J�����̉�]���x(Y��)
#define CAMERA_ROTATE_SPEED_PITCH (0.006f)	//�J�����̉�]���x(X��)
#define MAX_ROTATE_PITCH (-0.05f)	//�J������X����]�̍ő�
#define MIN_ROTATE_PITCH (-0.18f)	//�J������X����]�̍Œ�

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
	ZeroMemory(&m_viewport, sizeof(D3DVIEWPORT9));

	m_bLockControll = false;
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
CCamera* CCamera::Create() {
	CCamera* pCamera;
	pCamera = new CCamera;
	if (pCamera == nullptr) return nullptr;

	pCamera->Init();

	return pCamera;
}

//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT CCamera::Init(void) {
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot.x = D3DX_PI * -0.15f;
	m_rot.y = D3DX_PI * 0.0f;

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
	m_viewport.X = (DWORD)0;
	m_viewport.Y = (DWORD)0;
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
		MAX_DRAW_DISTANCE);							//���̈ʒu

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pRenderer->SetEffectMatrixProj(m_mtxProjection);

	//��x�N�g���̐ݒ�
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�r���[�}�g���b�N�X�̐ݒ�
	SetCamera();

	//�����̐ݒ�
	SetDistance(DEFAULT_CAMERA_DISTANCE);

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
	//�ړ�����
	//Move();

	//��]����
	if(!m_bLockControll) Rotate();
}

//=============================================================================
// �J�����̃Z�b�g
//=============================================================================
void CCamera::SetCamera(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�����_���[�̎擾
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;


	//------------------------------------
	//�v���C���[�Ƃ̈ʒu�̊Ԃ��擾
	//------------------------------------
	CScene* pScene = pManager->GetScene();
	CPlayer* pPlayer = nullptr;
	if (pScene != nullptr) pPlayer = pScene->GetPlayer();

	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();

	D3DXVECTOR3 posCamera = m_pos + (posPlayer - m_pos) / 2.0f;
	posCamera.y = m_pos.y;

	//------------------------------------
	//���_�ƒ����_�̐ݒ�
	//------------------------------------
	m_posR = posCamera;
	m_posV = D3DXVECTOR3(posCamera.x + sinf(m_rot.y) * m_fDistance * cosf(m_rot.x + D3DX_PI),
		posCamera.y + sinf(m_rot.x + D3DX_PI) * m_fDistance,
		posCamera.z + cosf(m_rot.y) * m_fDistance * cosf(m_rot.x + D3DX_PI));	//���_�����S�̏ꍇ����ύX�����Ƃ��Ɉړ����Ȃ��悤��X��]��PI�𑫂��Ă���

	//------------------------------------
	//�r���[�}�g���b�N�X�̍쐬
	//------------------------------------
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);
	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);	//�r���{�[�h�̂��߂Ɏc��
	//�V�F�[�_�Ƀr���[�}�g���b�N�X��ݒ�
	pRenderer->SetEffectMatrixView(m_mtxView);


	//------------------------------------
	//�V�F�[�_�̃��C�g�̈ʒu���X�V
	//------------------------------------
	D3DXMATRIX mtxLightView;   // ���C�g�r���[�ϊ�
	D3DXVECTOR3 posLight;
	//posLight = m_posV + D3DXVECTOR3(0.0f, 1600.0f, -800.0f);
	posLight = D3DXVECTOR3(0.0f, 2200.0f, -1200.0f);
	D3DXVECTOR3 vecLight = D3DXVECTOR3(0.0f, -1.0f, 0.4f);	//���C�g�̃x�N�g��
	//���C�g�̃r���[�}�g���b�N�X�𐶐�
	D3DXMatrixLookAtLH(&mtxLightView, &posLight, &D3DXVECTOR3(posLight + vecLight), &D3DXVECTOR3(0, 1, 0));
	pRenderer->SetEffectLightMatrixView(mtxLightView);

}

//=============================================================================
// �ړ�
//=============================================================================
void CCamera::Move(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	CInput* pInput = nullptr;	//���̓f�o�C�X�ւ̃|�C���^

	//���݂̓��̓f�o�C�X�̎擾
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//null�̏ꍇ�I��
	if (pInput == nullptr) return;

	//--------------------
	//���͏���
	//--------------------
	float fMoveSpeed = 5.0f;
	if (pInput->GetPress(CInput::CODE::DASH)) {
		fMoveSpeed *= 3;
	}

	if (pInput->GetPress(CInput::CODE::MOVE_UP)) {
		//�O�ړ�
		m_pos.z += fMoveSpeed * cosf(m_rot.y);
		m_pos.x += fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_DOWN)) {
		//���ړ�
		m_pos.z -= fMoveSpeed * cosf(m_rot.y);
		m_pos.x -= fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_LEFT)) {
		//���ړ�
		m_pos.x -= fMoveSpeed * cosf(m_rot.y);
		m_pos.z += fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RIGHT)) {
		//�E�ړ�
		m_pos.x += fMoveSpeed * cosf(m_rot.y);
		m_pos.z -= fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RISE)) {
		//�㏸
		m_pos.y += fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_FALL)) {
		//���~
		m_pos.y -= fMoveSpeed;
	}
}

//=============================================================================
// ��]
//=============================================================================
void CCamera::Rotate(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	CInput* pInput = nullptr;	//���̓f�o�C�X�ւ̃|�C���^

	//���݂̓��̓f�o�C�X�̎擾
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//null�̏ꍇ�I��
	if (pInput == nullptr) return;

	//--------------------
	//���͏���
	//--------------------
	if (pInput->GetPress(CInput::CODE::ROTATE_UP)) {
		//���_��������
		m_rot.x += CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
		if (m_rot.x > MAX_ROTATE_PITCH * D3DX_PI) {
			m_rot.x = MAX_ROTATE_PITCH * D3DX_PI;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_DOWN)) {
		//���_���グ��
		m_rot.x -= CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
		if (m_rot.x < MIN_ROTATE_PITCH * D3DX_PI) {
			m_rot.x = MIN_ROTATE_PITCH * D3DX_PI;
		}
	}

	if (pInput->GetPress(CInput::CODE::ROTATE_LEFT)) {
		//���_������]
		m_rot.y -= CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
		if (m_rot.y < -D3DX_PI) {
			m_rot.y = m_rot.y + D3DX_PI * 2;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_RIGHT)) {
		//���_���E��]
		m_rot.y += CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
		if (m_rot.y > D3DX_PI) {
			m_rot.y = m_rot.y - D3DX_PI * 2;
		}
	}

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
// �p�x�̐ݒ�
//=============================================================================
void CCamera::SetRot(D3DXVECTOR3 rot) {
	m_rot = rot;
}

//=============================================================================
// �p�x�̎擾
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void) {
	return m_rot;
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void CCamera::SetDistance(float fDistance) {
	m_fDistance = fDistance;
}

//=============================================================================
// �����̎擾
//=============================================================================
float CCamera::GetDistance(void) {
	return m_fDistance;
}