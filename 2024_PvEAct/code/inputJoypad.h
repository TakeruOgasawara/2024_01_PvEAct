//===============================================================================
//
// ジョイパッド入力処理[inputJoypad.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _INPUTJOYPAD_H_	// このマクロ定義がされていなかったら
#define _INPUTJOYPAD_H_	// 2重インクルード防止のマクロを定義

#include "input.h"

//***************************************************
// ジョイパッドクラス
//***************************************************
class CInputJoypad : public CInput
{
public:
	//ボタンの種類 20以内
	enum JOYKEY
	{
		BUTTON_0 = 0,				// 十字↑ 
		BUTTON_1,					// 十字↓
		BUTTON_2,					// 十字←
		BUTTON_3,					// 十字→
		BUTTON_START,				// start
		BUTTON_BACK,				// back
		BUTTON_LS,					// 左スティック押し込み
		BUTTON_RS,					// 右スティック押し込み
		BUTTON_LB,					// LB/5
		BUTTON_RB,					// RB/6
		BUTTON_10,					// ?
		BUTTON_11,					// ?
		BUTTON_A,					// A/3 
		BUTTON_B,					// B/4
		BUTTON_X,					// X/1
		BUTTON_Y,					// Y/2
		BUTTON_16,					// ?
		BUTTON_17,					// ?
		BUTTON_18,					// ?
		BUTTON_19,					// ?
		BUTTON_20,					// ?
		BUTTON_MAX
	};

public:
	CInputJoypad();
	~CInputJoypad();

	static CInputJoypad* Create(void);
	static CInputJoypad* GetInstance(void);
	static void Release(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	bool GetPress(JOYKEY nKey, int nPlayer);
	bool GetTrigger(JOYKEY nKey, int nPlayer);
	bool GetRelease(JOYKEY nKey, int nPlayer);

	float GetJoyStickLX(int nPlayer) const;
	float GetJoyStickLY(int nPlayer) const;
	float GetJoyStickRX(int nPlayer) const;
	float GetJoyStickRY(int nPlayer) const;

private:

	struct SInput
	{
		XINPUT_STATE aState[NUM_PLAYER];			//プレス
		XINPUT_STATE aStateTrigger[NUM_PLAYER];	//トリガー
		XINPUT_STATE aStateRelease[NUM_PLAYER];	//リリース
		XINPUT_STATE aStateRepeat[NUM_PLAYER];	//リピート
	};

	SInput m_input;

	static CInputJoypad* m_pJoyPad;
};

#endif