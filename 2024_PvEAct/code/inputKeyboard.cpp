//===========================================================================================
// 
// [inputKeyboard.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================

#include "inputKeyboard.h"

// �ÓI�����o�ϐ��錾
CInputKeyboard* CInputKeyboard::m_pKeyboard = nullptr;

//========================================================================
// �L�[�{�[�h�̃R���X�g���N�^
//========================================================================
CInputKeyboard::CInputKeyboard()
{
	// �l�̃N���A
	ZeroMemory(&m_input, sizeof(m_input));
}

//========================================================================
// �L�[�{�[�h�̃f�X�g���N�^
//========================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//========================================================================
// �C���X�^���X����
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
// �C���X�^���X�擾
//========================================================================
CInputKeyboard* CInputKeyboard::GetInstance(void)
{
	if (m_pKeyboard == nullptr)
	{
		assert(("�L�[���̎擾�Ɏ��s", false));
	}

	return m_pKeyboard;

}

//========================================================================
// �C���X�^���X�j��
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
// ����������
//========================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �e�N���X�̏�����
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}
	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//========================================================================
// �I������
//========================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//========================================================================
// �X�V����
//========================================================================
void CInputKeyboard::Update(void)
{
	// �ϐ��錾
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey = 0;


	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_input.aKeyTrigger[nCntKey] = (m_input.aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	// �g���K�[����ۑ�
			m_input.aKeyRelease[nCntKey] = (m_input.aKeyState[nCntKey] | aKeyState[nCntKey]) ^ aKeyState[nCntKey];	// �����[�X����ۑ�

			m_input.aKeyState[nCntKey] = aKeyState[nCntKey];	// �L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//========================================================================
// �v���X�����擾
//========================================================================
bool CInputKeyboard::GetPress(int nKey) const
{
	return (m_input.aKeyState[nKey] & 0x80) ? true : false;
}

//========================================================================
// �g���K�[�����擾
//========================================================================
bool CInputKeyboard::GetTrigger(int nKey) const
{
	return (m_input.aKeyTrigger[nKey] & 0x80) ? true : false;
}

//========================================================================
// �����[�X�����擾
//========================================================================
bool CInputKeyboard::GetRelease(int nKey) const
{
	return (m_input.aKeyRelease[nKey] & 0x80) ? true : false;
}
