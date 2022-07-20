//=============================================================================
//
// �i���o�[���� [number.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �i���o�[�N���X
//*****************************************************************************
class CNumber
{
public:
	CNumber();	//�f�t�H���g�R���X�g���N�^
	~CNumber();	//�f�X�g���N�^
	static CNumber* Create(CTexture::TEXTURE_TYPE type, int nNumDigit);	//��������
	HRESULT Init(D3DXVECTOR3 pos, float fSize);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetNumber(int nNumber);	//�����̐ݒ�
	void SetColor(D3DXCOLOR col);	//�����̐F�̐ݒ�

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	CTexture::TEXTURE_TYPE m_texType;	//�e�N�X�`���̃^�C�v
};

#endif // !_NUMBER_H_