//===========================================================================================
//
// リザルト画面[title.cpp]
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

//静的メンバ変数宣言
int CResult::m_nTextureIdx = 0;
CObject2D *CResult::m_pObject2D = nullptr;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CResult::CResult()
{
	m_pObject2D = nullptr;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CResult::~CResult()
{

}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CResult::Init()
{
	CTexture* pTexture = CTexture::GetInstance();

	//m_pObject2D = CObject2D::Create({ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f });
	//m_pObject2D->BindTexture(pTexture->Regist("data\\TEXTURE\\result\\result-1.png"));
	
	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CResult::Uninit()
{
	
}

//===========================================================================================
// 更新処理
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
// 描画処理
//===========================================================================================
void CResult::Draw()
{
	CScene::Draw();
}

