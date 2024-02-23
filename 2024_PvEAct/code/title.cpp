//==================================
// }=========================================================
//
// タイトル画面[title.cpp]
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

// 定数宣言
namespace
{
	static const float FADE_MAX = 1.0f;	// 最大値
	static const float FADE_MIN = 0.0f;	// 最小値
	static const float FADE_VALUE = 0.01f;	// 増減量
}

//===========================================================================================
// コンストラクタ
//===========================================================================================
CTitle::CTitle()
{
	ZeroMemory(&m_ui, sizeof(m_ui));
	m_state = STATE_NONE;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CTitle::~CTitle()
{

}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CTitle::Init()
{
	// 生成
	CreateBg();
	CreatePress();

	m_state = STATE_OUT;

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CTitle::Uninit()
{
	ZeroMemory(&m_ui, sizeof(m_ui));
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CTitle::Update()
{
	CInputKeyboard *pInputKey = CInputKeyboard::GetInstance();
	CInputJoypad *pJoyPad = CInputJoypad::GetInstance();

	// プレスUIの更新
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

	// シーンの更新
	CScene::Update();
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CTitle::Draw()
{
	CScene::Draw();
}

//===========================================================================================
// 背景の生成
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
// プレス文字の生成
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
// テキストの更新
//===========================================================================================
void CTitle::UpdatePress(void)
{
	D3DXCOLOR col = {};

	if (m_ui.pPress != nullptr)
	{
		// 色の取得
		col = m_ui.pPress->GetColor();

		if (m_state == STATE_IN)
		{// フェードイン状態

			col.a -= FADE_VALUE;	// ポリゴンを透明にしていく

			if (col.a <= FADE_MIN)
			{
				col.a = FADE_MIN;
				m_state = STATE_OUT;	// 何もしていない状態
			}
		}
		else if (m_state == STATE_OUT)
		{// フェードアウト状態

			col.a += FADE_VALUE;	// ポリゴンを不透明にしていく

			if (col.a >= FADE_MAX)
			{
				col.a = FADE_MAX;
				m_state = STATE_IN;	// 何もしていない状態
			}
		}

		// 色の反映
		m_ui.pPress->SetColor(col);
		m_ui.pPress->SetVertex_center();
	}
}

