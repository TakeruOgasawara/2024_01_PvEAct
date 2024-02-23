//===============================================================================
//
// ロックオン処理[lockOn.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _LOCKON_H_
#define _LOCKON_H_

#include "main.h"
#include "billboard.h"

// ロックオン
class CLockon : public CBillboard
{
public:
	CLockon();	// コンストラクタ
	~CLockon();	// デストラクタ

	static CLockon* Create(D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f });
	static CLockon* Create(const char* cTexPath, D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f});

	// プロトタイプ宣言
	HRESULT Init();
	HRESULT Init(D3DXVECTOR3 pos);
	HRESULT Init(const char* cTexPath, D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

private:
};

#endif