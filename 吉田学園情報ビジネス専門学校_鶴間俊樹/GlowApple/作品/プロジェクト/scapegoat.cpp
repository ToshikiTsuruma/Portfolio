//=============================================================================
//
// ���т̏��� [scapegoat.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "scapegoat.h"
#include "manager.h"
#include "sound.h"
#include "effect.h"
#include "gauge3D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DEFAULT_MAX_LIFE (666)		//�̗͂̍ő�l

#define NUM_COLLISION (8)			//�����蔻��̐�
#define COLLISION_RADIUS (20.0f)	//�����蔻��̔��a

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CScapegoat::CScapegoat() : CObjectModel(CModel::MODELTYPE::OBJ_SCAPEGOAT, false)
{
	SetObjType(OBJTYPE_SCAPEGOAT);	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	SetEnableCollision(true);		//�����蔻��L��

	m_nLife = DEFAULT_MAX_LIFE;

	//�̗̓Q�[�W�̐���
	m_pGaugeLife = CGauge3D::Create(DEFAULT_MAX_LIFE, false, m_nLife, FPS * 20, false);
	//�Q�[�W�̔w�i�̐ݒ�
	m_pGaugeLife->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55.0f, 7.0f);
	m_pGaugeLife->SetGaugeBGColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	//�Q�[�W�̐ݒ�
	m_pGaugeLife->CreateGauge(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, 4.0f);
	m_pGaugeLife->SetGaugeColor(D3DXCOLOR(0.1f, 1.0f, 0.0f, 1.0f));
	//�Q�[�W���\��
	m_pGaugeLife->SetGaugeDraw(false);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScapegoat::~CScapegoat()
{

}

//=============================================================================
// ���т̐�������
//=============================================================================
CScapegoat* CScapegoat::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot) {
	CScapegoat* pScapegoat;
	pScapegoat = new CScapegoat();
	if (pScapegoat == nullptr) return nullptr;

	pScapegoat->SetPos(pos);
	pScapegoat->SetRot(rot);
	if (pScapegoat->m_pGaugeLife != nullptr) {
		pScapegoat->m_pGaugeLife->SetAllGaugePos(pos + D3DXVECTOR3(0.0f, 200.0f, 0.0f));
	}
	pScapegoat->Init();

	return pScapegoat;
}

//=============================================================================
// ���т̏���������
//=============================================================================
HRESULT CScapegoat::Init(void) {
	CModel* pModel = GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetColorGlow(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));
	}

	CObjectModel::Init();

	return S_OK;
}

//=============================================================================
// ���т̏I������
//=============================================================================
void CScapegoat::Uninit(void) {
	CObjectModel::Uninit();
}

//=============================================================================
// ���т̍X�V����
//=============================================================================
void CScapegoat::Update(void) {
	CObjectModel::Update();
}

//=============================================================================
// ���т̕`�揈��
//=============================================================================
void CScapegoat::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// ���т̃_���[�W����
//=============================================================================
void CScapegoat::Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead) {
	//�_���[�W
	m_nLife -= nDamage;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	D3DXVECTOR3 posEffect = GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f);	//�G�t�F�N�g�����ʒu�̃I�t�Z�b�g
	float afRandPos[3];	//�I�t�Z�b�g�ɉ��Z���郉���_���Ȓl
	//�I�t�Z�b�g�������_���ł��炷
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		int nRandPos = rand() % 30 - 15;
		afRandPos[nCnt] = (float)nRandPos;
	}
	//�I�t�Z�b�g�Ƀ����_���Ȓl�����Z
	posEffect.x += afRandPos[0];
	posEffect.y += afRandPos[1];
	posEffect.z += afRandPos[2];

	//�U���̃^�C�v���Ƃ̏���
	switch (typeDamage)
	{
	case DAMAGE_TYPE::ENEMY_PUNCH:
		//�_���[�W�G�t�F�N�g�̐���
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::DAMAGE_ENEMY, 40.0f, 40.0f, false);
		//�_���[�W���̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		break;
	case DAMAGE_TYPE::FIRE:
		//�_���[�W�G�t�F�N�g�̐���
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::FIRE, 80.0f, 80.0f, false);
		break;
	case DAMAGE_TYPE::PLAYER_PUNCH:
		//�_���[�W�G�t�F�N�g�̐���
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::DAMAGE_PLAYER, 40.0f, 40.0f, false);
		//�_���[�W���̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		break;
	case DAMAGE_TYPE::THUNDERBOLT:
		//�_���[�W�G�t�F�N�g�̐���
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::THUNDER, 50.0f, 50.0f, false);
		//�_���[�W���̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_THUNDER);
		break;
	}

	//���S����
	if (m_nLife <= 0) {
		//���S
		if (pDead != nullptr) *pDead = true;
		Dead();
	}
	else {
		if (pDead != nullptr) *pDead = false;

		//�̗̓Q�[�W�̐ݒ�
		if (m_pGaugeLife != nullptr) {
			m_pGaugeLife->SetGaugeValue(m_nLife);
		}
	}
}

//=============================================================================
// ���т̎��S����
//=============================================================================
void CScapegoat::Dead(void) {
	//�����蔻��𖳌�
	SetEnableCollision(false);

	//�̗̓Q�[�W�̔j��
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//���̍Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::EXPLOSION);

	//�����G�t�F�N�g
	CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 80.0f, 0.0f), CEffect::EFFECT_TYPE::DEATH, 100.0f, 100.0f, false);

	//�I�u�W�F�N�g�̔j��
	Uninit();
}

//=============================================================================
// ���т̓����蔻��̏��̎擾
//=============================================================================
void CScapegoat::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//�K�v�ȃ|�C���^��null�̏ꍇ�I��
	if (pNumCol == nullptr || pRadiusCol == nullptr || ppPosColArray == nullptr) return;

	//pos�̃|�C���^�����łɑ��݂��Ă���ꍇ�I��
	if (*ppPosColArray != nullptr) return;

	//�����蔻��̐���ݒ�
	*pNumCol = NUM_COLLISION;
	//�����蔻��̔��a��ݒ�
	*pRadiusCol = COLLISION_RADIUS;
	//�����蔻��̈ʒu�̔z���ݒ�
	*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];	//���I�m��

	for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
	{
		(*ppPosColArray)[nCntCol] = D3DXVECTOR3(GetPos().x, GetPos().y + COLLISION_RADIUS * 0.5f * nCntCol, GetPos().z);
	}
}