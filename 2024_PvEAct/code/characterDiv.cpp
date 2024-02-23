//=============================================================================
// 
//  キャラクター処理[character.cpp]
// Author : Takeru Ogasawara
// 
//=============================================================================

// インクルード宣言
#include "characterDiv.h"
#include "motionDiv.h"
#include "renderer.h"
#include "manager.h"
#include "xfile.h"
#include "texture.h"

#include "input.h"
#include "general.h"
#include <stdio.h>

#define SPEED	(2.0f);

//=============================================================================
// コンストラクタ
//=============================================================================
CCharacterDiv::CCharacterDiv()
{
	// 値のクリア
	ZeroMemory(&m_info, sizeof(SCharacter));
}

//=============================================================================
// デストラクタ
//=============================================================================
CCharacterDiv::~CCharacterDiv()
{

}

//=============================================================================
//	生成
//=============================================================================
CCharacterDiv* CCharacterDiv::Create(const char* pUpperPath, const char* pLowerPath)
{
	CCharacterDiv* pChar = nullptr;

	if (pChar == nullptr)
	{
		pChar = new CCharacterDiv;

		if (pChar != nullptr)
		{
			pChar->Init(pUpperPath, pLowerPath);

			return pChar;
		}
	}

	return pChar;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCharacterDiv::Init()
{
	return S_OK;
}

//=============================================================================
// 初期化処理(オーバーロード)
//=============================================================================
HRESULT CCharacterDiv::Init(const char* pUpperPath, const char* pLowerPath)
{
	if (m_info.pBody == nullptr && pUpperPath != nullptr && pLowerPath != nullptr)
	{
		m_info.pBody = CMotionDiv::Create(pUpperPath, pLowerPath);

		if (m_info.pBody != nullptr)
		{
			m_info.pBody->SetPosition(GetPosition());
			m_info.pBody->SetRotation(GetRotation());
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCharacterDiv::Uninit()
{
	/*if (m_info.pBody != nullptr)
	{
		m_info.pBody->Uninit();
		m_info.pBody = nullptr;
	}*/

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCharacterDiv::Update()
{
	if (m_info.pBody != nullptr)
	{// 体の追従
		m_info.pBody->SetPosition(GetPosition());
		m_info.pBody->SetRotation(GetRotation());
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CCharacterDiv::Draw()
{
	// マトリックスの掛け合わせ
	general::SetMatrix(&m_info.mtxWorld, m_info.pos, m_info.rot);
}

//=============================================================================
// 位置設定
//=============================================================================
void CCharacterDiv::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;

	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetPosition(m_info.pos);
	}
}

//=============================================================================
// 向き設定
//=============================================================================
void CCharacterDiv::SetRotation(D3DXVECTOR3 rot)
{
	m_info.rot = rot;

	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetRotation(m_info.rot);
	}
}

//=====================================================
// モーション設定
//=====================================================
void CCharacterDiv::SetMotion(int nNum, int nMotion)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetMotion(nNum, nMotion);
	}
}

//=====================================================
// 体の設定
//=====================================================
void CCharacterDiv::SetBody(CMotionDiv* pBody)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody = pBody;
	}
}

//=====================================================
// モーション取得
//=====================================================
int CCharacterDiv::GetType(int nNumDiv)
{
	int nMotion = 0;

	if (m_info.pBody != nullptr)
	{
		nMotion = m_info.pBody->GetType(nNumDiv);
	}

	return nMotion;
}

//=====================================================
// 体の取得
//=====================================================
CMotionDiv* CCharacterDiv::GetBody(void)
{
	if (m_info.pBody != nullptr)
	{
		return m_info.pBody;
	}

	return nullptr;
}
