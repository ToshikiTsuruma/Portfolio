//=============================================================================
//
// �e�N�X�`������ [texture.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_FILE_NAME_TEXTURE "data/TEXT/load_texture.txt"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::m_apTexture[(int)TEXTURE_TYPE::ENUM_MAX] = {};
char CTexture::m_aFilePath[(int)TEXTURE_TYPE::ENUM_MAX][MAX_LOAD_TEXT_TEXTURE] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTexture::CTexture()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h
//=============================================================================
HRESULT CTexture::Load(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return S_OK;

	//-------------------------------------
	//�t�@�C���̓ǂݍ���
	//-------------------------------------
	FILE *pFile;		//�t�@�C���ւ̃|�C���^
	char sLoadText[MAX_LOAD_TEXT_TEXTURE];//��s���ǂݍ��ރt�@�C���̃e�L�X�g
	char *pLoadText;	//�t�@�C���̃e�L�X�g�𕪊�����������
	int nIdxType = 1;	//���ɓǂݍ��ރe�N�X�`���̎��	0��NONE�����邽��1����

	//�t�@�C�����J��
	pFile = fopen(TEXT_FILE_NAME_TEXTURE, "r");

	if (pFile != nullptr) {
		while (fgets(sLoadText, MAX_LOAD_TEXT_TEXTURE, pFile) != nullptr) //��s���Ƃɕ�������擾
		{
			pLoadText = strtok(sLoadText, " =\t\n");	//������̕����i�� �^�u ���s ���j
			if (pLoadText != nullptr) {
				//�R�����g
				if (strstr(pLoadText, "//") != nullptr) {
					continue;
				}
				//�f�B���N�g�����̃R�s�[
				strcpy(m_aFilePath[nIdxType], pLoadText);
				nIdxType++;	//�C���f�b�N�X�����Z
				if (nIdxType >= (int)TEXTURE_TYPE::ENUM_MAX) 	break;	//�ǂݍ��ރe�N�X�`���̍ő吔�𒴂�����I��	
			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
	else {
		//�t�@�C���̓ǂݍ��ݎ��s��
		return S_OK;
	}

	//0��NONE�����邽��1����
	for (int nCnt = 1; nCnt < (int)TEXTURE_TYPE::ENUM_MAX; nCnt++)
	{
		//���łɃe�N�X�`������������Ă����ꍇ�j��
		if (m_apTexture[nCnt] != nullptr) {
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}

		//�e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice,
			m_aFilePath[nCnt],	// �e�N�X�`���p�X
			&m_apTexture[nCnt]);
	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̃A�����[�h
//=============================================================================
void CTexture::Unload(void) {
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < (int)TEXTURE_TYPE::ENUM_MAX; nCnt++)
	{
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}

//=============================================================================
// �e�N�X�`���̎擾
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE_TYPE type) {
	return m_apTexture[(int)type];
}

//=============================================================================
// �e�N�X�`���̃p�X�̎擾
//=============================================================================
char* CTexture::GetPathName(TEXTURE_TYPE type) {
	return m_aFilePath[(int)type];
}