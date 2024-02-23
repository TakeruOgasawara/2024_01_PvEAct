//===========================================================================================
// 
// [inputKeyboard.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================

#include "inputKeyboard.h"

// 静的メンバ変数宣言
CInputKeyboard* CInputKeyboard::m_pKeyboard = nullptr;

//========================================================================
// キーボードのコンストラクタ
//========================================================================
CInputKeyboard::CInputKeyboard()
{
	// 値のクリア
	ZeroMemory(&m_input, sizeof(m_input));
}

//========================================================================
// キーボードのデストラクタ
//========================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//========================================================================
// インスタンス生成
//========================================================================
CInputKeyboard* CInputKeyboard::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pKeyboard == nullptr)
	{
		m_pKeyboard = new CInputKeyboard;

		if (m_pKeyboard != nullptr)
		{
			m_pKeyboard->Init(hInstance, hWnd);

			return m_pKeyboard;
		}
	}

	return m_pKeyboard;
}

//========================================================================
// インスタンス取得
//========================================================================
CInputKeyboard* CInputKeyboard::GetInstance(void)
{
	if (m_pKeyboard == nullptr)
	{
		assert(("キー情報の取得に失敗", false));
	}

	return m_pKeyboard;

}

//========================================================================
// インスタンス破棄
//========================================================================
void CInputKeyboard::Release(void)
{
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();

		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}
}

//========================================================================
// 初期化処理
//========================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 親クラスの初期化
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	// 強調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//========================================================================
// 終了処理
//========================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//========================================================================
// 更新処理
//========================================================================
void CInputKeyboard::Update(void)
{
	// 変数宣言
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey = 0;


	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_input.aKeyTrigger[nCntKey] = (m_input.aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	// トリガー情報を保存
			m_input.aKeyRelease[nCntKey] = (m_input.aKeyState[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];	// リリース情報を保存

			m_input.aKeyState[nCntKey] = aKeyState[nCntKey];	// キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevice->Acquire();	// キーボードへのアクセス権を獲得
	}
}

//========================================================================
// プレス情報を取得
//========================================================================
bool CInputKeyboard::GetPress(int nKey) const
{
	return (m_input.aKeyState[nKey] & 0x80) ? true : false;
}

//========================================================================
// トリガー情報を取得
//========================================================================
bool CInputKeyboard::GetTrigger(int nKey) const
{
	return (m_input.aKeyTrigger[nKey] & 0x80) ? true : false;
}

//========================================================================
// リリース情報を取得
//========================================================================
bool CInputKeyboard::GetRelease(int nKey) const
{
	return (m_input.aKeyRelease[nKey] & 0x80) ? true : false;
}
