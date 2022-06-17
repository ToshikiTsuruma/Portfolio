//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_LIGHT (3)

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CRenderer;
class CInput;
class CInputKeyboard;
class CInputGamepadX;
class CCamera;
class CLight;
class CSound;
class CFade;
class CTitle;
class CGame;
class CResult;
class CPause;

//*****************************************************************************
// �}�l�[�W���[�N���X
//*****************************************************************************
class CManager
{
public:
	enum class MODE {
		TITLE = 0,
		GAME,
		RESULT,
		ENUM_MAX
	};

	static void Create(void);		//�}�l�[�W���[�̐���
	static void Destroy(void);		//�}�l�[�W���[�̔j��
	static CManager* GetManager(void);	//�}�l�[�W���[�̎擾

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetMode(MODE mode);	//���[�h�̐ݒ�
	MODE GetMode(void);			//���[�h�̎擾

	CRenderer *GetRenderer(void);	//�����_���[�̎擾
	CInput *GetInputCur(void);	//���݂̓��̓f�o�C�X�̎擾
	CCamera* GetCamera(void);	//�J�����̎擾
	CSound *GetSound(void);		//�T�E���h�̎擾
	CFade *GetFade(void);		//�t�F�[�h�̎擾
	CPause *GetPause(void);		//�|�[�Y�̎擾
	CGame *GetGame(void);		//�Q�[�����[�h�̎擾

private:
	CManager();		//�f�t�H���g�R���X�g���N�^
	~CManager();	//�f�X�g���N�^
	static CManager* m_pManager;	//�}�l�[�W���[�̃C���X�^���X

	CRenderer *m_pRenderer;				//�����_���[�ւ̃|�C���^
	CInput *m_pInputCur;				//���݂̓��̓f�o�C�X�ւ̃|�C���^
	CInputKeyboard *m_pInputKeyboard;	//�L�[�{�[�h�ւ̃|�C���^
	CInputGamepadX *m_pInputGamepadX;	//�Q�[���p�b�h�ւ̃|�C���^(Xinput)
	CCamera* m_pCamera;					//�J�����ւ̃|�C���^
	CLight* m_apLight[NUM_LIGHT];		//���C�g�ւ̃|�C���^
	CSound *m_pSound;	//�T�E���h�ւ̃|�C���^
	CFade *m_pFade;		//�t�F�[�h�ւ̃|�C���^
	MODE m_mode;		//���[�h
	CTitle *m_pTitle;	//�^�C�g�����[�h
	CGame *m_pGame;		//�Q�[�����[�h
	CResult *m_pResult;	//���U���g���[�h
	CPause *m_pPause;	//�|�[�Y

};

#endif // !_MANAGER_H_
