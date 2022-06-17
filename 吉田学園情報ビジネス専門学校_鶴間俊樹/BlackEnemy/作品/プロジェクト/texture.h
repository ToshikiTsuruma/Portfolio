//=============================================================================
//
// �e�N�X�`������ [texture.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LOAD_TEXT_TEXTURE (128)		//��x�Ƀt�@�C������ǂݍ��ރe�L�X�g�̃o�C�g��

//*****************************************************************************
// �e�N�X�`���N���X
//*****************************************************************************
class CTexture
{
public:
	enum class TEXTURE_TYPE {
		NONE = 0,

		//�w�i�̃e�N�X�`��
		BG_TITLE,
		BG_RESULT,

		//UI�̃e�N�X�`��
		//�^�C�g��
		GAME_TITLE,
		QUIT_GAME,
		TITLE_SELECT_START,
		TITLE_SELECT_TUTORIAL,
		TITLE_SELECT_CREDIT,
		TITLE_TUTORIAL_001,
		TITLE_TUTORIAL_002,
		TITLE_TUTORIAL_003,
		TITLE_CREDIT,
		//�Q�[��
		PAUSE_BG,
		PAUSE_START,
		PAUSE_RETRY,
		PAUSE_QUIT,
		TIMER_FRAME,
		GAUGE_LIFE_PLAYER,
		GAUGE_LIFE_BOSS,
		COMPASS_DIAL,
		COMPASS_MARK,
		ITEM_FRAME,
		WEAPON_FRAME,
		ICON_NONE,
		ICON_APPLE,
		ICON_SWORD,
		ICON_KATANA,
		CLOCK,
		ICON_INTERACT_KEYBOARD,
		ICON_INTERACT_GAMEPAD,
		GAMEOVER,
		TEXT_ITEM,
		TEXT_WEAPON,
		TEXT_GAMECLEAR,
		TEXT_RANKING,

		//���f���̃e�N�X�`��

		//�G�t�F�N�g�̃e�N�X�`��
		EFFECT_EXPLOSION,		// �����G�t�F�N�g
		EFFECT_DAMAGE_PUNCH,	// �p���`�̃_���[�W�G�t�F�N�g
		EFFECT_SWORD_SLASH,		// ���̎a���G�t�F�N�g
		EFFECT_DAMAGE_ENEMY,	// �G�̍U���ɂ��̃_���[�W�G�t�F�N�g
		EFFECT_GET_ITEM,		// �R�C������G�t�F�N�g
		EFFECT_PARTICLE,		// �p�[�e�B�N��

		//�i���o�[�̃e�N�X�`��
		NUMBER_001,
		NUMBER_002,
		NUMBER_003,
		NUMBER_004,

		ENUM_MAX
	};

	CTexture();		//�f�t�H���g�R���X�g���N�^
	~CTexture();	//�f�X�g���N�^

	static HRESULT Load(void);	//�e�N�X�`���f�[�^�̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���f�[�^�̉��
	static LPDIRECT3DTEXTURE9 GetTexture(TEXTURE_TYPE type);	//�e�N�X�`���̎擾
	static char* GetPathName(TEXTURE_TYPE type);				//�e�N�X�`���̃p�X�̕�����̎擾

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[(int)TEXTURE_TYPE::ENUM_MAX];	//�e�N�X�`���ւ̃|�C���^
	static char m_aFilePath[(int)TEXTURE_TYPE::ENUM_MAX][MAX_LOAD_TEXT_TEXTURE];	// �t�@�C���̃p�X
};

#endif // !_TEXTURE_H_