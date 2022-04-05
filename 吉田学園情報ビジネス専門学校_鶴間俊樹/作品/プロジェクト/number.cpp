//=============================================================================
//
// �i���o�[���� [number.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "number.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CNumber::CNumber()
{
	m_pVtxBuff = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// �i���o�[�̐�������
//=============================================================================
CNumber* CNumber::Create(CTexture::TEXTURE_TYPE type, int nNumDigit) {
	CNumber* pNumberArray;
	pNumberArray = new CNumber[nNumDigit];

	if (pNumberArray != nullptr) {
		LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
		//�}�l�[�W���[�̎擾
		CManager* pManager = CManager::GetManager();	
		//�����_���[�̎擾
		CRenderer* pRenderer = nullptr;			
		if (pManager != nullptr) pRenderer = pManager->GetRenderer();
		//�f�o�C�X�̎擾
		if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

		if (pDevice != nullptr) {
			for (int nCnt = 0; nCnt < nNumDigit; nCnt++)
			{
				//���_�o�b�t�@�̐���
				pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_2D,
					D3DPOOL_MANAGED,
					&(pNumberArray[nCnt].m_pVtxBuff),
					nullptr);

				pNumberArray[nCnt].m_texType = type;
				pNumberArray[nCnt].Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f);
			}
		}
	}

	return pNumberArray;
}

//=============================================================================
// �i���o�[�̏���������
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fSize) {
	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_����ݒ� �|���S���̉E�オ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - fSize, pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x, pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fSize, pos.y + fSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x, pos.y + fSize, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
// �i���o�[�̏I������
//=============================================================================
void CNumber::Uninit(void) {
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// �i���o�[�̍X�V����
//=============================================================================
void CNumber::Update(void) {

}

//=============================================================================
// �i���o�[�̕`�揈��
//=============================================================================
void CNumber::Draw(void) {
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
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(m_texType);

	pDevice->SetFVF(FVF_VERTEX_2D);			//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetTexture(0, pTexture);		//�e�N�X�`���̐ݒ�
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0,
		2);
}


//=============================================================================
// �����̐ݒ�
//=============================================================================
void CNumber::SetNumber(int nNumber) {
	VERTEX_2D *pVtx;
	//���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_����ݒ�
	pVtx[0].tex = D3DXVECTOR2((float)nNumber / 10.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((float)(nNumber + 1) / 10.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((float)nNumber / 10.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((float)(nNumber + 1) / 10.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//�����̐F�̐ݒ�
//=============================================================================
void CNumber::SetColor(D3DXCOLOR col) {
	VERTEX_2D *pVtx;
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
