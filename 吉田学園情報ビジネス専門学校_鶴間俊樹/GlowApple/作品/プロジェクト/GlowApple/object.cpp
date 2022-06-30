//=============================================================================
//
// �I�u�W�F�N�g���� [object.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "object.h"
#include "objectList.h"

#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
bool CObject::m_abStopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CObject* CObject::m_pTopAll = nullptr;
CObject* CObject::m_pCurAll = nullptr;
CObject* CObject::m_apTopObjType[(int)OBJ_TYPE::ENUM_MAX] = {};
CObject* CObject::m_apCurObjType[(int)OBJ_TYPE::ENUM_MAX] = {};
CObject* CObject::m_apTopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CObject* CObject::m_apCurUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CObject* CObject::m_apTopDraw[(int)DRAW_PRIORITY::ENUM_MAX] = {};
CObject* CObject::m_apCurDraw[(int)DRAW_PRIORITY::ENUM_MAX] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CObject::CObject()
{
	m_bDeath = false;
	m_objType = OBJ_TYPE::NONE;
	m_updatePriority = UPDATE_PRIORITY::DEFAULT;
	m_drawPriority = DRAW_PRIORITY::DEFAULT;
	m_texType = CTexture::TEXTURE_TYPE::NONE;
	m_bDraw = true;

	//�S�I�u�W�F�N�g�̃��X�g�̏����ݒ�
	m_pPrevAll = m_pCurAll;	//�O�̃|�C���^�̐ݒ�
	m_pNextAll = nullptr;		//���̃|�C���^�̐ݒ�
	if (m_pTopAll == nullptr) m_pTopAll = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_pCurAll != nullptr) m_pCurAll->m_pNextAll = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_pCurAll = this;		//�Ō���̃|�C���^�̐ݒ�

	//�I�u�W�F�N�g�^�C�v���X�g�̏����ݒ�
	m_pPrevObjtype = m_apCurObjType[(int)m_objType];	//�O�̃|�C���^�̐ݒ�
	m_pNextObjtype = nullptr;						//���̃|�C���^�̐ݒ�
	if (m_apTopObjType[(int)m_objType] == nullptr) m_apTopObjType[(int)m_objType] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurObjType[(int)m_objType] != nullptr) m_apCurObjType[(int)m_objType]->m_pNextObjtype = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurObjType[(int)m_objType] = this;		//�Ō���̃|�C���^�̐ݒ�

	//�X�V���̃��X�g�̏����ݒ�
	m_pPrevUpdate = m_apCurUpdate[(int)m_updatePriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextUpdate = nullptr;								//���̃|�C���^�̐ݒ�
	if (m_apTopUpdate[(int)m_updatePriority] == nullptr) m_apTopUpdate[(int)m_updatePriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] != nullptr) m_apCurUpdate[(int)m_updatePriority]->m_pNextUpdate = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurUpdate[(int)m_updatePriority] = this;			//�Ō���̃|�C���^�̐ݒ�

	//�`�揇�̃��X�g�̏����ݒ�
	m_pPrevDraw = m_apCurDraw[(int)m_drawPriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextDraw = nullptr;							//���̃|�C���^�̐ݒ�
	if (m_apTopDraw[(int)m_drawPriority] == nullptr) m_apTopDraw[(int)m_drawPriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] != nullptr) m_apCurDraw[(int)m_drawPriority]->m_pNextDraw = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurDraw[(int)m_drawPriority] = this;			//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject::~CObject()
{
	//�S���X�g�̍X�V
	//�S�I�u�W�F�N�g���X�g
	if (m_pTopAll == this) m_pTopAll = m_pNextAll;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_pCurAll == this) m_pCurAll = m_pPrevAll;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevAll != nullptr) m_pPrevAll->m_pNextAll = m_pNextAll;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextAll != nullptr) m_pNextAll->m_pPrevAll = m_pPrevAll;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�I�u�W�F�N�g�^�C�v���X�g
	if (m_apTopObjType[(int)m_objType] == this) m_apTopObjType[(int)m_objType] = m_pNextObjtype;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurObjType[(int)m_objType] == this) m_apCurObjType[(int)m_objType] = m_pPrevObjtype;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevObjtype != nullptr) m_pPrevObjtype->m_pNextObjtype = m_pNextObjtype;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextObjtype != nullptr) m_pNextObjtype->m_pPrevObjtype = m_pPrevObjtype;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�X�V�����X�g
	if (m_apTopUpdate[(int)m_updatePriority] == this) m_apTopUpdate[(int)m_updatePriority] = m_pNextUpdate;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] == this) m_apCurUpdate[(int)m_updatePriority] = m_pPrevUpdate;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevUpdate != nullptr) m_pPrevUpdate->m_pNextUpdate = m_pNextUpdate;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextUpdate != nullptr) m_pNextUpdate->m_pPrevUpdate = m_pPrevUpdate;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�`�揇���X�g
	if (m_apTopDraw[(int)m_drawPriority] == this) m_apTopDraw[(int)m_drawPriority] = m_pNextDraw;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] == this) m_apCurDraw[(int)m_drawPriority] = m_pPrevDraw;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevDraw != nullptr) m_pPrevDraw->m_pNextDraw = m_pNextDraw;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextDraw != nullptr) m_pNextDraw->m_pPrevDraw = m_pPrevDraw;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����
}

//=============================================================================
// �S�I�u�W�F�N�g�̃��X�g��Top���擾
//=============================================================================
CObject* CObject::GetObjectTopAll(void) {
	return m_pTopAll;
}

//=============================================================================
// �S�I�u�W�F�N�g�̃��X�g��Next���擾
//=============================================================================
CObject* CObject::GetObjectNextAll(CObject* pObject) {
	if (pObject != nullptr) {
		return pObject->m_pNextAll;
	}
	return nullptr;
}

//=============================================================================
// �I�u�W�F�N�g�^�C�v�̃��X�g��Top���擾
//=============================================================================
CObject* CObject::GetObjectTopObjtype(OBJ_TYPE objType) {
	return m_apTopObjType[(int)objType];
}

//=============================================================================
// �I�u�W�F�N�g�^�C�v�̃��X�g��Next���擾
//=============================================================================
CObject* CObject::GetObjectNextObjtype(CObject* pObject) {
	if (pObject != nullptr) {
		return pObject->m_pNextObjtype;
	}
	return nullptr;
}

//=============================================================================
// �S�I�u�W�F�N�g�̉������
//=============================================================================
//���j�������I�u�W�F�N�g��ʂ̃N���X�Ń|�C���^��ێ����Ă����ꍇ�G���[���N���邽�ߒ���
void CObject::ReleaseAll(void) {
	CObject* pObjectAll = m_pTopAll;	//�S�I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pObjectAll != nullptr)
	{
		CObject* pObjectNext = pObjectAll->m_pNextAll;
		//�I���������s��
		if (!pObjectAll->m_bDeath) pObjectAll->Uninit();
		//���̃I�u�W�F�N�g����
		pObjectAll = pObjectNext;
	}
}

//=============================================================================
// �w�肵���^�C�v�̃I�u�W�F�N�g�̉������
//=============================================================================
//���j�������I�u�W�F�N�g��ʂ̃N���X�Ń|�C���^��ێ����Ă����ꍇ�G���[���N���邽�ߒ���
void CObject::ReleaseObjtype(OBJ_TYPE objtype) {
	CObject* pObjectObjtype = m_apTopObjType[(int)objtype];	//�w�肵���^�C�v�̃I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pObjectObjtype != nullptr)
	{
		CObject* pObjectNext = pObjectObjtype->m_pNextObjtype;
		//�I���������s��
		pObjectObjtype->Uninit();
		//���̃I�u�W�F�N�g����
		pObjectObjtype = pObjectNext;
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�̍X�V����
//=============================================================================
void CObject::UpdateAll(void) {
	for (int nCnt = 0; nCnt < (int)UPDATE_PRIORITY::ENUM_MAX; nCnt++) {
		//���̃I�u�W�F�N�g�^�C�v����~��Ԃ̏ꍇ���[�v���΂�
		if (m_abStopUpdate[nCnt]) continue;

		CObject* pObjectUpdate = m_apTopUpdate[nCnt];	//�X�V����I�u�W�F�N�g
		while (pObjectUpdate != nullptr) {
			CObject* pObjectNext = pObjectUpdate->m_pNextUpdate;
			//���S�t���O�������Ă��Ȃ��ꍇ�X�V
			if (!pObjectUpdate->m_bDeath) pObjectUpdate->Update();	
			pObjectUpdate = pObjectNext;
		}
	}
	//���S�t���O���������I�u�W�F�N�g��j��
	CObject* pObjectAll = m_pTopAll;	//�S�I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pObjectAll != nullptr)
	{
		CObject* pObjectNext = pObjectAll->m_pNextAll;
		//���S���Ă����ꍇ�j��
		if (pObjectAll->m_bDeath) {
			//�I�u�W�F�N�g���X�g�N���X�̃��X�g�̃m�[�h�ɔj�������I�u�W�F�N�g���܂܂�Ă���ꍇ�A���̃m�[�h��j������
			CObjectList::ReleaseDeleteNode(pObjectAll);
			//�I�u�W�F�N�g�̔j��
			delete pObjectAll;	
		}
		pObjectAll = pObjectNext;
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CObject::DrawAll(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer= pManager->GetRenderer();
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	for (int nCnt = 0; nCnt < (int)DRAW_PRIORITY::ENUM_MAX; nCnt++) {
		//UI�̕`��ɐ؂�ւ�����ꍇ�AZ�o�b�t�@��1.0f�ŃN���A����
		if(nCnt == (int)DRAW_PRIORITY::UI_BG && pDevice != nullptr) pDevice->Clear(0, nullptr, (D3DCLEAR_ZBUFFER), D3DXCOLOR(), 1.0f, 0);

		CObject* pObject = m_apTopDraw[nCnt];
		while (pObject != nullptr) {
			CObject* pObjectNext = pObject->m_pNextDraw;	//���̃I�u�W�F�N�g�擾
			//�`��\���̏ꍇ�`��
			if(pObject->m_bDraw && !pObject->m_bDeath) pObject->Draw();
			//���̃I�u�W�F�N�g����
			pObject = pObjectNext;
		}
	}
}

//=============================================================================
// �w�肵���^�C�v�̎��S����
//=============================================================================
void CObject::DeadObjtype(OBJ_TYPE objtype) {
	CObject* pObjectObjtype = m_apTopObjType[(int)objtype];	//�w�肵���^�C�v�̃I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pObjectObjtype != nullptr)
	{
		CObject* pObjectNext = pObjectObjtype->m_pNextObjtype;
		//���S�������s��
		if (!pObjectObjtype->m_bDeath) pObjectObjtype->Dead();
		//���̃I�u�W�F�N�g����
		pObjectObjtype = pObjectNext;
	}
}

//=============================================================================
// �X�V��~���邷�ׂĂ̍X�V���̐ݒ�
//=============================================================================
void CObject::SetStopUpdateAll(bool bFlag) {
	for (int nCnt = 0; nCnt < (int)UPDATE_PRIORITY::ENUM_MAX; nCnt++)
	{
		m_abStopUpdate[nCnt] = bFlag;
	}
}

//=============================================================================
// �I�u�W�F�N�g�̉������
//=============================================================================
void CObject::Release(void) {
	m_bDeath = true;	//���S�t���O�𗧂Ă�
}

//=============================================================================
// ���S�t���O�̎擾
//=============================================================================
bool CObject::GetDeath(void) {
	return m_bDeath;
}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ̐ݒ�
//=============================================================================
void CObject::SetObjType(OBJ_TYPE objType) {
	//�ύX�O�̃I�u�W�F�N�g�^�C�v���X�g�̐ݒ�
	if (m_apTopObjType[(int)m_objType] == this) m_apTopObjType[(int)m_objType] = m_pNextObjtype;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurObjType[(int)m_objType] == this) m_apCurObjType[(int)m_objType] = m_pPrevObjtype;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevObjtype != nullptr) m_pPrevObjtype->m_pNextObjtype = m_pNextObjtype;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextObjtype != nullptr) m_pNextObjtype->m_pPrevObjtype = m_pPrevObjtype;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�I�u�W�F�N�g�^�C�v�̃��X�g��ύX
	m_objType = objType;
	m_pPrevObjtype = m_apCurObjType[(int)m_objType];	//�O�̃|�C���^�̐ݒ�
	m_pNextObjtype = nullptr;						//���̃|�C���^�̐ݒ�
	if (m_apTopObjType[(int)m_objType] == nullptr) m_apTopObjType[(int)m_objType] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurObjType[(int)m_objType] != nullptr) m_apCurObjType[(int)m_objType]->m_pNextObjtype = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurObjType[(int)m_objType] = this;			//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ̎擾
//=============================================================================
CObject::OBJ_TYPE CObject::GetObjType(void) {
	return m_objType;
}

//=============================================================================
// �X�V�̗D�揇�ʂ̐ݒ�
//=============================================================================
void CObject::SetUpdatePriority(UPDATE_PRIORITY priority) {
	//�ύX�O�̍X�V�D�揇�̃I�u�W�F�N�g�̃��X�g�̐ݒ�
	if (m_apTopUpdate[(int)m_updatePriority] == this) m_apTopUpdate[(int)m_updatePriority] = m_pNextUpdate;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] == this) m_apCurUpdate[(int)m_updatePriority] = m_pPrevUpdate;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevUpdate != nullptr) m_pPrevUpdate->m_pNextUpdate = m_pNextUpdate;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextUpdate != nullptr) m_pNextUpdate->m_pPrevUpdate = m_pPrevUpdate;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�X�V�D�揇�̃��X�g��ύX
	m_updatePriority = priority;
	m_pPrevUpdate = m_apCurUpdate[(int)m_updatePriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextUpdate = nullptr;								//���̃|�C���^�̐ݒ�
	if (m_apTopUpdate[(int)m_updatePriority] == nullptr) m_apTopUpdate[(int)m_updatePriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] != nullptr) m_apCurUpdate[(int)m_updatePriority]->m_pNextUpdate = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurUpdate[(int)m_updatePriority] = this;			//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �`��̗D�揇�ʂ̐ݒ�
//=============================================================================
void CObject::SetDrawPriority(DRAW_PRIORITY priority) {
	//�ύX�O�̕`��D�揇�̃I�u�W�F�N�g�̃��X�g�̐ݒ�
	if (m_apTopDraw[(int)m_drawPriority] == this) m_apTopDraw[(int)m_drawPriority] = m_pNextDraw;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] == this) m_apCurDraw[(int)m_drawPriority] = m_pPrevDraw;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevDraw != nullptr) m_pPrevDraw->m_pNextDraw = m_pNextDraw;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextDraw != nullptr) m_pNextDraw->m_pPrevDraw = m_pPrevDraw;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�`��D�揇�̃��X�g��ύX
	m_drawPriority = priority;	//�`��D�揇�̐ݒ�
	m_pPrevDraw = m_apCurDraw[(int)m_drawPriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextDraw = nullptr;							//���̃|�C���^�̐ݒ�
	if (m_apTopDraw[(int)m_drawPriority] == nullptr) m_apTopDraw[(int)m_drawPriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] != nullptr) m_apCurDraw[(int)m_drawPriority]->m_pNextDraw = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurDraw[(int)m_drawPriority] = this;			//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �e�N�X�`���̐ݒ�
//=============================================================================
void CObject::SetTexType(CTexture::TEXTURE_TYPE type) {
	m_texType = type;
}

//=============================================================================
// �e�N�X�`���̎�ނ̎擾
//=============================================================================
CTexture::TEXTURE_TYPE CObject::GetTexType(void) {
	return m_texType;
}

//=============================================================================
// �����蔻��̗L����Ԃ�ݒ�
//=============================================================================
void CObject::SetEnableCollision(bool bEnable) {
	m_bEnableCollision = bEnable;
}

//=============================================================================
// �����蔻��̗L����Ԃ��擾
//=============================================================================
bool CObject::GetEnableCollision(void) {
	return m_bEnableCollision;
}

//=============================================================================
//���z�֐�
//=============================================================================
void CObject::SetPos(D3DXVECTOR3 pos) {}		//�ʒu���W�̐ݒ�
D3DXVECTOR3 CObject::GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//�ʒu���W�̎擾

D3DXVECTOR3 CObject::GetLastPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	//�Ō�̈ʒu���W�̎擾

D3DXVECTOR3 CObject::GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); };	//�p�x�̎擾

void CObject::SetMove(D3DXVECTOR3 move) {}	//�ړ��ʂ̐ݒ�
D3DXVECTOR3 CObject::GetMove(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f);}		//�ړ��ʂ̎擾

float CObject::GetRadius(void) { return 0.0f; }			//�����蔻��̔��a�̎擾

int CObject::GetNumCollisionParts(void) { return 1; }	//�����蔻�肪����p�[�c�̐��̎擾 ��{�͂P�Ƃ���
void CObject::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosCol, float* const pRadiusCol) {}	//�����蔻��̏��̎擾
void CObject::UpdateMtxWorldAll(void) {}			//�I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X�̍X�V

void CObject::Damage(int nDamage, bool* pDead) {}	//�_���[�W
bool CObject::GetItem(int nTypeItem) { return false; }	//�A�C�e���擾���̏���