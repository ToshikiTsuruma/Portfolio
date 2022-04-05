//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CMeshfield::CMeshfield()
{
	m_nNumX = 0;
	m_nNumZ = 0;
	m_fWidth = 0.0f;
	m_fDepth = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshfield::~CMeshfield()
{

}

//=============================================================================
// ��������
//=============================================================================
CMeshfield* CMeshfield::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumX, int nNumZ, float fWidth, float fDepth) {
	CMeshfield* pMeshfield;
	pMeshfield = new CMeshfield;
	if (pMeshfield != nullptr) {
		pMeshfield->SetPos(pos);
		pMeshfield->SetRot(rot);
		pMeshfield->m_nNumX = nNumX;
		pMeshfield->m_nNumZ = nNumZ;
		pMeshfield->m_fWidth = fWidth;
		pMeshfield->m_fDepth = fDepth;
		pMeshfield->SetNumVtx((nNumX + 1) * (nNumZ + 1));	//(X�����̃|���S���� + 1) * (Z�����̃|���S���� + 1)
		pMeshfield->SetNumIdx((2 * (nNumX + 1)) * nNumZ + (nNumZ - 1) * 2);	//X�����Ŏg�����_�� * Z�����̃|���S���� + �k�ރ|���S���̐�
		pMeshfield->Init();
	}

	return pMeshfield;
}

//=============================================================================
// ���b�V���t�B�[���h�̏���������
//=============================================================================
HRESULT CMeshfield::Init(void) {
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

	//�e�N�X�`���̐ݒ�
	SetTexType(CTexture::TEXTURE_TYPE::NONE);

	//���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9* ppVtxBuff = GetPtrVtxBuff();

	int nNumVtx = GetNumVtx();	//���_���̎擾
	int nNumIdx = GetNumIdx();	//�C���f�b�N�X���̎擾

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

			//���_���W�̐ݒ�
			pVtx[nCntVtx].pos = D3DXVECTOR3((nCntVtx % (m_nNumX + 1)) * m_fWidth - (m_fWidth / 2.0f) * m_nNumX,
				0.0f,
				(nCntVtx / (m_nNumX + 1)) * -m_fDepth + (m_fDepth) * m_nNumZ / 2);

			//�@���x�N�g���̐ݒ�
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[nCntVtx].tex = D3DXVECTOR2((float)(nCntVtx % (m_nNumX + 1)), (float)nCntVtx / (float)(m_nNumX + 1));
		}
		//���_�o�b�t�@���A�����b�N����
		(*ppVtxBuff)->Unlock();
	}

	//�C���f�b�N�X�o�b�t�@�̂̎擾
	LPDIRECT3DINDEXBUFFER9* ppIdxBuff = GetPtrIdxBuff();

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
		for (int nCntIdx = 0; nCntIdx < nNumIdx; nCntIdx++) {
			if (nCntIdx == (m_nNumX + 2) * 2 * (1 + nCntIdx / ((m_nNumX + 2) * 2)) - 2) {
				pIdx[nCntIdx] = pIdx[nCntIdx - 1];
			}
			else if (nCntIdx == (m_nNumX + 2) * 2 * (1 + nCntIdx / ((m_nNumX + 2) * 2)) - 1) {
				pIdx[nCntIdx] = (m_nNumX + 1) * (2 + nCntIdx / ((m_nNumX + 1) * 2 + 2));
			}
			else {
				pIdx[nCntIdx] = (m_nNumX + 1 + nCntIdx / 2) - (nCntIdx / ((m_nNumX + 2) * 2)) - ((m_nNumX + 1)*(nCntIdx % 2));//���@�k�ށ@�� �k�ނɏC��
			}
		}
		//�C���f�b�N�X�o�b�t�@���A�����b�N����
		(*ppIdxBuff)->Unlock();
	}

	CScene3D::Init();

	return S_OK;
}

//=============================================================================
// ���b�V���t�B�[���h�̏I������
//=============================================================================
void CMeshfield::Uninit(void) {
	CScene3D::Uninit();
}

//=============================================================================
// ���b�V���t�B�[���h�̍X�V����
//=============================================================================
void CMeshfield::Update(void) {
	CScene3D::Update();
}

//=============================================================================
// ���b�V���t�B�[���h�̕`�揈��
//=============================================================================
void CMeshfield::Draw(void) {
	CScene3D::Draw();
}