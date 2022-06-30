//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "billboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �G�t�F�N�g�N���X
//*****************************************************************************
class CEffect : public CBillboard
{
public:
	enum class EFFECT_TYPE{
		EXPLOSION = 0,	//����
		HEAL_APPLE,		//�ь�ɂ���
		ENUM_MAX
	};	//�G�t�F�N�g�̎��

	CEffect();	//�f�t�H���g�R���X�g���N�^
	virtual ~CEffect();	//�f�X�g���N�^
	static CEffect* Create(D3DXVECTOR3 pos, EFFECT_TYPE type, float fWidth, float fHeight);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

private:
	EFFECT_TYPE m_type;			//�G�t�F�N�g�̃^�C�v
	int m_nCntAnim;			//�A�j���[�V�����J�E���^�[
	int m_nSpanAnim;		//�A�j���[�V�����؂�ւ��̊Ԋu
	int m_nPatternAnim;		//�p�^�[���ԍ�
	int m_nPatternU, m_nPatternV;	//UV�e�����ւ̃p�^�[���̐�

	bool m_bDestBlendOne;	//�u�����h�W�����P�ɂ���H
};

#endif // !_EFFECT_H_