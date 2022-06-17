//=============================================================================
//
// モデルオブジェクト処理 [sceneModel.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SCENEMODEL_H_
#define _SCENEMODEL_H_

#include "main.h"
#include "scene.h"
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// モデルオブジェクトクラス
//*****************************************************************************
class CSceneModel : public CScene
{
public:

	CSceneModel();		//デフォルトコンストラクタ
	CSceneModel(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine);		//オーバーロードされたコンストラクタ
	~CSceneModel();		//デストラクタ
	static CSceneModel* Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//生成処理
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理

	CModel::MODELTYPE GetModelType(void);	//モデルの種類の取得
	CModel* GetPtrModel(void);		//モデルのポインタの取得
	void SetPos(D3DXVECTOR3 pos);	//位置座標の設定
	D3DXVECTOR3 GetPos(void);		//位置座標の取得
	void SetRot(D3DXVECTOR3 rot);	//角度の設定
	D3DXVECTOR3 GetRot(void);		//角度の取得

private:
	CModel* m_pModel;	//モデルへのポインタ

};

#endif // !_SCENEMODEL_H_