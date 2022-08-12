//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "sound.h"
#include "gameScene.h"
#include "player.h"
#include "terrain.h"
#include "effect.h"
#include "billboard.h"
#include "particleEffect.h"
#include "gauge3D.h"
#include "appleTree.h"
#include "apple_black.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define FALLDOWN_TIME (35)	//���S���̓|��鎞��
#define DEAD_TIME (120)	//���S�������܂ł̎���

#define GOLD_RATE (7)	//���̓G�ɂȂ�m��
#define DEFAULT_GROW_VALUE (1)	//�f�t�H���g�̓G���S���̗ь�̖ؐ�����
#define GOLD_GROW_VALUE (10)		//���̓G���S���̗ь�̖ؐ�����

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
bool CEnemy::m_bGoldRush = false;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy() : m_fMoveSpeed(0.0f), m_fRotateSpeed(0.0f), m_nMaxLife(0), m_fDistAttack(0.0f), m_fHeightLifeGauge(0.0f)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, float fMoveSpeed, float fRotSpeed, int nLife, float fDistAttack, float fHeightLifeGauge)
	: CObjectMotion(pPartsInfoArray, nNumParts, pMotionInfoArray, nNumTypeMotion, false), m_fMoveSpeed(fMoveSpeed), m_fRotateSpeed(fRotSpeed), m_nMaxLife(nLife), m_fDistAttack(fDistAttack), m_fHeightLifeGauge(fHeightLifeGauge)
{
	SetUpdatePriority(UPDATE_PRIORITY::ENEMY);	//�X�V���̐ݒ�
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//�`�揇�̐ݒ�
	SetObjType(OBJTYPE_ENEMY);	//�I�u�W�F�N�g�^�C�v�̐ݒ�

	SetColorOutlineAll(D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f));	//�֊s�̐F�̐ݒ�
	SetColorGlowAll(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	//�֊s�̔����F�̐ݒ�

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDestY = 0.0f;
	m_nLife = nLife;
	m_bDead = false;
	m_nCntDead = 0;
	m_bLand = false;
	m_bAttackDist = false;
	m_bGoldEnemy = false;

	//�̗̓Q�[�W�̐���
	m_pGaugeLife = CGauge3D::Create(m_nMaxLife, false, m_nLife, 600, false);
	//�Q�[�W�̔w�i�̐ݒ�
	m_pGaugeLife->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55.0f, 7.0f);
	m_pGaugeLife->SetGaugeBGColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	//�Q�[�W�̐ݒ�
	m_pGaugeLife->CreateGauge(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, 4.0f);
	m_pGaugeLife->SetGaugeColor(D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
	//�Q�[�W���\��
	m_pGaugeLife->SetGaugeDraw(false);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// �G�l�~�[�̏���������
//=============================================================================
HRESULT CEnemy::Init(void) {
	//�����蔻�肠��
	SetEnableCollision(true);

	//�����_���ŋ��F�̓G
	int nRandGold = rand() % GOLD_RATE;
	if (nRandGold == 0 || m_bGoldRush) {
		m_bGoldEnemy = true;
		SetDiffuseModelAll(D3DXCOLOR(0.7f, 0.5f, 0.0f, 1.0f), 0);
		SetSpecularModelAll(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f), 0);
	}

	//���[�V�����I�u�W�F�N�g�̏���������
	CObjectMotion::Init();
	return S_OK;
}

//=============================================================================
// �G�l�~�[�̏I������
//=============================================================================
void CEnemy::Uninit(void) {
	//�̗̓Q�[�W�̔j��
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}
	//�I������
	CObjectMotion::Uninit();
}

//=============================================================================
// �G�l�~�[�̍X�V����
//=============================================================================
void CEnemy::Update(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//----------------------------
	//���S��
	//----------------------------
	if (m_bDead) {
		m_nCntDead++;

		//�d��
		SetPos(GetPos() + D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f));

		//�n�`�Ƃ̓����蔻��
		bool bLand = false;	//�ڒn���Ă��邩�ǂ���
		D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//�ڐG�ʒu�A�J�n�x�N�g���A�I���x�N�g��
		vecStart = GetPos();	//�G�̈ʒu���擾
		vecEnd = vecStart;
		vecEnd.y += 1.0f;	//������̃x�N�g��

		bLand = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);
		//�ڒn��
		if (bLand) {
			SetPos(posColTerrain);	//�ʒu�̈ړ�
		}

		//�|��鎞��
		if (m_nCntDead == FALLDOWN_TIME) {
			//�|��鉹
			if (pSound != nullptr)pSound->PlaySound(CSound::SOUND_LABEL::STUN);

			//�p�[�e�B�N���G�t�F�N�g�̐���
			D3DXVECTOR3 posParticle;	//�p�[�e�B�N���̈ʒu
			GetPosCollision(&posParticle, D3DXVECTOR3(0.0f, 20.0f, 0.0f), 0);	//���̂̈ʒu�̎擾

			//�p�[�e�B�N���G�t�F�N�g�̍ŏ��̐F
			D3DXCOLOR colEffectStart = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			if (m_bGoldEnemy) colEffectStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

			//�p�[�e�B�N���̏��
			CParticleEffect::PARTICLE_INFO particleInfo = { 60, 25.0f, -0.25f, 2.0f, D3DXVECTOR3(0.0f, -0.01f, 0.0f), colEffectStart, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) };
			//�G�t�F�N�g�̐���
			CParticleEffect::Create(particleInfo, posParticle, DEAD_TIME - FALLDOWN_TIME, 10, 0.05f * D3DX_PI, true);
		}

		//�G�l�~�[�̔j��
		if (m_nCntDead > DEAD_TIME) {
			//�t�F�[�h�A�E�g
			bool bEndFade = false;
			bEndFade = FadeModelAll(0.0f, -0.01f);
			//�t�F�[�h�I����
			if (bEndFade) {
				//�I������
				Uninit();
				return;
			}
		}

		//���[�V�����̍X�V
		CObjectMotion::Update();
		return;
	}


	//----------------------------
	//��]
	//----------------------------
	D3DXVECTOR3 rot = GetRot();	//�p�x�̎擾
	
	//�ڕW�p�x�Ɗp�x���s��v�̏ꍇ
	if (m_rotDestY != GetRot().y) {	//GetMotionCategory() != MOTION_CATEGORY::ATTACK && 

		//�ڕW�p�x�։�]������
		float rotY = rot.y;	//Y���̊p�x

		//�ڕW�p�x���}�C�i�X�A�p�x���v���X�̂Ƃ��ɁA���Z�������ʂ�-�΂𒴂����ꍇ
		if (m_rotDestY - rotY < -D3DX_PI) {
			//���Z
			rot.y += m_fRotateSpeed;
		}
		//�ڕW�p�x���v���X�A�p�x���}�C�i�X�̂Ƃ��ɁA���Z�������ʂ��΂𒴂����ꍇ
		else if (m_rotDestY - rotY > D3DX_PI) {
			//���Z
			rot.y -= m_fRotateSpeed;
		}
		//�X�̐����͊֌W�Ȃ��ꍇ
		else if (m_rotDestY - rotY > 0) {
			//���Z
			rot.y += m_fRotateSpeed;
			//�ڕW�p�x���ߎ��̒���
			if (rot.y > m_rotDestY) {
				rot.y = m_rotDestY;
			}
		}
		else if (m_rotDestY - rotY < 0) {
			//���Z
			rot.y -= m_fRotateSpeed;
			//�ڕW�p�x���ߎ��̒���
			if (rot.y < m_rotDestY) {
				rot.y = m_rotDestY;
			}
		}


		//�Β��ߎ��̒���
		if (rot.y > D3DX_PI) {
			rot.y -= D3DX_PI * 2;
		}
		else if (rot.y < -D3DX_PI) {
			rot.y += D3DX_PI * 2;
		}

		//�p�x�̐ݒ�
		SetRot(rot);
	}

	//----------------------------
	//�ړ�
	//----------------------------
	//�ړ�
	if (m_bMove) {
		m_move.x = sinf(rot.y + D3DX_PI) * m_fMoveSpeed;
		m_move.z = cosf(rot.y + D3DX_PI) * m_fMoveSpeed;
	}

	//�ʒu�̐ݒ�
	SetPos(GetPos() + m_move);

	//�U���\�ȋ������ǂ����v�Z
	AttackDistance();

	//----------------------------
	//�����蔻��
	//----------------------------
	//�n�ʂƂ̓����蔻��
	bool bLand = false;	//�ڒn���Ă��邩�ǂ���
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//�ڐG�ʒu�A�J�n�x�N�g���A�I���x�N�g��
	vecStart = GetPos();	//�G�̈ʒu���擾
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//������̃x�N�g��

	bLand = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);
	//�ڒn��
	if (bLand) {
		m_bLand = true;
		m_move.y = -POWER_GRAVITY_GROUND;
		SetPos(posColTerrain);	//�ʒu�̈ړ�

		//�؂̕��֖ڕW�p�x��ݒ�
		SetRotTree();

		//���p�t�G���ړ����[�V�����Ȃ��ɂ��Ă���̂ňړ��s���ɐڒn���Ă���Έړ����[�V����
		if (!m_bMove && GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
			//�ړ����[�V�����̐ݒ�
			SetMoveMotion();
			m_bMove = true;
		}
	}
	else {
		m_bLand = false;
		//�d�͂��|����
		m_move.y -= POWER_GRAVITY;
	}

	//������
	if (GetPos().y < DEAD_HEIGHT) {
		Uninit();
		return;
	}

	//�U��
	Attack();

	//���[�V�����̍X�V
	CObjectMotion::Update();

	//���[�V�����I����
	if (GetEndMotion()) {
		MotionEnd();
	}

	//�I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X���X�V
	UpdateMtxWorldAll();
	//���[�V�������̏���
	MotionAct();

	if (m_pGaugeLife != nullptr) {
		//�̗̓Q�[�W�̍X�V
		m_pGaugeLife->Update();
		//�̗̓Q�[�W�̈ʒu���X�V
		m_pGaugeLife->SetAllGaugePos(D3DXVECTOR3(GetPos().x, GetPos().y + m_fHeightLifeGauge, GetPos().z));
	}
}

//=============================================================================
// �G�l�~�[�̕`�揈��
//=============================================================================
void CEnemy::Draw(void) {
	CObjectMotion::Draw();
}

//=============================================================================
// �ړ��ʂ̐ݒ�
//=============================================================================
void CEnemy::SetMove(D3DXVECTOR3 move) {
	m_move = move;
	//�e����΂��ꂽ�肵�ăW�����v����悤�Ȏ�������΃j���[�g�������[�V������ݒ�i�ړ����[�V�������͈ړ����Œ肳��邽�߁j
	if (m_move.y > 0.0f) {
		SetMotion(0);
		AttackStop();
		m_bMove = false;
	}
}

//=============================================================================
// �G�l�~�[�̃_���[�W
//=============================================================================
void CEnemy::Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead) {
	if (m_bDead) return;

	//�_���[�W
	m_nLife -= nDamage;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//�U���̃^�C�v���Ƃ̏���
	switch (typeDamage)
	{
	case DAMAGE_TYPE::PLAYER_PUNCH:
	case DAMAGE_TYPE::SHOCKWAVE:
		//�U���G�t�F�N�g�̐���
		CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f), CEffect::EFFECT_TYPE::DAMAGE_PLAYER, 40.0f, 40.0f, false);
		//�_���[�W���̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		break;
	case DAMAGE_TYPE::THUNDERBOLT:
		//�_���[�W�G�t�F�N�g�̐���
		CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f), CEffect::EFFECT_TYPE::THUNDER, 50.0f, 50.0f, false);
		//�_���[�W���̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_THUNDER);
		break;
	}

	//���S����
	if (m_nLife <= 0) {
		//���S
		if (pDead != nullptr) *pDead = true;
		Dead();
	}
	else {
		if (pDead != nullptr) *pDead = false;

		//�̗̓Q�[�W�̐ݒ�
		if (m_pGaugeLife != nullptr) {
			m_pGaugeLife->SetGaugeValue(m_nLife);
		}
	}
}

//=============================================================================
// �G�l�~�[�̎��S
//=============================================================================
void CEnemy::Dead(void) {
	//���S�̐ݒ�
	m_bDead = true;
	//���S�J�E���g�̏�����
	m_nCntDead = 0;
	//�����蔻��𖳌�
	SetEnableCollision(false);
	//���S���[�V�����̐ݒ�
	SetDeadMotion();
	//�֊s���\��
	SetDrawOutlineAll(false);

	//�̗̓Q�[�W�̔j��
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();

	//�Q�[���V�[���̎擾
	CGameScene* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//�ь�̖؂̎擾
	CAppleTree* pAppleTree = nullptr;
	if (pGame != nullptr) pAppleTree = pGame->GetAppleTree();
	//�ь�̖؂𐬒�������
	if (pAppleTree != nullptr) {
		int nGrowValue = DEFAULT_GROW_VALUE; //������
		//���̓G�̏ꍇ��������
		if (m_bGoldEnemy) nGrowValue = GOLD_GROW_VALUE;
		//�؂𐬒�
		pAppleTree->AddGrow(nGrowValue);
	}

	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//���̍Đ�
	//if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::EXPLOSION);
}

//=============================================================================
// �؂̕����ւ̊p�x�ɐݒ�
//=============================================================================
void CEnemy::SetRotTree(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�Q�[���V�[���̎擾
	CGameScene* pGame = pManager->GetGameScene();
	if (pGame == nullptr) return;
	//�ь�̖؂̎擾
	CObject* pAppleTree = pGame->GetAppleTree();
	if (pAppleTree == nullptr) return;
	D3DXVECTOR3 posAppleTree = pAppleTree->GetPos();//�؂̈ʒu���擾

	//�G����ь�̖؂ւ̊p�x
	float rotTreeY = (float)atan2(posAppleTree.x - GetPos().x, posAppleTree.z - GetPos().z);

	//�ڕW�p�x��ь�̖؂̌����֌�����
	m_rotDestY = rotTreeY;
	//���f�������΂Ȃ̂Ŕ��Ό����Ɍ�����
	if (m_rotDestY >= 0.0f) {
		m_rotDestY -= D3DX_PI;
	}
	else if (m_rotDestY < 0.0f) {
		m_rotDestY += D3DX_PI;
	}
}

//=============================================================================
// �U���\�ȋ������ǂ������v�Z
//=============================================================================
void CEnemy::AttackDistance(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�Q�[���V�[���̎擾
	CGameScene* pGame = pManager->GetGameScene();
	if (pGame == nullptr) return;
	//�ь�̖؂̎擾
	CObject* pAppleTree = pGame->GetAppleTree();
	if (pAppleTree == nullptr) return;
	D3DXVECTOR3 posAppleTree = pAppleTree->GetPos();//�؂̈ʒu���擾
	float fDistTree = D3DXVec2Length(&D3DXVECTOR2(posAppleTree.x - GetPos().x, posAppleTree.z - GetPos().z));	//�؂ƓG�Ƃ̋���

	m_bAttackDist = fDistTree <= m_fDistAttack ? true : false;
}

//=============================================================================
//�@�U���̏���
//=============================================================================
void CEnemy::Attack(void) {
	//�U���J�n�����𖞂����Ă��Ȃ��ꍇ�I��
	if (GetMotionCategory() == MOTION_CATEGORY::ATTACK || !m_bAttackDist || !m_bLand) return;

	//�U���ς݃��X�g�̏�����
	InitObjAttacked();
	//�ړ��ʂ��d�݂͂̂ɂ���
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bMove = false;
	//�U���J�n
	AttackStart();
}