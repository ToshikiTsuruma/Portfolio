//=============================================================================
//
// ビルボード処理 [billboard.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "billboard.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
#define MAX_DRAW_DISTANCE_DEFAULT (2000.0f)	//描画可能な最大の距離のデフォルト値

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CBillboard::CBillboard()
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	if (pDevice != nullptr) {
		//頂点バッファの生成
		if (m_pVtxBuff == NULL) {
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				NULL);
		}
	}

	SetDrawPriority(DRAW_PRIORITY::EFFECT);	//描画順の設定

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
// デストラクタ
//=============================================================================
CBillboard::~CBillboard()
{

}

//=============================================================================
// 生成処理
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
// ビルボードの初期化処理
//=============================================================================
HRESULT CBillboard::Init(void) {
	//頂点バッファの設定
	if (m_pVtxBuff != NULL) {
		VERTEX_3D *pVtx;	//頂点バッファのポインタ
		//頂点バッファをロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//頂点の設定

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f);

		//法線ベクトルの設定
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
		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ビルボードの終了処理
//=============================================================================
void CBillboard::Uninit(void) {
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// ビルボードの更新処理
//=============================================================================
void CBillboard::Update(void) {

}

//=============================================================================
// ビルボードの描画処理
//=============================================================================
void CBillboard::Draw(void) {
	if (m_bDraw == false) return;	//描画しない状態の場合終了

	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return;

	//距離によっては描画せずに終了
	if (m_bDrawAllDist == false) {
		//ゲームの取得
		CGame* pGame = nullptr;
		if (pManager != nullptr) pGame = pManager->GetGame();
		//プレイヤーの取得
		CPlayer* pPlayer = nullptr;
		if (pGame != nullptr) pPlayer = pGame->GetPlayer();
		//プレイヤーの位置を取得
		D3DXVECTOR3 posPlayer;
		if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();
		//プレイヤーとの距離を求める
		D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);
		float fDistPlayer = D3DXVec2Length(&vecPlayer);
		//プレイヤーから一定以上離れていた場合終了
		if (fDistPlayer > m_fDistDrawMax) return;
	}

	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス取得用
	LPDIRECT3DTEXTURE9 pTexture;	//テクスチャへのポインタ

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	//テクスチャの取得
	pTexture = CTexture::GetTexture(GetTexType());

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);//逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	//テクスチャの設定
	pDevice->SetTexture(0, pTexture); //テクスチャの設定
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//ライトを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (m_bEnableFog == false) {
		//フォグを無効
		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}
	if (m_bZtestAlways == true) {
		//Zテスト
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	}
	if (m_bZwriteEnable == false) {
		//Zバッファの更新
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0,
		2);

	if (m_bZtestAlways == true) {
		//Zテスト
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
	if (m_bZwriteEnable == false) {
		//Zバッファの更新
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
	if (m_bEnableFog == false) {
		//フォグを有効
		pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	}
	//ライトを有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 位置座標の設定
//=============================================================================
void CBillboard::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
}

//=============================================================================
// 位置座標の取得
//=============================================================================
D3DXVECTOR3 CBillboard::GetPos(void) {
	return m_pos;
}

//=============================================================================
// サイズの設定
//=============================================================================
void CBillboard::SetSize(D3DXVECTOR3 size) {
	m_fWidth = size.x;
	m_fHeight = size.y;

	VERTEX_3D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[1].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[3].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f, 0.0f) + m_offsetPos;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// サイズの取得
//=============================================================================
D3DXVECTOR3 CBillboard::GetSize(void) {
	return D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
}

//=============================================================================
// 幅の割合の設定
//=============================================================================
void CBillboard::SetRatioWidth(float fRatio) {
	m_fRatioWidth = fRatio;

	VERTEX_3D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[1].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[3].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f, 0.0f) + m_offsetPos;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 位置のオフセットの設定
//=============================================================================
void CBillboard::SetOffsetPos(D3DXVECTOR3 pos) {
	m_offsetPos = pos;

	VERTEX_3D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[1].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[3].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f, 0.0f) + m_offsetPos;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void CBillboard::SetTexPos(float startU, float startV, float endU, float endV) {
	VERTEX_3D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].tex = D3DXVECTOR2(startU, startV);
	pVtx[1].tex = D3DXVECTOR2(endU, startV);
	pVtx[2].tex = D3DXVECTOR2(startU, endV);
	pVtx[3].tex = D3DXVECTOR2(endU, endV);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 色の設定
//=============================================================================
void CBillboard::SetColor(D3DXCOLOR col) {
	VERTEX_3D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画するかどうかの設定
//=============================================================================
void CBillboard::SetDraw(bool bDraw) {
	m_bDraw = bDraw;
}

//=============================================================================
// フォグを有効にするかどうかの設定
//=============================================================================
void CBillboard::SetEnableFog(bool bEnable) {
	m_bEnableFog = bEnable;
}

//=============================================================================
// 深度関係なく描画できるかどうかの設定
//=============================================================================
void CBillboard::SetZtestAlways(bool bAlways) {
	m_bZtestAlways = bAlways;
}

//=============================================================================
// 深度が高いポリゴンを遮るかどうかの設定
//=============================================================================
void CBillboard::SetZwriteEnable(bool bEnable) {
	m_bZwriteEnable = bEnable;
}

//=============================================================================
// 全ての距離で描画可能かどうかの設定
//=============================================================================
void CBillboard::SetDrawAllDistance(bool bDraw) {
	m_bDrawAllDist = bDraw;
}

//=============================================================================
// 描画可能な距離の最大値の設定
//=============================================================================
void CBillboard::SetDistanceDrawMax(float fDist) {
	m_fDistDrawMax = fDist;
}