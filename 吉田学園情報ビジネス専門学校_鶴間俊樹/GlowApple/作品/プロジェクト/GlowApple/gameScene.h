//=============================================================================
//
// �Q�[���V�[������ [gameScene.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CStage;
class CAppleTree;
class CTimer;
class CSelectMenu2D;
class CEnemySpawner;

//*****************************************************************************
// �Q�[���V�[���N���X
//*****************************************************************************
class CGameScene : public CScene
{
public:
	CGameScene();	//�f�t�H���g�R���X�g���N�^
	virtual ~CGameScene();	//�f�X�g���N�^
	void Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void GameClear(void);	//�Q�[���N���A
	void GameOver(void);	//�Q�[���I�[�o�[

	bool GetGameClear(void) { return m_bGameClear; }	//�Q�[���N���A��Ԃ̃t���O
	bool GetGameOver(void) { return m_bGameOver; }		//�Q�[���I�[�o�[��Ԃ̃t���O

	CAppleTree* GetAppleTree(void) { return m_pAppleTree; }	//�ь�̖؂̎擾
	CEnemySpawner* GetEnemySpawner(void) { return m_pEnemySpawner; }	//�G�̃X�|�i�[���擾

private:
	void CreateMenuEndGame(void);	//�Q�[���I�����̑I�����j���[�̐���

	bool m_bGameClear;	//�Q�[���I�[�o�[
	bool m_bGameOver;	//�Q�[���I�[�o�[

	CStage* m_pStage;	//�X�e�[�W�ւ̃|�C���^
	CAppleTree* m_pAppleTree;	//�ь�̖؂̃|�C���^
	CTimer* m_pTimer;	//�Q�[���̃^�C�}�[
	CSelectMenu2D* m_pMenuGameEnd;	//�Q�[���I�����̑I�����j���[
	CEnemySpawner* m_pEnemySpawner;	//�G�̃X�|�i�[
};

#endif // !_GAME_SCENE_H_