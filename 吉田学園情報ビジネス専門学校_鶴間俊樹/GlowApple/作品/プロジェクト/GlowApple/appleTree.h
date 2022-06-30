//=============================================================================
//
// �ь�̖؂̏��� [appleTree.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _APPLE_TREE_H_
#define _APPLE_TREE_H_

#include "main.h"
#include "objectModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CAppleMenu;
class CGauge2D;
class CObject2D;

//*****************************************************************************
// �ь�̖؃N���X
//*****************************************************************************
class CAppleTree : public CObjectModel
{
public:
	CAppleTree();		//�f�t�H���g�R���X�g���N�^
	CAppleTree(D3DXVECTOR3 pos);		//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CAppleTree();		//�f�X�g���N�^
	static CAppleTree* Create(D3DXVECTOR3 pos);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void Damage(int nDamage, bool* pDead);		//�_���[�W
	void Dead(void);		//���S������

	void GrowUp(void);	//����
	D3DXVECTOR3 GetPosApple(void);	//�ь琶���ʒu�̎擾
	void SetMaxLife(int nMaxLife);	//�̗͂̍ő�l�̐ݒ�
	void AddMaxLife(int nAddLife);	//�̗͂̍ő�l�̑���
	void HealLife(int nHeal);		//�̗͂��񕜂���
	void AddGrow(int nAddValue);	//�����x�𑝂₷
	bool GetDead(void) { return m_bDead; }	//���S��Ԃ̎擾

	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//�����蔻��̏��̎擾

private:
	void CreateApple(void);	//�ь�̐���

	int m_nNumApple;	//���������ь�̐�

	//�̗�
	int m_nMaxLife;			//�̗͂̍ő�l
	int m_nLife;			//�̗�
	CGauge2D* m_pGaugeLife;	//�̗̓Q�[�W
	CObject2D* m_pIconHP;	//HP�̃A�C�R��
	//����
	int m_nGrowValue;	//�����x
	CGauge2D* m_pGaugeGrow;	//�����Q�[�W

	bool m_bDead;			//���S
	int m_nCntDead;			//���S��̃J�E���g

	CAppleMenu* m_pMenuApple;	//�ь�̃��j���[

};

#endif // !_APPLE_TREE_H_