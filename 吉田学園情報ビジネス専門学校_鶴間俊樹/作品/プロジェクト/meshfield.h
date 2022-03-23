//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// ���b�V���t�B�[���h�N���X
//*****************************************************************************
class CMeshfield : public CScene3D
{
public:
	CMeshfield();		//�f�t�H���g�R���X�g���N�^
	~CMeshfield();	//�f�X�g���N�^
	static CMeshfield* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumX, int nNumZ, float fWidth, float fDepth);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

private:
	int m_nNumX;	//X�����̃|���S���̐�
	int m_nNumZ;	//Z�����̃|���S���̐�
	float m_fWidth;	//�|���S���̕�
	float m_fDepth;//�|���S���̍���
};

#endif // !_MESHFIELD_H_