//=============================================================================
//
// メッシュ軌道処理 [meshorbit.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "meshorbit.h"
#include "manager.h"
#include "renderer.h"
#include "pause.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CMeshorbit::CMeshorbit() : m_nNumVtx(0)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CMeshorbit::CMeshorbit(int nNumVtx) : m_nNumVtx(nNumVtx)
{
	SetDrawPriority(DRAW_PRIORITY::CLEAR);	//描画順の設定

	m_pMtxParent = nullptr;
	m_pVtxBuff = nullptr;
	for (int nCnt = 0; nCnt < 2; nCnt++) {
		m_aOffset[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCol[nCnt] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
	m_nSpanUpdate = 0;
	m_nCntUpdate = 0;
	m_bBindPos = false;
	m_bVisible = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshorbit::~CMeshorbit()
{

}

//=============================================================================
// メッシュ軌道の生成処理
//=============================================================================
CMeshorbit* CMeshorbit::Create(D3DXMATRIX* pMtxParent, int nNumVtx, int nSpanUpdate, D3DXVECTOR3 offset0, D3DXVECTOR3 offset1, D3DXCOLOR col0, D3DXCOLOR col1) {
	CMeshorbit* pMeshorbit;
	pMeshorbit = new CMeshorbit(nNumVtx);
	if (pMeshorbit != nullptr) {
		pMeshorbit->m_pMtxParent = pMtxParent;
		pMeshorbit->m_aOffset[0] = offset0;
		pMeshorbit->m_aOffset[1] = offset1;
		pMeshorbit->m_aCol[0] = col0;
		pMeshorbit->m_aCol[1] = col1;
		pMeshorbit->m_nSpanUpdate = nSpanUpdate;
		pMeshorbit->Init();
	}

	return pMeshorbit;
}

//=============================================================================
// メッシュ軌道の初期化処理
//=============================================================================
HRESULT CMeshorbit::Init(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return S_OK;

	m_nCntUpdate = 0;

	//頂点バッファの生成
	if (m_pVtxBuff == nullptr) {
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr);
	}

	//頂点バッファの設定
	if (m_pVtxBuff != nullptr) {
		VERTEX_3D *pVtx;	//頂点バッファのポインタ
		//頂点バッファをロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//頂点の設定
		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++) {
			//頂点座標の設定
			pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//色の設定
			D3DXCOLOR col = m_aCol[nCntVtx % 2];
			col.a = 1 - (float)(nCntVtx / 2) / (float)(m_nNumVtx / 2);	//透明度を頂点数の割合で設定
			pVtx[nCntVtx].col = col;

			pVtx[nCntVtx].tex = D3DXVECTOR2((float)nCntVtx / (float)2, (float)(nCntVtx % 2));
		}
		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// メッシュ軌道の終了処理
//=============================================================================
void CMeshorbit::Uninit(void) {
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// メッシュ軌道の更新処理
//=============================================================================
void CMeshorbit::Update(void) {
	//モデルのワールドマトリックスはdrawで更新されるため、頂点の更新をdrawに移動
}

//=============================================================================
// メッシュ軌道の描画処理
//=============================================================================
void CMeshorbit::Draw(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	

	CRenderer* pRenderer = nullptr;		
	CPause* pPause = nullptr;

	if (pManager != nullptr) {
		//レンダラーの取得
		pRenderer = pManager->GetRenderer();
		//ポーズの取得
		pPause = pManager->GetPause();
	}
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return;

	//不可視の場合終了
	if (!m_bVisible) return;

	bool bPause = false;	//ポーズ状態かどうか
	if (pPause != nullptr) {
		bPause = pPause->GetPause();
	}

	//ポーズ中には軌跡の移動を行わない
	if (!bPause) {
		//頂点の更新
		VERTEX_3D *pVtx;
		D3DXMATRIX mtxWorldPoint[2];	//軌道の原点の頂点のワールドマトリックス
		D3DXMATRIX mtxTrans;			//計算用マトリックス
		D3DXVECTOR3 posPoint[2];		//軌道の原点の頂点の位置

		if (m_pVtxBuff != nullptr) {
			//頂点バッファをロックし、頂点データへのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			//軌道の原点の頂点を更新
			if (m_pMtxParent != nullptr && !m_bBindPos) {
				//軌道の原点の頂点の取得
				for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++) {
					//ワールドマトリックスの初期化
					D3DXMatrixIdentity(&mtxWorldPoint[nCntVtx]);
					//位置を反映
					D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntVtx].x, m_aOffset[nCntVtx].y, m_aOffset[nCntVtx].z);
					D3DXMatrixMultiply(&mtxWorldPoint[nCntVtx], &mtxWorldPoint[nCntVtx], &mtxTrans);
					//親マトリックスと掛け合わせる
					D3DXMatrixMultiply(&mtxWorldPoint[nCntVtx], &mtxWorldPoint[nCntVtx], m_pMtxParent);
					posPoint[nCntVtx].x = mtxWorldPoint[nCntVtx]._41;
					posPoint[nCntVtx].y = mtxWorldPoint[nCntVtx]._42;
					posPoint[nCntVtx].z = mtxWorldPoint[nCntVtx]._43;
				}
			}
			//親マトリックスがNULLの場合、１フレーム前の軌道の原点を移動させないようにする
			else {
				posPoint[0] = pVtx[0].pos;
				posPoint[1] = pVtx[1].pos;
			}

			bool bEqualALL = true;	//すべての頂点の位置情報が等しいかどうか
			if (m_nCntUpdate >= m_nSpanUpdate) {
				//ひとつ前の軌道の頂点をコピー
				for (int nCntVtx = m_nNumVtx - 1; nCntVtx > 1; nCntVtx--) {
					//一つ前の頂点を比較
					if (pVtx[nCntVtx].pos != pVtx[nCntVtx - 2].pos && bEqualALL) bEqualALL = false;	//異なった場合
					//頂点のコピー
					pVtx[nCntVtx].pos = pVtx[nCntVtx - 2].pos;
				}
				//バインド状態時に全頂点が同じ位置なら不可視状態にする
				if (m_bBindPos && bEqualALL) SetVisible(false);

				m_nCntUpdate = 0;
			}
			else {
				m_nCntUpdate++;
			}
			//軌道の原点の頂点を親のマトリックスの位置に移動させる
			pVtx[0].pos = posPoint[0];
			pVtx[1].pos = posPoint[1];

			m_pVtxBuff->Unlock();
		}
	}

	//テクスチャの取得
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(GetTexType());

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//-------------------------------
	//描画の設定
	//-------------------------------
	//ライト反映なし
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//カリングなし
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//Zバッファの更新が不可能
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//-------------------------------
	//描画
	//-------------------------------
	//初期化したワールドマトリックスを設定（頂点をワールド座標で設定しているため）
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//テクスチャの設定
	pDevice->SetTexture(0, pTexture);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0,
		m_nNumVtx - 2);

	//-------------------------------
	//描画の設定を戻す
	//-------------------------------
	//Zバッファの更新が可能
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//カリングの設定を戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//ライト反映
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// メッシュ軌道の位置を固定
//=============================================================================
void CMeshorbit::SetBindPos(void) {
	m_bBindPos = true;
}

//=============================================================================
// バインド状態の取得
//=============================================================================
bool CMeshorbit::GetBindPos(void) {
	return m_bBindPos;
}

//=============================================================================
// メッシュ軌道の可視状態の設定
//=============================================================================
void CMeshorbit::SetVisible(bool bVisible) {
	m_bVisible = bVisible;
	m_bBindPos = false;	//バインドを解除

	//可視状態に設定した場合、すべての頂点を親のマトリックスの位置に移動
	if (bVisible && m_pMtxParent != nullptr) {
		//頂点バッファの設定
		if (m_pVtxBuff != nullptr) {
			D3DXMATRIX mtxWorldPoint[2];	//軌道の原点の頂点のワールドマトリックス
			D3DXMATRIX mtxTrans;			//計算用マトリックス
			D3DXVECTOR3 posPoint[2];		//軌道の原点の頂点の位置
			//軌道の原点の頂点の取得
			for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++) {
				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&mtxWorldPoint[nCntVtx]);
				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, m_aOffset[nCntVtx].x, m_aOffset[nCntVtx].y, m_aOffset[nCntVtx].z);
				D3DXMatrixMultiply(&mtxWorldPoint[nCntVtx], &mtxWorldPoint[nCntVtx], &mtxTrans);
				//親マトリックスと掛け合わせる
				D3DXMatrixMultiply(&mtxWorldPoint[nCntVtx], &mtxWorldPoint[nCntVtx], m_pMtxParent);
				posPoint[nCntVtx].x = mtxWorldPoint[nCntVtx]._41;
				posPoint[nCntVtx].y = mtxWorldPoint[nCntVtx]._42;
				posPoint[nCntVtx].z = mtxWorldPoint[nCntVtx]._43;
			}


			VERTEX_3D *pVtx;	//頂点バッファのポインタ
			//頂点バッファをロック
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			//頂点の設定
			for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++) {
				//頂点座標の設定
				pVtx[nCntVtx].pos = posPoint[nCntVtx % 2];
			}
			//頂点バッファをアンロックする
			m_pVtxBuff->Unlock();
		}
	}
}

//=============================================================================
// メッシュ軌道の可視状態の取得
//=============================================================================
bool CMeshorbit::GetVisible(void) {
	return m_bVisible;
}