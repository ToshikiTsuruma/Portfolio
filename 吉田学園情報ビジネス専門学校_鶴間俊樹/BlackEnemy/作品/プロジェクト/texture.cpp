//=============================================================================
//
// テクスチャ処理 [texture.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_FILE_NAME_TEXTURE "data/TEXT/load_texture.txt"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::m_apTexture[(int)TEXTURE_TYPE::ENUM_MAX] = {};
char CTexture::m_aFilePath[(int)TEXTURE_TYPE::ENUM_MAX][MAX_LOAD_TEXT_TEXTURE] = {};

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTexture::CTexture()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// テクスチャのロード
//=============================================================================
HRESULT CTexture::Load(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return S_OK;

	//-------------------------------------
	//ファイルの読み込み
	//-------------------------------------
	FILE *pFile;		//ファイルへのポインタ
	char sLoadText[MAX_LOAD_TEXT_TEXTURE];//一行ずつ読み込むファイルのテキスト
	char *pLoadText;	//ファイルのテキストを分割した文字列
	int nIdxType = 1;	//次に読み込むテクスチャの種類	0にNONEがあるため1から

	//ファイルを開く
	pFile = fopen(TEXT_FILE_NAME_TEXTURE, "r");

	if (pFile != nullptr) {
		while (fgets(sLoadText, MAX_LOAD_TEXT_TEXTURE, pFile) != nullptr) //一行ごとに文字列を取得
		{
			pLoadText = strtok(sLoadText, " =\t\n");	//文字列の分割（空白 タブ 改行 ＝）
			if (pLoadText != nullptr) {
				//コメント
				if (strstr(pLoadText, "//") != nullptr) {
					continue;
				}
				//ディレクトリ名のコピー
				strcpy(m_aFilePath[nIdxType], pLoadText);
				nIdxType++;	//インデックスを加算
				if (nIdxType >= (int)TEXTURE_TYPE::ENUM_MAX) 	break;	//読み込むテクスチャの最大数を超えたら終了	
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else {
		//ファイルの読み込み失敗時
		return S_OK;
	}

	//0にNONEがあるため1から
	for (int nCnt = 1; nCnt < (int)TEXTURE_TYPE::ENUM_MAX; nCnt++)
	{
		//すでにテクスチャが生成されていた場合破棄
		if (m_apTexture[nCnt] != nullptr) {
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}

		//テクスチャの生成
		D3DXCreateTextureFromFile(pDevice,
			m_aFilePath[nCnt],	// テクスチャパス
			&m_apTexture[nCnt]);
	}

	return S_OK;
}

//=============================================================================
// テクスチャのアンロード
//=============================================================================
void CTexture::Unload(void) {
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < (int)TEXTURE_TYPE::ENUM_MAX; nCnt++)
	{
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}

//=============================================================================
// テクスチャの取得
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE_TYPE type) {
	return m_apTexture[(int)type];
}

//=============================================================================
// テクスチャのパスの取得
//=============================================================================
char* CTexture::GetPathName(TEXTURE_TYPE type) {
	return m_aFilePath[(int)type];
}