//=============================================================================
//
// ���b�V���O������ [meshorbit.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// ���b�V���O���N���X
//*****************************************************************************
class CMeshorbit : public CScene
{
public:
	CMeshorbit();		//�f�t�H���g�R���X�g���N�^
	CMeshorbit(int nNumVtx);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CMeshorbit();	//�f�X�g���N�^
	static CMeshorbit* Create(D3DXMATRIX* pMtxParent, int nNumVtx,int nSpanUpdate , D3DXVECTOR3 offset0, D3DXVECTOR3 offset1, D3DXCOLOR col0, D3DXCOLOR col1);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetBindPos(void);	//���b�V���O���̈ʒu���Œ�
	bool GetBindPos(void);	//�o�C���h��Ԃ̎擾
	void SetVisible(bool bVisible);	//����Ԃ̐ݒ�
	bool GetVisible(void);	//����Ԃ̎擾

private:
	const int m_nNumVtx;		//���_��
	D3DXMATRIX* m_pMtxParent;	//�e�}�g���b�N�X�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�̃|�C���^
	D3DXVECTOR3 m_aOffset[2];	//���[�̃I�t�Z�b�g
	D3DXCOLOR m_aCol[2];		//���[�̊�̐F
	int m_nSpanUpdate;		//���_�ʒu���X�V����Ԋu
	int m_nCntUpdate;		//���_�ʒu���X�V����J�E���^
	bool m_bBindPos;		//�ʒu�̌Œ���
	bool m_bVisible;		//�����
};

#endif // !_MESHORBIT_H_