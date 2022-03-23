//=============================================================================
//
// �����S�A�C�e���̏��� [item_apple.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _ITEM_APPLE_H_
#define _ITEM_APPLE_H_

#include "item.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �����S�A�C�e���N���X
//*****************************************************************************
class CItemApple : public CItem
{
public:

	CItemApple();	//�f�t�H���g�R���X�g���N�^
	CItemApple(D3DXVECTOR3 pos);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CItemApple();	//�f�X�g���N�^
	static CItemApple* Create(D3DXVECTOR3 pos);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	float GetRadius(void);			//�����蔻��̔��a�̎擾
	void CollisionObject(CScene* pScene);	//�A�C�e�����I�u�W�F�N�g�ƏՓ˂����Ƃ��̏���

private:

};

#endif // !_ITEM_APPLE_H_