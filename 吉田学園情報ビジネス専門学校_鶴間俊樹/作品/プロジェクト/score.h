//=============================================================================
//
// �X�R�A���� [score.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"
#include "number.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DIGIT_SCORE_RANKING (6)	//�����L���O�\�����̃X�R�A�̍ő包��

//*****************************************************************************
//�O���錾
//*****************************************************************************

//*****************************************************************************
// �X�R�A�N���X
//*****************************************************************************
class CScore : public CScene
{
public:
	CScore();	//�f�t�H���g�R���X�g���N�^
	CScore(int nNumDigit, CTexture::TEXTURE_TYPE type);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CScore();	//�f�X�g���N�^
	static CScore* Create(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize, float fSpace, int nLife);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetScore(int nScore);	//�X�R�A�̐ݒ�
	void AddScore(int nValue);	//�X�R�A�̒ǉ�
	int GetScore(void);			//�X�R�A�̎擾
	void SetColor(D3DXCOLOR col);	//�X�R�A�̐F�̐ݒ�

private:
	const int m_nNumDigit;		//�X�R�A�̌���
	CNumber* m_pNumberArray;	//�i���o�[�̓��I�z��̐擪�|�C���^
	int m_nScore;	//�X�R�A
	int m_nLife;	//���C�t
};

#endif // !_SCORE_H_