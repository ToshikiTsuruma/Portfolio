//=============================================================================
//
// �I�u�W�F�N�g���� [object.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define MAX_OBJECT (1024)
#define MAX_OBJECT_NAME (32)
//*****************************************************************************
//�O���錾
//*****************************************************************************

//*****************************************************************************
// �I�u�W�F�N�g�N���X
//*****************************************************************************
class CObject
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
		APPLE_TREE,	//�ь�̖�
		OTHER,		//�}�b�v�ؑ֎��ɔj�������悤�Ȃ���
		ENUM_MAX
	};	

	//�X�V���i�X�V��~�ł��K�v�ȏꍇ�͒�`�j
	enum class UPDATE_PRIORITY {
		DEFAULT = 0,	//�X�V�����e�����Ȃ�����
		MENU_UPDATE,	//���j���[�N���X�������ɍX�V����~����Ȃ��I�u�W�F�N�g
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
		BILLBOARD_UI,//UI�̃r���{�[�h
		UI_BG,		//UI�̔w�i
		UI,			//UI
		FRONT,		//�őO
		ENUM_MAX
	};	//�`�揇

	CObject();			//�f�t�H���g�R���X�g���N�^
	virtual ~CObject();	//�f�X�g���N�^
	virtual HRESULT Init(void) = 0;	//����������
	virtual void Uninit(void) = 0;	//�I������
	virtual void Update(void) = 0;	//�X�V����
	virtual void Draw(void) = 0;	//�`�揈��

	virtual void SetPos(D3DXVECTOR3 pos);	//�ʒu���W�̐ݒ�
	virtual D3DXVECTOR3 GetPos(void);		//�ʒu���W�̎擾
	virtual D3DXVECTOR3 GetLastPos(void);	//�Ō�̈ʒu���W�̎擾
	virtual D3DXVECTOR3 GetRot(void);		//�p�x�̎擾
	virtual void SetMove(D3DXVECTOR3 move);	//�ړ��ʂ̐ݒ�
	virtual D3DXVECTOR3 GetMove(void);		//�ړ��ʂ̎擾
	virtual float GetRadius(void);			//�����蔻��̔��a�̎擾
						
	virtual int GetNumCollisionParts(void);	//�����蔻�肪����p�[�c�̐��̎擾
	virtual void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosCol, float* const pRadiusCol);	//�����蔻��̏��̎擾
	virtual void UpdateMtxWorldAll(void);			//�I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X�̍X�V
	virtual void Damage(int nDamage, bool* pDead);	//�_���[�W
	virtual void Dead(void) {}						//���S����
	virtual bool GetItem(int nTypeItem);	//�A�C�e���擾���̏���
	virtual void SetRatioWidth(float fRatio) {}	//���̊����̐ݒ�
	virtual void SetColor(D3DXCOLOR col) {};	//�F�̐ݒ�

	static void ReleaseAll(void);	//�S�I�u�W�F�N�g�̉������
	static void UpdateAll(void);	//�S�I�u�W�F�N�g�̍X�V����
	static void DrawAll(void);		//�S�I�u�W�F�N�g�̕`�揈��
	static CObject* GetObjectTopAll(void);	//�S�I�u�W�F�N�g�̃��X�g��Top���擾
	static CObject* GetObjectNextAll(CObject* pObject);	//�S�I�u�W�F�N�g�̃��X�g��Next���擾
	static CObject* GetObjectTopObjtype(OBJ_TYPE objType);	//�I�u�W�F�N�g�^�C�v�̃��X�g��Top���擾
	static CObject* GetObjectNextObjtype(CObject* pObject);	//�I�u�W�F�N�g�^�C�v�̃��X�g�̈�����Next���擾
	static void ReleaseObjtype(OBJ_TYPE objtype);	//�w�肵���^�C�v�̃I�u�W�F�N�g�̉������
	static void DeadObjtype(OBJ_TYPE objtype);		//�w�肵���^�C�v�̎��S����

	static void SetStopUpdateAll(bool bFlag);	//�X�V��~���邷�ׂĂ̍X�V���̐ݒ�
	static void SetStopUpdate(UPDATE_PRIORITY update, bool bFlag) { m_abStopUpdate[(int)update] = bFlag; }	//�X�V��~����X�V���̐ݒ�

	bool GetDeath(void);	//���S�t���O�̎擾
	void SetObjType(OBJ_TYPE objType);	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	OBJ_TYPE GetObjType(void);			//�I�u�W�F�N�g�̎�ނ̎擾
	void SetUpdatePriority(UPDATE_PRIORITY priority);	//�X�V�̗D�揇�ʂ̐ݒ�
	void SetDrawPriority(DRAW_PRIORITY priority);		//�`��̗D�揇�ʂ̐ݒ�

	void SetTexType(CTexture::TEXTURE_TYPE type);	//�e�N�X�`���̎�ނ̐ݒ�
	CTexture::TEXTURE_TYPE GetTexType(void);		//�e�N�X�`���̎�ނ̎擾
	void SetEnableCollision(bool bEnable);	//�����蔻��̗L����Ԃ�ݒ�
	bool GetEnableCollision(void);			//�����蔻��̗L����Ԃ��擾
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }	//�`���Ԃ̐ݒ�
	bool GetDraw(void) { return m_bDraw; }			//�`���Ԃ̎擾

protected:
	void Release(void);	//�I�u�W�F�N�g�̉������

private:
	bool m_bDeath;		//���S�t���O
	OBJ_TYPE m_objType;	//�I�u�W�F�N�g�̎��
	UPDATE_PRIORITY m_updatePriority;	//�X�V�̗D�揇��
	DRAW_PRIORITY m_drawPriority;		//�`��̗D�揇��

	CTexture::TEXTURE_TYPE m_texType;	//�e�N�X�`���̃^�C�v
	bool m_bEnableCollision;	//�����蔻�肪�L��
	bool m_bDraw;	//�`�悷�邩�ǂ���

	static bool m_abStopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//�X�V��~����I�u�W�F�N�g�^�C�v

	//�S�I�u�W�F�N�g�̃��X�g
	static CObject* m_pTopAll;	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_pCurAll;	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pPrevAll;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNextAll;		//���̃I�u�W�F�N�g�ւ̃|�C���^

	//�I�u�W�F�N�g�^�C�v�̃��X�g
	static CObject* m_apTopObjType[(int)OBJ_TYPE::ENUM_MAX];	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_apCurObjType[(int)OBJ_TYPE::ENUM_MAX];	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pPrevObjtype;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNextObjtype;		//���̃I�u�W�F�N�g�ւ̃|�C���^
	
	//�X�V���̃��X�g
	static CObject* m_apTopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_apCurUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pPrevUpdate;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNextUpdate;		//���̃I�u�W�F�N�g�ւ̃|�C���^

	//�`�揇�̃��X�g
	static CObject* m_apTopDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_apCurDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pPrevDraw;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNextDraw;		//���̃I�u�W�F�N�g�ւ̃|�C���^
};

#endif // !_OBJECT_H_