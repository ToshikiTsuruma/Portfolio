//=============================================================================
//
// �v���C���[���� [player.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "sceneMotion.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CScene2D;
class CWeapon;
class CScore;

//*****************************************************************************
// �v���C���[�N���X
//*****************************************************************************
class CPlayer : public CSceneMotion
{
public:
	//���[�V�����̎��
	enum class MOTION_TYPE {
		NEUTRAL = 0,	//�j���[�g�������[�V����
		WALK,	//����
		DASH,	//����
		DODGE_FRONT,//�O�����
		DODGE_BACK,	//������
		PUNCH_1,		//�p���`
		PUNCH_2,		//�p���`
		ATTACK_SWORD_1,	//���U��
		ATTACK_SWORD_2,	//���U��
		ATTACK_SWORD_3,	//���U��
		ATTACK_KATANA_1,	//���U��
		ATTACK_KATANA_2,	//���U��
		ATTACK_KATANA_3,	//���U���@�˂�
		ATTACK_KATANA_DRAW,	//�����U��
		SHEATH,				//�[�����[�V����
		STUN,	//�X�^��
		ENUM_MAX
	};

	//���[�V�����̋敪
	enum class MOTION_CATEGORY {
		NONE = 0,	//���̑�
		MOVE,		//�ړ�
		ATTACK,		//�U��
		DODGE,		//���
		ENUM_MAX
	};	

	//����̎��
	enum class WEAPONTYPE {
		PUNCH = 0,
		SWORD,
		KATANA,
		ENUM_MAX
	};

	CPlayer();	//�f�t�H���g�R���X�g���N�^
	~CPlayer();	//�f�X�g���N�^

	static CPlayer* Create(D3DXVECTOR3 pos);	//��������
	static void LoadMotionInfo(void);	//���[�V�����������[�h
	static void UnloadMotionInfo(void);	//���[�V���������A�����[�h

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��
	void GameClear(void);	//�Q�[���N���A���̏���
	void GameOver(void);	//�Q�[���I�[�o�[���̏���
	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//�����蔻��̏��̎擾
	void Damage(int nDamage, bool* pDead);		//�_���[�W����
	bool GetItem(int nTypeItem);	//�A�C�e���擾���̏���
	D3DXVECTOR3 GetLastPos(void);	//�Ō�̈ʒu���W�̎擾
	void SetMove(D3DXVECTOR3 move);	//�ړ��ʂ̐ݒ�
	D3DXVECTOR3 GetMove(void);		//�ړ��ʂ̎擾
	float GetRadius(void);			//�����蔻��̔��a�̎擾
	bool GetStun(void);		//�X�^����Ԃ̎擾

private:
	MOTION_CATEGORY GetMotionCategory(void);	//���[�V�����̋敪�̎擾
	void ChangeWeaponType(WEAPONTYPE typeWeapon);	//����̕ύX

	static int m_nNumParts;	//�p�[�c��
	static PARTS_INFO* m_pPartsInfoArray;	//�p�[�c���̔z��̃|�C���^
	static MOTION_INFO m_aMotionInfo[(int)MOTION_TYPE::ENUM_MAX];	//���[�V�������
	static int m_nIdxWeapon;	//��������p�[�c�̃C���f�b�N�X

	D3DXVECTOR3	m_lastPos;	//�Ō�̈ʒu���W
	D3DXVECTOR3 m_destRot;	//�ڕW�̊p�x
	D3DXVECTOR3 m_move;		//�ړ���
	bool m_bLand;			//�ڒn�����ǂ���
	bool m_bValidAttack;	//�U�����L�����ǂ���
	int m_nNumKillEnemy;	//�G��|������
	bool m_bRecovery;		//�U��������̍d����
	int m_nCntDodge;		//��𒆂̃J�E���^
	int m_nCntAttackMove;	//�U�����̈ړ��J�E���^
	int m_nLife;			//�̗�
	bool m_bStun;			//�X�^�����
	int m_nCntStun;			//�X�^�����̃J�E���^
	CScene2D* m_pGaugeLife;	//�̗̓Q�[�W
	bool m_bUseItem;		//�A�C�e���g�p�����ǂ���
	int m_nCntUseItem;		//�A�C�e���g�p���̃J�E���g
	CScene2D* m_pGaugeUseItem;	//�A�C�e���g�p�Q�[�W
	CScore* m_pCounterItem;		//���ݏ������Ă���A�C�e���̐��̃J�E���^�[�̃|�C���^
	WEAPONTYPE m_typeWeapon;	//����̎��
	CWeapon* m_pWeapon;			//����N���X�ւ̃|�C���^
	CScene2D* m_pIconWeapon;	//����̃A�C�R��
};

#endif // !_PLAYER_H_