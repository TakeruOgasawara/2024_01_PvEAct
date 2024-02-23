//===============================================================================
//
// [inputManager.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _INPUTMANAGER_H_				//���̃}�N����`������Ă��Ȃ�������
#define _INPUTMANAGER_H_				//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"

// �O���錾
class CInputKeyboard;
class CInputJoypad;
class CInputMouse;

// ���̓}�l�[�W���[
class CManagerInput
{
public:
	CManagerInput();
	~CManagerInput();

	static CManagerInput* Create(HINSTANCE hInstance, HWND hWnd);
	static CManagerInput* GetInstance();
	static void Release();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();

private:
	static CManagerInput* m_pManager;
};
#endif