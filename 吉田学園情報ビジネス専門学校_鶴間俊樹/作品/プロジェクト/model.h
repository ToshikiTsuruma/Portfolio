//=============================================================================
//
// ���f������ [model.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LOAD_TEXT_MODEL (128)	//��x�Ƀt�@�C������ǂݍ��ރe�L�X�g�̃o�C�g��
#define MAX_MATERIAL (16)			//���f�����Ƃ̃}�e���A���̍ő吔

//*****************************************************************************
// ���f���N���X
//*****************************************************************************
class CModel
{
public:
	enum class MODELTYPE{
		//�n�`
		TERRAIN_START = 0,	//�n�`
		TERRAIN_BOSS,	//�{�X������n�`
		//����
		WEAPON_KATANA,	//��
		WEAPON_SWORD,	//��
		//�A�C�e��
		ITEM_APPLE, //�����S

		//�v���C���[
		pl_0, pl_1, pl_2, pl_3, _pl_4, _pl_5, pl_6, pl_7, pl_8, pl_9,

		//�G
		eNo_0, eNo_1, eNo_2, eNo_3, eNo_4,	//�ʏ�G
		eHu_0, eHu_1, eHu_2, eHu_3, eHu_4, eHu_5, eHu_6, eHu_7, eHu_8, eHu_9,	//�l�`�G
		eBo_0, eBo_1, eBo_2, eBo_3, eBo_4, eBo_5, eBo_6, eBo_7, eBo_8,	//�{�X

		//��
		tr_0, tr_1,

		//���̑�
		RAFT,

		ENUM_MAX	//�ő吔
	};

	typedef struct
	{
		LPD3DXMESH pMesh;	//���b�V���i���_���j�ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;	//�}�e���A���ւ̃|�C���^
		DWORD nNumMat;	//�}�e���A���̐�
	}ModelData;

	CModel();	//�f�t�H���g�R���X�g���N�^
	~CModel();	//�f�X�g���N�^
	static CModel* Create(MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine);	//��������
	static HRESULT Load(void);	//���f���f�[�^�̓ǂݍ���
	static void Unload(void);	//���f���f�[�^�̉��
	static ModelData GetModelData(MODELTYPE type);	//���f���f�[�^�̎擾
	static char* GetPathName(MODELTYPE type);		//�e�N�X�`���̃p�X�̕�����̎擾
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetPos(D3DXVECTOR3 pos);	//�ʒu���W�̐ݒ�
	D3DXVECTOR3 GetPos(void);		//�ʒu���W�̎擾
	void SetRot(D3DXVECTOR3 rot);	//�p�x�̐ݒ�
	D3DXVECTOR3 GetRot(void);		//�p�x�̎擾
	void UpdateMtxWorld(void);		//���[���h�}�g���b�N�X�̍X�V
	D3DXMATRIX GetMtxWorld(void);			//���[���h�}�g���b�N�X�̎擾
	D3DXMATRIX* GetPtrMtxWorld(void);		//���[���h�}�g���b�N�X�̃|�C���^�̎擾
	MODELTYPE GetModelType(void);			//���f���̃^�C�v�̎擾
	void SetMaterialAlpha(float fAlpha);	//�}�e���A���̃A���t�@�l��ݒ�
	float GetMaterialAlpha(void);			//�}�e���A���̃A���t�@�l���擾
	void SetColorOutline(D3DXCOLOR col);	//�֊s�̐F�̎w��
	void SetDrawOutline(bool bDraw);		//�֊s�̕\���ݒ�

private:
	static ModelData m_aModelData[(int)MODELTYPE::ENUM_MAX];	//�S���f���̏��
	static char m_aFilePath[(int)MODELTYPE::ENUM_MAX][MAX_LOAD_TEXT_MODEL];	// ���f���̃e�L�X�g�t�@�C���̃p�X
	static D3DXMATERIAL m_aMatDefault[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL];	//���f���̃}�e���A���̏����l
	static CTexture::TEXTURE_TYPE m_aTexType[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL];	//�}�e���A�����Ƃ̃e�N�X�`���̎��

	MODELTYPE m_modelType;	//���f���̎��
	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_rot;		//����
	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
	CModel* m_pParent;		//�e�̃��f���ւ̃|�C���^
	int m_nIdxParent;		//�e�̃C���f�b�N�X
	D3DXMATERIAL m_aMat[MAX_MATERIAL];	//���f���̃}�e���A��

	bool m_bOutline;			//�֊s�����邩�ǂ���
	LPD3DXMESH m_pCloneMesh;	//�����������b�V���̃|�C���^
};

#endif // !_MODEL_H_