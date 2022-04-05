//=============================================================================
//
// ���̏��� [katana.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "katana.h"
#include "meshorbit.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define COLOR_ORBIT_0 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//���b�V���O���̌��̍��{���̐F
#define COLOR_ORBIT_1 (D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f))	//���b�V���O���̌��̐n�摤�̐F
#define NUM_COLLISION (6)	//�����蔻��̐�

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CKatana::CKatana()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CKatana::CKatana(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent) : CWeapon(CModel::MODELTYPE::WEAPON_KATANA, pos, rot, pParent, true)
{
	m_pMeshorbit = nullptr;
	SetDrawOutline(false);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CKatana::~CKatana()
{

}


//=============================================================================
// ���̐�������
//=============================================================================
CKatana* CKatana::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent) {
	CKatana* pKatana;
	pKatana = new CKatana(pos, rot, pParent);
	if (pKatana != nullptr) {
		pKatana->Init();
	}

	return pKatana;
}

//=============================================================================
// ���̏���������
//=============================================================================
HRESULT CKatana::Init(void) {
	//���b�V���O���̐���
	CModel* pModel = GetPtrModel();	//����̃��f���̃|�C���^
	if (pModel != nullptr) m_pMeshorbit = CMeshorbit::Create(pModel->GetPtrMtxWorld(), 40, 0, D3DXVECTOR3(0.0f, 25.0f, 0.0f), D3DXVECTOR3(0.0f, 80.0f, 0.0f), COLOR_ORBIT_0, COLOR_ORBIT_1);
	if (m_pMeshorbit != nullptr)m_pMeshorbit->SetVisible(false);
	//����̏���������
	CWeapon::Init();
	return S_OK;
}

//=============================================================================
// ���̏I������
//=============================================================================
void CKatana::Uninit(void) {
	//���b�V���O���̔j��
	if (m_pMeshorbit != nullptr) {
		m_pMeshorbit->Uninit();
		m_pMeshorbit = nullptr;
	}
	//����̏I������
	CWeapon::Uninit();
}

//=============================================================================
// ���̍X�V����
//=============================================================================
void CKatana::Update(void) {
	if (m_pMeshorbit != nullptr) {
		//���b�V���O�����s����Ԃ����_�̈ʒu���Œ肳��Ă���ꍇ
		if (!m_pMeshorbit->GetVisible() || m_pMeshorbit->GetBindPos()) {
			//�U�����̏ꍇ
			if (GetAttack()) {
				m_pMeshorbit->SetVisible(true);	//���b�V���O��������
			}
		}
		else {
			//�U�����łȂ��ꍇ
			if (!GetAttack()) {
				m_pMeshorbit->SetBindPos();	//���b�V���O���̌��_�̈ʒu���Œ�
			}
		}
	}
	//����̍X�V����
	CWeapon::Update();
}

//=============================================================================
// ���̕`�揈��
//=============================================================================
void CKatana::Draw(void) {
	CWeapon::Draw();
}

//=============================================================================
// �����蔻��̈ʒu�̎擾
//=============================================================================
void CKatana::GetPosCollision(D3DXVECTOR3** ppPosCollision, int* const pNumCollision) {
	//�����蔻��̐���ݒ�
	if (pNumCollision != nullptr) *pNumCollision = NUM_COLLISION;

	//�����蔻��̈ʒu�𐔕�����
	if (*ppPosCollision == nullptr) {
		*ppPosCollision = new D3DXVECTOR3[NUM_COLLISION];
	}
	else {
		//���łɔz�񂪐�������Ă����ꍇ�I��
		return;
	}

	//�z�񂪐�������Ă��Ȃ��ꍇ�I��
	if (*ppPosCollision == nullptr) return;

	//����̃��f�����擾
	CModel* pModel = CSceneModel::GetPtrModel();

	//����̃��f�����Ȃ��ꍇ�I��
	if (pModel == nullptr) return;

	D3DXMATRIX mtxWeapon;	//����̃��f���̃��[���h�}�g���b�N�X

	//����̃��f���̃��[���h�}�g���b�N�X���X�V
	pModel->UpdateMtxWorld();
	//����̃��[���h�ϊ��s����擾
	mtxWeapon = pModel->GetMtxWorld();

	D3DXMATRIX mtxWorld;	//�U���ʒu�̃��[���h�}�g���b�N�X
	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X

	for (int nCntCollsion = 0; nCntCollsion < NUM_COLLISION; nCntCollsion++)
	{
		D3DXVECTOR3 posAttack = D3DXVECTOR3(0.0f, 25.0f + 10.0f * nCntCollsion, 0.0f);	//�U���̓����蔻��̈ʒu�̃I�t�Z�b�g

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);
		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, posAttack.x, posAttack.y, posAttack.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		//�Z�o�������[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&mtxWorld,
			&mtxWorld,
			&mtxWeapon);

		//�U���ʒu�̐ݒ�
		(*ppPosCollision)[nCntCollsion].x = mtxWorld._41;
		(*ppPosCollision)[nCntCollsion].y = mtxWorld._42;
		(*ppPosCollision)[nCntCollsion].z = mtxWorld._43;
	}
}

//=============================================================================
// ���f���̗֊s�̕\���ݒ�
//=============================================================================
void CKatana::SetDrawOutline(bool bDraw) {
	CModel* pModel = GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetDrawOutline(bDraw);
	}
}