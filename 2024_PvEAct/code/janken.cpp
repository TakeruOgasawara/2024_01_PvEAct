//===========================================================================================
// 
// じゃんけん処理[janken.cpp]
// Aouthor : Takeru Ogasawara
// 
//===========================================================================================
#include "janken.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "texture.h"

#include "debugproc.h"
#include "inputKeyboard.h"

// 定数宣言
namespace
{
	const static int  MAX_JANKEN = 5;
}

// 静的メンバ変数宣言
CJanken* CJanken::m_pJanken = nullptr;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CJanken::CJanken(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	ZeroMemory(&m_aPlayerInfo[0], sizeof(m_aPlayerInfo));
	m_bJanken = false;
	m_nNum = 0;
	m_flow = FLOW_NONE;
	m_nNumAllWin = 0;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CJanken::~CJanken()
{

}

//===========================================================================================
// 生成
//===========================================================================================
CJanken* CJanken::Create(void)
{
	if (m_pJanken == nullptr)
	{
		m_pJanken = new CJanken;

		m_pJanken->Init();
	}

	return m_pJanken;
}

//===========================================================================================
// インスタンスの取得
//===========================================================================================
CJanken* CJanken::GetInstace(void)
{
	if (m_pJanken == nullptr)
	{
		return nullptr;
		assert(("インスタンスの取得に失敗", false));
	}

	return m_pJanken;
}

//===========================================================================================
// 破棄
//===========================================================================================
void CJanken::Release(void)
{
	if (m_pJanken != nullptr)
	{
		m_pJanken->Uninit();

		delete m_pJanken;
		m_pJanken = nullptr;
	}
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CJanken::Init(void)
{
	CTexture* pTex = CTexture::GetInstance();

	for (int i = 0; i < 3; i++)
	{
		m_apChoices2dUI[i] = CObject2D::Create({(i * 300.0f) + (SCREEN_WIDTH * 0.5f) - 300.0f, 600.0f, 0.0f});

		switch (i)
		{
		case 0:
			m_apChoices2dUI[i]->BindTexture(pTex->Regist("data\\TEXTURE\\janken_A.png"));
			break;
		case 1:
			m_apChoices2dUI[i]->BindTexture(pTex->Regist("data\\TEXTURE\\janken_B.png"));
			break;
		case 2:
			m_apChoices2dUI[i]->BindTexture(pTex->Regist("data\\TEXTURE\\janken_X.png"));
			break;
		default:
			break;
		}
		m_apChoices2dUI[i]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CJanken::Uninit(void)
{
	for (int i = 0; i < 3; i++)
	{
		m_apChoices2dUI[i]->Uninit();
	}

	m_pJanken = nullptr;

	CObject::Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CJanken::Update(void)
{
	// 流れ
	Flow();

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print("\n\nCOM : %d\n", (int)m_aPlayerInfo[PLAYER_COM].janken);
	CManager::GetInstance()->GetDebugProc()->Print("プレイヤー : %d\n", (int)m_aPlayerInfo[PLAYER_PLAYER].janken);
	if (m_aPlayerInfo[PLAYER_PLAYER].result == RESULT_WIN)
	{
		CManager::GetInstance()->GetDebugProc()->Print("プレイヤーの勝ち\n");
	}
	else if (m_aPlayerInfo[PLAYER_PLAYER].result == RESULT_LOSE)
	{
		CManager::GetInstance()->GetDebugProc()->Print("プレイヤーの負け\n");
	}
	else  if (m_aPlayerInfo[PLAYER_PLAYER].result == RESULT_DRAW)
	{
		CManager::GetInstance()->GetDebugProc()->Print("あいこ\n");
	}
	else
	{
		CManager::GetInstance()->GetDebugProc()->Print("なし\n");
	}
	CManager::GetInstance()->GetDebugProc()->Print("連続勝利数 : %d\n\n", m_nNumAllWin);
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CJanken::Draw(void)
{

}

//===========================================================================================
// 流れ
//===========================================================================================
void CJanken::Flow(void)
{
	// じゃんけんの流れ
	switch (m_flow)
	{
	case FLOW_NONE:
		break;

	case FLOW_INIT:
		ComAnswer();
		break;

	case FLOW_CHECK_ANSWER:
		CheckAnswer();
		break;

	case FLOW_END:
		continuity();
		break;

	default:
		assert(("範囲外の数値が入っています", false));
		break;
	}
}

//===========================================================================================
// コンピュータの答え
//===========================================================================================
void CJanken::ComAnswer(void)
{
	if (m_bJanken && !m_aPlayerInfo[PLAYER_COM].bAnswer)
	{
		int nResult = rand() % (JANKE_MAX - 1) + 1;

		m_aPlayerInfo[PLAYER_COM].janken = (EJanken)nResult;

		// 答えた
		m_aPlayerInfo[PLAYER_COM].bAnswer = true;
	}
}

//===========================================================================================
// じゃんけん結果
//===========================================================================================
void CJanken::CheckAnswer(void)
{
	switch (m_aPlayerInfo[PLAYER_COM].janken)
	{// プレイヤーの選択したじゃんけん

	case JANKEN_NONE:	// 勝敗つかず
		m_aPlayerInfo[PLAYER_COM].result = RESULT_NONE;
		m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_NONE;
		break;

	case JANKEN_GU:
		if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_GU)
		{// あいこ
			m_aPlayerInfo[PLAYER_COM].result = RESULT_DRAW;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_DRAW;
		}
		else if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_CHOKI)
		{// プレイヤーの負け
			m_aPlayerInfo[PLAYER_COM].result = RESULT_WIN;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_LOSE;
		}
		else
		{// プレイヤーの勝ち
			m_aPlayerInfo[PLAYER_COM].result = RESULT_LOSE;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_WIN;
		}
		break;

	case JANKEN_CHOKI:
		if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_GU)
		{// プレイヤーの勝ち
			m_aPlayerInfo[PLAYER_COM].result = RESULT_LOSE;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_WIN;
		}
		else if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_CHOKI)
		{// あいこ
			m_aPlayerInfo[PLAYER_COM].result = RESULT_DRAW;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_DRAW;
		}
		else
		{// プレイヤーの負け
			m_aPlayerInfo[PLAYER_COM].result = RESULT_WIN;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_LOSE;
		}
		break;

	case JANKEN_PA:
		if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_GU)
		{// プレイヤーの負け
			m_aPlayerInfo[PLAYER_COM].result = RESULT_WIN;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_LOSE;
		}
		else if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_CHOKI)
		{// プレイヤーの勝ち
			m_aPlayerInfo[PLAYER_COM].result = RESULT_LOSE;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_WIN;
		}
		else
		{// あいこ
			m_aPlayerInfo[PLAYER_COM].result = RESULT_DRAW;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_DRAW;
		}
		break;

	default:
		assert(("範囲外の数値が入りました", false));
		break;
	}

	if (m_aPlayerInfo[PLAYER_PLAYER].result == RESULT_LOSE)
	{
		m_nNumAllWin = 0;
	}

	// 終了
	m_flow = FLOW_END;
}

//===========================================================================================
// 連続で行うかどうか
//===========================================================================================
void CJanken::continuity(void)
{
	m_nNum++;	// じゃんけん回数

	if (m_aPlayerInfo[PLAYER_PLAYER].result == RESULT_WIN)
	{
		// 最初からにする
		SetFlow(FLOW_INIT);

		// 答えていない状態
		m_aPlayerInfo[PLAYER_COM].bAnswer = false;
		m_aPlayerInfo[PLAYER_PLAYER].bAnswer = false;

		m_nNumAllWin++;	// 勝利回数
	}
	else
	{
		m_bJanken = false;
		//Uninit();
	}
}

//===========================================================================================
// じゃんけん設定
//===========================================================================================
void CJanken::SetJanken(EJanken janken)
{
	if (m_aPlayerInfo[PLAYER_PLAYER].bAnswer)
	{
		return;
	}

	if (janken < JANKE_MAX && janken > JANKEN_NONE && !m_aPlayerInfo[PLAYER_PLAYER].bAnswer)
	{
		m_aPlayerInfo[PLAYER_PLAYER].janken = janken;	// 設定されたじゃんけん
		m_aPlayerInfo[PLAYER_PLAYER].bAnswer = true;	// 答えた
		m_flow = FLOW_CHECK_ANSWER;	// 答え合わせ
	}
	else
	{
		assert(("じゃんけんの設定に失敗", false));
	}
}

//===========================================================================================
// 流れ設定
//===========================================================================================
void CJanken::SetFlow(EFlow flow)
{
	if (flow > FLOW_NONE && flow < FLOW_MAX)
	{
		m_flow = flow;
	}
	else
	{
		assert(("流れの設定に失敗しました", false));
	}
}

//===========================================================================================
// じゃんけん開始の判断
//===========================================================================================
void CJanken::IsJanken(bool bJanken)
{
	m_bJanken = bJanken;
	m_flow = FLOW_INIT;
}

