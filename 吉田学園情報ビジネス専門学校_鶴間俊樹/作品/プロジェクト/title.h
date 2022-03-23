//=============================================================================
//
// �^�C�g������ [title.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CScene2D;
//*****************************************************************************
// �^�C�g���N���X
//*****************************************************************************
class CTitle
{
public:
	CTitle();	//�f�t�H���g�R���X�g���N�^
	~CTitle();	//�f�X�g���N�^
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����

private:
	//�^�C�g���̑I�����̎��
	enum class SELECT_TYPE {
		START = 0,
		TUTORIAL,
		CREDIT,
		ENUM_MAX
	};	

		//�`���[�g���A����ʂ̎��
	enum class TUTORIAL_TYPE {
		CONTROLL = 0,	//������
		GAME,	//�Q�[�����
		BOSS,	//�{�X���
		ENUM_MAX
	};	

	CScene2D* m_apScene2D[(int)SELECT_TYPE::ENUM_MAX];	//�I�����̃|���S��
	bool m_bFade;			//�t�F�[�h�J�n�ς݂��ǂ���
	CScene2D* m_pTutorial;	//�`���[�g���A����ʂ̃|���S��
	bool m_bTutorial;		//�`���[�g���A�����J���Ă����Ԃ��ǂ���
	TUTORIAL_TYPE m_typeTutorial;	//�`���[�g���A����ʂ̎��
	CScene2D* m_pCredit;	//�N���W�b�g��ʂ̃|���S��
	bool m_bCredit;			//�N���W�b�g���J���Ă����Ԃ��ǂ���
	int m_nSelect;			//���݂̑I��
};

#endif // !_TITLE_H_