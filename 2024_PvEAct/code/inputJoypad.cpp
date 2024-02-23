//===========================================================================================
// 
// [.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================

#include "inputJoypad.h"

// 静的メンバ変数宣言
CInputJoypad* CInputJoypad::m_pJoyPad = nullptr;

//========================================================================
// コンストラクタ
//========================================================================
CInputJoypad::CInputJoypad()
{
	// 値のクリア
	ZeroMemory(&m_input, sizeof(m_input));
}

//========================================================================
// デストラクタ
//========================================================================
CInputJoypad::~CInputJoypad()
{

}

//========================================================================
// インスタンス生成
//========================================================================
CInputJoypad* CInputJoypad::Create(void)
{
	if (m_pJoyPad == nullptr)
	{
		m_pJoyPad = new CInputJoypad;

		m_pJoyPad->Init();

		return m_pJoyPad;
	}

	return m_pJoyPad;
}

//========================================================================
// インスタンス取得
//========================================================================
CInputJoypad* CInputJoypad::GetInstance(void)
{
	if (m_pJoyPad == nullptr)
	{
		assert(("パッド情報の取得に失敗", false));
	}

	return m_pJoyPad;
}

//========================================================================
// インスタンス破棄
//========================================================================
void CInputJoypad::Release(void)
{
	if (m_pJoyPad != nullptr)
	{
		m_pJoyPad->Uninit();

		delete m_pJoyPad;
		m_pJoyPad = nullptr;
	}
}

//========================================================================
// 初期化処理
//========================================================================
HRESULT CInputJoypad::Init(void)
{
	// 変数宣言
	int nCntPad;

	// XInputのステート(使う状態)
	XInputEnable(true);

	// メモリの初期化(プレイヤー分)
	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		memset(&m_input.aState[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&m_input.aStateTrigger[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&m_input.aStateRelease[nCntPad], 0, sizeof(XINPUT_STATE));
	}

	// ないとできない
	return S_OK;
}

//========================================================================
// 終了処理
//========================================================================
void CInputJoypad::Uninit(void)
{
	// XInputのステート(使わない状態)
	XInputEnable(false);
}

//========================================================================
// 更新処理
//========================================================================
void CInputJoypad::Update(void)
{
	XINPUT_STATE aGamePadState;	// ゲームパッドの入力情報

	for (int nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		// 入力デバイスからデータを取得
		if (XInputGetState(nCntPad, &aGamePadState) == ERROR_SUCCESS)
		{
			// ゲームパッドのトリガー情報を保存
			m_input.aStateTrigger[nCntPad].Gamepad.wButtons =
				(m_input.aState[nCntPad].Gamepad.wButtons ^ aGamePadState.Gamepad.wButtons) & aGamePadState.Gamepad.wButtons;

			// リピート情報
			m_input.aStateRepeat[nCntPad].Gamepad.wButtons =
				(m_input.aState[nCntPad].Gamepad.wButtons & aGamePadState.Gamepad.wButtons);

			// リリース情報
			m_input.aStateRelease[nCntPad].Gamepad.wButtons =
				(m_input.aState[nCntPad].Gamepad.wButtons | aGamePadState.Gamepad.wButtons) ^ aGamePadState.Gamepad.wButtons;

			// ゲームパッドのプレス情報を保存
			m_input.aState[nCntPad] = aGamePadState;
		}
	}
}

//========================================================================
// プレス情報を取得
//========================================================================
bool CInputJoypad::GetPress(JOYKEY nKey, int nPlayer)
{
	return (m_input.aState[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//========================================================================
// トリガー
//========================================================================
bool CInputJoypad::GetTrigger(JOYKEY nKey, int nPlayer)
{
	return (m_input.aStateTrigger[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//========================================================================
// リリース
//========================================================================
bool CInputJoypad::GetRelease(JOYKEY nKey, int nPlayer)
{
	return (m_input.aStateRelease[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//========================================================================
// 左スティック
//========================================================================
float CInputJoypad::GetJoyStickLX(int nPlayer) const
{
	return m_input.aState[nPlayer].Gamepad.sThumbLX / (float)SHRT_MAX;
}

//========================================================================
// 左スティック
//========================================================================
float CInputJoypad::GetJoyStickLY(int nPlayer) const
{
	return m_input.aState[nPlayer].Gamepad.sThumbLY / (float)SHRT_MAX;
}

//========================================================================
// 右スティック
//========================================================================
float CInputJoypad::GetJoyStickRX(int nPlayer) const
{
	return m_input.aState[nPlayer].Gamepad.sThumbRX / (float)SHRT_MAX;
}

//========================================================================
// 右スティック
//========================================================================
float CInputJoypad::GetJoyStickRY(int nPlayer) const
{
	return m_input.aState[nPlayer].Gamepad.sThumbRY / (float)SHRT_MAX;
}
