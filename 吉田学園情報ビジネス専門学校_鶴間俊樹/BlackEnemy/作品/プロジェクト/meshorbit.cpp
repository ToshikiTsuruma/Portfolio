//=============================================================================
//
// ���b�V���O������ [meshorbit.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "meshorbit.h"
#include "manager.h"
#include "renderer.h"
#include "pause.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CMeshorbit::CMeshorbit() : m_nNumVtx(0)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CMeshorbit::CMeshorbit(int nNumVtx) : m_nNumVtx(nNumVtx)
{
	SetDrawPriority(DRAW_PRIORITY::CLEAR);	//�`�揇�̐ݒ�

	m_pMtxParent = nullptr;
	m_pVtxBuff = nullptr;
	for (int nCnt = 0; nCnt < 2; nCnt++) {
		m_aOffset[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCol[nCnt] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
	m_nSpanUpdate = 0;
	m_nCntUpdate = 0;
	m_bBindPos = false;
	m_bVisible = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshorbit::~CMeshorbit()
{

}

//=============================================================================
// ���b�V���O���̐�������
//=============================================================================
CMeshorbit* CMeshorbit::Create(D3DXMATRIX* pMtxParent, int nNumVtx, int nSpanUpdate, D3DXVECTOR3 offset0, D3DXVECTOR3 offset1, D3DXCOLOR col0, D3DXCOLOR col1) {
	CMeshorbit* pMeshorbit;
	pMeshorbit = new CMeshorbit(nNumVtx);
	if (pMeshorbit != nullptr) {
		pMeshorbit->m_pMtxParent = pMtxParent;
		pMeshorbit->m_aOffset[0] = offset0;
		pMeshorbit->m_aOffset[1] = offset1;
		pMeshorbit->m_aCol[0] = col0;
		pMeshorbit->m_aCol[1] = col1;
		pMeshorbit->m_nSpanUpdate = nSpanUpdate;
		pMeshorbit->Init();
	}

	return pMeshorbit;
}

//=============================================================================
// ���b�V���O���̏���������
//=============================================================================
HRESULT CMeshorbit::Init(void) {
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

	m_nCntUpdate = 0;

	//���_�o�b�t�@�̐���
	if (m_pVtxBuff == nullptr) {
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr);
	}

	//���_�o�b�t�@�̐ݒ�
	if (m_pVtxBuff != nullptr) {
		VERTEX_3D *pVtx;	//���_�o�b�t�@�̃|�C���^
		//���_�o�b�t�@�����b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//���_�̐ݒ�
		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++) {
			//���_���W�̐ݒ�
			pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//�@���x�N�g���̐ݒ�
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//�F�̐ݒ�
			D3DXCOLOR col = m_aCol[nCntVtx % 2];
			col.a = 1 - (float)(nCntVtx / 2) / (float)(m_nNumVtx / 2);	//�����x�𒸓_���̊����Őݒ�
			pVtx[nCntVtx].col = col;

			pVtx[nCntVtx].tex = D3DXVECTOR2((float)nCntVtx / (float)2, (float)(nCntVtx % 2));
		}
		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���b�V���O���̏I������
//=============================================================================
void CMeshorbit::Uninit(void) {
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// ���b�V���O���̍X�V����
//=============================================================================
void CMeshorbit::Update(void) {
	//���f���̃��[���h�}�g���b�N�X��draw�ōX�V����邽�߁A���_�̍X�V��draw�Ɉړ�
}

//=============================================================================
// ���b�V���O���̕`�揈��
//=============================================================================
void CMeshorbit::Draw(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	

	CRenderer* pRenderer = nullptr;		
	CPause* pPause = nullptr;

	if (pManager != nullptr) {
		//�����_���[�̎擾
		pRenderer = pManager->GetRenderer();
		//�|�[�Y�̎擾
		pPause = pManager->GetPause();
	}
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return;

	//�s���̏ꍇ�I��
	if (!m_bVisible) return;

	bool bPause = false;	//�|�[�Y��Ԃ��ǂ���
	if (pPause != nullptr) {
		bPause = pPause->GetPause();
	}

	//�|�[�Y���ɂ͋O�Ղ̈ړ����s��Ȃ�
	if (!bPause) {
		//���_�̍X�V
		VERTEX_3D *pVtx;
		D3DXMATRIX mtxWorldPoint[2];	//�O���̌��_�̒��_�̃��[���h�}�g���b�N�X
		D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
		D3DXVECTOR3 posPoint[2];		//�O���̌��_�̒��_�̈ʒu

		if (m_pVtxBuff != nullptr) {
			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			//�O���̌��_�̒��_���X�V
			if (m_pMtxParent != nullptr && !m_bBindPos) {
				//�O���̌��_�̒��_�̎擾
				for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++) {
					//���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&mtxWorldPoint[nCntVtx]);
					//�ʒu�𔽉f
					D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntVtx].x, m_aOffset[nCntVtx].y, m_aOffset[nCntVtx].z);
					D3DXMatrixMultiply(&mtxWorldPoint[nCntVtx], &mtxWorldPoint[nCntVtx], &mtxTrans);
					//�e�}�g���b�N�X�Ɗ|�����킹��
					D3DXMatrixMultiply(&mtxWorldPoint[nCntVtx], &mtxWorldPoint[nCntVtx], m_pMtxParent);
					posPoint[nCntVtx].x = mtxWorldPoint[nCntVtx]._41;
					posPoint[nCntVtx].y = mtxWorldPoint[nCntVtx]._42;
					posPoint[nCntVtx].z = mtxWorldPoint[nCntVtx]._43;
				}
			}
			//�e�}�g���b�N�X��NULL�̏ꍇ�A�P�t���[���O�̋O���̌��_���ړ������Ȃ��悤�ɂ���
			else {
				posPoint[0] = pVtx[0].pos;
				posPoint[1] = pVtx[1].pos;
			}

			bool bEqualALL = true;	//���ׂĂ̒��_�̈ʒu��񂪓��������ǂ���
			if (m_nCntUpdate >= m_nSpanUpdate) {
				//�ЂƂO�̋O���̒��_���R�s�[
				for (int nCntVtx = m_nNumVtx - 1; nCntVtx > 1; nCntVtx--) {
					//��O�̒��_���r
					if (pVtx[nCntVtx].pos != pVtx[nCntVtx - 2].pos && bEqualALL) bEqualALL = false;	//�قȂ����ꍇ
					//���_�̃R�s�[
					pVtx[nCntVtx].pos = pVtx[nCntVtx - 2].pos;
				}
				//�o�C���h��Ԏ��ɑS���_�������ʒu�Ȃ�s����Ԃɂ���
				if (m_bBindPos && bEqualALL) SetVisible(false);

				m_nCntUpdate = 0;
			}
			else {
				m_nCntUpdate++;
			}
			//�O���̌��_�̒��_��e�̃}�g���b�N�X�̈ʒu�Ɉړ�������
			pVtx[0].pos = posPoint[0];
			pVtx[1].pos = posPoint[1];

			m_pVtxBuff->Unlock();
		}
	}

	//�e�N�X�`���̎擾
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(GetTexType());

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//-------------------------------
	//�`��̐ݒ�
	//-------------------------------
	//���C�g���f�Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//�J�����O�Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//Z�o�b�t�@�̍X�V���s�\
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//-------------------------------
	//�`��
	//-------------------------------
	//�������������[���h�}�g���b�N�X��ݒ�i���_�����[���h���W�Őݒ肵�Ă��邽�߁j
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture);
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0,
		m_nNumVtx - 2);

	//-------------------------------
	//�`��̐ݒ��߂�
	//-------------------------------
	//Z�o�b�t�@�̍X�V���\
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//�J�����O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//���C�g���f
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ���b�V���O���̈ʒu���Œ�
//=============================================================================
void CMeshorbit::SetBindPos(void) {
	m_bBindPos = true;
}

//=============================================================================
// �o�C���h��Ԃ̎擾
//=============================================================================
bool CMeshorbit::GetBindPos(void) {
	return m_bBindPos;
}

//=============================================================================
// ���b�V���O���̉���Ԃ̐ݒ�
//=============================================================================
void CMeshorbit::SetVisible(bool bVisible) {
	m_bVisible = bVisible;
	m_bBindPos = false;	//�o�C���h������

	//����Ԃɐݒ肵���ꍇ�A���ׂĂ̒��_��e�̃}�g���b�N�X�̈ʒu�Ɉړ�
	if (bVisible && m_pMtxParent != nullptr) {
		//���_�o�b�t�@�̐ݒ�
		if (m_pVtxBuff != nullptr) {
			D3DXMATRIX mtxWorldPoint[2];	//�O���̌��_�̒��_�̃��[���h�}�g���b�N�X
			D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
			D3DXVECTOR3 posPoint[2];		//�O���̌��_�̒��_�̈ʒu
			//�O���̌��_�̒��_�̎擾
			for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++) {
				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxWorldPoint[nCntVtx]);
				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntVtx].x, m_aOffset[nCntVtx].y, m_aOffset[nCntVtx].z);
				D3DXMatrixMultiply(&mtxWorldPoint[nCntVtx], &mtxWorldPoint[nCntVtx], &mtxTrans);
				//�e�}�g���b�N�X�Ɗ|�����킹��
				D3DXMatrixMultiply(&mtxWorldPoint[nCntVtx], &mtxWorldPoint[nCntVtx], m_pMtxParent);
				posPoint[nCntVtx].x = mtxWorldPoint[nCntVtx]._41;
				posPoint[nCntVtx].y = mtxWorldPoint[nCntVtx]._42;
				posPoint[nCntVtx].z = mtxWorldPoint[nCntVtx]._43;
			}


			VERTEX_3D *pVtx;	//���_�o�b�t�@�̃|�C���^
			//���_�o�b�t�@�����b�N
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			//���_�̐ݒ�
			for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++) {
				//���_���W�̐ݒ�
				pVtx[nCntVtx].pos = posPoint[nCntVtx % 2];
			}
			//���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();
		}
	}
}

//=============================================================================
// ���b�V���O���̉���Ԃ̎擾
//=============================================================================
bool CMeshorbit::GetVisible(void) {
	return m_bVisible;
}