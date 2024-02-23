//===============================================================================
//
// [inputManager.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _INPUTMANAGER_H_				//このマクロ定義がされていなかったら
#define _INPUTMANAGER_H_				//2重インクルード防止のマクロを定義

#include "main.h"

// 前方宣言
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;

// 入力マネージャー
class CManagerInput
{
public:
	CManagerInput();
	~CManagerInput();

	static CManagerInput* Create(HINSTANCE hInstance, HWND hWnd);
	static CManagerInput* GetInstance();
	static void Release();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();

private:
	static CManagerInput* m_pManager;
};
#endif