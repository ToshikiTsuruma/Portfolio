//=============================================================================
//
// 木の処理 [tree.cpp]
// Author : 鶴間俊樹
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
// マクロ定義
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_tree.txt"
#define NUM_COLLISION (16)	//当たり判定の数
#define COLLISION_RADIUS (8.0f)	//当たり判定の半径
#define MAX_LIFE (6)	//体力の最大値
#define LIFE_GAUGE_HEIGHT (100.0f)	//敵の位置からの体力ゲージの高さ
#define DEAD_TIME (120)	//死亡後消えるまでの時間
#define DRAW_DISTANCE (4500.0f)			//描画が可能になり始めるプレイヤーとの距離
#define MAX_DRAW_DISTANCE (4300.0f)		//描画可能な最大のプレイヤーとの距離

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CTree::m_nNumParts = 0;
CSceneMotion::PARTS_INFO* CTree::m_pPartsInfoArray = NULL;
CSceneMotion::MOTION_INFO CTree::m_aMotionInfo[(int)CTree::MOTION_TYPE::ENUM_MAX] = {};

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTree::CTree() : CSceneMotion(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, false)
{
	SetObjType(OBJ_TYPE::TREE);	//オブジェクトタイプの設定
	SetUpdatePriority(UPDATE_PRIORITY::ENEMY);

	m_nLife = MAX_LIFE;
	m_nCntDrawGaugeLife = 0;

	//体力ゲージの生成
	m_pGaugeLifeFrame = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 75.0f, 15.0f);
	m_pGaugeLife = CBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 70.0f, 10.0f);

	if (m_pGaugeLifeFrame != NULL) {
		m_pGaugeLifeFrame->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)); //体力ゲージの色の設定
		m_pGaugeLifeFrame->SetDraw(false);	//描画しないようにする
	}
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->SetColor(D3DXCOLOR(0.3f, 1.0f, 0.0f, 1.0f)); //体力ゲージの色の設定
		m_pGaugeLife->SetDraw(false);	//描画しないようにする
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CTree::~CTree()
{

}

//=============================================================================
// 同心円状にランダムに角度ずらして木を配置する
//=============================================================================
//引数：中心の位置　円の数　円ごとの距離（半径）　（同じ園の一つ前の木との）最短距離、最長距離
void CTree::SetTreeCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fDistCircle, int nMinDist, int nMaxDist) {
	int nCntCircle = 0;	//作成した木の円の数
	float fRotCreate = 0.0f;	//配置する角度
	float fRandStartRot = (rand() % 314) * 0.01f;	//配置の開始角度の位置をランダム 前方が過疎気味になるため

	while (nCntCircle < nNumCircle)
	{
		int nRandDist = rand() % (nMaxDist + 1 - nMinDist) + nMinDist;	//一つ前の木との距離
		fRotCreate += nRandDist / ((nCntCircle + 1) * fDistCircle * D3DX_PI) * D3DX_PI;	//ランダムな距離 / 円周の長さの半分 * pi でラジアンを求める

		//一周配置した場合
		if (fRotCreate >= D3DX_PI * 2.0f) {
			nCntCircle++;	//カウントの加算
			fRotCreate = 0.0f;	//配置する角度のリセット
			fRandStartRot = (rand() % 314) * 0.01f;	//配置する角度のランダム値を再設定
		}
		else {
			//木を生成する位置の設定
			D3DXVECTOR3 posCreate = posCenter;	//配置する位置
			posCreate.x = sinf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			posCreate.z = cosf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			//木の生成
			CTree::Create(posCreate);
		}
	}
}

//=============================================================================
// 木の生成処理
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
// モーション情報をロード
//=============================================================================
void CTree::LoadMotionInfo(void) {
	//ロードする前に動的確保したメモリをdeleteする
	UnloadMotionInfo();

	Load(TEXT_FILE_NAME_LOAD_MOTION, &m_pPartsInfoArray, &m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, NULL);
}

//=============================================================================
// モーション情報をアンロード
//=============================================================================
void CTree::UnloadMotionInfo(void) {
	m_nNumParts = 0;

	//パーツ情報の解放
	if (m_pPartsInfoArray != NULL) {
		delete[] m_pPartsInfoArray;
		m_pPartsInfoArray = NULL;
	}
	//モーション情報の解放
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
// 木の初期化処理
//=============================================================================
HRESULT CTree::Init(void) {
	//CSceneMotion::Init();	//モーションが終了した状態で始まるため無効

	//アニメーションの方向をランダムにしたいので角度をランダムに設定
	int nRandRot = rand() % 63; //0から62
	D3DXVECTOR3 rot = GetRot();
	rot.y = (float)nRandRot / 10.0f;	//0.0 から　6.2
	//角度の設定
	SetRot(rot);

	//地形との当たり判定（地表に位置を合わせるため）
	bool bLand = false;	//接地しているかどうか
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//接触位置、開始ベクトル、終了ベクトル
	vecStart = GetPos();	//プレイヤーの位置を取得
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//上向きのベクトル

	bLand = CTerrain::Collision(&posColTerrain, vecStart, vecEnd);

	if (bLand == true) {
		SetPos(posColTerrain);
	}

	//生成時から描画しない場合があるのでワールドマトリックスを更新する
	UpdateMtxWorldAll();

	return S_OK;
}

//=============================================================================
// 木の終了処理
//=============================================================================
void CTree::Uninit(void) {
	//体力ゲージの破棄
	if (m_pGaugeLife != NULL) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = NULL;
	}
	//体力ゲージの枠の破棄
	if (m_pGaugeLifeFrame != NULL) {
		m_pGaugeLifeFrame->Uninit();
		m_pGaugeLifeFrame = NULL;
	}
	//終了処理
	CSceneMotion::Uninit();
}

//=============================================================================
// 木の更新処理
//=============================================================================
void CTree::Update(void) {
	//サウンドの取得
	CManager* pManager = nullptr;
	pManager = CManager::GetManager();
	CSound *pSound = nullptr;
	if(pManager != nullptr) pSound = pManager->GetSound();

	if (m_bDead == true) {
		m_nCntDead++;

		//木の破棄
		if (m_nCntDead > DEAD_TIME) {
			//アイテムの生成
			D3DXVECTOR3 posItem = GetPos();
			CItemApple::Create(posItem);
			//消滅エフェクトの生成
			CEffect::Create(GetPos(), CEffect::EFFECT_TYPE::EXPLOSION, 50.0f, 50.0f);
			//消滅音の再生
			if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::EXPLOSION);

			//終了処理
			Uninit();
			return;
		}
		CSceneMotion::Update();
		return;
	}

	//体力ゲージの描画時間を減少
	if (m_nCntDrawGaugeLife > 0) {
		m_nCntDrawGaugeLife--;
		//描画時間がなくなった場合
		if (m_nCntDrawGaugeLife <= 0) {
			//描画しなくする
			if (m_pGaugeLife != NULL) m_pGaugeLife->SetDraw(false);
			if (m_pGaugeLifeFrame != NULL) m_pGaugeLifeFrame->SetDraw(false);
		}
		else {
			//位置を更新
			if (m_pGaugeLife != NULL) m_pGaugeLife->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + LIFE_GAUGE_HEIGHT, GetPos().z));
			if (m_pGaugeLifeFrame != NULL) m_pGaugeLifeFrame->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + LIFE_GAUGE_HEIGHT, GetPos().z));
		}
	}
}

//=============================================================================
// 木の描画処理
//=============================================================================
void CTree::Draw(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//ゲームの取得
	CGame* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGame();
	//プレイヤーの取得
	CPlayer* pPlayer = nullptr;
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	//プレイヤーの位置を取得
	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pPlayer != NULL) posPlayer = pPlayer->GetPos();
	//プレイヤーとの距離を求める
	D3DXVECTOR2 vecPlayer = D3DXVECTOR2(posPlayer.x - GetPos().x, posPlayer.z - GetPos().z);
	float fDistPlayer = D3DXVec2Length(&vecPlayer);
	//描画の設定
	if (m_bDraw == true) {
		//描画外の位置にいたら描画しないようにする
		if (fDistPlayer > MAX_DRAW_DISTANCE) {
			m_bDraw = false;
		}
	}
	else {
		//描画内の位置にいたら描画できるようにする
		if (fDistPlayer < DRAW_DISTANCE) {
			m_bDraw = true;
		}
	}

	//描画
	if (m_bDraw == true) {
		CSceneMotion::Draw();
	}
}

//=============================================================================
// 木の当たり判定の情報の取得
//=============================================================================
void CTree::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//当たり判定の数を設定
	if (pNumCol != NULL) *pNumCol = NUM_COLLISION;
	//当たり判定の半径を設定
	if (pRadiusCol != NULL) *pRadiusCol = COLLISION_RADIUS;
	//当たり判定の位置の配列を設定
	if (ppPosColArray != NULL) {
		//posのポインタが空だった場合
		if (*ppPosColArray == NULL) {
			//posの変数を当たり判定の数分配列で確保
			*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];

			D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//モデルのposからのオフセットの始点
			int nIdxModel = 0;	//木の幹

			//当たり判定の位置の取得
			for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
			{
				GetPosCollision(*ppPosColArray + nCntCol, posOffset, nIdxModel);
				posOffset += D3DXVECTOR3(0.0f, 5.0f, 0.0f);	//オフセットを加算
			}
		}
	}
}

//=============================================================================
// 木のダメージ処理
//=============================================================================
void CTree::Damage(int nDamage, bool* pDead) {
	if (m_bDead == true) return;

	//サウンドの取得
	CManager* pManager = nullptr;
	pManager = CManager::GetManager();
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//ダメージ
	m_nLife -= nDamage;

	//死亡時
	if (m_nLife <= 0) {
		m_bDead = true;
		if (pDead != NULL) *pDead = true;
		//死亡モーション設定
		SetMotion((int)MOTION_TYPE::BREAK);
		//倒壊音の再生
		if (pSound != NULL) pSound->CSound::PlaySound(CSound::SOUND_LABEL::BREAK_TREE);
		//死亡カウントの初期化
		m_nCntDead = 0;
		//当たり判定を無効
		SetEnableCollision(false);
		//体力ゲージの破棄
		if (m_pGaugeLife != NULL) {
			m_pGaugeLife->Uninit();
			m_pGaugeLife = NULL;
		}
		//体力ゲージの枠の破棄
		if (m_pGaugeLifeFrame != NULL) {
			m_pGaugeLifeFrame->Uninit();
			m_pGaugeLifeFrame = NULL;
		}
	}
	else {
		*pDead = false;
		//体力ゲージの描画時間の設定
		m_nCntDrawGaugeLife = 300;
		if (m_pGaugeLife != NULL) {
			//体力ゲージの幅を変える
			m_pGaugeLife->SetRatioWidth((float)m_nLife / (float)MAX_LIFE);
			//描画させる
			m_pGaugeLife->SetDraw(true);
		}
		if (m_pGaugeLifeFrame != NULL) {
			//描画させる
			m_pGaugeLifeFrame->SetDraw(true);
		}
	}
}