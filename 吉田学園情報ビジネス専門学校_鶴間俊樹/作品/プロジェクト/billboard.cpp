//=============================================================================
//
// �r���{�[�h���� [billboard.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
#define MAX_DRAW_DISTANCE_DEFAULT (2000.0f)	//�`��\�ȍő�̋����̃f�t�H���g�l

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CBillboard::CBillboard()
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	if (pDevice != nullptr) {
		//���_�o�b�t�@�̐���
		if (m_pVtxBuff == NULL) {
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				NULL);
		}
	}

	SetDrawPriority(DRAW_PRIORITY::EFFECT);	//�`�揇�̐ݒ�

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);
	m_fWidth = 0.0f;
	m_fRatioWidth = 1.0f;
	m_fHeight = 0.0f;
	m_offsetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bDraw = true;
	m_bEnableFog = false;
	m_bZtestAlways = true;
	m_bZwriteEnable = false;
	m_bDrawAllDist = false;
	m_fDistDrawMax = MAX_DRAW_DISTANCE_DEFAULT;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBillboard::~CBillboard()
{

}

//=============================================================================
// ��������
//=============================================================================
CBillboard* CBillboard::Create(D3DXVECTOR3 pos, CTexture::TEXTURE_TYPE type, float fWidth, float fHeight) {
	CBillboard* pBillboard;
	pBillboard = new CBillboard;
	if (pBillboard != NULL) {
		pBillboard->m_pos = pos;
		pBillboard->SetTexType(type);
		pBillboard->m_fWidth = fWidth;
		pBillboard->m_fHeight = fHeight;
		pBillboard->Init();
	}

	return pBillboard;
}

//=============================================================================
// �r���{�[�h�̏���������
//=============================================================================
HRESULT CBillboard::Init(void) {
	//���_�o�b�t�@�̐ݒ�
	if (m_pVtxBuff != NULL) {
		VERTEX_3D *pVtx;	//���_�o�b�t�@�̃|�C���^
		//���_�o�b�t�@�����b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//���_�̐ݒ�

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �r���{�[�h�̏I������
//=============================================================================
void CBillboard::Uninit(void) {
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �r���{�[�h�̍X�V����
//=============================================================================
void CBillboard::Update(void) {

}

//=============================================================================
// �r���{�[�h�̕`�揈��
//=============================================================================
void CBillboard::Draw(void) {
	if (m_bDraw == false) return;	//�`�悵�Ȃ���Ԃ̏ꍇ�I��

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

	//�����ɂ���Ă͕`�悹���ɏI��
	if (m_bDrawAllDist == false) {
		//�Q�[���̎擾
		CGame* pGame = nullptr;
		if (pManager != nullptr) pGame = pManager->GetGame();
		//�v���C���[�̎擾
		CPlayer* pPlayer = nullptr;
		if (pGame != nullptr) pPlayer = pGame->GetPlayer();
		//�v���C���[�̈ʒu���擾
		D3DXVECTOR3 posPlayer;
		if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();
		//�v���C���[�Ƃ̋��������߂�
		D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);
		float fDistPlayer = D3DXVec2Length(&vecPlayer);
		//�v���C���[������ȏ㗣��Ă����ꍇ�I��
		if (fDistPlayer > m_fDistDrawMax) return;
	}

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p
	LPDIRECT3DTEXTURE9 pTexture;	//�e�N�X�`���ւ̃|�C���^

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	//�e�N�X�`���̎擾
	pTexture = CTexture::GetTexture(GetTexType());

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);//�t�s������߂�
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture); //�e�N�X�`���̐ݒ�
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���C�g�𖳌�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (m_bEnableFog == false) {
		//�t�H�O�𖳌�
		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}
	if (m_bZtestAlways == true) {
		//Z�e�X�g
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	}
	if (m_bZwriteEnable == false) {
		//Z�o�b�t�@�̍X�V
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0,
		2);

	if (m_bZtestAlways == true) {
		//Z�e�X�g
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
	if (m_bZwriteEnable == false) {
		//Z�o�b�t�@�̍X�V
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
	if (m_bEnableFog == false) {
		//�t�H�O��L��
		pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	}
	//���C�g��L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �ʒu���W�̐ݒ�
//=============================================================================
void CBillboard::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
}

//=============================================================================
// �ʒu���W�̎擾
//=============================================================================
D3DXVECTOR3 CBillboard::GetPos(void) {
	return m_pos;
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CBillboard::SetSize(D3DXVECTOR3 size) {
	m_fWidth = size.x;
	m_fHeight = size.y;

	VERTEX_3D *pVtx;
	//���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[1].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[3].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f, 0.0f) + m_offsetPos;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �T�C�Y�̎擾
//=============================================================================
D3DXVECTOR3 CBillboard::GetSize(void) {
	return D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
}

//=============================================================================
// ���̊����̐ݒ�
//=============================================================================
void CBillboard::SetRatioWidth(float fRatio) {
	m_fRatioWidth = fRatio;

	VERTEX_3D *pVtx;
	//���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[1].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[3].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f, 0.0f) + m_offsetPos;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �ʒu�̃I�t�Z�b�g�̐ݒ�
//=============================================================================
void CBillboard::SetOffsetPos(D3DXVECTOR3 pos) {
	m_offsetPos = pos;

	VERTEX_3D *pVtx;
	//���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[1].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[3].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f, 0.0f) + m_offsetPos;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void CBillboard::SetTexPos(float startU, float startV, float endU, float endV) {
	VERTEX_3D *pVtx;
	//���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_����ݒ�
	pVtx[0].tex = D3DXVECTOR2(startU, startV);
	pVtx[1].tex = D3DXVECTOR2(endU, startV);
	pVtx[2].tex = D3DXVECTOR2(startU, endV);
	pVtx[3].tex = D3DXVECTOR2(endU, endV);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CBillboard::SetColor(D3DXCOLOR col) {
	VERTEX_3D *pVtx;
	//���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_����ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�悷�邩�ǂ����̐ݒ�
//=============================================================================
void CBillboard::SetDraw(bool bDraw) {
	m_bDraw = bDraw;
}

//=============================================================================
// �t�H�O��L���ɂ��邩�ǂ����̐ݒ�
//=============================================================================
void CBillboard::SetEnableFog(bool bEnable) {
	m_bEnableFog = bEnable;
}

//=============================================================================
// �[�x�֌W�Ȃ��`��ł��邩�ǂ����̐ݒ�
//=============================================================================
void CBillboard::SetZtestAlways(bool bAlways) {
	m_bZtestAlways = bAlways;
}

//=============================================================================
// �[�x�������|���S�����Ղ邩�ǂ����̐ݒ�
//=============================================================================
void CBillboard::SetZwriteEnable(bool bEnable) {
	m_bZwriteEnable = bEnable;
}

//=============================================================================
// �S�Ă̋����ŕ`��\���ǂ����̐ݒ�
//=============================================================================
void CBillboard::SetDrawAllDistance(bool bDraw) {
	m_bDrawAllDist = bDraw;
}

//=============================================================================
// �`��\�ȋ����̍ő�l�̐ݒ�
//=============================================================================
void CBillboard::SetDistanceDrawMax(float fDist) {
	m_fDistDrawMax = fDist;
}