//===============================================================================
//
// [object2D.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBJECT2DANIM_H_				//このマクロ定義がされていなかったら
#define _OBJECT2DANIM_H_				//2重インクルード帽子のマクロを定義

#include "main.h"
#include "object2D.h"

//アニメーションクラス
class CObject2Danim : public CObject2D
{
public:
	CObject2Danim();
	~CObject2Danim();

	static HRESULT Load(void);
	static CObject2Danim *Create(void);
	static CObject2Danim *Create(D3DXVECTOR3 pos);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetAnimation(int nPttern);
	void SetTexture(float fTexU, float fTexV);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへの頂点情報
	int m_nPttern;
	int m_nCntPttern;
	float m_fTexSizeX;
	float m_fTexSizeY;
};

#endif