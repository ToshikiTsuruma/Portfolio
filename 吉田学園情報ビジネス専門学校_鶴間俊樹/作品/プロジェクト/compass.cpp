//=============================================================================
//
// �R���p�X���� [compass.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "compass.h"
#include "manager.h"
#include "camera.h"
#include "game.h"
#include "sceneModel.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define COMPASS_WIDTH (800.0f)	//�R���p�X�̕�

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CCompass::CCompass()
{
	//�ڐ���̐���
	m_pDial = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::COMPASS_DIAL, COMPASS_WIDTH, 30.0f);
	if (m_pDial != nullptr) {
		m_pDial->SetDrawPriority(CScene::DRAW_PRIORITY::UI);
	}
	m_pMark = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 45.0f, 0.0f), CTexture::TEXTURE_TYPE::COMPASS_MARK, 50.0f, 50.0f);
	if (m_pMark != nullptr) {
		m_pMark->SetDrawPriority(CScene::DRAW_PRIORITY::UI);
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCompass::~CCompass()
{

}

//=============================================================================
// �R���p�X�̐�������
//=============================================================================
CCompass* CCompass::Create() {
	CCompass* pCompass;
	pCompass = new CCompass();
	if (pCompass != nullptr) {
		pCompass->Init();
	}

	return pCompass;
}

//=============================================================================
// �R���p�X�̏���������
//=============================================================================
HRESULT CCompass::Init(void) {
	return S_OK;
}

//=============================================================================
// �R���p�X�̏I������
//=============================================================================
void CCompass::Uninit(void) {
	//�ڐ���̔j��
	if (m_pDial != nullptr) {
		m_pDial->Uninit();
		m_pDial = nullptr;
	}
	//�}�[�N�̔j��
	if (m_pMark != nullptr) {
		m_pMark->Uninit();
		m_pMark = nullptr;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �R���p�X�̍X�V����
//=============================================================================
void CCompass::Update(void) {
	CManager* pManager = CManager::GetManager();

	CCamera* pCamera = nullptr;
	CGame* pGame = nullptr;
	if (pManager != nullptr) {
		//�J�����̎擾
		pCamera = pManager->GetCamera();
		//�Q�[���̎擾
		pGame = pManager->GetGame();
	}

	CSceneModel* pRaft = nullptr;
	if (pGame != nullptr) pRaft = pGame->GetRaft();

	if (m_pMark != nullptr) {
		if (pRaft != nullptr && pCamera != nullptr) {
			D3DXVECTOR2 vecCameraR = D3DXVECTOR2(pCamera->GetPosR().x - pCamera->GetPosV().x, pCamera->GetPosR().z - pCamera->GetPosV().z);	//�J�����̎��_���璍���_�ւ̏ォ�猩���x�N�g��
			D3DXVECTOR2 vecRaft = D3DXVECTOR2(pRaft->GetPos().x - pCamera->GetPosV().x, pRaft->GetPos().z - pCamera->GetPosV().z);			//�J�����̎��_���申�ւ̏ォ�猩���x�N�g��			
			float fRotCameraR = (float)atan2(vecCameraR.x, vecCameraR.y);	//���_���璍���_�ւ̊p�x
			float fRotRaft = (float)atan2(vecRaft.x, vecRaft.y);			//���_���申�ւ̊p�x

			//�p�x�𐳂̒l�ɂ���(0����2��)
			if (fRotCameraR < 0.0f) fRotCameraR += D3DX_PI * 2;
			if (fRotRaft < 0.0f) fRotRaft += D3DX_PI * 2;

			float fRotDelta = fRotRaft - fRotCameraR;	//�Q�̊p�x�̍���

			//�p�C���ߎ��̒���
			if (fRotDelta > D3DX_PI) {
				fRotDelta -= D3DX_PI * 2;
			}
			if (fRotDelta < -D3DX_PI) {
				fRotDelta += D3DX_PI * 2;
			}

			//�͈͂��@-��/2 �`�@��/2�@�ɒ���
			if (fRotDelta > D3DX_PI / 2.0f) fRotDelta = D3DX_PI / 2.0f;
			if (fRotDelta < -D3DX_PI / 2.0f) fRotDelta = -D3DX_PI / 2.0f;

			//�}�[�N�̈ʒu�̐ݒ�
			D3DXVECTOR3 posMark = m_pMark->GetPos();
			posMark.x = (SCREEN_WIDTH / 2.0f) + (fRotDelta / (D3DX_PI / 2.0f)) * (COMPASS_WIDTH / 2.0f);
			m_pMark->SetPos(posMark);
		}
		//�������݂��Ȃ��ꍇ
		else {
			//��ɒ���
			D3DXVECTOR3 posMark = m_pMark->GetPos();
			posMark.x = SCREEN_WIDTH / 2.0f;
			m_pMark->SetPos(posMark);
		}
	}
}

//=============================================================================
// �R���p�X�̕`�揈��
//=============================================================================
void CCompass::Draw(void) {}