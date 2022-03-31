//=============================================================================
//
// ���̏��� [sword.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SWORD_H_
#define _SWORD_H_

#include "weapon.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CMeshorbit;

//*****************************************************************************
// ����N���X
//*****************************************************************************
class CSword : public CWeapon
{
public:

	CSword();	//�f�t�H���g�R���X�g���N�^
	CSword(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CSword();	//�f�X�g���N�^
	static CSword* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void GetPosCollision(D3DXVECTOR3** const ppPosCollision, int* const pNumCollision);	//�����蔻��̈ʒu�̎擾

private:
	CMeshorbit* m_pMeshorbit;	//���b�V���O���ւ̃|�C���^

};

#endif // !_SWORD_H_