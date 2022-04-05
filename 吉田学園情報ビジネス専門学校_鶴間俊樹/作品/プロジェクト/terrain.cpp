//=============================================================================
//
// �n�`���� [terrain.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "terrain.h"
#include "manager.h"
#include "game.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTerrain::CTerrain()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CTerrain::CTerrain(CModel::MODELTYPE typeModel) : CSceneModel(typeModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), nullptr, false)
{
	SetObjType(OBJ_TYPE::TERRAIN);	//�I�u�W�F�N�g�^�C�v�̐ݒ�

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTerrain::~CTerrain()
{

}

//=============================================================================
// �n�`�̐�������
//=============================================================================
CTerrain* CTerrain::Create(TERRAIN_TYPE typeTerrain) {
	CModel::MODELTYPE typeModel = CModel::MODELTYPE::TERRAIN_START;
	switch (typeTerrain)
	{
	case TERRAIN_TYPE::START:
		typeModel = CModel::MODELTYPE::TERRAIN_START;
		break;

	case TERRAIN_TYPE::BOSS:
		typeModel = CModel::MODELTYPE::TERRAIN_BOSS;
		break;
	}

	CTerrain* pTerrain;
	pTerrain = new CTerrain(typeModel);
	if (pTerrain != nullptr) {
		pTerrain->Init();
	}

	return pTerrain;
}

//=============================================================================
// �n�`�̏���������
//=============================================================================
HRESULT CTerrain::Init(void) {
	CSceneModel::Init();

	return S_OK;
}

//=============================================================================
// �n�`�̏I������
//=============================================================================
void CTerrain::Uninit(void) {
	CSceneModel::Uninit();
}

//=============================================================================
// �n�`�̍X�V����
//=============================================================================
void CTerrain::Update(void) {
	CSceneModel::Update();
}

//=============================================================================
// �n�`�̕`�揈��
//=============================================================================
void CTerrain::Draw(void) {
	CSceneModel::Draw();
}

//=============================================================================
// �n�`�Ƃ̏Փ˔���
//=============================================================================
bool CTerrain::Collision(D3DXVECTOR3* pPosCollision, D3DXVECTOR3 vecStart, D3DXVECTOR3 vecEnd) {
	CScene* pScene;	//�I�u�W�F�N�g�ւ̃|�C���^
	pScene = GetSceneTopObjtype(OBJ_TYPE::TERRAIN);	//�n�`�̃I�u�W�F�N�g�^�C�v�̃��X�g�̐擪���擾

	while (pScene != nullptr) {
		CScene* pSceneNext = GetSceneNextObjtype(pScene);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾
		CTerrain* pTerrain = dynamic_cast<CTerrain*>(pScene);	//�n�`�N���X�ɃL���X�g
		//�L���X�g������
		if (pTerrain != nullptr) {
			//���S�t���O�������Ă��Ȃ��ꍇ
			if (pTerrain->GetDeath() == false) {
				//�n�`�̃��f���f�[�^�̎擾
				CModel::MODELTYPE modelType = pTerrain->GetModelType();	//���f���̎�ނ̎擾
				CModel::ModelData modelData = CModel::GetModelData(modelType);	//���f���̃f�[�^�̎擾
				LPD3DXMESH pMesh = modelData.pMesh;	//���b�V���f�[�^�̎擾
				if (pMesh == nullptr) return false;	//���b�V���f�[�^�������̏ꍇ�I��			

				D3DXVECTOR3 posTerrain = pTerrain->GetPos();//�n�`�̈ʒu				
				D3DXVECTOR3 rotTerrain = pTerrain->GetRot();//�n�`�̉�]				

				D3DXMATRIX mtxWorld;	//�n�`�̃��[���h�}�g���b�N�X
				D3DXMATRIX mtxTrans, mtxRot;	//�v�Z�p�}�g���b�N�X
				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxWorld);	
				//�n�`�̉�]���s��ɂ�����
				D3DXMatrixRotationYawPitchRoll(&mtxRot, rotTerrain.y, rotTerrain.x, rotTerrain.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
				//�n�`�̈ʒu���W���s��ɂ�����
				D3DXMatrixTranslation(&mtxTrans, posTerrain.x, posTerrain.y, posTerrain.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

				//�n�`�̍s��̋t�s����v�Z
				D3DXMatrixInverse(&mtxWorld, nullptr, &mtxWorld);
				//���C�ɒn�`�̋t�s���������
				D3DXVec3TransformCoord(&vecStart, &vecStart, &mtxWorld);
				D3DXVec3TransformCoord(&vecEnd, &vecEnd, &mtxWorld);

				D3DXVECTOR3 vecDir = vecEnd - vecStart;	//���C�̕����x�N�g��

				BOOL bHit;		//�Փ˔���t���O
				DWORD faceIdx;	//�������Ɉ�ԋ߂��ʂ̃C���f�b�N�X
				float fU, fV;	//���������O�p�`�̏d�S���W?

				//�����蔻��
				D3DXIntersect(pMesh, &vecStart, &vecDir, &bHit, &faceIdx, &fU, &fV, nullptr, nullptr, nullptr);

				//���C�Փˎ�
				if (bHit == TRUE) {
					//-------------------------------------------------
					//���肪�������O�p�`�̒��_�̃C���f�b�N�X�̎擾
					//-------------------------------------------------
					int nIdx[3];	//�O�p�`�̒��_�̃C���f�b�N�X
					WORD *pIdx;		//�C���f�b�N�X���ւ̃|�C���^
					//�C���f�b�N�X�o�b�t�@�̃��b�N
					pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIdx);
					for (int nCnt = 0; nCnt < 3; nCnt++) {
						nIdx[nCnt] = pIdx[(faceIdx * 3) + nCnt];
					}

					//�C���f�b�N�X�o�b�t�@�̃A�����b�N
					pMesh->UnlockIndexBuffer();

					//-------------------------------------------------
					//���肪�������O�p�`�̒��_�̈ʒu�̎擾
					//-------------------------------------------------
					D3DXVECTOR3 posVtx[3];	//�O�p�`�̒��_�̈ʒu���W

					DWORD dwStride = pMesh->GetNumBytesPerVertex();	//���_���Ƃ̃o�C�g��

					BYTE* pbVtx = nullptr;
					float* pfVtx = nullptr;

					//���_�o�b�t�@�̃��b�N
					pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pbVtx);
				
					for (int nCntPoly = 0; nCntPoly < 3; nCntPoly++) {
						//�C���f�b�N�X�ɉ��������_���W���擾
						pfVtx = (float*) &pbVtx[dwStride * nIdx[nCntPoly]];

						posVtx[nCntPoly].x = pfVtx[0];
						posVtx[nCntPoly].y = pfVtx[1];
						posVtx[nCntPoly].z = pfVtx[2];
					}

					//���_�o�b�t�@�̃A�����b�N
					pMesh->UnlockVertexBuffer();

					D3DXVECTOR3 posCol = D3DXVECTOR3(posVtx[0] + fU * (posVtx[1] - posVtx[0]) + fV * (posVtx[2] - posVtx[0]));	//�n�`�̃��[�J�����W�ł̏Փˈʒu

					//�n�`�Ƃ̏Փˈʒu�����[���h���W�ɕϊ�
					D3DXMATRIX mtxWorld;	//�n�`�̃��[���h�}�g���b�N�X
					D3DXMATRIX mtxTrans, mtxRot;	//�v�Z�p�}�g���b�N�X
					//���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&mtxWorld);
					//�n�`�̉�]���s��ɂ�����
					D3DXMatrixRotationYawPitchRoll(&mtxRot, rotTerrain.y, rotTerrain.x, rotTerrain.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
					//�n�`�̈ʒu���W���s��ɂ�����
					D3DXMatrixTranslation(&mtxTrans, posTerrain.x, posTerrain.y, posTerrain.z);
					D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
					//�ʒu�Ƀ}�g���b�N�X�𔽉f
					D3DXVec3TransformCoord(&posCol, &posCol, &mtxWorld);
					//�Փ˂����I�u�W�F�N�g�̈ʒu��ݒ�
					*pPosCollision = posCol;
					return true;
				}
			}
		}
		pScene = pSceneNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}

	return false;
}