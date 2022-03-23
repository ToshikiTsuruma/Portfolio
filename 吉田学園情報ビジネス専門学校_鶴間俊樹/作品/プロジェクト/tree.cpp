//=============================================================================
//
// �؂̏��� [tree.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "tree.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "effect.h"
#include "terrain.h"
#include "item_apple.h"
#include "billboard.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_tree.txt"
#define NUM_COLLISION (16)	//�����蔻��̐�
#define COLLISION_RADIUS (8.0f)	//�����蔻��̔��a
#define MAX_LIFE (6)	//�̗͂̍ő�l
#define LIFE_GAUGE_HEIGHT (100.0f)	//�G�̈ʒu����̗̑̓Q�[�W�̍���
#define DEAD_TIME (120)	//���S�������܂ł̎���
#define DRAW_DISTANCE (4500.0f)			//�`�悪�\�ɂȂ�n�߂�v���C���[�Ƃ̋���
#define MAX_DRAW_DISTANCE (4300.0f)		//�`��\�ȍő�̃v���C���[�Ƃ̋���

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CTree::m_nNumParts = 0;
CSceneMotion::PARTS_INFO* CTree::m_pPartsInfoArray = NULL;
CSceneMotion::MOTION_INFO CTree::m_aMotionInfo[(int)CTree::MOTION_TYPE::ENUM_MAX] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTree::CTree() : CSceneMotion(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, false)
{
	SetObjType(OBJ_TYPE::TREE);	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	SetUpdatePriority(UPDATE_PRIORITY::ENEMY);

	m_nLife = MAX_LIFE;
	m_nCntDrawGaugeLife = 0;

	//�̗̓Q�[�W�̐���
	m_pGaugeLifeFrame = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 75.0f, 15.0f);
	m_pGaugeLife = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 70.0f, 10.0f);

	if (m_pGaugeLifeFrame != NULL) {
		m_pGaugeLifeFrame->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)); //�̗̓Q�[�W�̐F�̐ݒ�
		m_pGaugeLifeFrame->SetDraw(false);	//�`�悵�Ȃ��悤�ɂ���
	}
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->SetColor(D3DXCOLOR(0.3f, 1.0f, 0.0f, 1.0f)); //�̗̓Q�[�W�̐F�̐ݒ�
		m_pGaugeLife->SetDraw(false);	//�`�悵�Ȃ��悤�ɂ���
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTree::~CTree()
{

}

//=============================================================================
// ���S�~��Ƀ����_���Ɋp�x���炵�Ė؂�z�u����
//=============================================================================
//�����F���S�̈ʒu�@�~�̐��@�~���Ƃ̋����i���a�j�@�i�������̈�O�̖؂Ƃ́j�ŒZ�����A�Œ�����
void CTree::SetTreeCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fDistCircle, int nMinDist, int nMaxDist) {
	int nCntCircle = 0;	//�쐬�����؂̉~�̐�
	float fRotCreate = 0.0f;	//�z�u����p�x
	float fRandStartRot = (rand() % 314) * 0.01f;	//�z�u�̊J�n�p�x�̈ʒu�������_�� �O�����ߑa�C���ɂȂ邽��

	while (nCntCircle < nNumCircle)
	{
		int nRandDist = rand() % (nMaxDist + 1 - nMinDist) + nMinDist;	//��O�̖؂Ƃ̋���
		fRotCreate += nRandDist / ((nCntCircle + 1) * fDistCircle * D3DX_PI) * D3DX_PI;	//�����_���ȋ��� / �~���̒����̔��� * pi �Ń��W�A�������߂�

		//����z�u�����ꍇ
		if (fRotCreate >= D3DX_PI * 2.0f) {
			nCntCircle++;	//�J�E���g�̉��Z
			fRotCreate = 0.0f;	//�z�u����p�x�̃��Z�b�g
			fRandStartRot = (rand() % 314) * 0.01f;	//�z�u����p�x�̃����_���l���Đݒ�
		}
		else {
			//�؂𐶐�����ʒu�̐ݒ�
			D3DXVECTOR3 posCreate = posCenter;	//�z�u����ʒu
			posCreate.x = sinf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			posCreate.z = cosf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			//�؂̐���
			CTree::Create(posCreate);
		}
	}
}

//=============================================================================
// �؂̐�������
//=============================================================================
CTree* CTree::Create(D3DXVECTOR3 pos) {
	CTree* pTree;
	pTree = new CTree();
	if (pTree != NULL) {
		pTree->CSceneMotion::SetPos(pos);
		pTree->Init();
	}

	return pTree;
}

//=============================================================================
// ���[�V�����������[�h
//=============================================================================
void CTree::LoadMotionInfo(void) {
	//���[�h����O�ɓ��I�m�ۂ�����������delete����
	UnloadMotionInfo();

	Load(TEXT_FILE_NAME_LOAD_MOTION, &m_pPartsInfoArray, &m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, NULL);
}

//=============================================================================
// ���[�V���������A�����[�h
//=============================================================================
void CTree::UnloadMotionInfo(void) {
	m_nNumParts = 0;

	//�p�[�c���̉��
	if (m_pPartsInfoArray != NULL) {
		delete[] m_pPartsInfoArray;
		m_pPartsInfoArray = NULL;
	}
	//���[�V�������̉��
	for (int nCntMotionInfo = 0; nCntMotionInfo < (int)MOTION_TYPE::ENUM_MAX; nCntMotionInfo++)
	{
		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
		{
			if (m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray != NULL) {
				delete[] m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray;
				m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray = NULL;
			}
		}
	}
}

//=============================================================================
// �؂̏���������
//=============================================================================
HRESULT CTree::Init(void) {
	//CSceneMotion::Init();	//���[�V�������I��������ԂŎn�܂邽�ߖ���

	//�A�j���[�V�����̕����������_���ɂ������̂Ŋp�x�������_���ɐݒ�
	int nRandRot = rand() % 63; //0����62
	D3DXVECTOR3 rot = GetRot();
	rot.y = (float)nRandRot / 10.0f;	//0.0 ����@6.2
	//�p�x�̐ݒ�
	SetRot(rot);

	//�n�`�Ƃ̓����蔻��i�n�\�Ɉʒu�����킹�邽�߁j
	bool bLand = false;	//�ڒn���Ă��邩�ǂ���
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//�ڐG�ʒu�A�J�n�x�N�g���A�I���x�N�g��
	vecStart = GetPos();	//�v���C���[�̈ʒu���擾
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//������̃x�N�g��

	bLand = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);

	if (bLand == true) {
		SetPos(posColTerrain);
	}

	//����������`�悵�Ȃ��ꍇ������̂Ń��[���h�}�g���b�N�X���X�V����
	UpdateMtxWorldAll();

	return S_OK;
}

//=============================================================================
// �؂̏I������
//=============================================================================
void CTree::Uninit(void) {
	//�̗̓Q�[�W�̔j��
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = NULL;
	}
	//�̗̓Q�[�W�̘g�̔j��
	if (m_pGaugeLifeFrame != NULL) {
		m_pGaugeLifeFrame->Uninit();
		m_pGaugeLifeFrame = NULL;
	}
	//�I������
	CSceneMotion::Uninit();
}

//=============================================================================
// �؂̍X�V����
//=============================================================================
void CTree::Update(void) {
	//�T�E���h�̎擾
	CManager* pManager = nullptr;
	pManager = CManager::GetManager();
	CSound *pSound = nullptr;
	if(pManager != nullptr) pSound = pManager->GetSound();

	if (m_bDead == true) {
		m_nCntDead++;

		//�؂̔j��
		if (m_nCntDead > DEAD_TIME) {
			//�A�C�e���̐���
			D3DXVECTOR3 posItem = GetPos();
			CItemApple::Create(posItem);
			//���ŃG�t�F�N�g�̐���
			CEffect::Create(GetPos(), CEffect::EFFECT_TYPE::EXPLOSION, 50.0f, 50.0f);
			//���ŉ��̍Đ�
			if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::EXPLOSION);

			//�I������
			Uninit();
			return;
		}
		CSceneMotion::Update();
		return;
	}

	//�̗̓Q�[�W�̕`�掞�Ԃ�����
	if (m_nCntDrawGaugeLife > 0) {
		m_nCntDrawGaugeLife--;
		//�`�掞�Ԃ��Ȃ��Ȃ����ꍇ
		if (m_nCntDrawGaugeLife <= 0) {
			//�`�悵�Ȃ�����
			if (m_pGaugeLife != NULL) m_pGaugeLife->SetDraw(false);
			if (m_pGaugeLifeFrame != NULL) m_pGaugeLifeFrame->SetDraw(false);
		}
		else {
			//�ʒu���X�V
			if (m_pGaugeLife != NULL) m_pGaugeLife->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + LIFE_GAUGE_HEIGHT, GetPos().z));
			if (m_pGaugeLifeFrame != NULL) m_pGaugeLifeFrame->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + LIFE_GAUGE_HEIGHT, GetPos().z));
		}
	}
}

//=============================================================================
// �؂̕`�揈��
//=============================================================================
void CTree::Draw(void) {
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
		CSceneMotion::Draw();
	}
}

//=============================================================================
// �؂̓����蔻��̏��̎擾
//=============================================================================
void CTree::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//�����蔻��̐���ݒ�
	if (pNumCol != NULL) *pNumCol = NUM_COLLISION;
	//�����蔻��̔��a��ݒ�
	if (pRadiusCol != NULL) *pRadiusCol = COLLISION_RADIUS;
	//�����蔻��̈ʒu�̔z���ݒ�
	if (ppPosColArray != NULL) {
		//pos�̃|�C���^���󂾂����ꍇ
		if (*ppPosColArray == NULL) {
			//pos�̕ϐ��𓖂��蔻��̐����z��Ŋm��
			*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];

			D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���f����pos����̃I�t�Z�b�g�̎n�_
			int nIdxModel = 0;	//�؂̊�

			//�����蔻��̈ʒu�̎擾
			for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
			{
				GetPosCollision(*ppPosColArray + nCntCol, posOffset, nIdxModel);
				posOffset += D3DXVECTOR3(0.0f, 5.0f, 0.0f);	//�I�t�Z�b�g�����Z
			}
		}
	}
}

//=============================================================================
// �؂̃_���[�W����
//=============================================================================
void CTree::Damage(int nDamage, bool* pDead) {
	if (m_bDead == true) return;

	//�T�E���h�̎擾
	CManager* pManager = nullptr;
	pManager = CManager::GetManager();
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//�_���[�W
	m_nLife -= nDamage;

	//���S��
	if (m_nLife <= 0) {
		m_bDead = true;
		if (pDead != NULL) *pDead = true;
		//���S���[�V�����ݒ�
		SetMotion((int)MOTION_TYPE::BREAK);
		//�|�󉹂̍Đ�
		if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::BREAK_TREE);
		//���S�J�E���g�̏�����
		m_nCntDead = 0;
		//�����蔻��𖳌�
		SetEnableCollision(false);
		//�̗̓Q�[�W�̔j��
		if (m_pGaugeLife != NULL) {
			m_pGaugeLife->Uninit();
			m_pGaugeLife = NULL;
		}
		//�̗̓Q�[�W�̘g�̔j��
		if (m_pGaugeLifeFrame != NULL) {
			m_pGaugeLifeFrame->Uninit();
			m_pGaugeLifeFrame = NULL;
		}
	}
	else {
		*pDead = false;
		//�̗̓Q�[�W�̕`�掞�Ԃ̐ݒ�
		m_nCntDrawGaugeLife = 300;
		if (m_pGaugeLife != NULL) {
			//�̗̓Q�[�W�̕���ς���
			m_pGaugeLife->SetRatioWidth((float)m_nLife / (float)MAX_LIFE);
			//�`�悳����
			m_pGaugeLife->SetDraw(true);
		}
		if (m_pGaugeLifeFrame != NULL) {
			//�`�悳����
			m_pGaugeLifeFrame->SetDraw(true);
		}
	}
}