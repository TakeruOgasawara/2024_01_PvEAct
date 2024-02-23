//===========================================================================================
//
// ゲーム画面[game.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <istream>

#include "game.h"

#include "managerEvent.h"
#include "managerSlow.h"
#include "managerUI.h"

#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "objectX.h"

#include "debugproc.h"
#include "sound.h"
#include "camera.h"
#include "billboard.h"

#include "inputKeyboard.h"
#include "fade.h"
#include "pause.h"

#include "player.h"
#include "enemyBoss.h"
#include "janken.h"

#include "player.h"

#include "meshWall.h"
#include "wepon.h"

#include "frame.h"
#include "gauge.h"

#include "lifeGauge.h"
#include "managerSlow.h"
#include "managerUI.h"

//静的メンバ変数宣言
CPause *CGame::m_pPause = nullptr;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CGame::CGame()
{
	m_pSound = nullptr;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CGame::~CGame()
{

}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CGame::Init()
{
	// ゲーム状態マネージャの生成
	CManagerEvent::Create();

	// 床の生成
	CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXTURE\\floor.png");

	// 敵の生成
	//CEnemyBoss::Create({ 100.0f, 0.0f, 0.0f });

	// プレイヤーの生成
	CPlayer::Create({ 0.0f, 0.0f, -200.0f });

	//CWepon::Create("data\\MODEL\\wepon\\sword.x");

	// UIマネージャの生成
	CManagerUI::Create();

	CObjectX::Create("data\\MODEL\\player1\\body.x");


	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CGame::Uninit()
{
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	// ゲーム状態の破棄
	//CManagerEvent::Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CGame::Update()
{
	// ポインタの取得
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	CInputKeyboard *pInputKey = CInputKeyboard::GetInstance();
	CFade *pFade = CManager::GetInstance()->GetFade();
	CEnemyBoss* pBoss = CEnemyBoss::GetInstance();

	//CManagerGameState::GetInstance()->Update();

	// シーンの更新
	CScene::Update();

	if (pBoss != nullptr)
	{
		if (pBoss->GetState() == pBoss->STATE_DEATH)
		{
			CManager::GetInstance()->GetFade()->Set(MODE_RESULT);
		}
	}
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CGame::Draw()
{
	CScene::Draw();

	if (m_pPause != nullptr)
	{
		m_pPause->Draw();
	}
}
