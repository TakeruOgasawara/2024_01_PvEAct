//===========================================================================================
// 
// [.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================

#include "inputJoypad.h"

// �ÓI�����o�ϐ��錾
CInputJoypad* CInputJoypad::m_pJoyPad = nullptr;

//========================================================================
// �R���X�g���N�^
//========================================================================
CInputJoypad::CInputJoypad()
{
	// �l�̃N���A
	ZeroMemory(&m_input, sizeof(m_input));
}

//========================================================================
// �f�X�g���N�^
//========================================================================
CInputJoypad::~CInputJoypad()
{

}

//========================================================================
// �C���X�^���X����
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
// �C���X�^���X�擾
//========================================================================
CInputJoypad* CInputJoypad::GetInstance(void)
{
	if (m_pJoyPad == nullptr)
	{
		assert(("�p�b�h���̎擾�Ɏ��s", false));
	}

	return m_pJoyPad;
}

//========================================================================
// �C���X�^���X�j��
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
// ����������
//========================================================================
HRESULT CInputJoypad::Init(void)
{
	// �ϐ��錾
	int nCntPad;

	// XInput�̃X�e�[�g(�g�����)
	XInputEnable(true);

	// �������̏�����(�v���C���[��)
	for (nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		memset(&m_input.aState[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&m_input.aStateTrigger[nCntPad], 0, sizeof(XINPUT_STATE));
		memset(&m_input.aStateRelease[nCntPad], 0, sizeof(XINPUT_STATE));
	}

	// �Ȃ��Ƃł��Ȃ�
	return S_OK;
}

//========================================================================
// �I������
//========================================================================
void CInputJoypad::Uninit(void)
{
	// XInput�̃X�e�[�g(�g��Ȃ����)
	XInputEnable(false);
}

//========================================================================
// �X�V����
//========================================================================
void CInputJoypad::Update(void)
{
	XINPUT_STATE aGamePadState;	// �Q�[���p�b�h�̓��͏��

	for (int nCntPad = 0; nCntPad < NUM_PLAYER; nCntPad++)
	{
		// ���̓f�o�C�X����f�[�^���擾
		if (XInputGetState(nCntPad, &aGamePadState) == ERROR_SUCCESS)
		{
			// �Q�[���p�b�h�̃g���K�[����ۑ�
			m_input.aStateTrigger[nCntPad].Gamepad.wButtons =
				(m_input.aState[nCntPad].Gamepad.wButtons ^ aGamePadState.Gamepad.wButtons) & aGamePadState.Gamepad.wButtons;

			// ���s�[�g���
			m_input.aStateRepeat[nCntPad].Gamepad.wButtons =
				(m_input.aState[nCntPad].Gamepad.wButtons & aGamePadState.Gamepad.wButtons);

			// �����[�X���
			m_input.aStateRelease[nCntPad].Gamepad.wButtons =
				(m_input.aState[nCntPad].Gamepad.wButtons | aGamePadState.Gamepad.wButtons) ^ aGamePadState.Gamepad.wButtons;

			// �Q�[���p�b�h�̃v���X����ۑ�
			m_input.aState[nCntPad] = aGamePadState;
		}
	}
}

//========================================================================
// �v���X�����擾
//========================================================================
bool CInputJoypad::GetPress(JOYKEY nKey, int nPlayer)
{
	return (m_input.aState[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//========================================================================
// �g���K�[
//========================================================================
bool CInputJoypad::GetTrigger(JOYKEY nKey, int nPlayer)
{
	return (m_input.aStateTrigger[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//========================================================================
// �����[�X
//========================================================================
bool CInputJoypad::GetRelease(JOYKEY nKey, int nPlayer)
{
	return (m_input.aStateRelease[nPlayer].Gamepad.wButtons & 0x01 << nKey) ? true : false;
}

//========================================================================
// ���X�e�B�b�N
//========================================================================
float CInputJoypad::GetJoyStickLX(int nPlayer) const
{
	return m_input.aState[nPlayer].Gamepad.sThumbLX / (float)SHRT_MAX;
}

//========================================================================
// ���X�e�B�b�N
//========================================================================
float CInputJoypad::GetJoyStickLY(int nPlayer) const
{
	return m_input.aState[nPlayer].Gamepad.sThumbLY / (float)SHRT_MAX;
}

//========================================================================
// �E�X�e�B�b�N
//========================================================================
float CInputJoypad::GetJoyStickRX(int nPlayer) const
{
	return m_input.aState[nPlayer].Gamepad.sThumbRX / (float)SHRT_MAX;
}

//========================================================================
// �E�X�e�B�b�N
//========================================================================
float CInputJoypad::GetJoyStickRY(int nPlayer) const
{
	return m_input.aState[nPlayer].Gamepad.sThumbRY / (float)SHRT_MAX;
}
