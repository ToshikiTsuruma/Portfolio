//=============================================================================
//
// 敵スポナー処理 [enemySpawner.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _ENEMY_SPAWNER_H_
#define _ENEMY_SPAWNER_H_

#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************

//*****************************************************************************
// 敵スポナークラス
//*****************************************************************************
class CEnemySpawner : public CObject
{
public:
	//敵の種類
	enum class ENEMYTYPE {
		NORMAL = 0,	//通常
		HUMAN,		//人形
		SORCERER,		//魔術師
		ENUM_MAX,	
	};

	CEnemySpawner();	//デフォルトコンストラクタ
	~CEnemySpawner();	//デストラクタ
	static CEnemySpawner* Create(int nSpan, int nDistMin, int nDistMax);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void) {}	//描画処理

	void AddLevel(int nLevel) { m_nLevel += nLevel; }	//レベルの増加
	void SetLevel(int nLevel) { m_nLevel = nLevel; }	//レベルの設定
	void AddSpan(int nSpan) { m_nSpanSpawn += nSpan; }	//スポーンのスパンを増加
	void AddDistEnemy(int nDist);	//敵の生成間隔を増加

private:
	static void GetSpawnRate(int nLevel, int* pRateArray);	//レベルごとの生成確率の取得
	void SetEnemyCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fRadiusCircle, int nMinDist, int nMaxDist);	//同心円状にランダムに角度ずらして敵を配置する

	int m_nLevel;		//スポナーのレベル
	int m_nSpanSpawn;	//スポーンのスパン
	int m_nCntSpawn;	//スポーンまでのカウント
	int m_nDistEnemyMin;	//スポーンする敵同士の距離の最小
	int m_nDistEnemyMax;	//スポーンする敵同士の距離の最大
};

#endif // !_ENEMY_SPAWNER_H_