//=============================================================================
//
// ボス処理 [boss.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "sceneMotion.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// ボスクラス
//*****************************************************************************
class CBoss : public CSceneMotion
{
public:
	//モーションの種類
	enum class MOTION_TYPE {
		NEUTRAL = 0,	//ニュートラルモーション
		WAIT,	//待機時
		MOVE,	//移動
		ATTACK_FRONT,	//前方攻撃
		ATTACK_UNDER,	//下攻撃
		ATTACK_RUSH,	//突進攻撃
		DEAD,	//死亡
		ENUM_MAX
	};

	//モーションの区分
	enum class MOTION_CATEGORY {
		NONE = 0,	//その他
		MOVE,		//移動
		ATTACK,		//攻撃
		ENUM_MAX
	};

	//攻撃の種類
	enum class ATTACK_TYPE {
		FRONT = 0,
		UNDER,
		RUSH,
		ENUM_MAX
	};

	//当たり判定の部位
	enum class PARTS_TYPE {
		BODY = 0,	//胴体
		ARM_RIGHT,	//右腕
		HAND_RIGHT,	//右手
		ARM_LEFT,	//左腕
		HAND_LEFT,	//左手
		LEG_RIGHT,	//右腿	
		FOOT_RIGHT,	//右足	
		LEG_LEFT,	//左腿	
		FOOT_LEFT,	//左足
		ENUM_MAX
	};

	CBoss();	//デフォルトコンストラクタ
	~CBoss();	//デストラクタ

	static CBoss* Create(D3DXVECTOR3 pos);	//生成処理
	static void LoadMotionInfo(void);	//モーション情報をロード
	static void UnloadMotionInfo(void);	//モーション情報をアンロード

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理
	int GetNumCollisionParts(void);	//当たり判定があるパーツの数の取得
	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//当たり判定の情報の取得
	void StartAttack(ATTACK_TYPE type);	//攻撃開始時の処理
	void Damage(int nDamage, bool* pDead);		//ダメージ処理

private:
	MOTION_CATEGORY GetMotionCategory(void);	//モーションの区分の取得

	static int m_nNumParts;	//パーツ数
	static PARTS_INFO* m_pPartsInfoArray;	//パーツ情報の配列のポインタ
	static MOTION_INFO m_aMotionInfo[(int)MOTION_TYPE::ENUM_MAX];	//モーション情報
	static int m_nIdxWeapon;	//武器を持つパーツのインデックス

	D3DXVECTOR3	m_lastPos;	//最後の位置座標
	D3DXVECTOR3 m_move;		//移動量
	float m_rotDestY;		//目標のY軸の角度
	bool m_bEncounterPlayer;//プレイヤーとエンカウントしているかどうか
	bool m_bStay;			//プレイヤーを待機中
	bool m_bRotateAttack;	//攻撃モーション中に回転できるかどうか
	bool m_bValidAttack;	//攻撃が有効かどうか
	int m_nCntNormalAttack;	//通常攻撃を行った回数
	int m_nCntAttackRush;	//突進攻撃を連続で行った回数
	bool m_bRecovery;		//攻撃後の硬直中
	int m_nLife;					//体力
	CScene2D* m_pGaugeLife;			//体力ゲージ
	CScene2D* m_pGaugeLifeBg;		//体力ゲージ背景
	CScene2D* m_pGaugeLifeFrame;	//体力ゲージの枠
	bool m_bDead;	//死亡
	int m_nCntDead;	//死亡後のカウント
};

#endif // !_BOSS_H_