//=============================================================================
//
// 3D�I�u�W�F�N�g���� [scene3D.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CScene3D::CScene3D()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pVtxBuff = NULL;
	m_pIdxBuff = NULL;
	m_nNumVtx = 0;
	m_nNumIdx = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
// ��������
//=============================================================================
CScene3D* CScene3D::Create(void) {
	CScene3D* pScene3D;
	pScene3D = new CScene3D;
	if (pScene3D != NULL) {
		pScene3D->Init();
	}

	return pScene3D;
}

//=============================================================================
// 3D�I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CScene3D::Init(void) {

	return S_OK;
}

//=============================================================================
// 3D�I�u�W�F�N�g�̏I������
//=============================================================================
void CScene3D::Uninit(void) {
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL) {
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// 3D�I�u�W�F�N�g�̍X�V����
//=============================================================================
void CScene3D::Update(void) {

}

//=============================================================================
// 3D�I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CScene3D::Draw(void) {
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

	//�e�N�X�`���̎擾
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(GetTexType());

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

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture); //�e�N�X�`���̐ݒ�
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nNumVtx,	//�g�p���钸�_��
		0,	//�����̒l���ŏ��̃C���f�b�N�X
		m_nNumIdx - 2);	//�O�p�`�̐�
}

//=============================================================================
// �ʒu���W�̐ݒ�
//=============================================================================
void CScene3D::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
}

//=============================================================================
// �ʒu���W�̎擾
//=============================================================================
D3DXVECTOR3 CScene3D::GetPos(void) {
	return m_pos;
}

//=============================================================================
// �p�x�̐ݒ�
//=============================================================================
void CScene3D::SetRot(D3DXVECTOR3 rot) { m_rot = rot; }

//=============================================================================
// �p�x�̎擾
//=============================================================================
D3DXVECTOR3 CScene3D::GetRot(void) { return m_rot; }

//=============================================================================
// ���_�o�b�t�@�̃|�C���^�̃|�C���^���擾
//=============================================================================
LPDIRECT3DVERTEXBUFFER9* CScene3D::GetPtrVtxBuff(void) {
	return &m_pVtxBuff;
}

//=============================================================================
// �C���f�b�N�X�o�b�t�@�̃|�C���^�̃|�C���^���擾
//=============================================================================
LPDIRECT3DINDEXBUFFER9* CScene3D::GetPtrIdxBuff(void) {
	return &m_pIdxBuff;
}

//=============================================================================
//���_���̐ݒ�
//=============================================================================
void CScene3D::SetNumVtx(int nNumVtx) {
	m_nNumVtx = nNumVtx;
}

//=============================================================================
//���_���̎擾
//=============================================================================
int CScene3D::GetNumVtx(void) {
	return m_nNumVtx;
}

//=============================================================================
//�C���f�b�N�X���̐ݒ�
//=============================================================================
void CScene3D::SetNumIdx(int nNumIdx) {
	m_nNumIdx = nNumIdx;
}

//=============================================================================
//�C���f�b�N�X���̎擾
//=============================================================================
int CScene3D::GetNumIdx(void) {
	return m_nNumIdx;
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CScene3D::SetColor(D3DXCOLOR col) {
	if (m_pVtxBuff != NULL) {
		VERTEX_3D *pVtx;
		//���_�o�b�t�@�̃��b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_����ݒ�
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}