//=============================================================================
//
// 2D�I�u�W�F�N�g���� [scene2D.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CScene2D::CScene2D()
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fRatioWidth = 1.0f;
	m_fHeight = 0.0f;
	m_fRatioHeight = 1.0f;
	m_fAngle = 0.0f;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	SetDrawPriority(DRAW_PRIORITY::UI);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// 2D�I�u�W�F�N�g�̐�������
//=============================================================================
CScene2D* CScene2D::Create(D3DXVECTOR3 pos, CTexture::TEXTURE_TYPE type, float fWidth, float fHeight) {
	CScene2D* pScene2D;
	pScene2D = new CScene2D;
	if (pScene2D != nullptr) {
		LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
		//�}�l�[�W���[�̎擾
		CManager* pManager = CManager::GetManager();	
		//�����_���[�̎擾
		CRenderer* pRenderer = nullptr;			
		if (pManager != nullptr) pRenderer = pManager->GetRenderer();
		//�f�o�C�X�̎擾
		if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

		if (pDevice != nullptr) {
			//���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&(pScene2D->m_pVtxBuff),
				nullptr);
		}

		pScene2D->m_pos = pos;
		pScene2D->m_fWidth = fWidth;
		pScene2D->m_fHeight = fHeight;
		pScene2D->SetTexType(type);
		pScene2D->Init();
	}

	return pScene2D;
}

//=============================================================================
// 2D�I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CScene2D::Init(void) {
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_����ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y - m_fHeight / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth / 2.0f, m_pos.y - m_fHeight / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
	return S_OK;
}

//=============================================================================
// 2D�I�u�W�F�N�g�̏I������
//=============================================================================
void CScene2D::Uninit(void) {
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//�I�u�W�F�N�g�̔j��
   	Release();
}

//=============================================================================
// 2D�I�u�W�F�N�g�̍X�V����
//=============================================================================
void CScene2D::Update(void) {

	//���_�o�b�t�@�̍X�V
	//SetVtxPos();
}

//=============================================================================
// 2D�I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CScene2D::Draw(void) {
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

	if (m_pVtxBuff != nullptr && pDevice != nullptr) {
		pDevice->SetFVF(FVF_VERTEX_2D);		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�		
		pDevice->SetTexture(0, pTexture);	//�e�N�X�`���̐ݒ�
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
			0,
			2);
	}
}

//=============================================================================
// 2D�I�u�W�F�N�g�̈ʒu���̐ݒ�
//=============================================================================
void CScene2D::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
	//���_�o�b�t�@�̍X�V
	SetVtxPos();
}

//=============================================================================
// 2D�I�u�W�F�N�g�̈ʒu���̎擾
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void) { return m_pos; }

//=============================================================================
// 2D�I�u�W�F�N�g�̈ړ��ʂ̐ݒ�
//=============================================================================
void CScene2D::SetMove(D3DXVECTOR3 move) { m_move = move; }

//=============================================================================
// 2D�I�u�W�F�N�g�̈ړ��ʂ̎擾
//=============================================================================
D3DXVECTOR3 CScene2D::GetMove(void) { return m_move; }

//=============================================================================
// 2D�I�u�W�F�N�g�̃T�C�Y�̐ݒ�
//=============================================================================
void CScene2D::SetSize(D3DXVECTOR3 size) {
	m_fWidth = size.x;
	m_fHeight = size.y;
	//���_�o�b�t�@�̍X�V
	SetVtxPos();
}

//=============================================================================
// 2D�I�u�W�F�N�g�̃T�C�Y�̎擾
//=============================================================================
D3DXVECTOR3 CScene2D::GetSize(void) { return D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f); }

//=============================================================================
// 2D�I�u�W�F�N�g�̕��̊����̐ݒ�
//=============================================================================
void CScene2D::SetRatioWidth(float fRatio) {
	m_fRatioWidth = fRatio;

	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//���_�o�b�t�@�̃��b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f, 0.0f);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 2D�I�u�W�F�N�g�̕��̊����̐ݒ�
//=============================================================================
void CScene2D::SetRatioHeight(float fRatio) {
	m_fRatioHeight = fRatio;

	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//���_�o�b�t�@�̃��b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f, 0.0f);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//�p�x�̐ݒ�
//=============================================================================
void CScene2D::SetAngle(float fAngle) {
	m_fAngle = fAngle;
	//���_�o�b�t�@�̍X�V
	SetVtxPos();
}

//=============================================================================
//�p�x�̎擾
//=============================================================================
float CScene2D::GetAngle(void) { return m_fAngle; }

//=============================================================================
//�e���_�̈ʒu���X�V
//=============================================================================
void CScene2D::SetVtxPos(void) {
	if (m_pVtxBuff != nullptr) {
		float fRadius = sqrtf(powf(m_fWidth / 2.0f, 2.0f) + powf(m_fHeight / 2.0f, 2.0f));	//�|���S���̑Ίp���̔���
		float fAngleDelta = atan2f(m_fWidth, m_fHeight);	//��̊p�x����̍����i���W�A���j

		VERTEX_2D *pVtx;
		//���_�o�b�t�@�̃��b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_����ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle * D3DX_PI - fAngleDelta) * fRadius, m_pos.y + cosf((m_fAngle + 1.0f) * D3DX_PI - fAngleDelta) * fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle * D3DX_PI + fAngleDelta) * fRadius, m_pos.y + cosf((m_fAngle + 1.0f) * D3DX_PI + fAngleDelta) * fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf((m_fAngle + 1.0f) * D3DX_PI + fAngleDelta) * fRadius, m_pos.y + cosf(m_fAngle * D3DX_PI + fAngleDelta) * fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf((m_fAngle + 1.0f) * D3DX_PI - fAngleDelta) * fRadius, m_pos.y + cosf(m_fAngle * D3DX_PI - fAngleDelta) * fRadius, 0.0f);
		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//�J���[�̐ݒ�
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR col) {
	m_col = col;

	//���_�o�b�t�@�̍X�V
	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//���_�o�b�t�@�̃��b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_����ݒ�
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//�J���[�̎擾
//=============================================================================
D3DXCOLOR CScene2D::GetColor(void) { return m_col; }

//=============================================================================
//�e�N�X�`�����W�̐ݒ�
//=============================================================================
void CScene2D::SetTexPos(float startU, float startV, float endU, float endV) {
	VERTEX_2D *pVtx;
	//���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_����ݒ�
	pVtx[0].tex = D3DXVECTOR2(startU, startV);
	pVtx[1].tex = D3DXVECTOR2(endU, startV);
	pVtx[2].tex = D3DXVECTOR2(startU, endV);
	pVtx[3].tex = D3DXVECTOR2(endU, endV);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}