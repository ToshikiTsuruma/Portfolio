//=============================================================================
//
// �Q�[������ [game.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HEIGHT_SEA (-10.0f)	//�C�̍���

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CTimer;
class CPlayer;
class CBoss;
class CScene2D;
class CSceneModel;
class CBillboard;
class CWallCylinder;

//*****************************************************************************
// �Q�[���N���X
//*****************************************************************************
class CGame
{
public:
	CGame();	//�f�t�H���g�R���X�g���N�^
	~CGame();	//�f�X�g���N�^
	static void SetScoreLate(int nScore);	//�Q�[���I�����̃X�R�A�̐ݒ�
	static int GetScoreLast(void);			//�Q�[���I�����̃X�R�A�̎擾
	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����

	bool GetChangeStage(void);	//�X�e�[�W�ύX�����ǂ����̎擾
	void SetFadeNextStage(void);//���̃X�e�[�W�ֈڍs���邽�߂̃t�F�[�h��ݒ�
	void SetNextStage(void);	//���̃X�e�[�W��ݒ�	���݂̓{�X�G���A�̂݁@����X�e�[�W�������o���ꍇ�͈����Ƀ^�C�v��ǉ�
	void DeadBoss(void);		//�{�X���S���̏���
	void GameOver(void);		//�Q�[���I�[�o�[���̏���
	bool GetGameOver(void);		//�Q�[���I�[�o�[���ǂ����̎擾
	void GameClear(void);		//�Q�[���N���A���̏���
	bool GetGameClear(void);	//�Q�[���N���A���ǂ����̎擾
	CPlayer* GetPlayer(void);	//�v���C���[�̎擾
	void ReleasePlayer(void);	//�Q�[�������v���C���[�ւ̃|�C���^��NULL�ɂ���
	CBoss* GetBoss(void);		//�{�X�̎擾
	void ReleaseBoss(void);		//�Q�[�������{�X�ւ̃|�C���^��NULL�ɂ���
	CTimer* GetTimer(void);		//�^�C�}�[�̎擾
	CSceneModel* GetRaft(void);			//���̎擾
	CBillboard* GetIconInteract(void);	//�C���^���N�g�L�[�L�����̃A�C�R���̎擾

private:
	static int m_nScoreLate;//�ŐV�̃Q�[���N���A���̃X�R�A

	bool m_bChangeStage;	//�X�e�[�W�ύX�̃t�F�[�h�����ǂ���
	CTimer* m_pTimer;		//�^�C�}�[�ւ̃|�C���^
	CPlayer* m_pPlayer;		//�v���C���[�ւ̃|�C���^
	CSceneModel* m_pRaft;			//���̃|�C���^	�X�e�[�W�ړ��̖���
	CBillboard* m_pIconInteract;	//�C���^���N�g�L�[�L�����̃A�C�R���̃|�C���^
	CBoss* m_pBoss;			//�{�X�ւ̃|�C���^
	CScene2D* m_pGameOver;	//�Q�[���I�[�o�[��ʂւ̃|�C���^
	CScene2D* m_pGameClear;	//�Q�[���N���A�\���ւ̃|�C���^
	bool m_bGameOver;		//�Q�[���I�[�o�[����
	bool m_bGameClear;		//�Q�[���N���A����
	int m_nCntFinish;		//�Q�[���I����̃J�E���g
	CBillboard* m_pFogBillboard;	//���̍ő勗���ɔz�u����r���{�[�h
};

#endif // !_GAME_H_