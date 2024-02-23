//======================================================================================
//
// スロー処理[slow.cpp]
// Author;小笠原　彪
//
//======================================================================================
#include "managerSlow.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "inputJoypad.h"
#include "inputKeyboard.h"
#include "inputMouse.h"

// 静的メンバ変数宣言
CSlowManager* CSlowManager::m_apSlow[CAMP_MAX][MAX_OBJECT] = {};
int CSlowManager::m_nNumAll = 0;

//======================================================================================
// コンストラクタ
//======================================================================================
CSlowManager::CSlowManager(int nCamp)
{
	m_nID = -1;
	m_tag = TAG_NONE;
	m_camp = (ECamp)nCamp;
	m_fValue = 1.0f;
	m_fCount = 0.0f;
	m_fTime = 0.0f;

	if (nCamp >= CAMP_MAX && nCamp < CAMP_NONE)
	{// 範囲外の場合、所属なし
		m_camp = CAMP_NONE;
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apSlow[m_camp][nCntObject] == nullptr)
		{
			m_apSlow[m_camp][nCntObject] = this;	//自分自身を代入
			m_nID = nCntObject;	//自分自身のIDを保存
			m_nNumAll++;	//総数をカウントアップ

			break;
		}
	}
}

//======================================================================================
// デストラクタ
//======================================================================================
CSlowManager::~CSlowManager()
{

}

//======================================================================================
// 生成
//======================================================================================
CSlowManager* CSlowManager::Create(void)
{
	CSlowManager* m_pManager = nullptr;

	if (m_pManager == nullptr)
	{
		m_pManager = new CSlowManager;

		m_pManager->Init();

		return m_pManager;
	}

	return m_pManager;
}

//======================================================================================
// 生成
//======================================================================================
CSlowManager* CSlowManager::Create(ECamp camp, ETag tag)
{
	CSlowManager* m_pManager = nullptr;

	if (m_pManager == nullptr)
	{
		m_pManager = new CSlowManager;

		m_pManager->Init();
		m_pManager->m_tag = tag;

		return m_pManager;
	}

	return m_pManager;
}

//======================================================================================
// 初期化処理
//======================================================================================
HRESULT CSlowManager::Init()
{
	m_fTime = 1.0f;

	return S_OK;
}

//======================================================================================
// 終了処理
//======================================================================================
void CSlowManager::Uninit()
{
	if (m_apSlow[m_camp][m_nID] != nullptr)
	{
		m_apSlow[m_camp][m_nID] = nullptr;
	}

	Release();
}

//======================================================================================
// 更新処理
//======================================================================================
void CSlowManager::Update()
{
	
}

//======================================================================================
// 全破棄
//======================================================================================
void CSlowManager::ReleaseAll(void)
{
	for (int nCntCamp = 0; nCntCamp < CAMP_MAX; nCntCamp++)
	{
		for (int i = 0; i < MAX_OBJECT; i++)
		{
			if (m_apSlow[nCntCamp][i] == nullptr)
			{
				continue;
			}

			delete m_apSlow[nCntCamp][i];
			m_apSlow[nCntCamp][i] = nullptr;
		}
	}
}

//======================================================================================
// 全体をスローにする
//======================================================================================
void CSlowManager::All(float fValue)
{
	for (int nCntCamp = 0; nCntCamp < CAMP_MAX; nCntCamp++)
	{
		for (int i = 0; i < MAX_OBJECT; i++)
		{
			if (m_apSlow[nCntCamp][i] == nullptr)
			{
				continue;
			}

			// 値を設定(全体)
			m_apSlow[nCntCamp][i]->SetValue(fValue);
		}
	}
}

//======================================================================================
// 全体を通常へ戻す
//======================================================================================
void CSlowManager::Default(void)
{
	for (int nCntCamp = 0; nCntCamp < CAMP_MAX; nCntCamp++)
	{
		for (int i = 0; i < MAX_OBJECT; i++)
		{
			if (m_apSlow[nCntCamp][i] == nullptr)
			{
				continue;
			}

			// 値をデフォルト(全体)
			m_apSlow[nCntCamp][i]->SetValue(1.0f);
		}
	}
}

//======================================================================================
// プレイヤー陣営の設定
//======================================================================================
void CSlowManager::Player(float fValue)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (m_apSlow[CAMP_PAYER][i] == nullptr)
		{
			continue;
		}

		// 値を設定(全体)
		m_apSlow[CAMP_PAYER][i]->SetValue(fValue);
	}
}

//======================================================================================
// 敵陣営の設定
//======================================================================================
void CSlowManager::Enemy(float fValue)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (m_apSlow[CAMP_ENEMY][i] == nullptr)
		{
			continue;
		}

		// 値を設定(全体)
		m_apSlow[CAMP_ENEMY][i]->SetValue(fValue);
	}
}

//======================================================================================
// 陣営のタグ設定
//======================================================================================
void CSlowManager::Tag(ECamp camp, ETag tag, float fValue)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (m_apSlow[camp][i] == nullptr)
		{
			continue;
		}

		if (m_apSlow[camp][i]->GetTag() == tag)
		{
			// 値を設定(陣営のタグ)
			m_apSlow[camp][i]->SetValue(fValue);
		}
	}
}
