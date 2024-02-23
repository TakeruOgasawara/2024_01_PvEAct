#include "managerUI.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "texture.h"
#include "managerEvent.h"
#include "lifeGauge.h"
#include "time.h"

#include "player.h"
#include "enemyBoss.h"

// 定数宣言
namespace
{
	// 体力UI =====================
	// プレイヤー
	// ゲージ
	static const D3DXVECTOR3 PLAYER_LIFE_GAUGE_POS = { 20.0f, 650.0f, 0.0f };	// 位置
	static const D3DXVECTOR2 PLAYER_LIFE_GAUGE_SIZE = { 600.0f, 20.0f };		// サイズ
	static const D3DXCOLOR PLAYER_LIFE_GAUGE_COL = { 0.0f, 1.0f, 0.0f, 1.0f };	// 色
	static const char* PLAYER_LIFE_GAUGE_PATH = "";
	// フレーム
	static const D3DXVECTOR3 PLAYER_LIFE_FRAME_POS = { 20.0f, 650.0f, 0.0f };	// 位置
	static const D3DXVECTOR2 PLAYER_LIFE_FRAME_SIZE = { 600.0f, 20.0f };		// サイズ
	static const D3DXCOLOR PLAYER_LIFE_FRAME_COL = { 0.5f, 0.5f, 0.5f, 1.0f };	// 色
	static const char* PLAYER_LIFE_FRAME_PATH = "";

	// 敵(boss)
	// ゲージ
	static const D3DXVECTOR3 ENEMY_LIFE_GAUGE_POS = { 20.0f, 20.0f, 0.0f };		// 位置
	static const D3DXVECTOR2 ENEMY_LIFE_GAUGE_SIZE = { 900.0f, 20.0f };			// サイズ
	static const D3DXCOLOR ENEMY_LIFE_GAUGE_COL = { 0.0f, 1.0f, 0.0f, 1.0f };	// 色
	static const char* ENEMY_LIFE_GAUGE_PATH = "";
	// フレーム
	static const D3DXVECTOR3 ENEMY_LIFE_FRAME_POS = { 20.0f, 20.0f, 0.0f };		// 位置
	static const D3DXVECTOR2 ENEMY_LIFE_FRAME_SIZE = { 900.0f, 20.0f };			// サイズ
	static const D3DXCOLOR ENEMY_LIFE_FRAME_COL = { 0.5f, 0.5f, 0.5f, 1.0f };	// 色
	static const char* ENEMY_LIFE_FRAME_PATH = "";
}

// 静的メンバ変数
CManagerUI* CManagerUI::m_pUiManager = nullptr;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CManagerUI::CManagerUI()
{
	// 値のクリア
	ZeroMemory(&m_ui, sizeof(m_ui));
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CManagerUI::~CManagerUI()
{
	
}

//===========================================================================================
// 生成
//===========================================================================================
CManagerUI* CManagerUI::Create(void)
{
	if (m_pUiManager == nullptr)
	{
		m_pUiManager = new CManagerUI;

		m_pUiManager->Init();

		return m_pUiManager;
	}

	return m_pUiManager;
}

//===========================================================================================
// インスタンス生成
//===========================================================================================
CManagerUI* CManagerUI::GetInstance(void)
{
	if (m_pUiManager == nullptr)
	{
		assert(("マネージャUIの取得に失敗", false));
	}

	return m_pUiManager;
}

//===========================================================================================
// インスタンスの破棄
//===========================================================================================
void CManagerUI::Release(void)
{
	if (m_pUiManager != nullptr)
	{
		m_pUiManager->Uninit();

		delete m_pUiManager;
		m_pUiManager = nullptr;
	}
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CManagerUI::Init(void)
{
	// 体力UIの生成
	CreateLife();

	CreateTime();

	SetLifeEnemy();

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CManagerUI::Uninit(void)
{
	if (m_ui.pRushUi != nullptr)
	{
		m_ui.pRushUi = nullptr;
	}

	CObject::Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CManagerUI::Update(void)
{
	// ゲーム状態の取得
	CManagerEvent* pEvent = CManagerEvent::GetInstance();	// ゲーム状態

	if (pEvent != nullptr)
	{
		// 戦闘の状態
		switch (pEvent->GetMode())
		{
		case pEvent->MODE_NORMAL:

			break;
		case pEvent->MODE_JUST:

			break;
		case pEvent->MODE_RUSH:
			if (m_ui.pRushUi == nullptr)
			{
				m_ui.pRushUi = CObject2D::Create({ 0.0f, 0.0f, 0.0f });
			}
			else
			{
				m_ui.pRushUi->BindTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\ui_attackLB.png"));
				m_ui.pRushUi->SetPosition({ SCREEN_WIDTH * 0.5f, 600.0f, 0.0f });
			}
			break;

		case pEvent->MODE_JANKEN:
			break;
		default:
			break;
		}
	}

	UpdateLife();
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CManagerUI::Draw(void)
{

}

//===========================================================================================
// 体力UIの生成
//===========================================================================================
void CManagerUI::CreateLife(void)
{
	// プレイヤーライフUI
	m_ui.pGaugePlayer = CLifeGauge::Create();

	if (m_ui.pGaugePlayer != nullptr)
	{
		m_ui.pGaugePlayer->SetPosition(PLAYER_LIFE_GAUGE_POS, PLAYER_LIFE_FRAME_POS);
		m_ui.pGaugePlayer->SetSizeFrame(PLAYER_LIFE_FRAME_SIZE.x, PLAYER_LIFE_FRAME_SIZE.y);
		m_ui.pGaugePlayer->SetSizeGauge(PLAYER_LIFE_GAUGE_SIZE.x, PLAYER_LIFE_GAUGE_SIZE.y);
		m_ui.pGaugePlayer->SetColor(PLAYER_LIFE_GAUGE_COL, PLAYER_LIFE_FRAME_COL);
		m_ui.pGaugePlayer->BindTexture(PLAYER_LIFE_GAUGE_PATH, PLAYER_LIFE_FRAME_PATH);
	}

	// 敵ライフUI
	m_ui.pGaugeEnemy = CLifeGauge::Create();

	if (m_ui.pGaugeEnemy != nullptr)
	{
		m_ui.pGaugeEnemy->SetPosition(ENEMY_LIFE_GAUGE_POS, ENEMY_LIFE_FRAME_POS);
		m_ui.pGaugeEnemy->SetSizeFrame(ENEMY_LIFE_FRAME_SIZE.x, ENEMY_LIFE_FRAME_SIZE.y);
		m_ui.pGaugeEnemy->SetSizeGauge(ENEMY_LIFE_GAUGE_SIZE.x, ENEMY_LIFE_GAUGE_SIZE.y);
		m_ui.pGaugeEnemy->SetColor(ENEMY_LIFE_GAUGE_COL, ENEMY_LIFE_FRAME_COL);
		m_ui.pGaugeEnemy->BindTexture(ENEMY_LIFE_GAUGE_PATH, ENEMY_LIFE_FRAME_PATH);

	}
}

//===========================================================================================
// 体力の設定
//===========================================================================================
void CManagerUI::SetLifePlayer(void)
{
	/*CPlayer* pPlayer = nullptr;
	pPlayer = CPlayer::GetInstance();

	if (m_ui.pGaugePlayer != nullptr && pPlayer != nullptr)
	{
		m_ui.pGaugePlayer->SetLife(pPlayer->GetL)
	}*/
}

//===========================================================================================
// 体力の設定
//===========================================================================================
void CManagerUI::SetLifeEnemy(void)
{
	CEnemyBoss* pEnemy = nullptr;
	pEnemy = CEnemyBoss::GetInstance();

	if (m_ui.pGaugeEnemy != nullptr && pEnemy != nullptr)
	{
		m_ui.pGaugeEnemy->SetLife(pEnemy->GetLife());
	}
}

//===========================================================================================
// ダメージの設定
//===========================================================================================
void CManagerUI::UpdateLife(void)
{
	CEnemyBoss* pEnemy = nullptr;
	pEnemy = CEnemyBoss::GetInstance();

	if (m_ui.pGaugeEnemy != nullptr && pEnemy != nullptr)
	{
		float fDamage = pEnemy->GetDamage();

		m_ui.pGaugeEnemy->Damage(fDamage);

		pEnemy->SetDamage(0.0f);
	}
}

//===========================================================================================
// タイムの生成
//===========================================================================================
void CManagerUI::CreateTime(void)
{
	m_ui.pTime = CTime::Create({ 1090.0f, 30.0f, 1.0f });

	if (m_ui.pTime != nullptr)
	{
		m_ui.pTime->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
		m_ui.pTime->SetSize(20.0f, 40.0f);
	}
}
