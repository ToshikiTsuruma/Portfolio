//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "sceneMotion.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CScene2D;
class CWeapon;
class CScore;

//*****************************************************************************
// プレイヤークラス
//*****************************************************************************
class CPlayer : public CSceneMotion
{
public:
	//モーションの種類
	enum class MOTION_TYPE {
		NEUTRAL = 0,	//ニュートラルモーション
		WALK,	//歩き
		DASH,	//走り
		DODGE_FRONT,//前方回避
		DODGE_BACK,	//後方回避
		PUNCH_1,		//パンチ
		PUNCH_2,		//パンチ
		ATTACK_SWORD_1,	//剣攻撃
		ATTACK_SWORD_2,	//剣攻撃
		ATTACK_SWORD_3,	//剣攻撃
		ATTACK_KATANA_1,	//刀攻撃
		ATTACK_KATANA_2,	//刀攻撃
		ATTACK_KATANA_3,	//刀攻撃　突き
		ATTACK_KATANA_DRAW,	//抜刀攻撃
		SHEATH,				//納刀モーション
		STUN,	//スタン
		ENUM_MAX
	};

	//モーションの区分
	enum class MOTION_CATEGORY {
		NONE = 0,	//その他
		MOVE,		//移動
		ATTACK,		//攻撃
		DODGE,		//回避
		ENUM_MAX
	};	

	//武器の種類
	enum class WEAPONTYPE {
		PUNCH = 0,
		SWORD,
		KATANA,
		ENUM_MAX
	};

	CPlayer();	//デフォルトコンストラクタ
	~CPlayer();	//デストラクタ

	static CPlayer* Create(D3DXVECTOR3 pos);	//生成処理
	static void LoadMotionInfo(void);	//モーション情報をロード
	static void UnloadMotionInfo(void);	//モーション情報をアンロード

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理
	void GameClear(void);	//ゲームクリア時の処理
	void GameOver(void);	//ゲームオーバー時の処理
	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//当たり判定の情報の取得
	void Damage(int nDamage, bool* pDead);		//ダメージ処理
	bool GetItem(int nTypeItem);	//アイテム取得時の処理
	D3DXVECTOR3 GetLastPos(void);	//最後の位置座標の取得
	void SetMove(D3DXVECTOR3 move);	//移動量の設定
	D3DXVECTOR3 GetMove(void);		//移動量の取得
	float GetRadius(void);			//当たり判定の半径の取得
	bool GetStun(void);		//スタン状態の取得

private:
	MOTION_CATEGORY GetMotionCategory(void);	//モーションの区分の取得
	void ChangeWeaponType(WEAPONTYPE typeWeapon);	//武器の変更

	static int m_nNumParts;	//パーツ数
	static PARTS_INFO* m_pPartsInfoArray;	//パーツ情報の配列のポインタ
	static MOTION_INFO m_aMotionInfo[(int)MOTION_TYPE::ENUM_MAX];	//モーション情報
	static int m_nIdxWeapon;	//武器を持つパーツのインデックス

	D3DXVECTOR3	m_lastPos;	//最後の位置座標
	D3DXVECTOR3 m_destRot;	//目標の角度
	D3DXVECTOR3 m_move;		//移動量
	bool m_bLand;			//接地中かどうか
	bool m_bValidAttack;	//攻撃が有効かどうか
	int m_nNumKillEnemy;	//敵を倒した数
	bool m_bRecovery;		//攻撃や回避後の硬直中
	int m_nCntDodge;		//回避中のカウンタ
	int m_nCntAttackMove;	//攻撃中の移動カウンタ
	int m_nLife;			//体力
	bool m_bStun;			//スタン状態
	int m_nCntStun;			//スタン時のカウンタ
	CScene2D* m_pGaugeLife;	//体力ゲージ
	bool m_bUseItem;		//アイテム使用中かどうか
	int m_nCntUseItem;		//アイテム使用中のカウント
	CScene2D* m_pGaugeUseItem;	//アイテム使用ゲージ
	CScore* m_pCounterItem;		//現在所持しているアイテムの数のカウンターのポインタ
	WEAPONTYPE m_typeWeapon;	//武器の種類
	CWeapon* m_pWeapon;			//武器クラスへのポインタ
	CScene2D* m_pIconWeapon;	//武器のアイコン
};

#endif // !_PLAYER_H_