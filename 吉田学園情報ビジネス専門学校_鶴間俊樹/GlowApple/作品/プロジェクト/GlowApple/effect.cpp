//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEffect::CEffect()
{
	m_nCntAnim = 0;
	m_nSpanAnim = 0;
	m_nPatternAnim = 0;
	m_nPatternU = 0;
	m_nPatternV = 0;
	m_bDestBlendOne = false;

	//�[�x�֌W�Ȃ��`��ł���悤�ɂ���
	SetZtestAlways(true);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ��������
//=============================================================================
CEffect* CEffect::Create(D3DXVECTOR3 pos, EFFECT_TYPE type, float fWidth, float fHeight) {
	CEffect* pEffect;
	pEffect = new CEffect;
	if (pEffect == nullptr) return nullptr;

	pEffect->m_type = type;
	pEffect->SetPos(pos);
	pEffect->SetSize(D3DXVECTOR3(fWidth, fHeight, 0.0f));

	switch ((EFFECT_TYPE)type)
	{
	case EFFECT_TYPE::EXPLOSION:
		pEffect->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_EXPLOSION);
		pEffect->m_nSpanAnim = 2;
		pEffect->m_nPatternU = 7;
		pEffect->m_nPatternV = 1;
		break;
	case EFFECT_TYPE::HEAL_APPLE:
		pEffect->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_HEAL_APPLE);
		pEffect->m_nSpanAnim = 1;
		pEffect->m_nPatternU = 9;
		pEffect->m_nPatternV = 1;
		pEffect->m_bDestBlendOne = true;
		break;
	}
	pEffect->Init();

	return pEffect;
}

//=============================================================================
// �G�t�F�N�g�̏���������
//=============================================================================
HRESULT CEffect::Init(void) {
	//�����o�ϐ��̏�����
	m_nCntAnim = 0;
	m_nPatternAnim = 0;
	//�r���{�[�h�̏�����
	CBillboard::Init();
	//�e�N�X�`�����W�̐ݒ�
	CBillboard::SetTexPos(0.0f, 0.0f, 1.0f / (float)m_nPatternU, 1.0f / (float)m_nPatternV);

	return S_OK;
}

//=============================================================================
// �G�t�F�N�g�̏I������
//=============================================================================
void CEffect::Uninit(void) {
	CBillboard::Uninit();
}

//=============================================================================
// �G�t�F�N�g�̍X�V����
//=============================================================================
void CEffect::Update(void) {
	CBillboard::Update();

	//�A�j���[�V�����؂�ւ�
	if (m_nCntAnim >= m_nSpanAnim) {
		m_nCntAnim = 0;
		m_nPatternAnim++;

		//�A�j���[�V�������I�������ꍇ
		if (m_nPatternAnim >= m_nPatternU * m_nPatternV) {
			Uninit();
			return;
		}

		//�e�N�X�`�����W�̐ݒ�
		CBillboard::SetTexPos(
			(1.0f / (float)m_nPatternU) * (m_nPatternAnim % m_nPatternU),
			(1.0f / (float)m_nPatternV) * (m_nPatternAnim / m_nPatternU),
			(1.0f / (float)m_nPatternU) * ((m_nPatternAnim % m_nPatternU) + 1),
			(1.0f / (float)m_nPatternV) * ((m_nPatternAnim / m_nPatternU) + 1)
		);
	}
	else {
		m_nCntAnim++;	//�J�E���^�[�̉��Z
	}
}

//=============================================================================
// �G�t�F�N�g�̕`�揈��
//=============================================================================
void CEffect::Draw(void) {
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

	//�t�H�O�𖳌�
	pRenderer->SetEffectFogEnable(false);
	//�A���t�@�e�X�g�@�L���^����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//�A���t�@�l�̎Q�ƒl
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	if (m_bDestBlendOne) {
		//���u�����f�B���O�̐ݒ�
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	CBillboard::Draw();

	//�t�H�O��L��
	pRenderer->SetEffectFogEnable(true);
	//�A���t�@�e�X�g�@�L���^����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//�A���t�@�l�̎Q�ƒl
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	if (m_bDestBlendOne) {
		//���u�����f�B���O��ʏ�ɖ߂�
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}