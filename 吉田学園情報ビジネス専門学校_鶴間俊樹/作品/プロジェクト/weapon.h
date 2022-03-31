//=============================================================================
//
// ����̏��� [weapon.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "sceneModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// ����N���X
//*****************************************************************************
class CWeapon : public CSceneModel
{
public:

	CWeapon();	//�f�t�H���g�R���X�g���N�^
	CWeapon(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CWeapon();	//�f�X�g���N�^
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��
	virtual void GetPosCollision(D3DXVECTOR3** const ppPosCollision, int* const pNumCollision) = 0;	//�����蔻��̈ʒu�̎擾
	virtual void SetDrawOutline(bool bDraw);	//���f���̗֊s�̕\���ݒ�
	void SetAttack(bool bAttack);	//�U�������ǂ����̐ݒ�
	bool GetAttack(void);	//�U�������ǂ����̎擾

private:
	bool m_bAttack;	//�U����

};

#endif // !_WEAPON_H_