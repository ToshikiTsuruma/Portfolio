//=============================================================================
//
// ���b�V���V�����_�[���� [meshcylinder.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CMeshcylinder::CMeshcylinder() : m_nNumXZ(0), m_nNumY(0)
{

}

//=============================================================================
//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CMeshcylinder::CMeshcylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadius, int nNumY, float fHeight, bool bOut, D3DXCOLOR col) : m_nNumXZ(nNumXZ), m_nNumY(nNumY)
{
	SetPos(pos);
	SetRot(rot);
	m_fRadius = fRadius;
	m_fHeight = fHeight;
	m_bOut = bOut;
	m_col = col;
	SetNumVtx((nNumXZ + 1) * (nNumY + 1));	//(XZ�����̃|���S���� + 1) * (Y�����̃|���S���� + 1)
	SetNumIdx((2 * (nNumXZ + 1)) * nNumY + (nNumY - 1) * 2);	//XZ�����Ŏg�����_�� * Y�����̃|���S���� + �k�ރ|���S���̐�
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshcylinder::~CMeshcylinder()
{

}

//=============================================================================
// ���b�V���V�����_�[�̐�������
//=============================================================================
CMeshcylinder* CMeshcylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadius, int nNumY, float fHeight,  bool bOut, D3DXCOLOR col) {
	CMeshcylinder* pMeshcylinder;
	pMeshcylinder = new CMeshcylinder(pos, rot, nNumXZ, fRadius, nNumY, fHeight, bOut, col);
	if (pMeshcylinder != nullptr) {
		pMeshcylinder->Init();
	}

	return pMeshcylinder;
}

//=============================================================================
// ���b�V���V�����_�[�̏���������
//=============================================================================
HRESULT CMeshcylinder::Init(void) {
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

	//���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9* ppVtxBuff = GetPtrVtxBuff();

	int nNumVtx = GetNumVtx();	//���_���̎擾

	//���_�o�b�t�@�̐���
	if (*ppVtxBuff == nullptr) {
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			ppVtxBuff,
			nullptr);
	}

	//���_�o�b�t�@�̐ݒ�
	if (*ppVtxBuff != nullptr) {
		VERTEX_3D *pVtx;	//���_�o�b�t�@�̃|�C���^
		//���_�o�b�t�@�����b�N
		(*ppVtxBuff)->Lock(0, 0, (void**)&pVtx, 0);
		//���_�̐ݒ�
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++) {
			float fRotXZ = ((nCntVtx % (m_nNumXZ + 1)) / (float)m_nNumXZ) * (D3DX_PI * 2);	//Y�����猩���p�x

			//�|���S���̌������O�����̏ꍇ
			if (m_bOut) {
				//�t�����ɉ�]
				fRotXZ *= -1.0f;
			}

			//���_���W�̐ݒ�
			pVtx[nCntVtx].pos = D3DXVECTOR3(sinf(fRotXZ) * m_fRadius,
				(nCntVtx / (m_nNumXZ + 1)) * -m_fHeight + m_fHeight * m_nNumY,
				cosf(fRotXZ) * m_fRadius);

			//�@���x�N�g���̐ݒ�
			D3DXVECTOR3 vecNor = D3DXVECTOR3(pVtx[nCntVtx].pos.x, 0.0f, pVtx[nCntVtx].pos.z);	//�@�������̃x�N�g��
			D3DXVec3Normalize(&vecNor, &vecNor);	//�x�N�g���̐��K��
			pVtx[nCntVtx].nor = vecNor;	//���K�������x�N�g����ݒ�

			//�~�����������̏ꍇ
			if (!m_bOut) {
				//�@�����t�ɐݒ�
				pVtx[nCntVtx].nor.x *= -1;
				pVtx[nCntVtx].nor.y *= -1;
				pVtx[nCntVtx].nor.z *= -1;
			}

			pVtx[nCntVtx].col = m_col;

			pVtx[nCntVtx].tex = D3DXVECTOR2((float)(nCntVtx % (m_nNumXZ + 1)), (float)nCntVtx / (float)(m_nNumXZ + 1));
		}
		//���_�o�b�t�@���A�����b�N����
		(*ppVtxBuff)->Unlock();
	}

	//�C���f�b�N�X�o�b�t�@�̂̎擾
	LPDIRECT3DINDEXBUFFER9* ppIdxBuff = GetPtrIdxBuff();

	int nNumIdx = GetNumIdx();	//�C���f�b�N�X���̎擾

	//�C���f�b�N�X�o�b�t�@�̐���
	if (*ppIdxBuff == nullptr) {
		pDevice->CreateIndexBuffer(sizeof(WORD) * nNumIdx,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			ppIdxBuff,
			nullptr);
	}
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	if (*ppIdxBuff != nullptr) {
		WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^
		//�C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
		(*ppIdxBuff)->Lock(0, 0, (void**)&pIdx, 0);
		//�ԍ��f�[�^�ݒ�
		for (int nCntIdx = 0; nCntIdx < (m_nNumXZ + 1) * 2 * m_nNumY + (m_nNumY - 1) * 2; nCntIdx++) {
			if (nCntIdx == (m_nNumXZ + 2) * 2 * (1 + nCntIdx / ((m_nNumXZ + 2) * 2)) - 2) {
				pIdx[nCntIdx] = pIdx[nCntIdx - 1];
			}
			else if (nCntIdx == (m_nNumXZ + 2) * 2 * (1 + nCntIdx / ((m_nNumXZ + 2) * 2)) - 1) {
				pIdx[nCntIdx] = (m_nNumXZ + 1) * (2 + nCntIdx / ((m_nNumXZ + 1) * 2 + 2));
			}
			else {
				pIdx[nCntIdx] = (m_nNumXZ + 1 + nCntIdx / 2) - (nCntIdx / ((m_nNumXZ + 2) * 2)) - ((m_nNumXZ + 1)*(nCntIdx % 2));//���@�k�ށ@�� �k�ނɏC��
			}
		}
		//�C���f�b�N�X�o�b�t�@���A�����b�N����
		(*ppIdxBuff)->Unlock();
	}

	CScene3D::Init();

	return S_OK;
}

//=============================================================================
// ���b�V���V�����_�[�̏I������
//=============================================================================
void CMeshcylinder::Uninit(void) {
	CScene3D::Uninit();
}

//=============================================================================
// ���b�V���V�����_�[�̍X�V����
//=============================================================================
void CMeshcylinder::Update(void) {
	CScene3D::Update();
}

//=============================================================================
// ���b�V���V�����_�[�̕`�揈��
//=============================================================================
void CMeshcylinder::Draw(void) {
	//�`��
	CScene3D::Draw();
}

//=============================================================================
// ���b�V���V�����_�[�̔��a�̎擾
//=============================================================================
float CMeshcylinder::GetRadius(void) {
	return m_fRadius;
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CMeshcylinder::SetColor(D3DXCOLOR col) {
	//���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9* ppVtxBuff = GetPtrVtxBuff();
	//���_���̎擾
	int nNumVtx = GetNumVtx();	

	//���_�o�b�t�@�̐ݒ�
	if (*ppVtxBuff != nullptr) {
		//�F�̐ݒ�
		m_col = col;

		VERTEX_3D *pVtx;	//���_�o�b�t�@�̃|�C���^
		//���_�o�b�t�@�����b�N
		(*ppVtxBuff)->Lock(0, 0, (void**)&pVtx, 0);
		//���_�̐ݒ�
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++) {
			pVtx[nCntVtx].col = m_col;
		}
		//���_�o�b�t�@���A�����b�N����
		(*ppVtxBuff)->Unlock();
	}
}

//=============================================================================
// �F�̎擾
//=============================================================================
D3DXCOLOR CMeshcylinder::GetColor(void) {
	return m_col;
}