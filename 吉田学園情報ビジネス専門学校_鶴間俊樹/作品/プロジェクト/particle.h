//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "billboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �p�[�e�B�N���N���X
//*****************************************************************************
class CParticle : public CBillboard
{
public:
	CParticle();	//�f�t�H���g�R���X�g���N�^
	CParticle(float fDecSize);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CParticle();	//�f�X�g���N�^
	static CParticle* Create(D3DXVECTOR3 pos, float fSize, float fDecSize, D3DXCOLOR col);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetDecSize(float fSize);	//�G�t�F�N�g�̃T�C�Y�̌����ʂ�ݒ�

private:
	float m_fDecSize;	//�T�C�Y�̌�����
};

#endif // !_PARTICLE_H_