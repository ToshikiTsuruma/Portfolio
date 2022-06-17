//=============================================================================
//
// ���f���I�u�W�F�N�g���� [sceneModel.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SCENEMODEL_H_
#define _SCENEMODEL_H_

#include "main.h"
#include "scene.h"
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// ���f���I�u�W�F�N�g�N���X
//*****************************************************************************
class CSceneModel : public CScene
{
public:

	CSceneModel();		//�f�t�H���g�R���X�g���N�^
	CSceneModel(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine);		//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CSceneModel();		//�f�X�g���N�^
	static CSceneModel* Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//��������
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
	CModel* m_pModel;	//���f���ւ̃|�C���^

};

#endif // !_SCENEMODEL_H_