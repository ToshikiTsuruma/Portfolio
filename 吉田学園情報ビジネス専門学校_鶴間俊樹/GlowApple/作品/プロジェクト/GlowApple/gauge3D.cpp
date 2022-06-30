//=============================================================================
//
// 3D�Q�[�W���� [gauge3D.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "gauge3D.h"
#include "billboard.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGauge3D::CGauge3D()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CGauge3D::CGauge3D(int nMaxValue, int nValue, int nDrawLifeMax) : CGauge(nMaxValue, nValue, nDrawLifeMax)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGauge3D::~CGauge3D()
{

}

//=============================================================================
// 3D�Q�[�W�̐�������
//=============================================================================
CGauge3D* CGauge3D::Create(int nMaxValue, int nValue, int nDrawLifeMax) {
	CGauge3D* pGauge3D;
	pGauge3D = new CGauge3D(nMaxValue, nValue, nDrawLifeMax);
	if (pGauge3D == nullptr) return nullptr;

	pGauge3D->Init();

	return pGauge3D;
}

//=============================================================================
// 3D�Q�[�W�̏���������
//=============================================================================
void CGauge3D::Init(void) {
	CGauge::Init();
}

//=============================================================================
// 3D�Q�[�W�̏I������
//=============================================================================
void CGauge3D::Uninit(void) {
	CGauge::Uninit();
}

//=============================================================================
// 3D�Q�[�W�̍X�V����
//=============================================================================
void CGauge3D::Update(void) {
	CGauge::Update();
}

//=============================================================================
// 3D�Q�[�W�𐶐�
//=============================================================================
void CGauge3D::CreateGauge(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//�Q�[�W�̃_�u���|�C���^���擾
	CObject** ppGauge = GetGaugePtr();

	//�Q�[�W�𐶐�����|�C���^�ϐ��̃A�h���X��null�̏ꍇ�I��
	if (ppGauge == nullptr) return;

	//���łɐ�������Ă���ꍇ�I��
	if (*ppGauge != nullptr) return;

	//�Q�[�W�̐���
	*ppGauge = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (*ppGauge != nullptr) (*ppGauge)->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
}

//=============================================================================
// 3D�Q�[�W�̔w�i�𐶐�
//=============================================================================
void CGauge3D::CreateGaugeBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//�Q�[�W�̔w�i�̃_�u���|�C���^���擾
	CObject** ppGaugeBG = GetGaugeBGPtr();

	//�Q�[�W�̔w�i�𐶐�����|�C���^�ϐ��̃A�h���X��null�̏ꍇ�I��
	if (ppGaugeBG == nullptr) return;

	//���łɐ�������Ă���ꍇ�I��
	if (*ppGaugeBG != nullptr) return;

	//�w�i�̐���
	*ppGaugeBG = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (*ppGaugeBG != nullptr) (*ppGaugeBG)->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
}

//=============================================================================
// 3D�Q�[�W�̘g�𐶐�
//=============================================================================
void CGauge3D::CreateGaugeFrame(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//�Q�[�W�̔w�i�̃_�u���|�C���^���擾
	CObject** ppGaugeFrame = GetGaugeFramePtr();

	//�Q�[�W�̔w�i�𐶐�����|�C���^�ϐ��̃A�h���X��null�̏ꍇ�I��
	if (ppGaugeFrame == nullptr) return;

	//���łɐ�������Ă���ꍇ�I��
	if (*ppGaugeFrame != nullptr) return;

	//�w�i�̐���
	*ppGaugeFrame = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (*ppGaugeFrame != nullptr) (*ppGaugeFrame)->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
}