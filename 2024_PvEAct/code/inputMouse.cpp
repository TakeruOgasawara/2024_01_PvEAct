//===========================================================================================
// 
// [.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================

#include "inputMouse.h"

// 前方宣言
CInputMouse* CInputMouse::m_pMouse = nullptr;

//========================================================================
// コンストラクタ
//========================================================================
CInputMouse::CInputMouse()
{
	// 値のクリア
	ZeroMemory(&m_input, sizeof(m_input));
}

//========================================================================
// デストラクタ
//========================================================================
CInputMouse::~CInputMouse()
{

}

//========================================================================
// インスタンス生成
//========================================================================
CInputMouse* CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pMouse == nullptr)
	{
		m_pMouse = new CInputMouse;

		m_pMouse->Init(hInstance, hWnd);

		return m_pMouse;
	}

	return m_pMouse;
}

//========================================================================
// インスタンス取得
//========================================================================
CInputMouse* CInputMouse::GetInstance(void)
{
	if (m_pMouse == nullptr)
	{
		assert(("マウス情報の取得に失敗", false));
	}

	return m_pMouse;
}

//========================================================================
// インスタンス破棄
//========================================================================
void CInputMouse::Release(void)
{
	if (m_pMouse != nullptr)
	{
		m_pMouse->Uninit();

		delete m_pMouse;
		m_pMouse = nullptr;
	}
}

//========================================================================
//　マウスの初期化処理
//========================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//入力デバイス(マウス)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}
	//強調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;			//相対値モードで設定 (絶対値はDIPROPAXISMODE_ABS)

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		//デバイスの設定に失敗
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//========================================================================
// マウスの終了処理
//========================================================================
void CInputMouse::Uninit(void)
{
	//入力デバイス(マウス)の破棄
	CInput::Uninit();
}

//========================================================================
// マウスの更新処理
//========================================================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 mouse;			//マウスの入力情報

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(mouse), &mouse)))
	{
		for (int nCnt = 0; nCnt < NUM_MOUCE_MAX; nCnt++)
		{
			//トリガー情報の取得
			m_input.mouseStateTrigger.rgbButtons[nCnt] = (m_input.mouseState.rgbButtons[nCnt] ^ mouse.rgbButtons[nCnt]) & mouse.rgbButtons[nCnt];		//マウスのトリガー情報を保存

			m_input.mouseStatePress.rgbButtons[nCnt] = m_input.mouseState.rgbButtons[nCnt];			//マウスのプレス情報を保存
		}

		//情報の取得
		m_input.mouseStatePress = mouse;

		//入力情報の保存
		m_input.mouseState = mouse;
	}
	else
	{
		m_pDevice->Acquire();				//キーボードへのアクセス権を獲得
	}
}

//========================================================================
// マウスのプレス情報を取得
//========================================================================
bool CInputMouse::GetPress(int nKey) const
{
	return (m_input.mouseStatePress.rgbButtons[nKey] & 0x80) ? true : false;
}

//========================================================================
// マウスのトリガー情報を取得
//========================================================================
bool CInputMouse::GetTrigger(int nKey) const
{
	return (m_input.mouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//========================================================================
// マウスのリリース情報を取得
//========================================================================
bool CInputMouse::GetRelease(int nKey) const
{
	return (m_input.mouseStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//========================================================================
// マウスの移動量を取得
//========================================================================
D3DXVECTOR3 CInputMouse::GetMove(void) const
{
	return D3DXVECTOR3((float)m_input.mouseState.lX, (float)m_input.mouseState.lY, (float)m_input.mouseState.lZ);
}
