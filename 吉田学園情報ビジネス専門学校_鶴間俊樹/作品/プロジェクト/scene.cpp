//=============================================================================
//
// �I�u�W�F�N�g���� [scene.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "scene.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CScene* CScene::m_pTopAll = NULL;
CScene* CScene::m_pCurAll = NULL;
CScene* CScene::m_apTopObjType[(int)OBJ_TYPE::ENUM_MAX] = {};
CScene* CScene::m_apCurObjType[(int)OBJ_TYPE::ENUM_MAX] = {};
CScene* CScene::m_apTopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CScene* CScene::m_apCurUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CScene* CScene::m_apTopDraw[(int)DRAW_PRIORITY::ENUM_MAX] = {};
CScene* CScene::m_apCurDraw[(int)DRAW_PRIORITY::ENUM_MAX] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CScene::CScene()
{
	m_bDeath = false;
	m_objType = OBJ_TYPE::NONE;
	m_updatePriority = UPDATE_PRIORITY::DEFAULT;
	m_drawPriority = DRAW_PRIORITY::DEFAULT;

	m_bEnableCollision = true;

	//�S�I�u�W�F�N�g�̃��X�g�̏����ݒ�
	m_pPrevAll = m_pCurAll;	//�O�̃|�C���^�̐ݒ�
	m_pNextAll = NULL;		//���̃|�C���^�̐ݒ�
	if (m_pTopAll == NULL) m_pTopAll = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_pCurAll != NULL) m_pCurAll->m_pNextAll = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_pCurAll = this;		//�Ō���̃|�C���^�̐ݒ�

							//�I�u�W�F�N�g�^�C�v���X�g�̏����ݒ�
	m_pPrevObjtype = m_apCurObjType[(int)m_objType];	//�O�̃|�C���^�̐ݒ�
	m_pNextObjtype = NULL;						//���̃|�C���^�̐ݒ�
	if (m_apTopObjType[(int)m_objType] == NULL) m_apTopObjType[(int)m_objType] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurObjType[(int)m_objType] != NULL) m_apCurObjType[(int)m_objType]->m_pNextObjtype = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurObjType[(int)m_objType] = this;		//�Ō���̃|�C���^�̐ݒ�

											//�X�V���̃��X�g�̏����ݒ�
	m_pPrevUpdate = m_apCurUpdate[(int)m_updatePriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextUpdate = NULL;								//���̃|�C���^�̐ݒ�
	if (m_apTopUpdate[(int)m_updatePriority] == NULL) m_apTopUpdate[(int)m_updatePriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] != NULL) m_apCurUpdate[(int)m_updatePriority]->m_pNextUpdate = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurUpdate[(int)m_updatePriority] = this;			//�Ō���̃|�C���^�̐ݒ�

													//�`�揇�̃��X�g�̏����ݒ�
	m_pPrevDraw = m_apCurDraw[(int)m_drawPriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextDraw = NULL;							//���̃|�C���^�̐ݒ�
	if (m_apTopDraw[(int)m_drawPriority] == NULL) m_apTopDraw[(int)m_drawPriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] != NULL) m_apCurDraw[(int)m_drawPriority]->m_pNextDraw = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurDraw[(int)m_drawPriority] = this;			//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene::~CScene()
{
	//�S���X�g�̍X�V
	//�S�I�u�W�F�N�g���X�g
	if (m_pTopAll == this) m_pTopAll = m_pNextAll;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_pCurAll == this) m_pCurAll = m_pPrevAll;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevAll != NULL) m_pPrevAll->m_pNextAll = m_pNextAll;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextAll != NULL) m_pNextAll->m_pPrevAll = m_pPrevAll;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�I�u�W�F�N�g�^�C�v���X�g
	if (m_apTopObjType[(int)m_objType] == this) m_apTopObjType[(int)m_objType] = m_pNextObjtype;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurObjType[(int)m_objType] == this) m_apCurObjType[(int)m_objType] = m_pPrevObjtype;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevObjtype != NULL) m_pPrevObjtype->m_pNextObjtype = m_pNextObjtype;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextObjtype != NULL) m_pNextObjtype->m_pPrevObjtype = m_pPrevObjtype;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�X�V�����X�g
	if (m_apTopUpdate[(int)m_updatePriority] == this) m_apTopUpdate[(int)m_updatePriority] = m_pNextUpdate;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] == this) m_apCurUpdate[(int)m_updatePriority] = m_pPrevUpdate;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevUpdate != NULL) m_pPrevUpdate->m_pNextUpdate = m_pNextUpdate;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextUpdate != NULL) m_pNextUpdate->m_pPrevUpdate = m_pPrevUpdate;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�`�揇���X�g
	if (m_apTopDraw[(int)m_drawPriority] == this) m_apTopDraw[(int)m_drawPriority] = m_pNextDraw;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] == this) m_apCurDraw[(int)m_drawPriority] = m_pPrevDraw;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevDraw != NULL) m_pPrevDraw->m_pNextDraw = m_pNextDraw;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextDraw != NULL) m_pNextDraw->m_pPrevDraw = m_pPrevDraw;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����
}

//=============================================================================
// �S�I�u�W�F�N�g�̃��X�g��Top���擾
//=============================================================================
CScene* CScene::GetSceneTopAll(void) {
	return m_pTopAll;
}

//=============================================================================
// �S�I�u�W�F�N�g�̃��X�g��Next���擾
//=============================================================================
CScene* CScene::GetSceneNextAll(CScene* pScene) {
	if (pScene != NULL) {
		return pScene->m_pNextAll;
	}
	return NULL;
}

//=============================================================================
// �I�u�W�F�N�g�^�C�v�̃��X�g��Top���擾
//=============================================================================
CScene* CScene::GetSceneTopObjtype(OBJ_TYPE objType) {
	return m_apTopObjType[(int)objType];
}

//=============================================================================
// �I�u�W�F�N�g�^�C�v�̃��X�g��Next���擾
//=============================================================================
CScene* CScene::GetSceneNextObjtype(CScene* pScene) {
	if (pScene != NULL) {
		return pScene->m_pNextObjtype;
	}
	return NULL;
}

//=============================================================================
// �S�I�u�W�F�N�g�̉������
//=============================================================================
void CScene::ReleaseAll(void) {
	CScene* pSceneAll = m_pTopAll;	//�S�I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pSceneAll != NULL)
	{
		CScene* pSceneNext = pSceneAll->m_pNextAll;
		//���S�t���O�������Ă��Ȃ������ꍇ
		if (pSceneAll->m_bDeath == false) {
			pSceneAll->Uninit();	//�I�u�W�F�N�g�̏I������
		}
		delete pSceneAll;	//�I�u�W�F�N�g�̔j��
		pSceneAll = pSceneNext;
	}

	//���X�g�̐擪�ƍŌ������ɂ���
	m_pTopAll = NULL;
	m_pCurAll = NULL;
	for (int nCnt = 0; nCnt < (int)OBJ_TYPE::ENUM_MAX; nCnt++)
	{
		m_apTopObjType[nCnt] = NULL;
		m_apCurObjType[nCnt] = NULL;
	}
	for (int nCnt = 0; nCnt < (int)UPDATE_PRIORITY::ENUM_MAX; nCnt++)
	{
		m_apTopUpdate[nCnt] = NULL;
		m_apCurUpdate[nCnt] = NULL;
	}
	for (int nCnt = 0; nCnt < (int)DRAW_PRIORITY::ENUM_MAX; nCnt++)
	{
		m_apTopDraw[nCnt] = NULL;
		m_apCurDraw[nCnt] = NULL;
	}
}

//=============================================================================
// �w�肵���^�C�v�̃I�u�W�F�N�g�̉������
//=============================================================================
void CScene::ReleaseObjtype(OBJ_TYPE objtype) {
	CScene* pSceneObjtype = m_apTopObjType[(int)objtype];	//�w�肵���^�C�v�̃I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pSceneObjtype != NULL)
	{
		CScene* pSceneNext = pSceneObjtype->m_pNextObjtype;
		//���S�t���O�������Ă��Ȃ������ꍇ
		if (pSceneObjtype->m_bDeath == false) {
			pSceneObjtype->Uninit();	//�I�u�W�F�N�g�̏I������
		}
		delete pSceneObjtype;	//�I�u�W�F�N�g�̔j��
		pSceneObjtype = pSceneNext;
	}

	//���X�g�̐擪�ƍŌ������ɂ���
	m_apTopObjType[(int)objtype] = NULL;
	m_apCurObjType[(int)objtype] = NULL;
}

//=============================================================================
// �S�I�u�W�F�N�g�̍X�V����
//=============================================================================
void CScene::UpdateAll(void) {
	for (int nCnt = 0; nCnt < (int)UPDATE_PRIORITY::ENUM_MAX; nCnt++) {
		CScene* pSceneUpdate = m_apTopUpdate[nCnt];	//�X�V����I�u�W�F�N�g
		while (pSceneUpdate != NULL) {
			CScene* pSceneNext = pSceneUpdate->m_pNextUpdate;
			if (pSceneUpdate->m_bDeath == false) pSceneUpdate->Update();	//���S�t���O�������Ă��Ȃ��ꍇ�X�V
			pSceneUpdate = pSceneNext;
		}
	}
	//���S�t���O���������I�u�W�F�N�g��j��
	CScene* pSceneAll = m_pTopAll;	//�S�I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pSceneAll != NULL)
	{
		CScene* pSceneNext = pSceneAll->m_pNextAll;
		//���S���Ă����ꍇ�j��
		if (pSceneAll->m_bDeath == true) {
			delete pSceneAll;	//�I�u�W�F�N�g�̔j��
		}
		pSceneAll = pSceneNext;
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CScene::DrawAll(void) {
	for (int nCnt = 0; nCnt < (int)DRAW_PRIORITY::ENUM_MAX; nCnt++) {
		CScene* pScene = m_apTopDraw[nCnt];
		while (pScene != NULL) {
			CScene* pSceneNext = pScene->m_pNextDraw;
			pScene->Draw();
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g�̉������
//=============================================================================
void CScene::Release(void) {
	m_bDeath = true;	//���S�t���O�𗧂Ă�
}

//=============================================================================
// ���S�t���O�̎擾
//=============================================================================
bool CScene::GetDeath(void) {
	return m_bDeath;
}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ̐ݒ�
//=============================================================================
void CScene::SetObjType(OBJ_TYPE objType) {
	//�ύX�O�̃I�u�W�F�N�g�^�C�v���X�g�̐ݒ�
	if (m_apTopObjType[(int)m_objType] == this) m_apTopObjType[(int)m_objType] = m_pNextObjtype;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurObjType[(int)m_objType] == this) m_apCurObjType[(int)m_objType] = m_pPrevObjtype;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevObjtype != NULL) m_pPrevObjtype->m_pNextObjtype = m_pNextObjtype;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextObjtype != NULL) m_pNextObjtype->m_pPrevObjtype = m_pPrevObjtype;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�I�u�W�F�N�g�^�C�v�̃��X�g��ύX
	m_objType = objType;
	m_pPrevObjtype = m_apCurObjType[(int)m_objType];	//�O�̃|�C���^�̐ݒ�
	m_pNextObjtype = NULL;						//���̃|�C���^�̐ݒ�
	if (m_apTopObjType[(int)m_objType] == NULL) m_apTopObjType[(int)m_objType] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurObjType[(int)m_objType] != NULL) m_apCurObjType[(int)m_objType]->m_pNextObjtype = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurObjType[(int)m_objType] = this;			//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ̎擾
//=============================================================================
CScene::OBJ_TYPE CScene::GetObjType(void) {
	return m_objType;
}

//=============================================================================
// �X�V�̗D�揇�ʂ̐ݒ�
//=============================================================================
void CScene::SetUpdatePriority(UPDATE_PRIORITY priority) {
	//�ύX�O�̍X�V�D�揇�̃I�u�W�F�N�g�̃��X�g�̐ݒ�
	if (m_apTopUpdate[(int)m_updatePriority] == this) m_apTopUpdate[(int)m_updatePriority] = m_pNextUpdate;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] == this) m_apCurUpdate[(int)m_updatePriority] = m_pPrevUpdate;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevUpdate != NULL) m_pPrevUpdate->m_pNextUpdate = m_pNextUpdate;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextUpdate != NULL) m_pNextUpdate->m_pPrevUpdate = m_pPrevUpdate;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�X�V�D�揇�̃��X�g��ύX
	m_updatePriority = priority;
	m_pPrevUpdate = m_apCurUpdate[(int)m_updatePriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextUpdate = NULL;								//���̃|�C���^�̐ݒ�
	if (m_apTopUpdate[(int)m_updatePriority] == NULL) m_apTopUpdate[(int)m_updatePriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] != NULL) m_apCurUpdate[(int)m_updatePriority]->m_pNextUpdate = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurUpdate[(int)m_updatePriority] = this;			//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �`��̗D�揇�ʂ̐ݒ�
//=============================================================================
void CScene::SetDrawPriority(DRAW_PRIORITY priority) {
	//�ύX�O�̕`��D�揇�̃I�u�W�F�N�g�̃��X�g�̐ݒ�
	if (m_apTopDraw[(int)m_drawPriority] == this) m_apTopDraw[(int)m_drawPriority] = m_pNextDraw;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] == this) m_apCurDraw[(int)m_drawPriority] = m_pPrevDraw;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevDraw != NULL) m_pPrevDraw->m_pNextDraw = m_pNextDraw;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextDraw != NULL) m_pNextDraw->m_pPrevDraw = m_pPrevDraw;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�`��D�揇�̃��X�g��ύX
	m_drawPriority = priority;	//�`��D�揇�̐ݒ�
	m_pPrevDraw = m_apCurDraw[(int)m_drawPriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextDraw = NULL;							//���̃|�C���^�̐ݒ�
	if (m_apTopDraw[(int)m_drawPriority] == NULL) m_apTopDraw[(int)m_drawPriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] != NULL) m_apCurDraw[(int)m_drawPriority]->m_pNextDraw = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurDraw[(int)m_drawPriority] = this;			//�Ō���̃|�C���^�̐ݒ�
}

//�����蔻��̎g�p��Ԃ̐ݒ�
void CScene::SetEnableCollision(bool bUse) {
	m_bEnableCollision = bUse;
}

//�����蔻��̎g�p��Ԃ̎擾
bool CScene::GetEnableCollision(void) {
	return m_bEnableCollision;
}

//�e�N�X�`���̐ݒ�
void CScene::SetTexType(CTexture::TEXTURE_TYPE type) {
	m_texType = type;
}
//�e�N�X�`���̎�ނ̎擾
CTexture::TEXTURE_TYPE CScene::GetTexType(void) {
	return m_texType;
}


//=============================================================================
//���z�֐�
//=============================================================================
void CScene::SetPos(D3DXVECTOR3 pos) {}		//�ʒu���W�̐ݒ�
D3DXVECTOR3 CScene::GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//�ʒu���W�̎擾

D3DXVECTOR3 CScene::GetLastPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	//�Ō�̈ʒu���W�̎擾

void CScene::SetMove(D3DXVECTOR3 move) {}	//�ړ��ʂ̐ݒ�
D3DXVECTOR3 CScene::GetMove(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f);}		//�ړ��ʂ̎擾

float CScene::GetRadius(void) { return 0.0f; }			//�����蔻��̔��a�̎擾

int CScene::GetNumCollisionParts(void) { return 1; }	//�����蔻�肪����p�[�c�̐��̎擾 ��{�͂P�Ƃ���
void CScene::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosCol, float* const pRadiusCol) {}	//�����蔻��̏��̎擾
void CScene::UpdateMtxWorldAll(void) {}			//�I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X�̍X�V

void CScene::Damage(int nDamage, bool* pDead) {}	//�_���[�W
bool CScene::GetItem(int nTypeItem) { return false; }	//�A�C�e���擾���̏���