//===============================================================================
//
// �}�E�X���͏���[inputMouse.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _INPUTMOUSE_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _INPUTMOUSE_H_	// 2�d�C���N���[�h�h�~�̃}�N�����`

#include "input.h"

//***************************************************
// �}�E�X�N���X
//***************************************************
class CInputMouse : CInput
{
public:
	//�}�E�X�{�^���̎��
	typedef enum
	{
		MOUSE_LEFT = 0,
		MOUSE_RIGHT,
		MOUSE_HWEEL,
		MOUSE_SIDE1,
		MOUSE_SIDE2,
		MOUSE_6,
		MOUSE_7,
		MOUSE_8,
		MOUSE_9,
		MOUSE_10,
		MOUSE_11,
		MOUSE_12,
		MOUSE_13,
		MOUSE_14,
		MOUSE_15,
		MOUSE_16,
		MOUSE_17,
		MOUSE_18,
		MOUSE_MAX
	} MOUSE_BUTTON;

public:
	CInputMouse();	// �R���X�g���N�^
	~CInputMouse();	// �f�X�g���N�^

	static CInputMouse* Create(HINSTANCE hInstance, HWND hWnd);
	static CInputMouse* GetInstance(void);
	static void Release(void);

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey) const;
	bool GetTrigger(int nKey) const;
	bool GetRelease(int nKey) const;
	D3DXVECTOR3 GetMove(void) const;

private:
	struct SInput
	{
		DIMOUSESTATE2 mouseState;			// �S���͏��̕ۊ�
		DIMOUSESTATE2 mouseStatePress;	// �v���X
		DIMOUSESTATE2 mouseStateTrigger;	// �g���K�[
		DIMOUSESTATE2 mouseStateRelease;	// �����[�X
		BYTE oldState;					// �O��̓��͏���ۑ�
	};
	SInput m_input;
	static CInputMouse* m_pMouse;
};

#endif