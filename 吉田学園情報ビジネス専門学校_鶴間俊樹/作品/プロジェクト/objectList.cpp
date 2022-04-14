//=============================================================================
//
// �I�u�W�F�N�g���X�g���� [objectList.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "objectList.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CObjectList::CObjectList()
{
	m_pTopNode = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObjectList::~CObjectList()
{
	ListNode* pListNode = m_pTopNode;	//�擪�̃m�[�h���擾

	while (pListNode != nullptr)
	{
		//���̃m�[�h��ۑ�
		ListNode* pNextNode = pListNode->pNextNode;
		//�m�[�h�̔j��
		delete pListNode;
		//���̃m�[�h��ݒ�
		pListNode = pNextNode;
	}

	m_pTopNode = nullptr;
}

//=============================================================================
// �I�u�W�F�N�g���X�g�̏���������
//=============================================================================
void CObjectList::Init(void) {
	ListNode* pListNode = m_pTopNode;	//�擪�̃m�[�h���擾

	while (pListNode != nullptr)
	{
		//���̃m�[�h��ۑ�
		ListNode* pNextNode = pListNode->pNextNode;
		//�m�[�h�̔j��
		delete pListNode;
		//���̃m�[�h��ݒ�
		pListNode = pNextNode;
	}

	m_pTopNode = nullptr;
}

//=============================================================================
// �I�u�W�F�N�g���X�g�Ƀm�[�h��ǉ�
//=============================================================================
void CObjectList::AppendNode(CScene* pScene) {
	ListNode* pNewNode = new ListNode;	//�V�����ǉ�����m�[�h
	if (pNewNode != nullptr) {
		pNewNode->pScene = pScene;
		pNewNode->pNextNode = nullptr;
	}

	//�擪�̃m�[�h���ݒ肳��Ă��Ȃ��ꍇ
	if (m_pTopNode == nullptr)
	{
		//�V�����ǉ�����m�[�h��擪�̃m�[�h�Ƃ��Đݒ�
		m_pTopNode = pNewNode;
	}
	//�擪�̃m�[�h�����łɂ���ꍇ
	else
	{
		ListNode* pListNode = m_pTopNode;	//�擪�̃m�[�h���擾

		//���̃m�[�h��null�ł͂Ȃ��ꍇ
		while (pListNode->pNextNode != nullptr)
		{
			//���̃m�[�h���擾
			pListNode = pListNode->pNextNode;
		}

		//���̃m�[�h��null�̂��߁A���̃m�[�h�Ƃ��ĐV�����ǉ�����m�[�h��ݒ�
		if(pListNode != nullptr) pListNode->pNextNode = pNewNode;
	}
}

//=============================================================================
// �I�u�W�F�N�g���X�g�̃m�[�h�̍폜
//=============================================================================
// �����얢�m�F�̂��ߎg�p���͒��Ӂ�
bool CObjectList::DeleteNode(CScene* pScene) {
	if (m_pTopNode == nullptr) return false;	//�擪�̃m�[�h��null�̏ꍇ�I��

	bool bDeleteNode = false;	//�m�[�h���폜�������ǂ���

	//�擪�̃m�[�h����v���Ă���ꍇ
	while (m_pTopNode->pScene == pScene)
	{
		//���̃m�[�h��ۑ�
		ListNode* pNextNode = m_pTopNode->pNextNode;
		//�m�[�h�j���t���O��ݒ�
		bDeleteNode = true;
		//�m�[�h��j��
		delete m_pTopNode;
		//�擪�̃m�[�h�����̃m�[�h�ɐݒ�
		m_pTopNode = pNextNode;
		//�擪�̃m�[�h��null�ɂȂ����ꍇ�I��
		if (m_pTopNode == nullptr) return true;
	}

	ListNode* pNode = m_pTopNode->pNextNode;	//�擪�̃m�[�h�̎��̃m�[�h���擾


	//�����ꂾ��pNode����v���Ă��Ă��j������Ȃ�

	//�擪�̃m�[�h�̎��̃m�[�h��null�̏ꍇ�I��
	if (pNode == nullptr) return bDeleteNode;
	
	//���̃m�[�h��null�ł͂Ȃ��ꍇ
	while (pNode->pNextNode != nullptr)
	{
		//���̃m�[�h�̃I�u�W�F�N�g�ƈ�������v���Ă����ꍇ
		if (pNode->pNextNode->pScene == pScene) {
			//���̃m�[�h�̎��̃m�[�h��ۑ�
			ListNode* pNextNodeNext = pNode->pNextNode->pNextNode;	
			//�m�[�h�̔j��
			delete pNode->pNextNode;
			//�m�[�h�j���t���O��ݒ�
			bDeleteNode = true;
			//pNextNode�����̎��̃m�[�h�ɂ���
			pNode->pNextNode = pNextNodeNext;
		}
		//�m�[�h�����ɐi�߂�
		pNode = pNode->pNextNode;
	}

	return bDeleteNode;
}

//=============================================================================
// �I�u�W�F�N�g���X�g�Ɉ�v����I�u�W�F�N�g�����邩�ǂ���
//=============================================================================
bool CObjectList::MatchObject(CScene* pScene) {
	ListNode* pListNode = m_pTopNode;	//�擪�̃m�[�h���擾

	//�m�[�h��null�ł͂Ȃ��ꍇ
	while (pListNode != nullptr)
	{
		//�I�u�W�F�N�g���X�g�Ɉ�v����I�u�W�F�N�g������ꍇ
		if (pListNode->pScene == pScene) {
			return true;
		}
		//���̃m�[�h�ɂ���
		pListNode = pListNode->pNextNode;
	}

	return false;
}