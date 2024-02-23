//===========================================================================================
// 
// [.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================

#include "inputMouse.h"

// �O���錾
CInputMouse* CInputMouse::m_pMouse = nullptr;

//========================================================================
// �R���X�g���N�^
//========================================================================
CInputMouse::CInputMouse()
{
	// �l�̃N���A
	ZeroMemory(&m_input, sizeof(m_input));
}

//========================================================================
// �f�X�g���N�^
//========================================================================
CInputMouse::~CInputMouse()
{

}

//========================================================================
// �C���X�^���X����
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
// �C���X�^���X�擾
//========================================================================
CInputMouse* CInputMouse::GetInstance(void)
{
	if (m_pMouse == nullptr)
	{
		assert(("�}�E�X���̎擾�Ɏ��s", false));
	}

	return m_pMouse;
}

//========================================================================
// �C���X�^���X�j��
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
//�@�}�E�X�̏���������
//========================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//���̓f�o�C�X(�}�E�X)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;			//���Βl���[�h�Őݒ� (��Βl��DIPROPAXISMODE_ABS)

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		//�f�o�C�X�̐ݒ�Ɏ��s
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//========================================================================
// �}�E�X�̏I������
//========================================================================
void CInputMouse::Uninit(void)
{
	//���̓f�o�C�X(�}�E�X)�̔j��
	CInput::Uninit();
}

//========================================================================
// �}�E�X�̍X�V����
//========================================================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 mouse;			//�}�E�X�̓��͏��

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(mouse), &mouse)))
	{
		for (int nCnt = 0; nCnt < NUM_MOUCE_MAX; nCnt++)
		{
			//�g���K�[���̎擾
			m_input.mouseStateTrigger.rgbButtons[nCnt] = (m_input.mouseState.rgbButtons[nCnt] ^ mouse.rgbButtons[nCnt]) & mouse.rgbButtons[nCnt];		//�}�E�X�̃g���K�[����ۑ�

			m_input.mouseStatePress.rgbButtons[nCnt] = m_input.mouseState.rgbButtons[nCnt];			//�}�E�X�̃v���X����ۑ�
		}

		//���̎擾
		m_input.mouseStatePress = mouse;

		//���͏��̕ۑ�
		m_input.mouseState = mouse;
	}
	else
	{
		m_pDevice->Acquire();				//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//========================================================================
// �}�E�X�̃v���X�����擾
//========================================================================
bool CInputMouse::GetPress(int nKey) const
{
	return (m_input.mouseStatePress.rgbButtons[nKey] & 0x80) ? true : false;
}

//========================================================================
// �}�E�X�̃g���K�[�����擾
//========================================================================
bool CInputMouse::GetTrigger(int nKey) const
{
	return (m_input.mouseStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//========================================================================
// �}�E�X�̃����[�X�����擾
//========================================================================
bool CInputMouse::GetRelease(int nKey) const
{
	return (m_input.mouseStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}

//========================================================================
// �}�E�X�̈ړ��ʂ��擾
//========================================================================
D3DXVECTOR3 CInputMouse::GetMove(void) const
{
	return D3DXVECTOR3((float)m_input.mouseState.lX, (float)m_input.mouseState.lY, (float)m_input.mouseState.lZ);
}
