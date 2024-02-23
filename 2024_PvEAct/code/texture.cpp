//===========================================================================================
// 
// [texture.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <stdlib.h>
#include "texture.h"
#include "renderer.h"
#include "manager.h"

// 静的メンバ変数宣言
CTexture* CTexture::m_pTexture = nullptr;
int CTexture::m_nNumAll = 0;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CTexture::CTexture()
{
	ZeroMemory(&m_aData, sizeof(m_aData));
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CTexture::~CTexture()
{

}

//===========================================================================================
// 生成
//===========================================================================================
CTexture* CTexture::Create(void)
{
	if (m_pTexture == nullptr)
	{
		m_pTexture = new CTexture;

		m_pTexture->Load();

		return m_pTexture;
	}

	return m_pTexture;
}

//===========================================================================================
//	インスタンスの取得
//===========================================================================================
CTexture* CTexture::GetInstance(void)
{
	if (m_pTexture == nullptr)
	{
		assert(("テクスチャ情報の取得に失敗", false));
	}

	return m_pTexture;
}

//===========================================================================================
// インスタンスの破棄
//===========================================================================================
void CTexture::Release(void)
{
	if (m_pTexture == nullptr)
	{
		return;
	}
	else
	{
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

//===========================================================================================
// テクスチャを全て読み込む(初期化処理)
//===========================================================================================
HRESULT CTexture::Load(void)
{
	//// ファイル名
	//const char *c_apTexName[MAX_TEXTURE] =
	//{
	//	"",
	//};

	//// デバイスの取得
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	//{
	//	if (c_apTexName[nCntTex] != nullptr)
	//	{
	//		Regist(c_apTexName[nCntTex]);
	//	}
	//	else
	//	{
	//		return NULL;
	//	}
	//}

	return S_OK;
}

//===========================================================================================
// テクスチャを全て破棄する(終了処理)
//===========================================================================================
void CTexture::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		if (m_aData[nCntTex].pTexture != nullptr)
		{
			m_aData[nCntTex].pTexture->Release();
			m_aData[nCntTex].pTexture = nullptr;
		}	
		if (m_aData[nCntTex].aName != "")
		{	
			ZeroMemory(&m_aData[nCntTex].aName[0], sizeof(m_aData[nCntTex].aName));
		}
	}
}

//===========================================================================================
// テクスチャの割り当て
//===========================================================================================
int CTexture::Regist(const char * pTexPath)
{	
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{// テクスチャの最大数分まわる

		if (m_aData[nCntTex].pTexture != nullptr && m_aData[nCntTex].aName != "")
		{
			if (strcmp(&m_aData[nCntTex].aName[0], pTexPath) == 0)
			{// 保存されたファイル名と引数のファイル名が同じだった場合
				return nCntTex;	//番号を返す
			}
		}
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	
	// テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice, pTexPath, &m_aData[m_nNumAll].pTexture)))
	{
		return -1;
	}

	// テクスチャ名を書き込む
	strcpy(&m_aData[m_nNumAll].aName[0], pTexPath);

	int nId = m_nNumAll;

	m_nNumAll++;	// テクスチャ総数のカウントアップ

	return nId;
}

//===========================================================================================
// テクスチャ番号
//===========================================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (m_aData[nIdx].pTexture == nullptr)
	{
		return nullptr;
	}

	return m_aData[nIdx].pTexture;
}
