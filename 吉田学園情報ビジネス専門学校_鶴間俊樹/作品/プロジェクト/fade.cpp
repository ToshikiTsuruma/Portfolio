//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "fade.h"
#include "renderer.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CFade::CFade()
{
	m_pVtxBuff = NULL;
	m_fFadeSpeed = 0.0f;
	m_bFadein = false;
	m_nextMode = CManager::MODE::TITLE;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// �t�F�[�h�̏���������
//=============================================================================
HRESULT CFade::Init(void) {
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

	VERTEX_2D *pVtx;
	m_bFadein = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �t�F�[�h�̏I������
//=============================================================================
void CFade::Uninit(void) {
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// �t�F�[�h�̍X�V����
//=============================================================================
void CFade::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	

	//���[�h�؂�ւ����̏I��
	if (m_bChangeFade) m_bChangeFade = false;

	//�t�F�[�h�A�E�g
	if (!m_bFadein && m_colA > 0.0f) {
		m_colA -= m_fFadeSpeed;
		if (m_colA < 0.0f) {
			m_colA = 0.0f;
		}
	}

	//�t�F�[�h�C��
	if (m_bFadein && m_colA <= 1.0f) {
		m_colA += m_fFadeSpeed;
		//�t�F�[�h������
		if (m_colA >= 1.0f) {
			m_colA = 1.0f;
			m_bFadein = false;
			//�V�[���J��
			if (pManager != nullptr) {
				m_bChangeFade = true;	//���[�h�؂�ւ���
				//���̃��[�h���͈͓��������ꍇ���[�h�ؑ�
				if((int)m_nextMode >= 0 && m_nextMode < CManager::MODE::ENUM_MAX) pManager->SetMode(m_nextMode);
			}
		}
	}

	//�t�F�[�h���̂ݍX�V
	if (m_bFadein || m_colA > 0.0f) {
		VERTEX_2D *pVtx;

		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_����ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colA);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colA);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colA);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_colA);
		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �t�F�[�h�̕`�揈��
//=============================================================================
void CFade::Draw(void) {
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

	//�t�F�[�h���̂ݕ`��
	if (m_bFadein || m_colA > 0.0f) {
		pDevice->SetFVF(FVF_VERTEX_2D);			//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetTexture(0, NULL);		//�e�N�X�`���̐ݒ�
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
			0,
			2);
	}
}

//=============================================================================
//�t�F�[�h�̐ݒ�
//=============================================================================
void CFade::SetFade(CManager::MODE mode, float fFadeSpeed) {
	if (!m_bFadein) {
		m_bFadein = true;
		m_nextMode = mode;
		m_fFadeSpeed = fFadeSpeed;
	}
}

//=============================================================================
// �t�F�[�h�A�E�g���X�L�b�v������
//=============================================================================
void CFade::SkipFade(void) {
	m_colA = 0.0f;
	m_bFadein = false;
}

//=============================================================================
// �t�F�[�h�����ǂ������擾
//=============================================================================
bool CFade::GetFade(void) {
	if (m_bFadein || m_colA > 0.0f) {
		return true;
	}
	return false;
}

//=============================================================================
// �t�F�[�h�C������t�F�[�h�A�E�g�ɐ؂�ւ�����^�C�~���O���ǂ���
//=============================================================================
bool CFade::GetChangeFade(void) {
	return m_bChangeFade;
}