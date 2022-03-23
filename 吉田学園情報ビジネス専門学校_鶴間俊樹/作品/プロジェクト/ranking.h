//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANKING (10)

//*****************************************************************************
// �����L���O�N���X
//*****************************************************************************
class CRanking : public CScene
{
public:
	CRanking();	//�f�t�H���g�R���X�g���N�^
	~CRanking();	//�f�X�g���N�^
	static CRanking* Create(CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize, float fSpace, float fSpaceScore);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

private:
	typedef struct {
		CTexture::TEXTURE_TYPE type;
		D3DXVECTOR3 pos;
		float fSize;
		float fSpace;
	} NumberData;	//���������p�̃f�[�^

	NumberData m_numberData;	//���������p�̃f�[�^
	float m_fSpaceScore;		//�X�R�A��Y���W�̊Ԋu
	CScore* m_apScore[MAX_RANKING];		//�����L���O�̃X�R�A�ւ̃|�C���^ 
	CScore* m_pScoreOutRank;			//����̃X�R�A�������N�O���������̃X�R�A�ւ̃|�C���^
	int m_aRankingData[MAX_RANKING];	//�����L���O�̃f�[�^
};

#endif // !_RANKING_H_