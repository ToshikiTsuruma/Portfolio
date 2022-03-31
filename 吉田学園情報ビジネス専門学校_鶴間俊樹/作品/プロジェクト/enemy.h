//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "sceneMotion.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DIGIT_ADD_TIME_ENEMY (2)	//�G��|�����Ƃ��ɒǉ�����鎞�Ԃ̍ő包��

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CBillboard;

//*****************************************************************************
// �G�l�~�[�N���X
//*****************************************************************************
class CEnemy : public CSceneMotion
{
public:
	enum class MOTION_CATEGORY {
		NEUTRAL = 0,	//�j���[�g����
		MOVE,			//�ړ�
		ATTACK,			//�U��
		ENUM_MAX
	};	//���[�V�����̋敪

	CEnemy();	//�f�t�H���g�R���X�g���N�^
	CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, bool bOutline, int nLife, float fDistAttack, int nAddTime);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CEnemy();	//�f�X�g���N�^

	static void SetEnemyCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fRadiusCircle, int nMinDist, int nMaxDist);	//���S�~��Ƀ����_���Ɋp�x���炵�ēG��z�u����
	virtual HRESULT Init(void);		//����������
	virtual void Uninit(void);		//�I������
	virtual void Update(void);		//�X�V����
	virtual void Draw(void);		//�`�揈��
	void Damage(int nDamage, bool* pDead);		//�_���[�W
	void Dead(void);		//���S������
	void SetRotateAttack(bool bRotateAttack);	//�U�����[�V�������ɉ�]�ł��邩�ǂ����̐ݒ�

	virtual MOTION_CATEGORY GetMotionCategory(void) = 0;	//���[�V�����̋敪�̎擾
	virtual void SetMoveMotion(void) = 0;	//�ړ����̃��[�V������ݒ�
	virtual void SetChaseMotion(void) = 0;	//�ǐՎ��̃��[�V������ݒ�
	virtual void SetDeadMotion(void) = 0;	//���S���̃��[�V������ݒ�
	virtual bool AttackStart(void) = 0;		//�U���J�n��
	virtual void MotionEnd(void) = 0;	//���[�V�����I����
	virtual void MotionAct(void) = 0;	//���[�V�������̍s��

private:
	const float m_fDistAttack;	//�U�����鋗��
	const int m_nMaxLife;	//�ő�̗̑�
	const int m_nAddTime;	//�|�����Ƃ��̎��Ԓǉ�

	D3DXVECTOR3 m_move;		//�ړ���
	float m_rotDestY;		//�ڕW��Y���̊p�x
	bool m_bRotateAttack;	//�U�����[�V�������ɉ�]�ł��邩�ǂ���
	int m_nCntMove;			//�ړ��̐؂�ւ��̃J�E���g
	int m_nMoveTimeRand;	//�ړ����Ԃ̗���
	bool m_bDetectPlayer;	//�v���C���[�����������ǂ���
	int m_nLife;			//�̗�
	bool m_bDead;			//���S
	int m_nCntDead;			//���S��̃J�E���g
	CBillboard* m_pGaugeLife;		//�̗̓Q�[�W�̃r���{�[�h�̃|�C���^
	CBillboard* m_pGaugeLifeFrame;	//�̗̓Q�[�W�̃r���{�[�h�̘g�̃|�C���^
	int m_nCntDrawGaugeLife;		//�̗̓Q�[�W�̕`�掞��
	CBillboard* m_pClock;	//���v�̉摜�̃r���{�[�h
	CBillboard* m_pAddTime[MAX_DIGIT_ADD_TIME_ENEMY];	//�ǉ�����b���̃r���{�[�h �������p��
	bool m_bDraw;	//�`�悷�邩�ǂ���
};

#endif // !_ENEMY_H_