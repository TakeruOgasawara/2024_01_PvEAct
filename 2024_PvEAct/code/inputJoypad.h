//===============================================================================
//
// �W���C�p�b�h���͏���[inputJoypad.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _INPUTJOYPAD_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _INPUTJOYPAD_H_	// 2�d�C���N���[�h�h�~�̃}�N�����`

#include "input.h"

//***************************************************
// �W���C�p�b�h�N���X
//***************************************************
class CInputJoypad : public CInput
{
public:
	//�{�^���̎�� 20�ȓ�
	enum JOYKEY
	{
		BUTTON_0 = 0,				// �\���� 
		BUTTON_1,					// �\����
		BUTTON_2,					// �\����
		BUTTON_3,					// �\����
		BUTTON_START,				// start
		BUTTON_BACK,				// back
		BUTTON_LS,					// ���X�e�B�b�N��������
		BUTTON_RS,					// �E�X�e�B�b�N��������
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
		XINPUT_STATE aState[NUM_PLAYER];			//�v���X
		XINPUT_STATE aStateTrigger[NUM_PLAYER];	//�g���K�[
		XINPUT_STATE aStateRelease[NUM_PLAYER];	//�����[�X
		XINPUT_STATE aStateRepeat[NUM_PLAYER];	//���s�[�g
	};

	SInput m_input;

	static CInputJoypad* m_pJoyPad;
};

#endif