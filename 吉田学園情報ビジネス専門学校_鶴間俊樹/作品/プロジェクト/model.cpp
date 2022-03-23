//=============================================================================
//
// ���f������ [model.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_FILE_NAME_MODEL "data/TEXT/load_model.txt"
#define OUTLINE_SIZE (1.0f)	//�֊s�̃T�C�Y

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CModel::ModelData CModel::m_aModelData[(int)MODELTYPE::ENUM_MAX] = {};
char CModel::m_aFilePath[(int)MODELTYPE::ENUM_MAX][MAX_LOAD_TEXT_MODEL] = {};
D3DXMATERIAL CModel::m_aMatDefault[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL] = {};
CTexture::TEXTURE_TYPE CModel::m_aTexType[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	m_modelType = (MODELTYPE) 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pParent = NULL;
	m_nIdxParent = -1;
	m_bOutline = false;
	m_pCloneMesh = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// ���f���̐�������
//=============================================================================
CModel* CModel::Create(MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine) {
	CModel* pModel;
	pModel = new CModel;
	if (pModel != NULL) {
		pModel->m_modelType = type;
		pModel->m_pos = pos;
		pModel->m_rot = rot;
		pModel->m_pParent = pParent;
		pModel->m_bOutline = bOutLine;
		//�}�e���A���̐ݒ�
		for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
		{
			pModel->m_aMat[nCntMat] = m_aMatDefault[(int)type][nCntMat];
		}

		pModel->Init();
	}

	return pModel;
}

//=============================================================================
//���f���f�[�^�̓ǂݍ���
//=============================================================================
HRESULT CModel::Load(void) {
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
	char sModelText[MAX_LOAD_TEXT_MODEL];//��s���ǂݍ��ރt�@�C���̃e�L�X�g
	char *pModelText;	//�t�@�C���̃e�L�X�g�𕪊�����������
	int nIdxType = 0;	//���ɓǂݍ��ރ��f���̎��

	pFile = fopen(TEXT_FILE_NAME_MODEL, "r");

	if (pFile != NULL) {
		while (fgets(sModelText, MAX_LOAD_TEXT_MODEL, pFile) != NULL) //��s���Ƃɕ�������擾
		{
			pModelText = strtok(sModelText, " =\t\n");	//������̕����i�� �^�u ���s ���j
			if (pModelText != NULL) {
				//�R�����g
				if (strstr(pModelText, "//") != NULL) {
					continue;
				}
				//�f�B���N�g�����̃R�s�[
				strcpy(m_aFilePath[nIdxType], pModelText);
				nIdxType++;	//�C���f�b�N�X�����Z
				if (nIdxType >= (int)MODELTYPE::ENUM_MAX) 	break;	//�ǂݍ��ރ��f���̍ő吔�𒴂�����I��	
			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
	else {
		//�t�@�C���̓ǂݍ��ݎ��s��
		return S_OK;
	}

	//-------------------------------------
	//���f���̓ǂݍ���
	//-------------------------------------
	for (int nCntModel = 0; nCntModel < (int)MODELTYPE::ENUM_MAX; nCntModel++) {
		//���łɃ��f������������Ă����ꍇ
		//�}�e���A���o�b�t�@�̉��
		if (m_aModelData[nCntModel].pBuffMat != NULL) {
			m_aModelData[nCntModel].pBuffMat->Release();
			m_aModelData[nCntModel].pBuffMat = NULL;
		}
		//���b�V���̉��
		if (m_aModelData[nCntModel].pMesh != NULL) {
			m_aModelData[nCntModel].pMesh->Release();
			m_aModelData[nCntModel].pMesh = NULL;
		}

		//���f���̐���
		D3DXLoadMeshFromX(m_aFilePath[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,		//�����܂ŌŒ�
			&m_aModelData[nCntModel].pBuffMat,
			NULL,
			&m_aModelData[nCntModel].nNumMat,
			&m_aModelData[nCntModel].pMesh);

		//�e�N�X�`���^�C�v�̐ݒ�	
		if (m_aModelData[nCntModel].pBuffMat == NULL || m_aModelData[nCntModel].nNumMat <= 0) continue;	//�}�e���A���o�b�t�@��NULL���A�}�e���A������0�ȉ��Ȃ�ȍ~�̏������΂�

		D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^
		pMat = (D3DXMATERIAL*)m_aModelData[nCntModel].pBuffMat->GetBufferPointer();	//�}�e���A�����ɑ΂���|�C���^���擾

		for (int nCntMat = 0; nCntMat < (int)m_aModelData[nCntModel].nNumMat; nCntMat++) {
			//�f�t�H���g�̃}�e���A�����擾
			m_aMatDefault[nCntModel][nCntMat] = pMat[nCntMat];

			if (pMat[nCntMat].pTextureFilename != NULL) {
				//�e�N�X�`���N���X�̃p�X�Ɣ�r
				for (int nCntTex = 1; nCntTex < (int)CTexture::TEXTURE_TYPE::ENUM_MAX; nCntTex++) {
					//�����񂪈�v�Ńe�N�X�`���̔ԍ������߂�
					if (strcmp(pMat[nCntMat].pTextureFilename, CTexture::GetPathName((CTexture::TEXTURE_TYPE)nCntTex)) == 0) {
						m_aTexType[nCntModel][nCntMat] = (CTexture::TEXTURE_TYPE)nCntTex;
						break;
					}
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
//���f���f�[�^�̉��
//=============================================================================
void CModel::Unload(void) {
	for (int nCnt = 0; nCnt < (int)MODELTYPE::ENUM_MAX; nCnt++) {
		//�}�e���A���o�b�t�@�̉��
		if (m_aModelData[nCnt].pBuffMat != NULL) {
			m_aModelData[nCnt].pBuffMat->Release();
			m_aModelData[nCnt].pBuffMat = NULL;
		}
		//���b�V���̉��
		if (m_aModelData[nCnt].pMesh != NULL) {
			m_aModelData[nCnt].pMesh->Release();
			m_aModelData[nCnt].pMesh = NULL;
		}
	}
}

//=============================================================================
// ���f���f�[�^�̎擾
//=============================================================================
CModel::ModelData CModel::GetModelData(MODELTYPE type) {
	return m_aModelData[(int)type];
}

//=============================================================================
// ���f���̃p�X�̎擾
//=============================================================================
char* CModel::GetPathName(MODELTYPE type) {
	return m_aFilePath[(int)type];
}
//=============================================================================
// ���f���̏���������
//=============================================================================
HRESULT CModel::Init(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	//�֊s������ꍇ
	if (m_bOutline == true && pDevice != nullptr) {
		//���b�V���̕���
		if (m_aModelData[(int)m_modelType].pMesh != NULL) {
			//���ʂ̕������Ɩ@���̎擾���@��m��Ȃ��̂�FVF���w�肵�ĕ���
			m_aModelData[(int)m_modelType].pMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX_3D, pDevice, &m_pCloneMesh);
		}

		if (m_pCloneMesh != NULL) {
			//���_���̎擾
			int nNumVtx = 0;
			nNumVtx = m_pCloneMesh->GetNumVertices();

			VERTEX_3D *pVtx;	//FVF��ύX���ĕ��������̂ł���ɍ��킹���\���̂̒��_�o�b�t�@�̃|�C���^

			//���_�o�b�t�@�̃��b�N
			m_pCloneMesh->LockVertexBuffer(0, (void**)&pVtx);

			for (int nCnt = 0; nCnt < nNumVtx; nCnt++) {
				//�S���_��@�������֖c��
				pVtx[nCnt].pos = D3DXVECTOR3(pVtx[nCnt].pos.x + pVtx[nCnt].nor.x * OUTLINE_SIZE,
					pVtx[nCnt].pos.y + pVtx[nCnt].nor.y * OUTLINE_SIZE,
					pVtx[nCnt].pos.z + pVtx[nCnt].nor.z * OUTLINE_SIZE);

				//�����̎d���̂������}�e���A�������f����Ȃ��̂ł����ŐF���w��
				pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			//���_�o�b�t�@�̃A�����b�N
			m_pCloneMesh->UnlockVertexBuffer();
		}
	}

	return S_OK;
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CModel::Uninit(void) {
	//�������b�V���̔j��
	if (m_pCloneMesh != NULL) {
		m_pCloneMesh->Release();
		m_pCloneMesh = NULL;
	}
}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CModel::Update(void) {

}

//=============================================================================
// ���f���̕`�揈��
//=============================================================================
void CModel::Draw(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return;

	//���f���̏�񂪐������m�ۂ���Ă��Ȃ��ꍇ�`��I��
	if (m_aModelData[(int)m_modelType].pBuffMat == NULL || m_aModelData[(int)m_modelType].pMesh == NULL || m_aModelData[(int)m_modelType].nNumMat <= 0)return;

	//----------------------------------------------------
	//�}�g���b�N�X�̐ݒ�
	//----------------------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;			//�e�̃}�g���b�N�X

	//�e�p�[�c�̐e�̃}�g���b�N�X��ݒ�
	if (m_pParent != NULL) {
		mtxParent = m_pParent->GetMtxWorld();	//�e�̃��f������}�g���b�N�X���擾
	}
	else {
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	//�Ō�ɃZ�b�g�������[���h�}�g���b�N�X��e�̃}�g���b�N�X�Ƃ��Đݒ�
	}

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//�Z�o�������[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//----------------------------------------------------
	//���f���̕`��
	//----------------------------------------------------
	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�֊s�̕`��
	if (m_bOutline == true && m_pCloneMesh != NULL) {
		//����
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		//���C�g�𖳌�
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		/* �F�͒��_�Őݒ�
		D3DMATERIAL9 matOutline;	//�֊s�p�}�e���A��
		//�}�e���A�����̐ݒ�
		matOutline.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&matOutline);
		*/

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		for (int nCntMat = 0; nCntMat < (int)m_aModelData[(int)m_modelType].nNumMat; nCntMat++) {
			//���f���i�p�[�c�j�̕`��	
			m_pCloneMesh->DrawSubset(nCntMat);
		}

		// �\��
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	
		//���C�g��L��
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	//���f���̕`��
	if (m_aModelData[(int)m_modelType].pMesh != NULL) {
		for (int nCntMat = 0; nCntMat < (int)m_aModelData[(int)m_modelType].nNumMat; nCntMat++) {
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&m_aMat[nCntMat].MatD3D);
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, CTexture::GetTexture(m_aTexType[(int)m_modelType][nCntMat]));
			//���f���i�p�[�c�j�̕`��	
			m_aModelData[(int)m_modelType].pMesh->DrawSubset(nCntMat);
		}
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// ���f���̈ʒu���̐ݒ�
//=============================================================================
void CModel::SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

//=============================================================================
// ���f���̈ʒu���̎擾
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void) { return m_pos; }

//=============================================================================
// ���f���̊p�x�̐ݒ�
//=============================================================================
void CModel::SetRot(D3DXVECTOR3 rot) { m_rot = rot; }

//=============================================================================
// ���f���̊p�x�̎擾
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void) { return m_rot; }

//=============================================================================
// ���[���h�}�g���b�N�X�̍X�V
//=============================================================================
void CModel::UpdateMtxWorld(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return;

	//----------------------------------------------------
	//�}�g���b�N�X�̐ݒ�
	//----------------------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;			//�e�̃}�g���b�N�X

	//�e�p�[�c�̐e�̃}�g���b�N�X��ݒ�
	if (m_pParent != NULL) {
		mtxParent = m_pParent->GetMtxWorld();	//�e�̃��f������}�g���b�N�X���擾
	}
	else {
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	//�Ō�ɃZ�b�g�������[���h�}�g���b�N�X��e�̃}�g���b�N�X�Ƃ��Đݒ�
	}

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//�Z�o�������[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=============================================================================
//���[���h�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX CModel::GetMtxWorld(void) {
	return m_mtxWorld;
}

//=============================================================================
// ���[���h�}�g���b�N�X�̃|�C���^�̎擾
//=============================================================================
D3DXMATRIX* CModel::GetPtrMtxWorld(void) {
	return &m_mtxWorld;
}

//=============================================================================
// ���f���̃^�C�v�̎擾
//=============================================================================
CModel::MODELTYPE CModel::GetModelType(void) {
	return m_modelType;
}

//=============================================================================
// �}�e���A���̃A���t�@�l��ݒ�
//=============================================================================
void CModel::SetMaterialAlpha(float fAlpha) {
	for (int nCnt = 0; nCnt < MAX_MATERIAL; nCnt++)
	{
		m_aMat[nCnt].MatD3D.Diffuse.a = fAlpha;
	}
}

//=============================================================================
// �}�e���A���̃A���t�@�l���擾
//=============================================================================
float CModel::GetMaterialAlpha(void) {
	return m_aMat[0].MatD3D.Diffuse.a;
}

//=============================================================================
// �֊s�̐F�̎w��
//=============================================================================
void CModel::SetColorOutline(D3DXCOLOR col) {
	if (m_pCloneMesh == NULL) return;
	//���_���̎擾
	int nNumVtx = 0;
	nNumVtx = m_pCloneMesh->GetNumVertices();

	VERTEX_3D *pVtx;	//FVF��ύX���ĕ��������̂ł���ɍ��킹���\���̂̒��_�o�b�t�@�̃|�C���^

						//���_�o�b�t�@�̃��b�N
	m_pCloneMesh->LockVertexBuffer(0, (void**)&pVtx);

	for (int nCnt = 0; nCnt < nNumVtx; nCnt++) {
		pVtx[nCnt].col = col;
	}

	//���_�o�b�t�@�̃A�����b�N
	m_pCloneMesh->UnlockVertexBuffer();
}

//=============================================================================
// �֊s�̕\���ݒ�
//=============================================================================
void CModel::SetDrawOutline(bool bDraw) {
	m_bOutline = bDraw;
	//���f���������ɗ֊s�𐶐����Ă��Ȃ��ꍇ�͐V���ɍ��K�v����
}