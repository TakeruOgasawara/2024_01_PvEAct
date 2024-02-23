//==================================
// }=========================================================
//
// �^�C�g�����[title.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "inputKeyboard.h"
#include "inputJoypad.h"
#include "object2D.h"
#include "texture.h"
#include "fade.h"
#include "general.h"

// �萔�錾
namespace
{
	static const float FADE_MAX = 1.0f;	// �ő�l
	static const float FADE_MIN = 0.0f;	// �ŏ��l
	static const float FADE_VALUE = 0.01f;	// ������
}

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CTitle::CTitle()
{
	ZeroMemory(&m_ui, sizeof(m_ui));
	m_state = STATE_NONE;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CTitle::~CTitle()
{

}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CTitle::Init()
{
	// ����
	CreateBg();
	CreatePress();

	m_state = STATE_OUT;

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CTitle::Uninit()
{
	ZeroMemory(&m_ui, sizeof(m_ui));
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CTitle::Update()
{
	CInputKeyboard *pInputKey = CInputKeyboard::GetInstance();
	CInputJoypad *pJoyPad = CInputJoypad::GetInstance();

	// �v���XUI�̍X�V
	UpdatePress();

	if (pInputKey != nullptr)
	{
		if (pInputKey->GetPress(DIK_1)/* || pJoyPad->GetTrigger(pJoyPad->BUTTON_A, 0)*/)
		{
			return;
		}
	}

	if (pInputKey->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->BUTTON_A, 0))
	{
		CManager::GetInstance()->GetFade()->Set(MODE_GAME);
	}

	// �V�[���̍X�V
	CScene::Update();
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CTitle::Draw()
{
	CScene::Draw();
}

//===========================================================================================
// �w�i�̐���
//===========================================================================================
void CTitle::CreateBg(void)
{
	CTexture* pTexture = CTexture::GetInstance();

	m_ui.pBg = CObject2D::Create();

	if (m_ui.pBg != nullptr)
	{
		//m_ui.pBg->BindTexture(pTexture->Regist("data\\TEXTURE\\title\\00.png"));
		m_ui.pBg->SetColor({ 0.0f, 0.5f, 0.0f, 1.0f });
		m_ui.pBg->SetSize(1280.0f, 720.0f);
		m_ui.pBg->SetVertex_center();
	}
}

//===========================================================================================
// �v���X�����̐���
//===========================================================================================
void CTitle::CreatePress(void)
{
	CTexture* pTexture = CTexture::GetInstance();

	m_ui.pPress = CObject2D::Create();

	if (m_ui.pPress != nullptr)
	{
		m_ui.pPress->BindTexture(pTexture->Regist("data\\TEXTURE\\title\\00.png"));
		m_ui.pPress->SetPosition({ SCREEN_WIDTH * 0.5f, 550.0f, 0.0f });
		m_ui.pPress->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
		m_ui.pPress->SetSize(600.0f, 100.0f);
		m_ui.pPress->SetVertex_center();
	}
}

//===========================================================================================
// �e�L�X�g�̍X�V
//===========================================================================================
void CTitle::UpdatePress(void)
{
	D3DXCOLOR col = {};

	if (m_ui.pPress != nullptr)
	{
		// �F�̎擾
		col = m_ui.pPress->GetColor();

		if (m_state == STATE_IN)
		{// �t�F�[�h�C�����

			col.a -= FADE_VALUE;	// �|���S���𓧖��ɂ��Ă���

			if (col.a <= FADE_MIN)
			{
				col.a = FADE_MIN;
				m_state = STATE_OUT;	// �������Ă��Ȃ����
			}
		}
		else if (m_state == STATE_OUT)
		{// �t�F�[�h�A�E�g���

			col.a += FADE_VALUE;	// �|���S����s�����ɂ��Ă���

			if (col.a >= FADE_MAX)
			{
				col.a = FADE_MAX;
				m_state = STATE_IN;	// �������Ă��Ȃ����
			}
		}

		// �F�̔��f
		m_ui.pPress->SetColor(col);
		m_ui.pPress->SetVertex_center();
	}
}

