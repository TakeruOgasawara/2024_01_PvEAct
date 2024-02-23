//===========================================================================================
// 
// UI処理[UI.h]
// Author : Takeru Ogasawara
// 
//===========================================================================================
#ifndef _UI_H_
#define _UI_H_

#include "object2D.h"

// UIクラス
class CUI : public CObject2D
{
public:
	CUI(int nPriority = 7);	// コンストラクタ
	~CUI();	// デストラクタ

	static CUI* Create();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
};

#endif // !_LIFE_H_