//=============================================================================
//
// ���b�V���V�����_�[���� [meshcylinder.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// ���b�V���V�����_�[�N���X
//*****************************************************************************
class CMeshcylinder : public CScene3D
{
public:
	CMeshcylinder();	//�f�t�H���g�R���X�g���N�^
	CMeshcylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadius, int nNumY, float fHeight, bool bOut, D3DXCOLOR col);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CMeshcylinder();	//�f�X�g���N�^
	static CMeshcylinder* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadius, int nNumY, float fHeight, bool bOut, D3DXCOLOR col);	//��������
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��
	float GetRadius(void);		//���a�̎擾
	void SetColor(D3DXCOLOR col);	//�F�̐ݒ�
	D3DXCOLOR GetColor(void);		//�F�̎擾

private:
	const int m_nNumXZ;	//XZ�����̃|���S���̐�
	const int m_nNumY;	//Y�����̃|���S���̐�
	float m_fHeight;	//�|���S���̍���
	float m_fRadius;	//�~���̔��a
	bool m_bOut;		//�|���S�����O�������ǂ���
	D3DXCOLOR m_col;	//�F
};

#endif // !_MESHCYLINDER_H_