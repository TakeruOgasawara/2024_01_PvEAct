//======================================================================================
//
//フェード処理[Bg.cpp]
//Author;小笠原　彪
//
//======================================================================================
#include "game.h"
#include "pause.h"
#include "inputKeyboard.h"
#include "inputJoypad.h"
#include "fade.h"
#include "texture.h"

//マクロ定義
#define SIZE_X			(100.0f)
#define SIZE_Y			(50.0f)

//======================================================================================
// 
//======================================================================================
CPause::CPause()
{
	m_menu = MENU_CONTINUE;
	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{
		m_aMenuInfo[nCntMenu] = {};
	}
	m_fColor_a = 0.0f;
	m_nSelect = 0;
	m_bUse = false;
}

//======================================================================================
// 
//======================================================================================
CPause::~CPause()
{

}

//======================================================================================
// 
//======================================================================================
CPause * CPause::Create(void)
{
	CPause *pPause = nullptr;

	pPause = new CPause;

	if (pPause != nullptr)
	{
		pPause->Init();

		return pPause;
	}
	else
	{
		return nullptr;
	}

	return pPause;
}

//======================================================================================
// 
//======================================================================================
void CPause::Init()
{
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{

		m_aMenuInfo[nCnt].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		// 2Dオブジェクトの生成
		//m_aMenuInfo[nCnt].pObject2D = CObject2D::Create(object::NUM_PRIORITY - 1);

		if (m_aMenuInfo[nCnt].pObject2D != nullptr)
		{
			switch (nCnt)
			{
			case MENU_CONTINUE:
				m_aMenuInfo[nCnt].pObject2D->BindTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\pause_0.png"));
				break;

			case MENU_RETRY:
				m_aMenuInfo[nCnt].pObject2D->BindTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\pause_1.png"));
				break;

			case MENU_QUIT:
				m_aMenuInfo[nCnt].pObject2D->BindTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\pause_2.png"));
				break;
			}
		
			m_aMenuInfo[nCnt].pObject2D->SetPosition(D3DXVECTOR3(640.0f, 200.0f + nCnt * 110.0f, 0.0f));
		}
	}

	m_nSelect = m_menu;

	SelectMenu();
}

//======================================================================================
// 終了処理
//======================================================================================
void CPause::Uninit()
{
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_aMenuInfo[nCnt].pObject2D != nullptr)
		{
			m_aMenuInfo[nCnt].pObject2D->Uninit();
			m_aMenuInfo[nCnt].pObject2D = nullptr;
		}
	}
}

//======================================================================================
// 更新
//======================================================================================
void CPause::Update()
{
	CInputKeyboard *pInputKey = CInputKeyboard::GetInstance();
	CInputJoypad *pjoyPad = CInputJoypad::GetInstance();
	CFade *pFade = CManager::GetInstance()->GetFade();

	if (pFade->GetState() != CFade::STATE_NONE)
	{// 押された状態の場合
		return;
	}

	if (pInputKey->GetTrigger(DIK_P) == true)
	{
		m_bUse = m_bUse ? false : true;
	}

	if (m_bUse == false)	//チェック
	{
		return;
	}

	if (pInputKey->GetTrigger(DIK_W) == true)
	{
		m_nSelect = (m_nSelect - 1 + MENU_MAX) % MENU_MAX;

		m_menu = (MENU)m_nSelect;

		SelectMenu();
	}
	else if (pInputKey->GetTrigger(DIK_S) == true)
	{
		m_nSelect = (m_nSelect + 1 + MENU_MAX) % MENU_MAX;

		m_menu = (MENU)m_nSelect;

		SelectMenu();
	}
	else if (pInputKey->GetTrigger(DIK_RETURN) == true)
	{
		if (pFade == nullptr)
		{
			return;
		}

		switch (m_menu)
		{
		case MENU_CONTINUE:
			m_bUse = false;
			break;
		case MENU_RETRY:
			//SetFadeMode(FADEMODE_NORMAL);
			pFade->Set(CScene::MODE_GAME);
			break;
		case MENU_QUIT:
			//SetFadeMode(FADEMODE_NORMAL);
			pFade->Set(CScene::MODE_TITLE);
			break;
		}
	}

}

//======================================================================================
// 
//======================================================================================
void CPause::Draw()
{

}

//======================================================================================
// 選択しているメニュー
//======================================================================================
void CPause::SelectMenu(void)
{
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_menu == nCnt)
		{//選択しているメニュと合っていたら

			m_aMenuInfo[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			m_aMenuInfo[nCnt].col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		}

		m_aMenuInfo[nCnt].pObject2D->SetColor(m_aMenuInfo[nCnt].col);
	}
}
