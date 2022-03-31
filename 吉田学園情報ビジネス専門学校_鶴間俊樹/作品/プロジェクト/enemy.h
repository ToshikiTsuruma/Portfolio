//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "sceneMotion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DIGIT_ADD_TIME_ENEMY (2)	//敵を倒したときに追加される時間の最大桁数

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CBillboard;

//*****************************************************************************
// エネミークラス
//*****************************************************************************
class CEnemy : public CSceneMotion
{
public:
	enum class MOTION_CATEGORY {
		NEUTRAL = 0,	//ニュートラル
		MOVE,			//移動
		ATTACK,			//攻撃
		ENUM_MAX
	};	//モーションの区分

	CEnemy();	//デフォルトコンストラクタ
	CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, bool bOutline, int nLife, float fDistAttack, int nAddTime);	//オーバーロードされたコンストラクタ
	~CEnemy();	//デストラクタ

	static void SetEnemyCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fRadiusCircle, int nMinDist, int nMaxDist);	//同心円状にランダムに角度ずらして敵を配置する
	virtual HRESULT Init(void);		//初期化処理
	virtual void Uninit(void);		//終了処理
	virtual void Update(void);		//更新処理
	virtual void Draw(void);		//描画処理
	void Damage(int nDamage, bool* pDead);		//ダメージ
	void Dead(void);		//死亡時処理
	void SetRotateAttack(bool bRotateAttack);	//攻撃モーション中に回転できるかどうかの設定

	virtual MOTION_CATEGORY GetMotionCategory(void) = 0;	//モーションの区分の取得
	virtual void SetMoveMotion(void) = 0;	//移動時のモーションを設定
	virtual void SetChaseMotion(void) = 0;	//追跡時のモーションを設定
	virtual void SetDeadMotion(void) = 0;	//死亡時のモーションを設定
	virtual bool AttackStart(void) = 0;		//攻撃開始時
	virtual void MotionEnd(void) = 0;	//モーション終了時
	virtual void MotionAct(void) = 0;	//モーション中の行動

private:
	const float m_fDistAttack;	//攻撃する距離
	const int m_nMaxLife;	//最大の体力
	const int m_nAddTime;	//倒したときの時間追加

	D3DXVECTOR3 m_move;		//移動量
	float m_rotDestY;		//目標のY軸の角度
	bool m_bRotateAttack;	//攻撃モーション中に回転できるかどうか
	int m_nCntMove;			//移動の切り替えのカウント
	int m_nMoveTimeRand;	//移動時間の乱数
	bool m_bDetectPlayer;	//プレイヤー発見したかどうか
	int m_nLife;			//体力
	bool m_bDead;			//死亡
	int m_nCntDead;			//死亡後のカウント
	CBillboard* m_pGaugeLife;		//体力ゲージのビルボードのポインタ
	CBillboard* m_pGaugeLifeFrame;	//体力ゲージのビルボードの枠のポインタ
	int m_nCntDrawGaugeLife;		//体力ゲージの描画時間
	CBillboard* m_pClock;	//時計の画像のビルボード
	CBillboard* m_pAddTime[MAX_DIGIT_ADD_TIME_ENEMY];	//追加する秒数のビルボード 桁数分用意
	bool m_bDraw;	//描画するかどうか
};

#endif // !_ENEMY_H_