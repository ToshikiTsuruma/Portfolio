//=============================================================================
//
// �A�C�e������ [item.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "sceneModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;

//*****************************************************************************
// �A�C�e���N���X
//*****************************************************************************
class CItem : public CSceneModel
{
public:
	enum class ITEM_TYPE {
		APPLE = 0,
		ENUM_MAX
	};

	CItem();	//�f�t�H���g�R���X�g���N�^
	CItem(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CItem();	//�f�X�g���N�^
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��
	static void Collision(CScene* pSceneCol);	//�Փ˔���
	virtual void CollisionObject(CScene* pScene) = 0;	//�A�C�e�����I�u�W�F�N�g�ƏՓ˂����Ƃ��̏���

private:
	bool m_bDraw;	//�`�悷�邩�ǂ���
};

#endif // !_ITEM_H_