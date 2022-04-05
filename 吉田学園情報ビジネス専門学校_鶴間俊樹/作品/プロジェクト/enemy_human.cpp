//=============================================================================
//
// 人形敵処理 [enemy_human.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "enemy_human.h"
#include "particle.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_enemyHuman.txt"
#define ROTATE_SPEED (0.1f)			//回転速度
#define NUM_COLLISION (10)			//当たり判定の数
#define COLLISION_RADIUS (15.0f)	//当たり判定の半径
#define MAX_LIFE (8)				//体力の最大値
#define DISTANCE_ATTACK (70.0f)		//攻撃する距離
#define ADD_TIME (15)	//倒したときの時間追加

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CEnemyHuman::m_nNumParts = 0;
CSceneMotion::PARTS_INFO* CEnemyHuman::m_pPartsInfoArray = nullptr;
CSceneMotion::MOTION_INFO CEnemyHuman::m_aMotionInfo[(int)CEnemyHuman::MOTION_TYPE::ENUM_MAX] = {};

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemyHuman::CEnemyHuman() : CEnemy(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, true, MAX_LIFE, DISTANCE_ATTACK, ADD_TIME)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemyHuman::~CEnemyHuman()
{

}

//=============================================================================
// 人形敵の生成処理
//=============================================================================
CEnemyHuman* CEnemyHuman::Create(D3DXVECTOR3 pos) {
	CEnemyHuman* pEnemyHuman;
	pEnemyHuman = new CEnemyHuman();
	if (pEnemyHuman != nullptr) {
		pEnemyHuman->SetPos(pos);
		pEnemyHuman->Init();
	}

	return pEnemyHuman;
}

//=============================================================================
// モーション情報をロード
//=============================================================================
void CEnemyHuman::LoadMotionInfo(void) {
	//ロードする前に動的確保したメモリをdeleteする
	UnloadMotionInfo();

	Load(TEXT_FILE_NAME_LOAD_MOTION, &m_pPartsInfoArray, &m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, nullptr);
}

//=============================================================================
// モーション情報をアンロード
//=============================================================================
void CEnemyHuman::UnloadMotionInfo(void) {
	m_nNumParts = 0;

	//パーツ情報の解放
	if (m_pPartsInfoArray != nullptr) {
		delete[] m_pPartsInfoArray;
		m_pPartsInfoArray = nullptr;
	}
	//モーション情報の解放
	for (int nCntMotionInfo = 0; nCntMotionInfo < (int)MOTION_TYPE::ENUM_MAX; nCntMotionInfo++)
	{
		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
		{
			if (m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray != nullptr) {
				delete[] m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray;
				m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray = nullptr;
			}
		}
	}
}

//=============================================================================
// 人形敵の初期化処理
//=============================================================================
HRESULT CEnemyHuman::Init(void) {
	SetMotion((int)MOTION_TYPE::NEUTRAL);	//モーションの初期設定

	//敵の初期化処理
	CEnemy::Init();
	return S_OK;
}

//=============================================================================
// 人形敵の終了処理
//=============================================================================
void CEnemyHuman::Uninit(void) {
	//終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 人形敵の更新処理
//=============================================================================
void CEnemyHuman::Update(void) {
	//敵の更新
	CEnemy::Update();
}

//=============================================================================
// 人形敵の描画処理
//=============================================================================
void CEnemyHuman::Draw(void) {
	CEnemy::Draw();
}

//=============================================================================
// 人形敵の当たり判定の情報の取得
//=============================================================================
void CEnemyHuman::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//当たり判定の数を設定
	if (pNumCol != nullptr) *pNumCol = NUM_COLLISION;
	//当たり判定の半径を設定
	if (pRadiusCol != nullptr) *pRadiusCol = COLLISION_RADIUS;
	//当たり判定の位置の配列を設定
	if (ppPosColArray != nullptr) {
		//posのポインタが空だった場合
		if (*ppPosColArray == nullptr) {
			//posの変数を当たり判定の数分配列で確保
			*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];

			D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//モデルのposからのオフセットの始点
			int nIdxModel = 0;	//胴体

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
// モーションの区分の取得
//=============================================================================
CEnemy::MOTION_CATEGORY CEnemyHuman::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//ニュートラル
	case MOTION_TYPE::NEUTRAL:
	default:
		return MOTION_CATEGORY::NEUTRAL;

		//移動
	case MOTION_TYPE::WALK:
	case MOTION_TYPE::DASH:
		return MOTION_CATEGORY::MOVE;

		//攻撃
	case MOTION_TYPE::ATTACK:
		return MOTION_CATEGORY::ATTACK;
	}
}

//=============================================================================
// 移動時のモーションを設定
//=============================================================================
void CEnemyHuman::SetMoveMotion(void) {
	SetMotion((int)MOTION_TYPE::WALK);
}

//=============================================================================
// 追跡時のモーションを設定
//=============================================================================
void CEnemyHuman::SetChaseMotion(void) {
	if (GetMotionType() != (int)MOTION_TYPE::DASH) {
		SetMotion((int)MOTION_TYPE::DASH);
	}
}

//=============================================================================
// 死亡時のモーションを設定
//=============================================================================
void CEnemyHuman::SetDeadMotion(void) {
	SetMotion((int)MOTION_TYPE::DEAD);
}

//=============================================================================
// 攻撃開始時
//=============================================================================
bool CEnemyHuman::AttackStart(void) {
	//攻撃モーション以外のとき
	if (GetMotionType() != (int)MOTION_TYPE::ATTACK) {
		//攻撃モーションの設定
		SetMotion((int)MOTION_TYPE::ATTACK);
		//攻撃モーション中に回転可能
		SetRotateAttack(true);
		return true;
	}
	return false;
}

//=============================================================================
// モーション終了時
//=============================================================================
void CEnemyHuman::MotionEnd(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//攻撃モーション
	case MOTION_TYPE::ATTACK:
		SetMotion((int)MOTION_TYPE::NEUTRAL);
		//攻撃モーション中の回転を制限
		SetRotateAttack(false);
		break;
	}
}

//=============================================================================
// モーション中の行動
//=============================================================================
void CEnemyHuman::MotionAct(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//攻撃
	case MOTION_TYPE::ATTACK:
		//攻撃のタイミング
		if ((GetCurrentKey() == 1 || GetCurrentKey() == 2) && !GetTransMotion()) {
			const int nNumCollision = 3;	//攻撃の当たり判定の数
			D3DXVECTOR3 aPosCol[nNumCollision] = {};	//当たり判定の位置
			int nIdxModel = 3;	//右手
			D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//モデルからのオフセット

			for (int nCntCol = 0; nCntCol < nNumCollision; nCntCol++)
			{
				//当たり判定の位置の取得
				GetPosCollision(&aPosCol[nCntCol], posOffset, nIdxModel);
				//オフセットを加算
				posOffset += D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
			}

			//当たり判定の数分攻撃する
			for (int nCntAttack = 0; nCntAttack < nNumCollision; nCntAttack++)
			{
				//攻撃
				CSceneMotion::Attack(OBJ_TYPE::PLAYER, aPosCol[nCntAttack], 10.0f, 1, DAMAGE_TYPE::ENEMY, nullptr);
			}
			//攻撃の先端にパーティクルの生成
			//CParticle::Create(aPosCol[nNumCollision - 1], 20.0f, 1.0f, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f));
		}

		if (GetCurrentKey() >= 1) {
			//攻撃モーション中の回転を制限
			SetRotateAttack(false);
		}
		break;	//_攻撃
	}
}