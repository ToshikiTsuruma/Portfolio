//=============================================================================
//
// �؂̏��� [tree.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _TREE_H_
#define _TREE_H_

#include "sceneMotion.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CBillboard;

//*****************************************************************************
// �؃N���X
//*****************************************************************************
class CTree : public CSceneMotion
{
public:
	//���[�V�����̎��
	enum class MOTION_TYPE {
		BREAK = 0,	//�j�󃂁[�V����
		ENUM_MAX
	};

	CTree();	//�f�t�H���g�R���X�g���N�^
	~CTree();	//�f�X�g���N�^

	static void SetTreeCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fDistCircle, int nMinDist, int nMaxDist);	//���S�~��Ƀ����_���Ɋp�x���炵�Ė؂�z�u����
	static CTree* Create(D3DXVECTOR3 pos);	//��������
	static void LoadMotionInfo(void);	//���[�V�����������[�h
	static void UnloadMotionInfo(void);	//���[�V���������A�����[�h

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��
	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//�����蔻��̏��̎擾
	void Damage(int nDamage, bool* pDead);		//�_���[�W����

private:
	static int m_nNumParts;	//�p�[�c��
	static PARTS_INFO* m_pPartsInfoArray;	//�p�[�c���̔z��̃|�C���^
	static MOTION_INFO m_aMotionInfo[(int)MOTION_TYPE::ENUM_MAX];	//���[�V�������

	bool m_bDead;	//���S
	int m_nCntDead;	//���S��j�������܂ł̃J�E���g
	int m_nLife;	//�̗�
	CBillboard* m_pGaugeLife;	//�̗̓Q�[�W�̃r���{�[�h�̃|�C���^
	CBillboard* m_pGaugeLifeFrame;	//�̗̓Q�[�W�̃r���{�[�h�̘g�̃|�C���^
	int m_nCntDrawGaugeLife;	//�̗̓Q�[�W�̕`�掞��
	bool m_bDraw;	//�`�悷�邩�ǂ���
};

#endif // !_TREE_H_