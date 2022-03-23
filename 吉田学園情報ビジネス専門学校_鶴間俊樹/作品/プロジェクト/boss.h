//=============================================================================
//
// �{�X���� [boss.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "sceneMotion.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// �{�X�N���X
//*****************************************************************************
class CBoss : public CSceneMotion
{
public:
	//���[�V�����̎��
	enum class MOTION_TYPE {
		NEUTRAL = 0,	//�j���[�g�������[�V����
		WAIT,	//�ҋ@��
		MOVE,	//�ړ�
		ATTACK_FRONT,	//�O���U��
		ATTACK_UNDER,	//���U��
		ATTACK_RUSH,	//�ːi�U��
		DEAD,	//���S
		ENUM_MAX
	};

	//���[�V�����̋敪
	enum class MOTION_CATEGORY {
		NONE = 0,	//���̑�
		MOVE,		//�ړ�
		ATTACK,		//�U��
		ENUM_MAX
	};

	//�U���̎��
	enum class ATTACK_TYPE {
		FRONT = 0,
		UNDER,
		RUSH,
		ENUM_MAX
	};

	//�����蔻��̕���
	enum class PARTS_TYPE {
		BODY = 0,	//����
		ARM_RIGHT,	//�E�r
		HAND_RIGHT,	//�E��
		ARM_LEFT,	//���r
		HAND_LEFT,	//����
		LEG_RIGHT,	//�E��	
		FOOT_RIGHT,	//�E��	
		LEG_LEFT,	//����	
		FOOT_LEFT,	//����
		ENUM_MAX
	};

	CBoss();	//�f�t�H���g�R���X�g���N�^
	~CBoss();	//�f�X�g���N�^

	static CBoss* Create(D3DXVECTOR3 pos);	//��������
	static void LoadMotionInfo(void);	//���[�V�����������[�h
	static void UnloadMotionInfo(void);	//���[�V���������A�����[�h

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��
	int GetNumCollisionParts(void);	//�����蔻�肪����p�[�c�̐��̎擾
	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//�����蔻��̏��̎擾
	void StartAttack(ATTACK_TYPE type);	//�U���J�n���̏���
	void Damage(int nDamage, bool* pDead);		//�_���[�W����

private:
	MOTION_CATEGORY GetMotionCategory(void);	//���[�V�����̋敪�̎擾

	static int m_nNumParts;	//�p�[�c��
	static PARTS_INFO* m_pPartsInfoArray;	//�p�[�c���̔z��̃|�C���^
	static MOTION_INFO m_aMotionInfo[(int)MOTION_TYPE::ENUM_MAX];	//���[�V�������
	static int m_nIdxWeapon;	//��������p�[�c�̃C���f�b�N�X

	D3DXVECTOR3	m_lastPos;	//�Ō�̈ʒu���W
	D3DXVECTOR3 m_move;		//�ړ���
	float m_rotDestY;		//�ڕW��Y���̊p�x
	bool m_bEncounterPlayer;//�v���C���[�ƃG���J�E���g���Ă��邩�ǂ���
	bool m_bStay;			//�v���C���[��ҋ@��
	bool m_bRotateAttack;	//�U�����[�V�������ɉ�]�ł��邩�ǂ���
	bool m_bValidAttack;	//�U�����L�����ǂ���
	int m_nCntNormalAttack;	//�ʏ�U�����s������
	int m_nCntAttackRush;	//�ːi�U����A���ōs������
	bool m_bRecovery;		//�U����̍d����
	int m_nLife;					//�̗�
	CScene2D* m_pGaugeLife;			//�̗̓Q�[�W
	CScene2D* m_pGaugeLifeBg;		//�̗̓Q�[�W�w�i
	CScene2D* m_pGaugeLifeFrame;	//�̗̓Q�[�W�̘g
	bool m_bDead;	//���S
	int m_nCntDead;	//���S��̃J�E���g
};

#endif // !_BOSS_H_