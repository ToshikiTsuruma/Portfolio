//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CParticle::CParticle()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CParticle::CParticle(float fDecSize) {
	//�`�揇�̐ݒ�
	SetDrawPriority(DRAW_PRIORITY::EFFECT);

	m_fDecSize = fDecSize;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// ��������
//=============================================================================
CParticle* CParticle::Create(D3DXVECTOR3 pos, float fSize, float fDecSize, D3DXCOLOR col) {
	CParticle* pParticle;
	pParticle = new CParticle(fDecSize);
	if (pParticle != NULL) {
		pParticle->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_PARTICLE);
		pParticle->SetPos(pos);
		pParticle->SetSize(D3DXVECTOR3(fSize, fSize, 0.0f));

		//������
		pParticle->Init();

		//�F�̕ύX
		pParticle->SetColor(col);
	}

	return pParticle;
}

//=============================================================================
// �p�[�e�B�N���̏���������
//=============================================================================
HRESULT CParticle::Init(void) {
	//�r���{�[�h�̏�����
	CBillboard::Init();

	return S_OK;
}

//=============================================================================
// �p�[�e�B�N���̏I������
//=============================================================================
void CParticle::Uninit(void) {
	CBillboard::Uninit();
}

//=============================================================================
// �p�[�e�B�N���̍X�V����
//=============================================================================
void CParticle::Update(void) {
	CBillboard::Update();

	D3DXVECTOR3 size = GetSize();
	//�T�C�Y�̌��Z
	size.x -= m_fDecSize;
	size.y -= m_fDecSize;
	//�T�C�Y���}�C�i�X�ɂȂ����������
	if (size.x <= 0) {
		Uninit();
		return;
	}
	//�T�C�Y�̐ݒ�
	SetSize(size);
	return;
}

//=============================================================================
// �p�[�e�B�N���̕`�揈��
//=============================================================================
void CParticle::Draw(void) {
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

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CBillboard::Draw();

	//���u�����f�B���O��ʏ�ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}