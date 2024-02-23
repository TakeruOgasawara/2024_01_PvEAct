//===========================================================================================
//
//
//
//
//===========================================================================================
#include "managerInput.h"

#include "inputKeyboard.h"
#include "inputJoyPad.h"
#include "inputMouse.h"

// 静的メンバ変数宣言
CManagerInput* CManagerInput::m_pManager = nullptr;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CManagerInput::CManagerInput()
{
	
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CManagerInput::~CManagerInput()
{
	
}

//===========================================================================================
// インスタンス生成
//===========================================================================================
CManagerInput* CManagerInput::Create(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pManager == nullptr)
	{
		m_pManager = new CManagerInput;

		m_pManager->Init(hInstance, hWnd);

		return m_pManager;
	}

	return m_pManager;
}

//===========================================================================================
// インスタンスの取得
//===========================================================================================
CManagerInput* CManagerInput::GetInstance()
{
	if (m_pManager == nullptr)
	{
		assert(("インプットマネージャーの取得に失敗", false));
	}

	return m_pManager;
}

//===========================================================================================
// 破棄
//===========================================================================================
void CManagerInput::Release()
{
	if (m_pManager == nullptr)
	{
		return;
	}
	else
	{
		m_pManager->Uninit();

		delete m_pManager;
		m_pManager = nullptr;
	}
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CManagerInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// インスタンスの生成
	CInputKeyboard::Create(hInstance, hWnd);
	CInputJoypad::Create();
	CInputMouse::Create(hInstance, hWnd);

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CManagerInput::Uninit()
{
	// インスタンスの破棄
	CInputKeyboard::Release();
	CInputJoypad::Release();
	CInputMouse::Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CManagerInput::Update()
{
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();
	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();

	if (pJoypad != nullptr)
	{
		pJoypad->Update();
	}

	if (pKeyboard != nullptr)
	{
		pKeyboard->Update();
	}

	if (pMouse != nullptr)
	{
		pMouse->Update();
	}
}
