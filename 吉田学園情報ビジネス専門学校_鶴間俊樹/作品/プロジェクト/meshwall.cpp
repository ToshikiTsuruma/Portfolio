//=============================================================================
//
// ���b�V���E�H�[������ [meshwall.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "meshwall.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CMeshwall::CMeshwall()
{
	m_nNumX = 0;
	m_nNumY = 0;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshwall::~CMeshwall()
{

}

//=============================================================================
// ��������
//=============================================================================
CMeshwall* CMeshwall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumX, int nNumY, float fWidth, float fHeight) {
	CMeshwall* pMeshwall;
	pMeshwall = new CMeshwall;
	if (pMeshwall != NULL) {
		pMeshwall->SetPos(pos);
		pMeshwall->SetRot(rot);
		pMeshwall->m_nNumX = nNumX;
		pMeshwall->m_nNumY = nNumY;
		pMeshwall->m_fWidth = fWidth;
		pMeshwall->m_fHeight = fHeight;
		pMeshwall->SetNumVtx((nNumX + 1) * (nNumY + 1));	//(X�����̃|���S���� + 1) * (Y�����̃|���S���� + 1)
		pMeshwall->SetNumIdx((2 * (nNumX + 1)) * nNumY + (nNumY - 1) * 2);	//X�����Ŏg�����_�� * Y�����̃|���S���� + �k�ރ|���S���̐�
		pMeshwall->Init();
	}

	return pMeshwall;
}

//=============================================================================
// ���b�V���E�H�[���̏���������
//=============================================================================
HRESULT CMeshwall::Init(void) {
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
	SetTexType(CTexture::TEXTURE_TYPE::EFFECT_EXPLOSION);

	//���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9* ppVtxBuff = GetPtrVtxBuff();

	int nNumVtx = GetNumVtx();	//���_���̎擾
	int nNumIdx = GetNumIdx();	//�C���f�b�N�X���̎擾

	//���_�o�b�t�@�̐���
	if (*ppVtxBuff == NULL) {
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			ppVtxBuff,
			NULL);
	}

	//���_�o�b�t�@�̐ݒ�
	if (*ppVtxBuff != NULL) {
		VERTEX_3D *pVtx;	//���_�o�b�t�@�̃|�C���^
		//���_�o�b�t�@�����b�N
		(*ppVtxBuff)->Lock(0, 0, (void**)&pVtx, 0);
		//���_�̐ݒ�
		for (int nCntVtx = 0; nCntVtx < (m_nNumX + 1) * (m_nNumY + 1); nCntVtx++) {

			//���_���W�̐ݒ�
			pVtx[nCntVtx].pos = D3DXVECTOR3((nCntVtx % (m_nNumX + 1)) * m_fWidth - (m_fWidth / 2.0f) * m_nNumX,
				(nCntVtx / (m_nNumX + 1)) * -m_fHeight + m_fHeight * m_nNumY,
				0.0f);

			//�@���x�N�g���̐ݒ�
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[nCntVtx].tex = D3DXVECTOR2((float)(nCntVtx % (m_nNumX + 1)), (float)nCntVtx / (float)(m_nNumX + 1));
		}
		//���_�o�b�t�@���A�����b�N����
		(*ppVtxBuff)->Unlock();
	}

	//�C���f�b�N�X�o�b�t�@�̂̎擾
	LPDIRECT3DINDEXBUFFER9* ppIdxBuff = GetPtrIdxBuff();

	//�C���f�b�N�X�o�b�t�@�̐���
	if (*ppIdxBuff == NULL) {
		pDevice->CreateIndexBuffer(sizeof(WORD) * nNumIdx,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			ppIdxBuff,
			NULL);
	}
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	if (*ppIdxBuff != NULL) {
		WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^
		//�C���f�b�N�X�o�b�t�@�����b�N���A�ԍ��f�[�^�ւ̃|�C���^���擾
		(*ppIdxBuff)->Lock(0, 0, (void**)&pIdx, 0);
		//�ԍ��f�[�^�ݒ�
		for (int nCntIdx = 0; nCntIdx < (m_nNumX + 1) * 2 * m_nNumY + (m_nNumY - 1) * 2; nCntIdx++) {
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
// ���b�V���E�H�[���̏I������
//=============================================================================
void CMeshwall::Uninit(void) {
	CScene3D::Uninit();
}

//=============================================================================
// ���b�V���E�H�[���̍X�V����
//=============================================================================
void CMeshwall::Update(void) {
	CScene3D::Update();
}

//=============================================================================
// ���b�V���E�H�[���̕`�揈��
//=============================================================================
void CMeshwall::Draw(void) {
	CScene3D::Draw();
}