//=============================================================================
//
// �J�������� [camera.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �J�����N���X
//*****************************************************************************
class CCamera
{
public:
	CCamera();		//�f�t�H���g�R���X�g���N�^
	~CCamera();	//�f�X�g���N�^
	static CCamera* Create(void);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void SetCamera(void);	//�J�����̃Z�b�g
	void SetPos(D3DXVECTOR3 pos);	//�ʒu���W�̐ݒ�
	D3DXVECTOR3 GetPos(void);		//�ʒu���W�̎擾
	D3DXVECTOR3 GetPosV(void);		//���_�̈ʒu���W�̎擾
	D3DXVECTOR3 GetPosR(void);		//�����_�̈ʒu���W�̎擾
	void SetRot(D3DXVECTOR3 rot);	//�p�x�̐ݒ�
	D3DXVECTOR3 GetRot(void);		//�p�x�̎擾
	void SetDistance(float fDistance);	//�����̐ݒ�
	float GetDistance(void);			//�����̎擾

private:
	D3DXVECTOR3 m_pos;			//�J�����̈ʒu
	D3DXVECTOR3 m_rot;			//�J�����̌���
	D3DXVECTOR3 m_posV;			//���_
	D3DXVECTOR3 m_posR;			//�����_
	float m_fDistance;			//���_�ƒ����_�̋���
	D3DXVECTOR3 m_vecU;			//������x�N�g��
	D3DXMATRIX m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;		//�r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;	//�r���[�|�[�g
};

#endif // !_CAMERA_H_