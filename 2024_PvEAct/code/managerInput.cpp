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

// �ÓI�����o�ϐ��錾
CManagerInput* CManagerInput::m_pManager = nullptr;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CManagerInput::CManagerInput()
{
	
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CManagerInput::~CManagerInput()
{
	
}

//===========================================================================================
// �C���X�^���X����
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
// �C���X�^���X�̎擾
//===========================================================================================
CManagerInput* CManagerInput::GetInstance()
{
	if (m_pManager == nullptr)
	{
		assert(("�C���v�b�g�}�l�[�W���[�̎擾�Ɏ��s", false));
	}

	return m_pManager;
}

//===========================================================================================
// �j��
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
// ����������
//===========================================================================================
HRESULT CManagerInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �C���X�^���X�̐���
	CInputKeyboard::Create(hInstance, hWnd);
	CInputJoypad::Create();
	CInputMouse::Create(hInstance, hWnd);

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CManagerInput::Uninit()
{
	// �C���X�^���X�̔j��
	CInputKeyboard::Release();
	CInputJoypad::Release();
	CInputMouse::Release();
}

//===========================================================================================
// �X�V����
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
