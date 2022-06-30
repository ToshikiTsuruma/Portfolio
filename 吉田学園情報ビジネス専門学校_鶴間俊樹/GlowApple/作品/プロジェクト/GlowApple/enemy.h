//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "objectMotion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DIGIT_ADD_TIME_ENEMY (2)	//敵を倒したときに追加される時間の最大桁数

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CBillboard;
class CGauge3D;

//*****************************************************************************
// エネミークラス
//*****************************************************************************
class CEnemy : public CObjectMotion
{
public:
	enum class MOTION_CATEGORY {
		NEUTRAL = 0,	//ニュートラル
		MOVE,			//移動
		ATTACK,			//攻撃
		DEAD,			//死亡
		ENUM_MAX
	};	//モーションの区分

	CEnemy();	//デフォルトコンストラクタ
	CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, float fMoveSpeed, int nLife, float fDistAttack);	//オーバーロードされたコンストラクタ
	~CEnemy();	//デストラクタ

	static void SetEnemyCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fRadiusCircle, int nMinDist, int nMaxDist);	//同心円状にランダムに角度ずらして敵を配置する
	virtual HRESULT Init(void);		//初期化処理
	virtual void Uninit(void);		//終了処理
	virtual void Update(void);		//更新処理
	virtual void Draw(void);		//描画処理
	void Damage(int nDamage, bool* pDead);		//ダメージ
	void Dead(void);		//死亡時処理

	virtual MOTION_CATEGORY GetMotionCategory(void) = 0;	//モーションの区分の取得
	virtual void SetMoveMotion(void) = 0;	//移動時のモーションを設定
	virtual void SetDeadMotion(void) = 0;	//死亡時のモーションを設定
	virtual void AttackStart(void) = 0;		//攻撃開始時
	virtual void MotionEnd(void) = 0;	//モーション終了時
	virtual void MotionAct(void) = 0;	//モーション中の行動

private:
	void SetRotTree(void);		//木の方向への角度に設定
	void AttackDistance(void);	//攻撃可能な距離かどうかを計算
	void Attack(void);	//攻撃の処理

	const float m_fMoveSpeed;	//移動速度
	const float m_fDistAttack;	//攻撃する距離
	const int m_nMaxLife;	//最大の体力

	D3DXVECTOR3 m_move;		//移動量
	float m_rotDestY;		//目標のY軸の角度
	bool m_bAttackDist;		//攻撃可能な距離かどうか
	int m_nLife;			//体力
	bool m_bDead;			//死亡
	int m_nCntDead;			//死亡後のカウント
	bool m_bGoldEnemy;		//金色の敵かどうか

	CGauge3D* m_pGaugeLife;	//体力ゲージ
};

#endif // !_ENEMY_H_