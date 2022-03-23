//=============================================================================
//
// �n�`���� [terrain.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "sceneModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �n�`�N���X
//*****************************************************************************
class CTerrain : public CSceneModel
{
public:
	enum class TERRAIN_TYPE {
		START = 0,	//�Q�[���J�n��
		BOSS,		//�{�X�펞
		ENUM_MAX
	};

	CTerrain();	//�f�t�H���g�R���X�g���N�^
	CTerrain(CModel::MODELTYPE typeModel);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CTerrain();	//�f�X�g���N�^
	static CTerrain* Create(TERRAIN_TYPE typeTerrain);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	static bool Collision(D3DXVECTOR3* posCollision, D3DXVECTOR3 vecStart, D3DXVECTOR3 vecEnd);	//�Փ˔���

private:

};

#endif // !_TERRAIN_H_