//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�N���X
//*****************************************************************************
class CSound
{
public:
	enum class SOUND_LABEL {
		NONE = 0,
		BGM_TITLE,		//�^�C�g��BGM
		BGM_GAME,		//�Q�[��BGM
		BGM_GAME_BOSS,	//�{�X�ƑΛ����̃Q�[��BGM
		BGM_RESULT,		//���U���gBGM

		TITLE_START,	//�^�C�g����ʂ̃Q�[���X�^�[�g��
		TITLE_SWITCH,	//�^�C�g����ʂ̑I��ؑ֎�
		TITLE_OPEN,		//�^�C�g����ʂ̃E�B���h�E���J������
		TITLE_CLOSE,	//�^�C�g����ʂ̃E�B���h�E�������

		CANSEL,			//�L�����Z����
		DODGE,			//�����
		SWISH_PUNCH,	//����U�鉹
		SWISH_SWORD,	//����U�鉹
		SWISH_KATANA,	//����U�鉹
		DAMAGE_PUNCH,	//���ɂ��_���[�W��
		DAMAGE_SWORD,	//���ɂ��_���[�W��
		DAMAGE_SWORD_CRITICAL,	//���ɂ��N���e�B�J���_���[�W��
		CHANGE_PUNCH,	//���ɕ����ύX���鉹
		CHANGE_SWORD,	//���ɕ����ύX���鉹
		CHANGE_KATANA,	//���ɕ����ύX���鉹
		ATTACK_BOSS,	//�{�X�̍U�����̏Ռ���
		HEAL,			//�񕜉�
		STUN,			//�C�≹
		EXPLOSION,		//������
		GET_ITEM,		//�A�C�e���擾��
		ADD_TIME,		//���Ԓǉ���
		TIME_ALERT,		//���Ԃ̌x����
		BREAK_TREE,		//�؂����鉹
		GAUGE,			//�Q�[�W�i�s��
		CHANGE_STAGE,	//�X�e�[�W�ύX��
		GAMEOVER,		//�Q�[���I�[�o�[
		GAMECLEAR,		//�Q�[���N���A

		PAUSE,			//�|�[�Y
		PAUSE_SELECT,	//�|�[�Y��ʂł̑I��

		ENUM_MAX
	};

	CSound();	//�f�t�H���g�R���X�g���N�^
	~CSound();	//�f�X�g���N�^
	HRESULT Init(HWND hWnd);	//����������
	void Uninit(void);			//�I������
	HRESULT PlaySound(SOUND_LABEL label);	//���̍Đ�
	void StopSound(SOUND_LABEL label);		//���̒�~
	void StopSound(void);					//���̒�~
	void PauseSound(SOUND_LABEL label);		//���̈ꎞ��~
	void StartSound(SOUND_LABEL label);		//���̍ĊJ

	static void SetBGM(SOUND_LABEL label);			//�Đ���BGM�̐ݒ�
	static SOUND_LABEL GetBGM(void);				//�Đ���BGM�̎擾

private:
	// �p�����[�^�\���̒�`
	typedef struct
	{
		char *filename;		// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} PARAM;

	//�����o�֐�
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	
	//�����o�ϐ�
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[(int)SOUND_LABEL::ENUM_MAX] = {};	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[(int)SOUND_LABEL::ENUM_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[(int)SOUND_LABEL::ENUM_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y
	static PARAM m_aParam[(int)SOUND_LABEL::ENUM_MAX];						//�t�@�C�����ƃ��[�v

	static SOUND_LABEL m_playBGM;	//�Đ�����BGM
};

#endif // !_SOUND_H_
