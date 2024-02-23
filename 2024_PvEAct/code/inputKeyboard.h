//===============================================================================
//
// �L�[�{�[�h�̓��͏���[inputKeyboard.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _INPUTKEYBOARD_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _INPUTKEYBOARD_H_	// 2�d�C���N���[�h�h�~�̃}�N�����`

#include "input.h"

//***************************************************
// �L�[�{�[�h�N���X
//***************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	static CInputKeyboard* Create(HINSTANCE hInstance, HWND hWnd);
	static CInputKeyboard* GetInstance(void);
	static void Release(void);

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey) const;
	bool GetTrigger(int nKey) const;
	bool GetRelease(int nKey) const;

private:
	struct SInput
	{
		BYTE aKeyState[NUM_KEY_MAX];	// �v���X
		BYTE aKeyTrigger[NUM_KEY_MAX];	// �g���K�[
		BYTE aKeyRelease[NUM_KEY_MAX];	// �����[�X
	};
	SInput m_input;
	
	static CInputKeyboard* m_pKeyboard;
};

#endif