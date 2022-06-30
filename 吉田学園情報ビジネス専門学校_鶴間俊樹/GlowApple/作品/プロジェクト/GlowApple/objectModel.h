//=============================================================================
//
// ���f���I�u�W�F�N�g���� [objectModel.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _OBJECT_MODEL_H_
#define _OBJECT_MODEL_H_

#include "main.h"
#include "object.h"
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// ���f���I�u�W�F�N�g�N���X
//*****************************************************************************
class CObjectModel : public CObject
{
public:

	CObjectModel();		//�f�t�H���g�R���X�g���N�^
	CObjectModel(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine);		//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CObjectModel();		//�f�X�g���N�^
	static CObjectModel* Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine);	//��������
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��

	CModel::MODELTYPE GetModelType(void);	//���f���̎�ނ̎擾
	CModel* GetPtrModel(void);		//���f���̃|�C���^�̎擾
	void SetPos(D3DXVECTOR3 pos);	//�ʒu���W�̐ݒ�
	D3DXVECTOR3 GetPos(void);		//�ʒu���W�̎擾
	void SetRot(D3DXVECTOR3 rot);	//�p�x�̐ݒ�
	D3DXVECTOR3 GetRot(void);		//�p�x�̎擾

private:
	void Move(void);	//�ړ�
	void Rotate(void);	//��]

	CModel* m_pModel;	//���f���ւ̃|�C���^

};

#endif // !_OBJECT_MODEL_H_