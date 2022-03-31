//=============================================================================
//
// 円柱の壁の処理 [wallCylinder.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "wallCylinder.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUM_POLYGON_XZ (100)	//壁のXZ方向のポリゴンの分割数
#define WALL_HEIGHT (5000.0f)	//壁の高さ
#define WALL_COLOR (D3DXCOLOR(1.0f, 0.55f, 0.0f, 1.0f))	//壁の色
#define DISTANCE_COLOR_CHANGE (600.0f)	//色が変わり始める距離	プレイヤーからカメラの距離以上の値にする
#define DISTANCE_COLOR_MAX (200.0f)		//色の変化が最大になる距離
#define MAX_COLOR_ALPHA (0.8f)	//変更した色のアルファ値の最大値

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CWallCylinder::CWallCylinder() : CMeshcylinder()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CWallCylinder::CWallCylinder(D3DXVECTOR3 pos, float fRadius) : CMeshcylinder(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), NUM_POLYGON_XZ, fRadius, 1, WALL_HEIGHT, false, WALL_COLOR)
{
	SetObjType(OBJ_TYPE::WALL);	//オブジェクトタイプの設定
	SetDrawPriority(DRAW_PRIORITY::BG);	//描画順の設定
}

//=============================================================================
// デストラクタ
//=============================================================================
CWallCylinder::~CWallCylinder()
{

}

//=============================================================================
// 円柱の壁の生成処理
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
// 円柱の壁の初期化処理
//=============================================================================
HRESULT CWallCylinder::Init(void) {
	CMeshcylinder::Init();

	return S_OK;
}

//=============================================================================
// 円柱の壁の終了処理
//=============================================================================
void CWallCylinder::Uninit(void) {
	CMeshcylinder::Uninit();
}

//=============================================================================
// 円柱の壁の更新処理
//=============================================================================
void CWallCylinder::Update(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//ゲームの取得
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//プレイヤーの取得
	CPlayer* pPlayer = nullptr;
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	if (pPlayer != nullptr) {
		float fRadiusWall = GetRadius();
		D3DXVECTOR3 posPlayer = pPlayer->GetPos();	//プレイヤーの位置の取得
		float fDistance = sqrtf(powf(posPlayer.x - GetPos().x, 2.0f) + powf(posPlayer.z - GetPos().z, 2.0f));	//プレイヤーの位置と円柱の中心との距離
		D3DXCOLOR col = GetColor();	//色の取得

		if (fDistance > fRadiusWall - DISTANCE_COLOR_CHANGE) {
			col.a = (fDistance - (fRadiusWall - DISTANCE_COLOR_CHANGE)) / (fRadiusWall - DISTANCE_COLOR_MAX - (fRadiusWall - DISTANCE_COLOR_CHANGE));
				if (col.a > MAX_COLOR_ALPHA)col.a = MAX_COLOR_ALPHA;
		}
		else {
			col.a = 0.0f;
		}
		//色の設定
		SetColor(col);
	}
	CMeshcylinder::Update();
}

//=============================================================================
// 円柱の壁の描画処理
//=============================================================================
void CWallCylinder::Draw(void) {
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

	//カリングなし
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//ライトを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//描画
	CMeshcylinder::Draw();

	//カリングの設定を戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//ライトを有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 円柱の壁との当たり判定
//=============================================================================
bool CWallCylinder::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 lastPos, float fRadius) {
	if (pPos == NULL) return false;

	CScene* pScene;	//オブジェクトへのポインタ
	pScene = GetSceneTopObjtype(OBJ_TYPE::WALL);	//壁のオブジェクトタイプのリストの先頭を取得

	while (pScene != NULL) {
		CScene* pSceneNext = GetSceneNextObjtype(pScene);	//リストの次のオブジェクトのポインタを取得
		CWallCylinder* pWallCylinder = dynamic_cast<CWallCylinder*>(pScene);	//円柱の壁クラスにキャスト
		//キャスト成功時
		if (pWallCylinder != NULL) {
			//死亡フラグが立っていない場合
			if (pWallCylinder->GetDeath() == false) {
				float fRadiusWall = pWallCylinder->GetRadius();
				D3DXVECTOR3 posWallCylinder = pWallCylinder->GetPos();
				//当たり判定
				float fDistance = sqrtf(powf(pPos->x - posWallCylinder.x, 2.0f) + powf(pPos->z - posWallCylinder.z, 2.0f));	//オブジェクトの位置と円柱の中心との距離
				float fLastDistance = sqrtf(powf(lastPos.x - posWallCylinder.x, 2.0f) + powf(lastPos.z - posWallCylinder.z, 2.0f));	//オブジェクトの前の位置と円柱の中心との距離
				//オブジェクトの半径を足す
				fDistance += fRadius;

				//範囲外に出ようとしたとき
				if (fLastDistance <= fRadiusWall && fDistance > fRadiusWall) {
					D3DXVECTOR2 vecNor = D3DXVECTOR2(pPos->x - posWallCylinder.x, pPos->z - posWallCylinder.z);	//円柱の中心からオブジェクトの位置への正規化ベクトル
					D3DXVec2Normalize(&vecNor, &vecNor);	//正規化

					pPos->x = posWallCylinder.x + vecNor.x * (fRadiusWall - fRadius);
					pPos->z = posWallCylinder.z + vecNor.y * (fRadiusWall - fRadius);
					return true;
				}
			}
		}
		pScene = pSceneNext;	//リストの次のオブジェクトを代入
	}
	return false;
}

//=============================================================================
// 壁と線分の交点（壁の内側から外側へのベクトルのみ）
//=============================================================================
void CWallCylinder::CrossPoint(D3DXVECTOR2* pPosCross, const D3DXVECTOR3* pPosStart, const D3DXVECTOR3* pPosEnd, float fDelta) {
	if (pPosStart == NULL || pPosEnd == NULL) return;

	CScene* pScene;	//オブジェクトへのポインタ
	pScene = GetSceneTopObjtype(OBJ_TYPE::WALL);	//壁のオブジェクトタイプのリストの先頭を取得

	while (pScene != NULL) {
		CScene* pSceneNext = GetSceneNextObjtype(pScene);	//リストの次のオブジェクトのポインタを取得
		CWallCylinder* pWallCylinder = dynamic_cast<CWallCylinder*>(pScene);	//円柱の壁クラスにキャスト
		//キャスト成功時
		if (pWallCylinder != NULL) {
			//死亡フラグが立っていない場合
			if (pWallCylinder->GetDeath() == false) {
				float fRadiusWall = pWallCylinder->GetRadius();
				//壁の位置
				D3DXVECTOR3 posWallCylinder = pWallCylinder->GetPos();

				bool bOutStart = (sqrtf(powf(pPosStart->x - posWallCylinder.x, 2.0f) + powf(pPosStart->z - posWallCylinder.z, 2.0f))) > (fRadiusWall - fDelta);	//線分の始点が壁の外側かどうか
				bool bOutEnd = (sqrtf(powf(pPosEnd->x - posWallCylinder.x, 2.0f) + powf(pPosEnd->z - posWallCylinder.z, 2.0f))) > (fRadiusWall - fDelta);		//線分の終点が壁の外側かどうか
				
				//線分が壁の内側から外側の場合
				if (bOutStart == false && bOutEnd == true) {
					D3DXVECTOR2 vecCenterToStart = D3DXVECTOR2(pPosStart->x - posWallCylinder.x, pPosStart->z - posWallCylinder.z);	//円の中心から線分の始点のベクトル
					D3DXVECTOR2 vecNorLine = D3DXVECTOR2(-(pPosEnd->z - pPosStart->z), pPosEnd->x - pPosStart->x);	//線分の法線ベクトル
					//法線ベクトルを正規化
					D3DXVec2Normalize(&vecNorLine, &vecNorLine);

					//法線ベクトルとvecCenterToStartが直角の場合
					if (D3DXVec2Dot(&vecCenterToStart, &vecNorLine) == 0) {
						//線分のベクトルが円の中心を通っているのでvecCenterToStartを正規化して半径の長さをかける
						D3DXVec2Normalize(&vecCenterToStart, &vecCenterToStart);
						*pPosCross = D3DXVECTOR2(posWallCylinder.x + vecCenterToStart.x * (fRadiusWall - fDelta), posWallCylinder.z + vecCenterToStart.y * (fRadiusWall - fDelta));
					}
					else {
						//法線ベクトルがvecCenterToStartの反対側の場合ベクトルを反転
						if (D3DXVec2Dot(&vecCenterToStart, &vecNorLine) < 0) {
							vecNorLine *= -1;
						}
						//円の中心から直線への最短距離を内積で計算
						float fDistanceShort = D3DXVec2Dot(&vecNorLine, &vecCenterToStart);	//最短距離
						//法線ベクトルに最短距離をかける
						vecNorLine *= fDistanceShort;
						//円の中心から直線への最短距離の位置
						D3DXVECTOR2 posPointShort = D3DXVECTOR2(posWallCylinder.x + vecNorLine.x, posWallCylinder.z + vecNorLine.y);
						//posPointShortからpPosEndへのベクトル
						D3DXVECTOR2 vecShortToEnd = D3DXVECTOR2(pPosEnd->x - posPointShort.x, pPosEnd->z - posPointShort.y);
						D3DXVec2Normalize(&vecShortToEnd, &vecShortToEnd);//正規化
						//vecShortToEndの長さを求めて円との交点を求める
						float fDistanceCross = sqrtf(powf((fRadiusWall - fDelta), 2.0f) - powf(fDistanceShort, 2.0f));	//posPointShortから交点への距離
						//交点を設定
						*pPosCross = posPointShort + vecShortToEnd * fDistanceCross;
					}
					return;
				}
			}
		}
		pScene = pSceneNext;	//リストの次のオブジェクトを代入
	}
}