//===========================================================================================
//
// ���U���g���[title.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "inputKeyboard.h"
#include "inputJoypad.h"
#include "object2D.h"
#include "texture.h"
#include "fade.h"

//�ÓI�����o�ϐ��錾
int CResult::m_nTextureIdx = 0;
CObject2D *CResult::m_pObject2D = nullptr;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CResult::CResult()
{
	m_pObject2D = nullptr;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CResult::~CResult()
{

}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CResult::Init()
{
	CTexture* pTexture = CTexture::GetInstance();

	//m_pObject2D = CObject2D::Create({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f });
	//m_pObject2D->BindTexture(pTexture->Regist("data\\TEXTURE\\result\\result-1.png"));
	
	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CResult::Uninit()
{
	
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CResult::Update()
{
	CInputKeyboard *pInputKey = CInputKeyboard::GetInstance();
	CInputJoypad *pJoyPad = CInputJoypad::GetInstance();

	CScene::Update();

	if (pInputKey->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->BUTTON_A, 0))
	{
		CManager::GetInstance()->GetFade()->Set(MODE_TITLE);
	}
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CResult::Draw()
{
	CScene::Draw();
}

