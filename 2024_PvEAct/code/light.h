//===============================================================================
//
// [light.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _LIGHT_H_				//このマクロ定義がされていなかったら
#define _LIGHT_H_				//2重インクルード防止のマクロを定義

#include "main.h"

//マクロ定義
#define MAX_LIGHT			(3)			//ライトの最大数

//ライトクラス
class CLight
{
public:
	CLight();			//コンストラクタ
	~CLight();			//デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DLIGHT9 m_aLight[MAX_LIGHT];
};

#endif