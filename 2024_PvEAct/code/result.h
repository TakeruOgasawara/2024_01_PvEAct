//===============================================================================
//
// リザルト画面[result.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _RESULT_H_				//このマクロ定義がされていなかったら
#define _RESULT_H_				//2重インクルード防止のマクロを定義

#include "main.h"
#include "manager.h"

//前方宣言
class CObject2D;

//タイトルクラス
class CResult : public CScene
{
public:
	CResult();
	~CResult();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
	static int m_nTextureIdx;			//テクスチャへの頂点情報
	static CObject2D *m_pObject2D;
};

#endif