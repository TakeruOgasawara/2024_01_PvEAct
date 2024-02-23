//===============================================================================
//
// キーボードの入力処理[inputKeyboard.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _INPUTKEYBOARD_H_	// このマクロ定義がされていなかったら
#define _INPUTKEYBOARD_H_	// 2重インクルード防止のマクロを定義

#include "input.h"

//***************************************************
// キーボードクラス
//***************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	static CInputKeyboard* Create(HINSTANCE hInstance, HWND hWnd);
	static CInputKeyboard* GetInstance(void);
	static void Release(void);

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey) const;
	bool GetTrigger(int nKey) const;
	bool GetRelease(int nKey) const;

private:
	struct SInput
	{
		BYTE aKeyState[NUM_KEY_MAX];	// プレス
		BYTE aKeyTrigger[NUM_KEY_MAX];	// トリガー
		BYTE aKeyRelease[NUM_KEY_MAX];	// リリース
	};
	SInput m_input;
	
	static CInputKeyboard* m_pKeyboard;
};

#endif