//===============================================================================
//
// [pause.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

class CObject2D;

//ポーズクラス
class CPause
{
private:
	enum MENU
	{
		MENU_CONTINUE = 0,
		MENU_RETRY,
		MENU_QUIT,
		MENU_MAX
	};

	struct MenuInfo
	{
		CObject2D *pObject2D;
		D3DXCOLOR col;
	};

public:
	CPause();
	~CPause();

	static CPause *Create(void);

	//プロトタイプ宣言
	void Init();
	void Uninit();
	void Update();
	void Draw();

	//取得
	bool GetPause(void) { return m_bUse; }

private:
	void SelectMenu(void);

	MENU m_menu;
	MenuInfo m_aMenuInfo[MENU_MAX];
	bool m_bUse;
	int m_nSelect;
	float m_fColor_a;
};

#endif