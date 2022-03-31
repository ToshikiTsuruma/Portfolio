//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "item.h"
#include "manager.h"
#include "game.h"
#include "player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define ROTATE_SPEED (0.03f)	//��]���x
#define DRAW_DISTANCE (1800.0f)			//�`�悪�\�ɂȂ�n�߂�v���C���[�Ƃ̋���
#define MAX_DRAW_DISTANCE (2000.0f)		//�`��\�ȍő�̃v���C���[�Ƃ̋���

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CItem::CItem()
{
	m_bDraw = true;
}

//=============================================================================
//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CItem::CItem(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine) : CSceneModel(typeModel, pos, rot, NULL, bOutLine)
{
	SetObjType(OBJ_TYPE::ITEM);	//�I�u�W�F�N�g�^�C�v�̐ݒ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CItem::~CItem()
{

}

//=============================================================================
// �A�C�e���̏���������
//=============================================================================
HRESULT CItem::Init(void) {
	CSceneModel::Init();

	return S_OK;
}

//=============================================================================
// �A�C�e���̏I������
//=============================================================================
void CItem::Uninit(void) {
	CSceneModel::Uninit();
}

//=============================================================================
// �A�C�e���̍X�V����
//=============================================================================
void CItem::Update(void) {
	CSceneModel::Update();
	//��]
	SetRot(D3DXVECTOR3(0.0f, GetRot().y + ROTATE_SPEED, 0.0f));
}

//=============================================================================
// �A�C�e���̕`�揈��
//=============================================================================
void CItem::Draw(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�Q�[���̎擾
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//�v���C���[�̎擾
	CPlayer* pPlayer = nullptr;
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	//�v���C���[�̈ʒu���擾
	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pPlayer != NULL) posPlayer = pPlayer->GetPos();
	//�v���C���[�Ƃ̋��������߂�
	D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);
	float fDistPlayer = D3DXVec2Length(&vecPlayer);
	//�`��̐ݒ�
	if (m_bDraw == true) {
		//�`��O�̈ʒu�ɂ�����`�悵�Ȃ��悤�ɂ���
		if (fDistPlayer > MAX_DRAW_DISTANCE) {
			m_bDraw = false;
		}
	}
	else {
		//�`����̈ʒu�ɂ�����`��ł���悤�ɂ���
		if (fDistPlayer < DRAW_DISTANCE) {
			m_bDraw = true;
		}
	}

	//�`��
	if (m_bDraw == true) {
		CSceneModel::Draw();
	}
}

//=============================================================================
// �A�C�e���Ƃ̏Փ˔���
//=============================================================================
void CItem::Collision(CScene* pSceneCol) {
	if (pSceneCol == NULL) return;

	D3DXVECTOR3 posCol = pSceneCol->GetPos();			//�����蔻�肷��I�u�W�F�N�g�̈ʒu���W
	float radiusCol = pSceneCol->GetRadius();			//�����蔻�肷��I�u�W�F�N�g�̔��a

	CScene* pScene;	//�I�u�W�F�N�g�ւ̃|�C���^
	pScene = GetSceneTopObjtype(OBJ_TYPE::ITEM);	//�A�C�e���̃I�u�W�F�N�g�^�C�v�̃��X�g�̐擪���擾

	while (pScene != NULL) {
		CScene* pSceneNext = GetSceneNextObjtype(pScene);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾
		CItem* pItem = dynamic_cast<CItem*>(pScene);	//�A�C�e���N���X�ɃL���X�g
		//�L���X�g������
		if (pItem != NULL) {
			//���S�t���O�������Ă��Ȃ��ꍇ
			if (pItem->GetDeath() == false) {
				D3DXVECTOR3 posItem = pItem->GetPos();	//�A�C�e���̈ʒu�̎擾
				float fRadiusItem = pItem->GetRadius();	//�A�C�e���̔��a�̎擾
				posItem.y += fRadiusItem;	//�A�C�e���̓��[�J�����W�Œ�ӂ�0�Ȃ̂Œ����ɗ���悤�ɒ�������

				D3DXVECTOR3 posColCenter = posCol;
				posColCenter.y += radiusCol;	//�Փ˃I�u�W�F�N�g�̈ʒu�������ɍ��킹��

				D3DXVECTOR3 vecItem = posItem - posColCenter;	//�Փ˃I�u�W�F�N�g����A�C�e���ւ̃x�N�g��
				float fDistCol = D3DXVec3Length(&vecItem);	//�A�C�e���̒��S�ƏՓ˃I�u�W�F�N�g�̒��S�̋��������߂�
				//�����蔻��
				if (fDistCol < radiusCol + fRadiusItem) {
					//�A�C�e�����擾���ꂽ�Ƃ��̏���
					pItem->CollisionObject(pSceneCol);
				}
			}
		}
		pScene = pSceneNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}
}