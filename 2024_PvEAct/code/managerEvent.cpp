#include "managerEvent.h"
#include "object.h"

#include "debugproc.h"

#include "inputKeyboard.h"
#include "fade.h"
#include "pause.h"

#include "player.h"
#include "enemyBoss.h"

#include "frame.h"
#include "managerSlow.h"
#include "texture.h"
#include "managerUI.h"

// 定数定義
namespace
{
	static const float SLOW_TIME = 10.0f;	// スロー時間
	static const int RUSH_TIME = 10;	// ラッシュ時間
}

// 静的メンバ変数
CManagerEvent* CManagerEvent::m_pManager = nullptr;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CManagerEvent::CManagerEvent()
{
	m_mode = MODE_NORMAL;
	m_fSlowTime = 0.0f;
	m_nRushTime = 0;
	m_pSlowManager = nullptr;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CManagerEvent::~CManagerEvent()
{

}

//===========================================================================================
// インスタンス生成
//===========================================================================================
CManagerEvent* CManagerEvent::Create(void)
{
	if (m_pManager == nullptr)
	{
		m_pManager = new CManagerEvent;

		m_pManager->Init();

		return m_pManager;
	}

	return m_pManager;
}

//===========================================================================================
// インスタンス取得
//===========================================================================================
CManagerEvent* CManagerEvent::GetInstance(void)
{
	if (m_pManager == nullptr)
	{
		//assert(("イベントマネージャの取得に失敗", false));
	}

	return m_pManager;
}

//===========================================================================================
// インスタンスの破棄
//===========================================================================================
void CManagerEvent::Release(void)
{
	if (m_pManager != nullptr)
	{
		m_pManager->Uninit();
		delete m_pManager;
		m_pManager = nullptr;
	}
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CManagerEvent::Init(void)
{
	// スローマネージャの生成
	m_pSlowManager = CSlowManager::Create();

	m_fSlowTime = SLOW_TIME;
	m_nRushTime = RUSH_TIME;
	m_mode = MODE_NORMAL;

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CManagerEvent::Uninit(void)
{
	if (m_pSlowManager != nullptr)
	{
		m_pSlowManager->Uninit();
		m_pSlowManager = nullptr;
	}

 	CObject::Release();

	// UIマネージャの破棄
	//CManagerUI::Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CManagerEvent::Update(void)
{
	// スローの更新
	SlowUpdate();

	// 戦闘の状態
	switch (m_mode)
	{
	case CManagerEvent::EGameMode::MODE_NORMAL:

		break;
	case CManagerEvent::EGameMode::MODE_JUST:

		break;
	case CManagerEvent::EGameMode::MODE_RUSH:
		if (m_ui.pRushUI == nullptr)
		{
			m_ui.pRushUI = CObject2D::Create({ 0.0f, 0.0f, 0.0f });
		}
		else
		{
			m_ui.pRushUI->BindTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\ui_attackLB.png"));
			m_ui.pRushUI->SetPosition({ SCREEN_WIDTH * 0.5f, 600.0f, 0.0f });
		}
		break;

	case CManagerEvent::EGameMode::MODE_JANKEN:
		break;
	default:
		break;
	}
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CManagerEvent::Draw(void)
{

}

//===========================================================================================
// スロー更新
//===========================================================================================
void CManagerEvent::SlowUpdate(void)
{
	if (m_pSlowManager != nullptr)
	{
		CPlayer* pPlayer = CPlayer::GetInstance();
		CEnemyBoss* pBoss = CEnemyBoss::GetInstance();

		if (pPlayer != nullptr)
		{
			if (pPlayer->GetIsJust())
			{// ジャスト回避していた場合

				if (m_fSlowTime > 0.0f)
				{// すべての速度を落とす(登録しているオブジェクトのみ)
					m_fSlowTime -= CManager::GetInstance()->GetTick() * 10.0f;

					m_pSlowManager->All(0.05f);
				}
				else
				{
					m_pSlowManager->Player(1.0f);
					m_pSlowManager->Enemy(0.05f);
					//m_fSlowTime = 0.0f;
				}
			}
		}
	}
}

//===========================================================================================
// 通常
//===========================================================================================
void CManagerEvent::Normal(void)
{

}

//===========================================================================================
// ラッシュ
//===========================================================================================
void CManagerEvent::Rush(void)
{

}

//===========================================================================================
// じゃんけん
//===========================================================================================
void CManagerEvent::Janken(void)
{

}
