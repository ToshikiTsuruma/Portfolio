//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "objectMotion.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DIGIT_ADD_TIME_ENEMY (2)	//�G��|�����Ƃ��ɒǉ�����鎞�Ԃ̍ő包��

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CBillboard;
class CGauge3D;

//*****************************************************************************
// �G�l�~�[�N���X
//*****************************************************************************
class CEnemy : public CObjectMotion
{
public:
	enum class MOTION_CATEGORY {
		NEUTRAL = 0,	//�j���[�g����
		MOVE,			//�ړ�
		ATTACK,			//�U��
		DEAD,			//���S
		ENUM_MAX
	};	//���[�V�����̋敪

	CEnemy();	//�f�t�H���g�R���X�g���N�^
	CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, float fMoveSpeed, int nLife, float fDistAttack);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CEnemy();	//�f�X�g���N�^

	static void SetEnemyCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fRadiusCircle, int nMinDist, int nMaxDist);	//���S�~��Ƀ����_���Ɋp�x���炵�ēG��z�u����
	virtual HRESULT Init(void);		//����������
	virtual void Uninit(void);		//�I������
	virtual void Update(void);		//�X�V����
	virtual void Draw(void);		//�`�揈��
	void Damage(int nDamage, bool* pDead);		//�_���[�W
	void Dead(void);		//���S������

	virtual MOTION_CATEGORY GetMotionCategory(void) = 0;	//���[�V�����̋敪�̎擾
	virtual void SetMoveMotion(void) = 0;	//�ړ����̃��[�V������ݒ�
	virtual void SetDeadMotion(void) = 0;	//���S���̃��[�V������ݒ�
	virtual void AttackStart(void) = 0;		//�U���J�n��
	virtual void MotionEnd(void) = 0;	//���[�V�����I����
	virtual void MotionAct(void) = 0;	//���[�V�������̍s��

private:
	void SetRotTree(void);		//�؂̕����ւ̊p�x�ɐݒ�
	void AttackDistance(void);	//�U���\�ȋ������ǂ������v�Z
	void Attack(void);	//�U���̏���

	const float m_fMoveSpeed;	//�ړ����x
	const float m_fDistAttack;	//�U�����鋗��
	const int m_nMaxLife;	//�ő�̗̑�

	D3DXVECTOR3 m_move;		//�ړ���
	float m_rotDestY;		//�ڕW��Y���̊p�x
	bool m_bAttackDist;		//�U���\�ȋ������ǂ���
	int m_nLife;			//�̗�
	bool m_bDead;			//���S
	int m_nCntDead;			//���S��̃J�E���g
	bool m_bGoldEnemy;		//���F�̓G���ǂ���

	CGauge3D* m_pGaugeLife;	//�̗̓Q�[�W
};

#endif // !_ENEMY_H_