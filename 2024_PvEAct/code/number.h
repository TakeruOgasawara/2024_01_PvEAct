//===============================================================================
//
// [number.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _NUMBER_H_				//このマクロ定義がされていなかったら
#define _NUMBER_H_				//2重インクルード防止のマクロを定義

#include "main.h"
#include "object.h"

//前方宣言
class CUI;

//ナンバークラス
class CNumber
{
private:
	struct SInfo
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		//int nIdxTexture;	// テクスチャ番号
	};

public:
	CNumber();
	~CNumber();

	static CNumber *Create(D3DXVECTOR3 pos);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPosition(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 rot);
	void SetColor(D3DXCOLOR col);
	void SetSize(float fWidth, float fHeight);
	void SetTex(D3DXVECTOR2 left, D3DXVECTOR2 right);
	void BindTexture(const char* pTexPath);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);

private:
	SInfo m_info;
	CUI *m_pNumberUI;
};

#endif