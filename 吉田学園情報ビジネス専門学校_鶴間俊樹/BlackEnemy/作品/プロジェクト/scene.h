//=============================================================================
//
// �I�u�W�F�N�g���� [scene.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SCENE (1024)

//*****************************************************************************
//�O���錾
//*****************************************************************************

//*****************************************************************************
// �I�u�W�F�N�g�N���X
//*****************************************************************************
class CScene
{
public:
	//�I�u�W�F�N�g�̎�� (�����蔻���S�����ŕK�v�ȏꍇ�̂���)
	enum class OBJ_TYPE {
		NONE = 0,	
		TERRAIN,	//�n�`
		WALL,		//��	
		PLAYER,		//�v���C���[
		ENEMY,		//�G
		ITEM,		//�A�C�e��
		TREE,		//��
		OTHER,		//�}�b�v�ؑ֎��ɔj�������悤�Ȃ���
		ENUM_MAX
	};	

	//�X�V��
	enum class UPDATE_PRIORITY {
		DEFAULT = 0,	//�X�V�����e�����Ȃ�����
		PLAYER,			//�v���C���[
		ENEMY,			//�G
		ENUM_MAX
	};	

	enum class DRAW_PRIORITY {
		NONE = 0,	
		BG,			//�w�i
		DEFAULT,	//�f�t�H���g
		CHARA,		//�v���C���[���̃L�����N�^�[�Ɋ֘A�������
		CLEAR,		//���߂���I�u�W�F�N�g
		EFFECT,		//�G�t�F�N�g
		UI,			//UI
		FRONT,		//�őO
		ENUM_MAX
	};	//�`�揇

	CScene();			//�f�t�H���g�R���X�g���N�^
	virtual ~CScene();	//�f�X�g���N�^
	virtual HRESULT Init(void) = 0;	//����������
	virtual void Uninit(void) = 0;	//�I������
	virtual void Update(void) = 0;	//�X�V����
	virtual void Draw(void) = 0;	//�`�揈��

	virtual void SetPos(D3DXVECTOR3 pos);	//�ʒu���W�̐ݒ�
	virtual D3DXVECTOR3 GetPos(void);		//�ʒu���W�̎擾
	virtual D3DXVECTOR3 GetLastPos(void);	//�Ō�̈ʒu���W�̎擾
	virtual void SetMove(D3DXVECTOR3 move);	//�ړ��ʂ̐ݒ�
	virtual D3DXVECTOR3 GetMove(void);		//�ړ��ʂ̎擾
	virtual float GetRadius(void);			//�����蔻��̔��a�̎擾
						
	virtual int GetNumCollisionParts(void);	//�����蔻�肪����p�[�c�̐��̎擾
	virtual void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosCol, float* const pRadiusCol);	//�����蔻��̏��̎擾
	virtual void UpdateMtxWorldAll(void);			//�I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X�̍X�V
	virtual void Damage(int nDamage, bool* pDead);	//�_���[�W
	virtual bool GetItem(int nTypeItem);	//�A�C�e���擾���̏���

	static void ReleaseAll(void);	//�S�I�u�W�F�N�g�̉������
	static void UpdateAll(void);	//�S�I�u�W�F�N�g�̍X�V����
	static void DrawAll(void);		//�S�I�u�W�F�N�g�̕`�揈��
	static CScene* GetSceneTopAll(void);	//�S�I�u�W�F�N�g�̃��X�g��Top���擾
	static CScene* GetSceneNextAll(CScene* pScene);	//�S�I�u�W�F�N�g�̃��X�g��Next���擾
	static CScene* GetSceneTopObjtype(OBJ_TYPE objType);	//�I�u�W�F�N�g�^�C�v�̃��X�g��Top���擾
	static CScene* GetSceneNextObjtype(CScene* pScene);	//�I�u�W�F�N�g�^�C�v�̃��X�g�̈�����Next���擾
	static void ReleaseObjtype(OBJ_TYPE objtype);	//�w�肵���^�C�v�̃I�u�W�F�N�g�̉������

	bool GetDeath(void);	//���S�t���O�̎擾
	void SetObjType(OBJ_TYPE objType);	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	OBJ_TYPE GetObjType(void);			//�I�u�W�F�N�g�̎�ނ̎擾
	void SetUpdatePriority(UPDATE_PRIORITY priority);	//�X�V�̗D�揇�ʂ̐ݒ�
	void SetDrawPriority(DRAW_PRIORITY priority);		//�`��̗D�揇�ʂ̐ݒ�

	void SetEnableCollision(bool bUse);		//�����蔻��̎g�p��Ԃ̐ݒ�
	bool GetEnableCollision(void);			//�����蔻��̎g�p��Ԃ̎擾
	void SetTexType(CTexture::TEXTURE_TYPE type);	//�e�N�X�`���̎�ނ̐ݒ�
	CTexture::TEXTURE_TYPE GetTexType(void);			//�e�N�X�`���̎�ނ̎擾

protected:
	void Release(void);	//�I�u�W�F�N�g�̉������

private:
	bool m_bDeath;		//���S�t���O
	OBJ_TYPE m_objType;	//�I�u�W�F�N�g�̎��
	UPDATE_PRIORITY m_updatePriority;	//�X�V�̗D�揇��
	DRAW_PRIORITY m_drawPriority;		//�`��̗D�揇��

	bool m_bEnableCollision;	//�����蔻�肪�L�����ǂ���
	CTexture::TEXTURE_TYPE m_texType;	//�e�N�X�`���̃^�C�v

	//�S�I�u�W�F�N�g�̃��X�g
	static CScene* m_pTopAll;	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CScene* m_pCurAll;	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CScene* m_pPrevAll;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene* m_pNextAll;		//���̃I�u�W�F�N�g�ւ̃|�C���^

	//�I�u�W�F�N�g�^�C�v�̃��X�g
	static CScene* m_apTopObjType[(int)OBJ_TYPE::ENUM_MAX];	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CScene* m_apCurObjType[(int)OBJ_TYPE::ENUM_MAX];	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CScene* m_pPrevObjtype;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene* m_pNextObjtype;		//���̃I�u�W�F�N�g�ւ̃|�C���^
	
	//�X�V���̃��X�g
	static CScene* m_apTopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CScene* m_apCurUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CScene* m_pPrevUpdate;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene* m_pNextUpdate;		//���̃I�u�W�F�N�g�ւ̃|�C���^

	//�`�揇�̃��X�g
	static CScene* m_apTopDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CScene* m_apCurDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CScene* m_pPrevDraw;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene* m_pNextDraw;		//���̃I�u�W�F�N�g�ւ̃|�C���^
};

#endif // !_SCENE_H_