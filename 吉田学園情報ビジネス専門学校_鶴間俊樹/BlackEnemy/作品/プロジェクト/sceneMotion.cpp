//=============================================================================
//
// モーションオブジェクト処理 [sceneMotion.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "sceneMotion.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "objectList.h"
#include "effect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_LOAD_TEXT (128)		//一度にファイルから読み込むテキストのバイト数
#define MAX_ATTACK_DISTANCE (300.0f)	//オブジェクトに攻撃可能な最大の距離
#define CRITICAL_DAMAGE (5)	//クリティカルダメージ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CSceneMotion::CSceneMotion() : m_nNumParts(0), m_nNumTypeMotion(0)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CSceneMotion::CSceneMotion(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, bool bOutline) : m_nNumParts(nNumParts), m_nNumTypeMotion(nNumTypeMotion)
{
	//モデルのポインタの生成
	m_ppModelArray = new CModel*[nNumParts];
	if (m_ppModelArray != nullptr && pPartsInfoArray != nullptr) {
		//モデルを生成
		for (int nCnt = 0; nCnt < nNumParts; nCnt++)
		{
			CModel* pParentParts = nullptr;	//親モデルのポインタ
			int nIdxParent = pPartsInfoArray[nCnt].nIdxParent;	//親インデックス
			if (nIdxParent >= 0 && nIdxParent < nNumParts) pParentParts = m_ppModelArray[nIdxParent];
			m_ppModelArray[nCnt] = CModel::Create(pPartsInfoArray[nCnt].modelType, pPartsInfoArray[nCnt].offset, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pParentParts, bOutline);
		}
	}

	//モーション更新情報の生成
	m_pUpdateMotionInfoArray = new UPDATE_MOTION_INFO[nNumParts];
	if (m_pUpdateMotionInfoArray != nullptr) {
		//初期化
		for (int nCnt = 0; nCnt < nNumParts; nCnt++)
		{
			m_pUpdateMotionInfoArray[nCnt].deltaPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_pUpdateMotionInfoArray[nCnt].deltaRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_pUpdateMotionInfoArray[nCnt].curPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_pUpdateMotionInfoArray[nCnt].curRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			if (pPartsInfoArray != nullptr) m_pUpdateMotionInfoArray[nCnt].offsetPos = pPartsInfoArray[nCnt].offset;
		}
	}

	//モーション情報の生成
	m_pMotionInfoArray = new MOTION_INFO[nNumTypeMotion];
	if (m_pMotionInfoArray != nullptr && pMotionInfoArray != nullptr) {
		//モーション情報の設定
		for (int nCntMotion = 0; nCntMotion < nNumTypeMotion; nCntMotion++)
		{
			for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
			{
				m_pMotionInfoArray[nCntMotion].bLoop = pMotionInfoArray[nCntMotion].bLoop;
				m_pMotionInfoArray[nCntMotion].nNumKey = pMotionInfoArray[nCntMotion].nNumKey;
				m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].nFrame = pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].nFrame;

				//キー情報の生成
				m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray = new KEY[nNumParts];

				if (m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray != nullptr && pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray != nullptr) {
					//情報の初期化
					for (int nCntKey = 0; nCntKey < nNumParts; nCntKey++)
					{
						m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray[nCntKey].pos = pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray[nCntKey].pos;
						m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray[nCntKey].rot = pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray[nCntKey].rot;
					}
				}
			}
		}
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);
	m_bEndMotion = false;
	m_pListAttacked = new CObjectList;

	m_nTypeMotion = 0;
	m_bLoopMotion = false;
	m_nKey = 0;
	m_nCntMotion = 0;
	m_bTransMotion = false;
	m_bSetCurMotion = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneMotion::~CSceneMotion()
{
	if (m_ppModelArray != nullptr) {
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
		{
			if (m_ppModelArray[nCntParts] != nullptr) {
				//モデルの破棄
				m_ppModelArray[nCntParts]->Uninit();
				delete m_ppModelArray[nCntParts];
				m_ppModelArray[nCntParts] = nullptr;
			}
		}
		//モデルのポインタの配列の破棄
		delete[] m_ppModelArray;
		m_ppModelArray = nullptr;
	}

	if (m_pUpdateMotionInfoArray != nullptr) {
		//モーションの更新情報の破棄
		delete[] m_pUpdateMotionInfoArray;
		m_pUpdateMotionInfoArray = nullptr;
	}

	if (m_pMotionInfoArray != nullptr) {
		for (int nCntMotion = 0; nCntMotion < m_nNumTypeMotion; nCntMotion++)
		{
			for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
			{
				if (m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray != nullptr) {
					//モーション情報内のキーの配列の破棄
					delete[] m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray;
					m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray = nullptr;
				}
			}
		}
		//モーション情報の配列の破棄
		delete[] m_pMotionInfoArray;
		m_pMotionInfoArray = nullptr;
	}

	if (m_pListAttacked != nullptr) delete m_pListAttacked;
}

//=============================================================================
// モーション情報をロード
//=============================================================================
void CSceneMotion::Load(char* pLoadDirectory, PARTS_INFO** ppPartsInfoArray, int* pNumParts, MOTION_INFO* pMotionInfoArray, int nNumMotionType, int* pIdxWeapon) {
	//-------------------------------------
	//ファイルの読み込み
	//-------------------------------------
	FILE *pFile;	//ファイルへのポインタ
	char sLoadText[MAX_LOAD_TEXT];		//一行ずつ読み込むファイルのテキスト
	char *pLoadText;	//ファイルのテキストを分割した文字列

	//モデル取得に使用する変数
	int nNumLoadModel = 0;	//ロードしたモデル数
	CModel::MODELTYPE* pLoadModelTypeArray = nullptr;	//ロードしたモデルの種類の配列のポインタ
	int nCntLoadModel = 0;	//モデルのディレクトリを読み込んだ数

	//パーツ設定に使用する変数
	bool bSetCharacter = false;	//キャラクター設定中かどうか
	bool bSetNumParts = false;	//パーツ数設定したかどうか
	bool bSetParts = false;	//パーツ設定中かどうか
	int nCntParts = 0;		//パーツ設定数

	//モーション情報取得に使用する変数
	bool bMotionSet = false;	//モーション設定中かどうか
	bool bKeySet = false;		//キー情報設定中かどうか
	bool bKey = false;			//キーの位置と角度を設定中かどうか
	int nCntMotion = 0;	//モーションの設定数
	int nCntKeySet = 0;	//キー設定数
	int nCntKey = 0;	//キー設定したパーツ数

	//ファイルを開く
	pFile = fopen(pLoadDirectory, "r");

	if (pFile != nullptr) {
		//読み込み開始位置まで行を飛ばす
		while (fgets(sLoadText, MAX_LOAD_TEXT, pFile) != nullptr) //一行ごとに文字列を取得
		{
			pLoadText = strtok(sLoadText, " =\t\n");	//文字列の分割（空白 タブ 改行 ＝）
			if (pLoadText != nullptr) {
				//読み込み開始
				if (strcmp(pLoadText, "SCRIPT") == 0) {
					break;
				}
			}
		}

		while (fgets(sLoadText, MAX_LOAD_TEXT, pFile) != nullptr) //一行ごとに文字列を取得
		{
			pLoadText = strtok(sLoadText, " =\t\n");	//文字列の分割（空白 タブ 改行 ＝）
			if (pLoadText != nullptr) {
				//コメント
				if (strstr(pLoadText, "#") != nullptr) {
					continue;
				}
				//読み込み終了
				if (strcmp(pLoadText, "END_SCRIPT") == 0) {
					break;
				}

				//-------------------------------------------------
				//モデル読み込み
				//-------------------------------------------------
				//モデル数設定
				if (strcmp(pLoadText, "NUM_MODEL") == 0) {
					pLoadText = strtok(nullptr, " =#\t\n");	//文字列の分割
					if (pLoadText != nullptr) {
						nNumLoadModel = atoi(pLoadText);	//文字列を数字に変換
						pLoadModelTypeArray = new CModel::MODELTYPE[nNumLoadModel];	//読み込んだモデル数分のタイプ保存用の変数を生成
					}
					continue;
				}
				//モデルタイプ設定
				if (strcmp(pLoadText, "MODEL_FILENAME") == 0 && pLoadModelTypeArray != nullptr && nCntLoadModel < nNumLoadModel) {
					pLoadText = strtok(nullptr, " =#\t\n");	//文字列の分割
					if (pLoadText != nullptr) {
						for (int nCntModel = 0; nCntModel < (int)CModel::MODELTYPE::ENUM_MAX; nCntModel++)
						{
							//モデルのディレクトリ名と比較する
							if (strcmp(pLoadText, CModel::GetPathName((CModel::MODELTYPE)nCntModel)) == 0) {
								//モデルの種類の番号を設定
								pLoadModelTypeArray[nCntLoadModel] = (CModel::MODELTYPE)nCntModel;
								nCntLoadModel++;
								break;
							}
						}
					}
					continue;
				}

				//-------------------------------------------------
				//キャラクター情報読み込み
				//-------------------------------------------------
				//キャラクター設定開始
				if (strcmp(pLoadText, "CHARACTERSET") == 0) {
					bSetCharacter = true;
					//各変数の初期化
					bSetNumParts = false;
					nCntParts = 0;

					continue;
				}
				if (bSetCharacter) {
					//キャラクター設定終了
					if (strcmp(pLoadText, "END_CHARACTERSET") == 0) {
						bSetCharacter = false;

						continue;
					}
					//武器を持つパーツのインデックス設定
					if (strcmp(pLoadText, "INDEX_WEAPON") == 0 && pIdxWeapon != nullptr) {
						pLoadText = strtok(nullptr, " =#\t\n");	//文字列の分割
						if (pLoadText != nullptr) {
							*pIdxWeapon = atoi(pLoadText);	//文字列を数字に変換
						}
						continue;
					}
					//パーツ数設定
					if (strcmp(pLoadText, "NUM_PARTS") == 0 && !bSetNumParts && pMotionInfoArray != nullptr) {
						pLoadText = strtok(nullptr, " =#\t\n");	//文字列の分割
						if (pLoadText != nullptr) {
							*pNumParts = atoi(pLoadText);	//文字列を数字に変換
							//パーツ情報をパーツ数分生成
							*ppPartsInfoArray = new PARTS_INFO[*pNumParts];
							//モーション情報のキー情報をパーツ分生成
							for (int nCntMotionInfo = 0; nCntMotionInfo < nNumMotionType; nCntMotionInfo++)
							{
								for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
								{
									pMotionInfoArray[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray = new KEY[*pNumParts];
								}
							}
							bSetNumParts = true;
						}
						continue;
					}


					if (*ppPartsInfoArray != nullptr && bSetNumParts) {
						//-------------------------------------------------
						//パーツ情報読み込み
						//-------------------------------------------------
						//パーツ設定開始
						if (strcmp(pLoadText, "PARTSSET") == 0 && nCntParts < *pNumParts) {
							bSetParts = true;
							continue;
						}
						if (bSetParts) {
							//パーツ設定終了
							if (strcmp(pLoadText, "END_PARTSSET") == 0) {
								bSetParts = false;
								//カウンター加算
								nCntParts++;
								continue;
							}
							//番号設定
							if (strcmp(pLoadText, "INDEX") == 0) {
								pLoadText = strtok(nullptr, " =#\t\n");	//文字列の分割
								if (pLoadText != nullptr) {
									int nIdx = atoi(pLoadText);	//文字列を数字に変換
									//インデックスの設定
									(*ppPartsInfoArray)[nCntParts].nIdx = nIdx;
									//モデルのタイプを設定
									(*ppPartsInfoArray)[nCntParts].modelType = pLoadModelTypeArray[nIdx];
								}
								continue;
							}
							//親パーツの番号設定
							if (strcmp(pLoadText, "PARENT") == 0) {
								pLoadText = strtok(nullptr, " =#\t\n");	//文字列の分割
								if (pLoadText != nullptr) {
									(*ppPartsInfoArray)[nCntParts].nIdxParent = atoi(pLoadText);	//文字列を数字に変換
								}
								continue;
							}
							//オフセットの設定
							if (strcmp(pLoadText, "POS") == 0) {
								pLoadText = strtok(nullptr, " =#\t\n");	//文字列の分割
								if (pLoadText != nullptr) {
									(*ppPartsInfoArray)[nCntParts].offset.x = (float)atof(pLoadText);	//X座標の設定
									pLoadText = strtok(nullptr, " ");	//文字列の分割
									(*ppPartsInfoArray)[nCntParts].offset.y = (float)atof(pLoadText);	//Y座標の設定
									pLoadText = strtok(nullptr, " ");	//文字列の分割
									(*ppPartsInfoArray)[nCntParts].offset.z = (float)atof(pLoadText);	//Z座標の設定
								}
								continue;
							}
						} //bSetParts
					} //bSetNumParts
				} // bSetCharacter
				//-------------------------------------------------
				//モーションの読み込み
				//-------------------------------------------------
				if (pMotionInfoArray != nullptr) {
					//モーションセット
					if (strcmp(pLoadText, "MOTIONSET") == 0 && nCntMotion < nNumMotionType) {
						bMotionSet = true;
						nCntKeySet = 0;
						continue;
					}
					//モーションセットの終了
					if (strcmp(pLoadText, "END_MOTIONSET") == 0) {
						bMotionSet = false;
						nCntMotion++;
						continue;
					}
					if (bMotionSet) {
						//ループ
						if (strcmp(pLoadText, "LOOP") == 0) {
							pLoadText = strtok(nullptr, " =#\t\n");
							if (pLoadText != nullptr) {
								pMotionInfoArray[nCntMotion].bLoop = atoi(pLoadText) != 0;
							}
							continue;
						}
						//キー数
						if (strcmp(pLoadText, "NUM_KEY") == 0) {
							pLoadText = strtok(nullptr, " =#\t\n");
							if (pLoadText != nullptr) {
								pMotionInfoArray[nCntMotion].nNumKey = atoi(pLoadText);
							}
							continue;
						}
						//キーセット
						if (strcmp(pLoadText, "KEYSET") == 0 && nCntKeySet < MAX_KEY_MOTION) {
							bKeySet = true;
							nCntKey = 0;
							continue;
						}
						//エンドキーセット
						if (strcmp(pLoadText, "END_KEYSET") == 0) {
							nCntKeySet++;
							bKeySet = false;
							continue;
						}
						//フレーム
						if (strcmp(pLoadText, "FRAME") == 0 && bKeySet) {
							pLoadText = strtok(nullptr, " =#\t\n");
							if (pLoadText != nullptr) {
								pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].nFrame = atoi(pLoadText);
							}
							continue;
						}
						//各パーツごとの設定
						if (strcmp(pLoadText, "KEY") == 0 && bKeySet && nCntKey < *pNumParts) {
							bKey = true;
							continue;
						}
						//各パーツごとの設定の終了
						if (strcmp(pLoadText, "END_KEY") == 0 && bKeySet) {
							bKey = false;
							nCntKey++;
							continue;
						}
						if (pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray != nullptr) {
							//位置座標の設定
							if (strcmp(pLoadText, "POS") == 0 && bKeySet && bKey) {
								pLoadText = strtok(nullptr, " =#\t\n");
								if (pLoadText != nullptr) {
									pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].pos.x = (float)atof(pLoadText);
								}
								pLoadText = strtok(nullptr, " =#\t\n");
								if (pLoadText != nullptr) {
									pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].pos.y = (float)atof(pLoadText);
								}
								pLoadText = strtok(nullptr, " =#\t\n");
								if (pLoadText != nullptr) {
									pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].pos.z = (float)atof(pLoadText);
								}
								continue;
							}
							//角度の設定
							if (strcmp(pLoadText, "ROT") == 0 && bMotionSet && bKeySet && bKey) {
								pLoadText = strtok(nullptr, " =#\t\n");
								if (pLoadText != nullptr) {
									pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].rot.x = (float)atof(pLoadText);
								}
								pLoadText = strtok(nullptr, " =#\t\n");
								if (pLoadText != nullptr) {
									pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].rot.y = (float)atof(pLoadText);
								}
								pLoadText = strtok(nullptr, " =#\t\n");
								if (pLoadText != nullptr) {
									pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].rot.z = (float)atof(pLoadText);
								}
								continue;
							}
						}
					} //MotionSet
				} //pMotionInfo
			} //LoadText
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else {
		//ファイルの読み込み失敗時
	}

	if (pLoadModelTypeArray != nullptr) {
		delete[] pLoadModelTypeArray;
	}

}

//=============================================================================
// モーションオブジェクトの初期化処理
//=============================================================================
HRESULT CSceneMotion::Init(void) {
	//モーションの0フレームの状態でスタートさせる
	if (m_ppModelArray != nullptr) {
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
			if (m_ppModelArray[nCntParts] != nullptr && m_pMotionInfoArray != nullptr && m_pUpdateMotionInfoArray != nullptr) {
				if (m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].pKeyArray != nullptr)
				{
					D3DXVECTOR3 rot;
					rot = m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].pKeyArray[nCntParts].rot;
					m_ppModelArray[nCntParts]->SetPos(m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].pKeyArray[nCntParts].pos + m_pUpdateMotionInfoArray[nCntParts].offsetPos);
					m_ppModelArray[nCntParts]->SetRot(m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].pKeyArray[nCntParts].rot);
				}
			}
		}
	}
	return S_OK;
}

//=============================================================================
// モーションオブジェクトの終了処理
//=============================================================================
void CSceneMotion::Uninit(void) {

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// モーションオブジェクトの更新処理
//=============================================================================
void CSceneMotion::Update(void) {
	if (m_ppModelArray == nullptr) return;	//モデルの配列がnullの場合終了
	//モデルがnullの場合終了
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		if (m_ppModelArray[nCnt] == nullptr) return;
	}
	if (m_pMotionInfoArray == nullptr || m_pUpdateMotionInfoArray == nullptr) return;
	
	for (int nCntMotion = 0; nCntMotion < m_nNumTypeMotion; nCntMotion++)
	{
		for (int nCntKey = 0; nCntKey < MAX_KEY_MOTION; nCntKey++)
		{
			if (m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKey].pKeyArray == nullptr) return;
		}
	}
	//----------------------------------------------------------
	//モーションの遷移
	//----------------------------------------------------------
	if (m_bTransMotion) {
		//モーションカウンターの加算
		m_nCntMotion++;

		//現在のモデルのパーツごとの位置と角度の保存
		if (!m_bSetCurMotion) {
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].curRot = m_ppModelArray[nCntParts]->GetRot();
			}
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].curPos = m_ppModelArray[nCntParts]->GetPos();
			}
			m_bSetCurMotion = true;

			//角度の差分
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].deltaRot = m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].pKeyArray[nCntParts].rot - m_pUpdateMotionInfoArray[nCntParts].curRot;
			}
			//位置の差分
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].deltaPos = m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].pKeyArray[nCntParts].pos - m_pUpdateMotionInfoArray[nCntParts].curPos + m_pUpdateMotionInfoArray[nCntParts].offsetPos;
			}
		}

		//角度の反映
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
			D3DXVECTOR3 rotNext;	//次のモデルの角度
			rotNext = m_pUpdateMotionInfoArray[nCntParts].curRot + (m_pUpdateMotionInfoArray[nCntParts].deltaRot * (float)m_nCntMotion / (float)m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].nFrame);
			//角度の設定
			m_ppModelArray[nCntParts]->SetRot(rotNext);
		}
		//位置の反映
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
			D3DXVECTOR3 posNext;	//次のモデルの位置座標
			posNext = m_pUpdateMotionInfoArray[nCntParts].curPos + m_pUpdateMotionInfoArray[nCntParts].deltaPos * (float)m_nCntMotion / (float)m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].nFrame;
			//位置座標の設定
			m_ppModelArray[nCntParts]->SetPos(posNext);
		}
		//カウンターが一定の値を超えたら次のモーションを始める
		if (m_nCntMotion >= m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].nFrame) {
			m_nKey = 0;
			m_nCntMotion = 0;
			m_bTransMotion = false;
			m_bSetCurMotion = false;
		}
	}
	//----------------------------------------------------------
	//通常のモーションの処理
	//----------------------------------------------------------
	else if (m_nKey < m_pMotionInfoArray[m_nTypeMotion].nNumKey - 1) {
		//モーションカウンターの加算
		m_nCntMotion++;

		//現在のモデルのパーツごとの位置と角度の保存
		if (!m_bSetCurMotion) {
			m_bSetCurMotion = true;

			//現在（キー遷移時）の角度の設定
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].curRot = m_ppModelArray[nCntParts]->GetRot();
			}
			//現在（キー遷移時）の位置の設定
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].curPos = m_ppModelArray[nCntParts]->GetPos();
			}

			//角度の差分
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].deltaRot = m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[m_nKey + 1].pKeyArray[nCntParts].rot - m_pUpdateMotionInfoArray[nCntParts].curRot;
			}
			//位置の差分
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].deltaPos = m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[m_nKey + 1].pKeyArray[nCntParts].pos - m_pUpdateMotionInfoArray[nCntParts].curPos + m_pUpdateMotionInfoArray[nCntParts].offsetPos;
			}
		}
		//角度の反映
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
			D3DXVECTOR3 rotNext;	//次のモデルの角度
			rotNext = m_pUpdateMotionInfoArray[nCntParts].curRot + m_pUpdateMotionInfoArray[nCntParts].deltaRot * (float)m_nCntMotion / (float)m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[m_nKey + 1].nFrame;
			//角度の設定
			m_ppModelArray[nCntParts]->SetRot(rotNext);
		}
		//位置の反映
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
			D3DXVECTOR3 posNext;	//次のモデルの位置座標
			posNext = m_pUpdateMotionInfoArray[nCntParts].curPos + m_pUpdateMotionInfoArray[nCntParts].deltaPos * (float)m_nCntMotion / (float)m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[m_nKey + 1].nFrame;
			//位置座標の設定
			m_ppModelArray[nCntParts]->SetPos(posNext);
		}

		//カウンターが一定の値を超えたら次のモーションを始める
		if (m_nCntMotion >= m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[m_nKey + 1].nFrame) {
			m_nCntMotion = 0;
			m_bSetCurMotion = false;
			m_nKey++;

			//最後のキー終了後
			if (m_nKey == m_pMotionInfoArray[m_nTypeMotion].nNumKey - 1)
			{
				//ループモーションの場合
				if (m_bLoopMotion) {
					SetMotion(m_nTypeMotion);
				}
				//ループモーションでは無い場合
				else {
					m_bEndMotion = true;
				}
			}
		}
	}
}

//=============================================================================
// モーションオブジェクトの描画処理
//=============================================================================
void CSceneMotion::Draw(void) {
	if (m_ppModelArray == nullptr) return;	//モデルの配列がnullの場合終了

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

	//-----------------------------------
	//モーションオブジェクトのワールドマトリックスの設定
	//-----------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-----------------------------------
	//モデルの描画
	//-----------------------------------
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		if (m_ppModelArray[nCnt] != nullptr) {
			m_ppModelArray[nCnt]->Draw();
		}
	}
}

//=============================================================================
// モーションの設定
//=============================================================================
void CSceneMotion::SetMotion(int nType) {
	//モーションタイプの設定
	m_nTypeMotion = nType;
	//ループするかどうかの設定
	if(m_pMotionInfoArray != nullptr) m_bLoopMotion = m_pMotionInfoArray[nType].bLoop;
	//キーの初期化
	m_nKey = 0;
	//カウンターの初期化
	m_nCntMotion = 0;
	//モーションの補完
	m_bTransMotion = true;
	//現在（キー遷移時）の保存
	m_bSetCurMotion = false;
	//モーションの終了判定をfalseに設定
	m_bEndMotion = false;
}

//=============================================================================
// 攻撃済みリストの初期化
//=============================================================================
void CSceneMotion::InitObjAttacked(void) {
	if (m_pListAttacked != nullptr) m_pListAttacked->Init();
}

//=============================================================================
// 攻撃
//=============================================================================
void CSceneMotion::Attack(OBJ_TYPE objtype, D3DXVECTOR3 posAttack, float fRadiusAttack, int nDamage, CSceneMotion::DAMAGE_TYPE typeDamage, int* pNumKill) {
	CScene* pScene;	//オブジェクトへのポインタ
	pScene = GetSceneTopObjtype(objtype);	//当たり判定を行うオブジェクトタイプのリストの先頭を取得

	while (pScene != nullptr) {
		CScene* pSceneNext = GetSceneNextObjtype(pScene);	//リストの次のオブジェクトのポインタを取得

		//すでに攻撃が当たっている場合
		bool bAttacked = false;	//すでに攻撃されているかどうか

		if (m_pListAttacked != nullptr) {
			//リストにすでに追加されている場合
			bAttacked = m_pListAttacked->MatchObject(pScene);
		}

		//攻撃可能な範囲にいるかどうか　（遠くのものすべてと当たり判定を行うとかなり処理が重くなるため）
		D3DXVECTOR3 vecObj = pScene->GetPos() - posAttack;	//攻撃位置とオブジェクトのベクトル
		float fDistObj = D3DXVec3Length(&vecObj);	//攻撃位置とオブジェクトの距離

		//次のループに飛ばす
		if (bAttacked || pScene->GetDeath() || !pScene->GetEnableCollision() || fDistObj > MAX_ATTACK_DISTANCE) {
			pScene = pSceneNext;	//リストの次のオブジェクトを代入
			continue;	
		}

		//---------------------------
		//当たり判定
		//---------------------------
		bool bCollision = false;	//当たり判定のフラグ
		D3DXVECTOR3 posHit;	//当たった位置

		//当たり判定があるオブジェクトの全モデルのワールドマトリックスを更新
		pScene->UpdateMtxWorldAll();	//当たり判定取得時に位置のズレが生じるため

		//当たり判定があるパーツの数の取得
		int nNumColParts = pScene->GetNumCollisionParts();

		//パーツごとに当たり判定
		for (int nCntColParts = 0; nCntColParts < nNumColParts; nCntColParts++)
		{
			int nNumCollision = 0;		//当たり判定の数
			D3DXVECTOR3* pPosColArray = nullptr;	//当たり判定の位置の配列のポインタ
			float fRadiusCol = 0.0f;	//当たり判定の半径

			//当たり判定の情報の取得
			pScene->GetCollisionInfo(nCntColParts, &nNumCollision, &pPosColArray, &fRadiusCol);

			if (pPosColArray != nullptr) {
				for (int nCntCol = 0; nCntCol < nNumCollision; nCntCol++)
				{
					//球と球の当たり判定
					D3DXVECTOR3 vecCol = pPosColArray[nCntCol] - posAttack;	//当たり判定から当たり判定へのベクトル
					float fDistCol = D3DXVec3Length(&vecCol);	//当たり判定同士の距離
					//距離と当たり判定同士の距離を比較
					if (fDistCol < fRadiusAttack + fRadiusCol) {
						bCollision = true;
						posHit = pPosColArray[nCntCol];
						break;
					}
				}

				//位置情報の破棄
				delete[] pPosColArray;
			}

			//すでに当たっていたら当たり判定のループ終了
			if (bCollision) break;
		}

		//---------------------------
		//当たり判定時の処理
		//---------------------------
		if (bCollision) {
			bool bDead = false;	//攻撃されたオブジェクトが死亡したかどうか
			//オブジェクトにダメージを与える
			pScene->Damage(nDamage, &bDead);
			//倒した数の加算
			if (bDead && pNumKill != nullptr) (*pNumKill)++;

			//攻撃のタイプ
			if (typeDamage > DAMAGE_TYPE::NONE && typeDamage < DAMAGE_TYPE::ENUM_MAX) {
				//マネージャーの取得
				CManager* pManager = CManager::GetManager();
				//サウンドの取得
				CSound *pSound = nullptr;
				if (pManager != nullptr) pSound = pManager->GetSound();

				switch ((DAMAGE_TYPE)typeDamage)
				{
				case DAMAGE_TYPE::PUNCH:
					//攻撃エフェクトの生成
					CEffect::Create(posHit, CEffect::EFFECT_TYPE::PUNCH_DAMAGE, 30.0f, 30.0f);
					//ダメージ音の再生
					if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
					break;

				case DAMAGE_TYPE::SWORD:
					//攻撃エフェクトの生成
					CEffect::Create(posHit, CEffect::EFFECT_TYPE::SWORD_SLASH, 70.0f, 70.0f);
					//ダメージ音の再生
					if (pSound != nullptr) {
						//クリティカルダメージだった場合
						if (nDamage >= CRITICAL_DAMAGE) {
							pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_SWORD_CRITICAL);
						}
						//通常のダメージ音
						else {
							pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_SWORD);
						}
					}
					break;

				case DAMAGE_TYPE::ENEMY:
					//攻撃エフェクトの生成
					CEffect::Create(posHit, CEffect::EFFECT_TYPE::ENEMY_DAMAGE, 40.0f, 40.0f);
					//ダメージ音の再生
					if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
					break;
				}
			}
			//オブジェクトが死亡していない場合攻撃済みリストに追加
			if (!pScene->GetDeath()) {
				if (m_pListAttacked != nullptr) m_pListAttacked->AppendNode(pScene);
			}
		}

		pScene = pSceneNext;	//リストの次のオブジェクトを代入
	}
}

//=============================================================================
// 当たり判定の位置を取得
//=============================================================================
void CSceneMotion::GetPosCollision(D3DXVECTOR3* const pPosCollision, D3DXVECTOR3 posOffset, int nIdxParts) {
	//位置のポインタがNULLの場合終了
	if (pPosCollision == nullptr) return;

	//当たり判定があるモデルを取得
	CModel* pModel = GetPtrModel(nIdxParts);

	if (pModel != nullptr) {
		D3DXMATRIX mtxParent;	//当たり判定があるモデルのワールドマトリックス
		//モデルのワールド変換行列を取得
		mtxParent = pModel->GetMtxWorld();

		D3DXMATRIX mtxWorld;	//当たり判定のワールドマトリックス
		D3DXMATRIX mtxTrans;	//計算用マトリックス

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorld);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, posOffset.x, posOffset.y, posOffset.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		//算出したワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&mtxWorld,
			&mtxWorld,
			&mtxParent);

		//攻撃位置の設定
		pPosCollision->x = mtxWorld._41;
		pPosCollision->y = mtxWorld._42;
		pPosCollision->z = mtxWorld._43;

	}
	else {
		//モーションオブジェクトの位置から当たり判定の位置を取得		
		*pPosCollision = m_pos + posOffset;
	}
}

//=============================================================================
// モーションの種類の取得
//=============================================================================
int CSceneMotion::GetMotionType(void) {
	return m_nTypeMotion;
}

//=============================================================================
// モーションオブジェクトの位置情報の設定
//=============================================================================
void CSceneMotion::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
}

//=============================================================================
// モーションオブジェクトの位置座標の取得
//=============================================================================
D3DXVECTOR3 CSceneMotion::GetPos(void) {
	return m_pos;
}

//=============================================================================
// モーションオブジェクトの位置座標のポインタの取得
//=============================================================================
D3DXVECTOR3* CSceneMotion::GetPtrPos(void) {
	return &m_pos;
}

//=============================================================================
// モーションオブジェクトの角度の設定
//=============================================================================
void CSceneMotion::SetRot(D3DXVECTOR3 rot) {
	m_rot = rot;
}

//=============================================================================
// モーションオブジェクトの角度の取得
//=============================================================================
D3DXVECTOR3 CSceneMotion::GetRot(void) {
	return m_rot;
}

//=============================================================================
// モーションオブジェクトの全モデルのワールドマトリックスの更新
//=============================================================================
void CSceneMotion::UpdateMtxWorldAll(void) {
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

	//モデルの配列がNULLの場合終了
	if (m_ppModelArray == nullptr) return;

	//-----------------------------------
	//モーションオブジェクトのワールドマトリックスの設定
	//-----------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//ワールドマトリックスを更新
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_ppModelArray[nCnt] != nullptr)m_ppModelArray[nCnt]->UpdateMtxWorld();
	}
}

//=============================================================================
// モーションオブジェクトの全モデルの輪郭の色の指定
//=============================================================================
void CSceneMotion::SetColorOutlineAll(D3DXCOLOR col) {
	if (m_ppModelArray == nullptr) return;
	//輪郭の色を変更
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_ppModelArray[nCnt] != nullptr)m_ppModelArray[nCnt]->SetColorOutline(col);
	}
}

//=============================================================================
// ワールドマトリックスのポインタを取得
//=============================================================================
D3DXMATRIX* CSceneMotion::GetPtrMtxWorld(void) {
	return &m_mtxWorld;
}

//=============================================================================
// モデルのポインタを取得
//=============================================================================
CModel* CSceneMotion::GetPtrModel(int nIdxParts) {
	if (nIdxParts >= 0 && nIdxParts < m_nNumParts && m_ppModelArray != nullptr) {
		return m_ppModelArray[nIdxParts];
	}
	return nullptr;
}

//=============================================================================
// モーションの遷移判定
//=============================================================================
bool CSceneMotion::GetTransMotion(void) {
	return m_bTransMotion;
}

//=============================================================================
// モーションの終了判定
//=============================================================================
bool CSceneMotion::GetEndMotion(void) {
	return m_bEndMotion;
}

//=============================================================================
// 現在のモーションのキーの取得
//=============================================================================
int CSceneMotion::GetCurrentKey(void) {
	return m_nKey;
}

//=============================================================================
// モデルの色をフェードさせる
//=============================================================================
bool CSceneMotion::FadeModelAll(float fDestAlpha, float fSpeedFade) {
	bool bEndFade = false;
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		if (m_ppModelArray[nCnt] != nullptr) {
			//透明度の取得
			float fAlpha = m_ppModelArray[nCnt]->GetMaterialAlpha();
			//透明度の加算
			fAlpha += fSpeedFade;
			//加算する透明度が正の値
			if (fSpeedFade >= 0.0f) {
				if (fAlpha >= fDestAlpha) {
					fAlpha = fDestAlpha;
					bEndFade = true;
				}
			}
			//負の値
			else {
				if (fAlpha <= fDestAlpha) {
					fAlpha = fDestAlpha;
					bEndFade = true;
				}
			}
			m_ppModelArray[nCnt]->SetMaterialAlpha(fAlpha);
		}
	}

	return bEndFade;
}

//=============================================================================
// モデルの輪郭の表示設定
//=============================================================================
void CSceneMotion::SetDrawOutlineAll(bool bDraw) {
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		if (m_ppModelArray[nCnt] != nullptr) {
			m_ppModelArray[nCnt]->SetDrawOutline(bDraw);
		}
	}
}