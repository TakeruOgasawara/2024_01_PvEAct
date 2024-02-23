//===============================================================================
//
// [texture.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _TEXTURE_H_				//このマクロ定義がされていなかったら
#define _TEXTURE_H_				//2重インクルード防止のマクロを定義

#include "main.h"

//マクロ定義
#define MAX_TEXTURE			(256)

//テクスチャクラス
class CTexture
{
private:
	struct Data
	{
		LPDIRECT3DTEXTURE9 pTexture;	// テクスチャポインタ
		char aName[MAX_TEXTURE];	// テクスチャ名
	};

public:
	CTexture();	// コンストラクタ
	~CTexture();	// デストラクタ

	static CTexture* Create(void);
	static CTexture* GetInstance(void);
	static void Release(void);

	HRESULT Load(void);
	void Unload(void);

	int Regist(const char *pTexPath);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:
	// メンバ変数
	static CTexture* m_pTexture;

	Data m_aData[MAX_TEXTURE];
	static int m_nNumAll;	// 総数
};

#endif