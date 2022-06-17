//=============================================================================
//
// ���̏��� [katana.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _KATANA_H_
#define _KATANA_H_

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
class CKatana : public CWeapon
{
public:

	CKatana();	//�f�t�H���g�R���X�g���N�^
	CKatana(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CKatana();	//�f�X�g���N�^
	static CKatana* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void GetPosCollision(D3DXVECTOR3** const ppPosCollision, int* const pNumCollision);	//�����蔻��̈ʒu�̎擾
	void SetDrawOutline(bool bDraw);	//���f���̗֊s�̕\���ݒ�

private:
	CMeshorbit* m_pMeshorbit;	//���b�V���O���ւ̃|�C���^

};

#endif // !_KATANA_H_