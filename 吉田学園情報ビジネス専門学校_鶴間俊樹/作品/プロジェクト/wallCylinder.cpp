//=============================================================================
//
// �~���̕ǂ̏��� [wallCylinder.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "wallCylinder.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUM_POLYGON_XZ (100)	//�ǂ�XZ�����̃|���S���̕�����
#define WALL_HEIGHT (5000.0f)	//�ǂ̍���
#define WALL_COLOR (D3DXCOLOR(1.0f, 0.55f, 0.0f, 1.0f))	//�ǂ̐F
#define DISTANCE_COLOR_CHANGE (600.0f)	//�F���ς��n�߂鋗��	�v���C���[����J�����̋����ȏ�̒l�ɂ���
#define DISTANCE_COLOR_MAX (200.0f)		//�F�̕ω����ő�ɂȂ鋗��
#define MAX_COLOR_ALPHA (0.8f)	//�ύX�����F�̃A���t�@�l�̍ő�l

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CWallCylinder::CWallCylinder() : CMeshcylinder()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CWallCylinder::CWallCylinder(D3DXVECTOR3 pos, float fRadius) : CMeshcylinder(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), NUM_POLYGON_XZ, fRadius, 1, WALL_HEIGHT, false, WALL_COLOR)
{
	SetObjType(OBJ_TYPE::WALL);	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	SetDrawPriority(DRAW_PRIORITY::BG);	//�`�揇�̐ݒ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CWallCylinder::~CWallCylinder()
{

}

//=============================================================================
// �~���̕ǂ̐�������
//=============================================================================
CWallCylinder* CWallCylinder::Create(D3DXVECTOR3 pos, float fRadius) {
	CWallCylinder* pWallCylinder;
	pWallCylinder = new CWallCylinder(pos, fRadius);
	if (pWallCylinder != NULL) {
		pWallCylinder->Init();
	}

	return pWallCylinder;
}

//=============================================================================
// �~���̕ǂ̏���������
//=============================================================================
HRESULT CWallCylinder::Init(void) {
	CMeshcylinder::Init();

	return S_OK;
}

//=============================================================================
// �~���̕ǂ̏I������
//=============================================================================
void CWallCylinder::Uninit(void) {
	CMeshcylinder::Uninit();
}

//=============================================================================
// �~���̕ǂ̍X�V����
//=============================================================================
void CWallCylinder::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�Q�[���̎擾
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//�v���C���[�̎擾
	CPlayer* pPlayer = nullptr;
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	if (pPlayer != nullptr) {
		float fRadiusWall = GetRadius();
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();	//�v���C���[�̈ʒu�̎擾
		float fDistance = sqrtf(powf(posPlayer.x - GetPos().x, 2.0f) + powf(posPlayer.z - GetPos().z, 2.0f));	//�v���C���[�̈ʒu�Ɖ~���̒��S�Ƃ̋���
		D3DXCOLOR col = GetColor();	//�F�̎擾

		if (fDistance > fRadiusWall - DISTANCE_COLOR_CHANGE) {
			col.a = (fDistance - (fRadiusWall - DISTANCE_COLOR_CHANGE)) / (fRadiusWall - DISTANCE_COLOR_MAX - (fRadiusWall - DISTANCE_COLOR_CHANGE));
				if (col.a > MAX_COLOR_ALPHA)col.a = MAX_COLOR_ALPHA;
		}
		else {
			col.a = 0.0f;
		}
		//�F�̐ݒ�
		SetColor(col);
	}
	CMeshcylinder::Update();
}

//=============================================================================
// �~���̕ǂ̕`�揈��
//=============================================================================
void CWallCylinder::Draw(void) {
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

	//�J�����O�Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//���C�g�𖳌�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�`��
	CMeshcylinder::Draw();

	//�J�����O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//���C�g��L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �~���̕ǂƂ̓����蔻��
//=============================================================================
bool CWallCylinder::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 lastPos, float fRadius) {
	if (pPos == NULL) return false;

	CScene* pScene;	//�I�u�W�F�N�g�ւ̃|�C���^
	pScene = GetSceneTopObjtype(OBJ_TYPE::WALL);	//�ǂ̃I�u�W�F�N�g�^�C�v�̃��X�g�̐擪���擾

	while (pScene != NULL) {
		CScene* pSceneNext = GetSceneNextObjtype(pScene);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾
		CWallCylinder* pWallCylinder = dynamic_cast<CWallCylinder*>(pScene);	//�~���̕ǃN���X�ɃL���X�g
		//�L���X�g������
		if (pWallCylinder != NULL) {
			//���S�t���O�������Ă��Ȃ��ꍇ
			if (pWallCylinder->GetDeath() == false) {
				float fRadiusWall = pWallCylinder->GetRadius();
				D3DXVECTOR3 posWallCylinder = pWallCylinder->GetPos();
				//�����蔻��
				float fDistance = sqrtf(powf(pPos->x - posWallCylinder.x, 2.0f) + powf(pPos->z - posWallCylinder.z, 2.0f));	//�I�u�W�F�N�g�̈ʒu�Ɖ~���̒��S�Ƃ̋���
				float fLastDistance = sqrtf(powf(lastPos.x - posWallCylinder.x, 2.0f) + powf(lastPos.z - posWallCylinder.z, 2.0f));	//�I�u�W�F�N�g�̑O�̈ʒu�Ɖ~���̒��S�Ƃ̋���
				//�I�u�W�F�N�g�̔��a�𑫂�
				fDistance += fRadius;

				//�͈͊O�ɏo�悤�Ƃ����Ƃ�
				if (fLastDistance <= fRadiusWall && fDistance > fRadiusWall) {
					D3DXVECTOR2 vecNor = D3DXVECTOR2(pPos->x - posWallCylinder.x, pPos->z - posWallCylinder.z);	//�~���̒��S����I�u�W�F�N�g�̈ʒu�ւ̐��K���x�N�g��
					D3DXVec2Normalize(&vecNor, &vecNor);	//���K��

					pPos->x = posWallCylinder.x + vecNor.x * (fRadiusWall - fRadius);
					pPos->z = posWallCylinder.z + vecNor.y * (fRadiusWall - fRadius);
					return true;
				}
			}
		}
		pScene = pSceneNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}
	return false;
}

//=============================================================================
// �ǂƐ����̌�_�i�ǂ̓�������O���ւ̃x�N�g���̂݁j
//=============================================================================
void CWallCylinder::CrossPoint(D3DXVECTOR2* pPosCross, const D3DXVECTOR3* pPosStart, const D3DXVECTOR3* pPosEnd, float fDelta) {
	if (pPosStart == NULL || pPosEnd == NULL) return;

	CScene* pScene;	//�I�u�W�F�N�g�ւ̃|�C���^
	pScene = GetSceneTopObjtype(OBJ_TYPE::WALL);	//�ǂ̃I�u�W�F�N�g�^�C�v�̃��X�g�̐擪���擾

	while (pScene != NULL) {
		CScene* pSceneNext = GetSceneNextObjtype(pScene);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾
		CWallCylinder* pWallCylinder = dynamic_cast<CWallCylinder*>(pScene);	//�~���̕ǃN���X�ɃL���X�g
		//�L���X�g������
		if (pWallCylinder != NULL) {
			//���S�t���O�������Ă��Ȃ��ꍇ
			if (pWallCylinder->GetDeath() == false) {
				float fRadiusWall = pWallCylinder->GetRadius();
				//�ǂ̈ʒu
				D3DXVECTOR3 posWallCylinder = pWallCylinder->GetPos();

				bool bOutStart = (sqrtf(powf(pPosStart->x - posWallCylinder.x, 2.0f) + powf(pPosStart->z - posWallCylinder.z, 2.0f))) > (fRadiusWall - fDelta);	//�����̎n�_���ǂ̊O�����ǂ���
				bool bOutEnd = (sqrtf(powf(pPosEnd->x - posWallCylinder.x, 2.0f) + powf(pPosEnd->z - posWallCylinder.z, 2.0f))) > (fRadiusWall - fDelta);		//�����̏I�_���ǂ̊O�����ǂ���
				
				//�������ǂ̓�������O���̏ꍇ
				if (bOutStart == false && bOutEnd == true) {
					D3DXVECTOR2 vecCenterToStart = D3DXVECTOR2(pPosStart->x - posWallCylinder.x, pPosStart->z - posWallCylinder.z);	//�~�̒��S��������̎n�_�̃x�N�g��
					D3DXVECTOR2 vecNorLine = D3DXVECTOR2(-(pPosEnd->z - pPosStart->z), pPosEnd->x - pPosStart->x);	//�����̖@���x�N�g��
					//�@���x�N�g���𐳋K��
					D3DXVec2Normalize(&vecNorLine, &vecNorLine);

					//�@���x�N�g����vecCenterToStart�����p�̏ꍇ
					if (D3DXVec2Dot(&vecCenterToStart, &vecNorLine) == 0) {
						//�����̃x�N�g�����~�̒��S��ʂ��Ă���̂�vecCenterToStart�𐳋K�����Ĕ��a�̒�����������
						D3DXVec2Normalize(&vecCenterToStart, &vecCenterToStart);
						*pPosCross = D3DXVECTOR2(posWallCylinder.x + vecCenterToStart.x * (fRadiusWall - fDelta), posWallCylinder.z + vecCenterToStart.y * (fRadiusWall - fDelta));
					}
					else {
						//�@���x�N�g����vecCenterToStart�̔��Α��̏ꍇ�x�N�g���𔽓]
						if (D3DXVec2Dot(&vecCenterToStart, &vecNorLine) < 0) {
							vecNorLine *= -1;
						}
						//�~�̒��S���璼���ւ̍ŒZ��������ςŌv�Z
						float fDistanceShort = D3DXVec2Dot(&vecNorLine, &vecCenterToStart);	//�ŒZ����
						//�@���x�N�g���ɍŒZ������������
						vecNorLine *= fDistanceShort;
						//�~�̒��S���璼���ւ̍ŒZ�����̈ʒu
						D3DXVECTOR2 posPointShort = D3DXVECTOR2(posWallCylinder.x + vecNorLine.x, posWallCylinder.z + vecNorLine.y);
						//posPointShort����pPosEnd�ւ̃x�N�g��
						D3DXVECTOR2 vecShortToEnd = D3DXVECTOR2(pPosEnd->x - posPointShort.x, pPosEnd->z - posPointShort.y);
						D3DXVec2Normalize(&vecShortToEnd, &vecShortToEnd);//���K��
						//vecShortToEnd�̒��������߂ĉ~�Ƃ̌�_�����߂�
						float fDistanceCross = sqrtf(powf((fRadiusWall - fDelta), 2.0f) - powf(fDistanceShort, 2.0f));	//posPointShort�����_�ւ̋���
						//��_��ݒ�
						*pPosCross = posPointShort + vecShortToEnd * fDistanceCross;
					}
					return;
				}
			}
		}
		pScene = pSceneNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}
}