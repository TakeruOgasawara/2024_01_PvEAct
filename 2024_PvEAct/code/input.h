//===============================================================================
//
// 入力処理[input.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _INPUT_H_				//このマクロ定義がされていなかったら
#define _INPUT_H_				//2重インクルード防止のマクロを定義

#include "main.h"

// パッドを使うために必要
#include "Xinput.h"
#pragma comment(lib, "xinput.lib")

//***************************************************
// マクロ定義
//***************************************************
namespace
{
	const int NUM_KEY_MAX = 256;	//キーの最大数
	const int NUM_MOUCE_MAX = 8;	//マウスボタンの最大数
	const int NUM_PLAYER = 1;	//プレイヤー数
}

//***************************************************
// 入力クラス
//***************************************************
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

#endif