//=============================================================================
//
// ���C�g���� [ligth.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// ���C�g�N���X
//*****************************************************************************
class CLight
{
public:
	CLight();	//�f�t�H���g�R���X�g���N�^
	~CLight();	//�f�X�g���N�^
	static CLight* Create(D3DXVECTOR3 vecDir, D3DXCOLOR col);	//��������
	HRESULT Init(D3DXVECTOR3 vecDir, D3DXCOLOR col);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	static void SetIdxNext(CLight* pLight);	//���̃��C�g�̃C���f�b�N�X��ݒ�

private:
	D3DLIGHT9 m_light;		//���C�g�̏��
	D3DXVECTOR3 m_vecDir;	//���C�g�̕����x�N�g��
	int m_nIdx;				//���C�g�̃C���f�b�N�X

	//���C�g�̃��X�g
	static CLight* m_pTop;	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CLight* m_pCur;	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CLight* m_pPrev;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CLight* m_pNext;		//���̃I�u�W�F�N�g�ւ̃|�C���^
};

#endif // !_LIGHT_H_