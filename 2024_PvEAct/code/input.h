//===============================================================================
//
// ���͏���[input.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _INPUT_H_				//���̃}�N����`������Ă��Ȃ�������
#define _INPUT_H_				//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"

// �p�b�h���g�����߂ɕK�v
#include "Xinput.h"
#pragma comment(lib, "xinput.lib")

//***************************************************
// �}�N����`
//***************************************************
namespace
{
	const int NUM_KEY_MAX = 256;	//�L�[�̍ő吔
	const int NUM_MOUCE_MAX = 8;	//�}�E�X�{�^���̍ő吔
	const int NUM_PLAYER = 1;	//�v���C���[��
}

//***************************************************
// ���̓N���X
//***************************************************
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

#endif