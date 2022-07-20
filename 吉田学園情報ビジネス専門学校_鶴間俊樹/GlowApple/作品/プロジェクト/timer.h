//=============================================================================
//
// �^�C�}�[���� [timer.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "object.h"
#include "number.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************

//*****************************************************************************
// �^�C�}�[�N���X
//*****************************************************************************
class CTimer : public CObject
{
public:
	CTimer();	//�f�t�H���g�R���X�g���N�^
	CTimer(int nNumDigit, CTexture::TEXTURE_TYPE type);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CTimer();	//�f�X�g���N�^
	static CTimer* Create(int nStartTime, int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize, float fSpace);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void AddTime(int nTime);	//�^�C�}�[�̎��Ԃ̒ǉ�
	int GetTime(void);	//�^�C�}�[�̎c�莞�Ԃ��擾
	int GetCountTimer(void) { return m_nCntTime; }	//�^�C�}�[�̃J�E���g���擾
	void SetStop(bool bStop);	//�^�C�}�[�̒�~��Ԃ�ݒ�

private:
	const int m_nNumDigit;		//�X�R�A�̌���
	CNumber* m_pNumberArray;	//�i���o�[�̓��I�z��̐擪�|�C���^
	bool m_bStop;		//�^�C�}�[����~�����ǂ���
	int m_nTime;		//�^�C�}�[�̎c�莞��
	int m_nCntTime;		//�^�C�}�[�̎c�莞�Ԃ�����܂ł̃J�E���g
};

#endif // !_TIMER_H_