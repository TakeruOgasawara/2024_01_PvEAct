//=============================================================================
// 
//  キャラクター処理[character.cpp]
// Author : Takeru Ogasawara
// 
//=============================================================================

// インクルード宣言
#include "character.h"
#include "motion.h"
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
CCharacter::CCharacter()
{
	// 値のクリア
	ZeroMemory(&m_info, sizeof(SCharacter));
}

//=============================================================================
// デストラクタ
//=============================================================================
CCharacter::~CCharacter()
{

}

//=============================================================================
//	生成
//=============================================================================
CCharacter* CCharacter::Create(const char* pPath)
{
	CCharacter* pChar = nullptr;

	if (pChar == nullptr)
	{
		pChar = new CCharacter;

		if (pChar != nullptr)
		{
			pChar->Init(pPath);

			return pChar;
		}
	}

	return pChar;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCharacter::Init()
{
	return S_OK;
}

//=============================================================================
// 初期化処理(オーバーロード)
//=============================================================================
HRESULT CCharacter::Init(const char* pPath)
{
	m_info.pBody = CMotion::Create(pPath);

	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetPosition(GetPosition());
		m_info.pBody->SetRotation(GetRotation());
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCharacter::Uninit()
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
void CCharacter::Update()
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
void CCharacter::Draw()
{
	// マトリックスの掛け合わせ
	general::SetMatrix(&m_info.mtxWorld, m_info.pos, m_info.rot);
}

//=============================================================================
// 位置設定
//=============================================================================
void CCharacter::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;

	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetPosition(GetPosition());
	}
}

//=============================================================================
// 向き設定
//=============================================================================
void CCharacter::SetRotation(D3DXVECTOR3 rot)
{
	m_info.rot = rot;

	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetRotation(GetRotation());
	}
}

//=====================================================
// モーション設定
//=====================================================
void CCharacter::SetMotion(int nMotion)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetMotion(nMotion);
	}
}

//=====================================================
// 体の設定
//=====================================================
void CCharacter::SetBody(CMotion* pBody)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody = pBody;
	}
}

//=====================================================
// モーション取得
//=====================================================
int CCharacter::GetType(void)
{
	int nMotion = 0;

	if (m_info.pBody != nullptr)
	{
		nMotion = m_info.pBody->GetType();
	}

	return nMotion;
}

//=====================================================
// 体の取得
//=====================================================
CMotion* CCharacter::GetBody(void)
{
	if (m_info.pBody != nullptr)
	{
		return m_info.pBody;
	}

	return nullptr;
}
