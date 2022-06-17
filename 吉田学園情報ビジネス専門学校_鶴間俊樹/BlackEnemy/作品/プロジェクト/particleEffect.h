//=============================================================================
//
// �p�[�e�B�N���G�t�F�N�g���� [particleEffect.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _PARTICLE_EFFECT_H_
#define _PARTICLE_EFFECT_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************

//*****************************************************************************
// �p�[�e�B�N���G�t�F�N�g�N���X
//*****************************************************************************
class CParticleEffect : public CScene
{
public:
	//��������p�[�e�B�N���̏��
	typedef struct {
		int nLife;			//���C�t
		float fSizeStart;	//�J�n���̃T�C�Y
		float fAddSize;		//�T�C�Y�̒ǉ���
		float fSpeedMove;	//�ړ����x
		D3DXVECTOR3 addMove;//������
		D3DXCOLOR colStart;	//�������̐F
		D3DXCOLOR colEnd;	//���Ŏ��̐F
	} PARTICLE_INFO;

	CParticleEffect();	//�f�t�H���g�R���X�g���N�^
	CParticleEffect(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nLife, int nSpanCreate, float fAngleEmit);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CParticleEffect();	//�f�X�g���N�^
	static CParticleEffect* Create(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nLife, int nSpanCreate, float fAngleEmit);	//��������
	static void EmitCircle(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nNumCreate, float fAngleEmit);	//�~��Ƀp�[�e�B�N������˂���
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

private:
	PARTICLE_INFO m_particleInfo;	//��������p�[�e�B�N���̏��
	D3DXVECTOR3 m_pos;	//�G�t�F�N�g�̈ʒu
	bool m_bLoop;		//���[�v���邩�ǂ���
	int m_nLife;		//�G�t�F�N�g�����݂��鎞��
	int m_nCntCreate;	//�p�[�e�B�N�������̃J�E���g
	int m_nSpanCreate;	//�p�[�e�B�N�������̊Ԋu
	float m_fAngleEmit;	//���˂���p�x
};

#endif // !_PARTICLE_EFFECT_H_