//===============================================================================
//
// マウス入力処理[inputMouse.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _INPUTMOUSE_H_	// このマクロ定義がされていなかったら
#define _INPUTMOUSE_H_	// 2重インクルード防止のマクロを定義

#include "input.h"

//***************************************************
// マウスクラス
//***************************************************
class CInputMouse : CInput
{
public:
	//マウスボタンの種類
	typedef enum
	{
		MOUSE_LEFT = 0,
		MOUSE_RIGHT,
		MOUSE_HWEEL,
		MOUSE_SIDE1,
		MOUSE_SIDE2,
		MOUSE_6,
		MOUSE_7,
		MOUSE_8,
		MOUSE_9,
		MOUSE_10,
		MOUSE_11,
		MOUSE_12,
		MOUSE_13,
		MOUSE_14,
		MOUSE_15,
		MOUSE_16,
		MOUSE_17,
		MOUSE_18,
		MOUSE_MAX
	} MOUSE_BUTTON;

public:
	CInputMouse();	// コンストラクタ
	~CInputMouse();	// デストラクタ

	static CInputMouse* Create(HINSTANCE hInstance, HWND hWnd);
	static CInputMouse* GetInstance(void);
	static void Release(void);

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey) const;
	bool GetTrigger(int nKey) const;
	bool GetRelease(int nKey) const;
	D3DXVECTOR3 GetMove(void) const;

private:
	struct SInput
	{
		DIMOUSESTATE2 mouseState;			// 全入力情報の保管
		DIMOUSESTATE2 mouseStatePress;	// プレス
		DIMOUSESTATE2 mouseStateTrigger;	// トリガー
		DIMOUSESTATE2 mouseStateRelease;	// リリース
		BYTE oldState;					// 前回の入力情報を保存
	};
	SInput m_input;
	static CInputMouse* m_pMouse;
};

#endif