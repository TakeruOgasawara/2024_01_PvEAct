//===============================================================================
//
// [number.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#include "number.h"
#include "renderer.h"
#include "manager.h"
#include "UI.h"
#include "texture.h"

// 定数定義
namespace
{
	static const D3DXVECTOR2 NUMBER_SIZE = { 10.0f, 20.0f };
}

//===========================================================================================
// コンストラクタ
//===========================================================================================
CNumber::CNumber()
{
	ZeroMemory(&m_info, sizeof(m_info));
	m_pNumberUI = nullptr;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CNumber::~CNumber()
{

}

//===========================================================================================
// 生成
//===========================================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos)
{
	CNumber *pNumber = nullptr;

	if (pNumber == nullptr)
	{
		//オブジェクト2Dの生成
		pNumber = new CNumber;

		if (pNumber != nullptr)
		{
			//初期化処理
			pNumber->SetPosition(pos);
			pNumber->Init();
		}

		return pNumber;
	}

	return pNumber;
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CNumber::Init()
{
	// インスタンス生成
	m_pNumberUI = CUI::Create();

	if (m_pNumberUI != nullptr)
	{
		// サイズ設定(デフォルト)
		m_pNumberUI->SetSize(NUMBER_SIZE.x, NUMBER_SIZE.y);

		// テクスチャ割り当て(デフォルト)
		BindTexture("data\\TEXTURE\\number\\number0.png");
	}
	
	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CNumber::Uninit()
{
	if (m_pNumberUI != nullptr)
	{
		m_pNumberUI = nullptr;
	}
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CNumber::Update()
{
	if (m_pNumberUI != nullptr)
	{
		m_pNumberUI->SetVertex_center();
	}
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CNumber::Draw()
{
	if (m_pNumberUI != nullptr)
	{
		m_pNumberUI->Draw();
	}
}

//===========================================================================================
// 位置設定
//===========================================================================================
void CNumber::SetPosition(D3DXVECTOR3 pos)
{
	if (m_pNumberUI != nullptr)
	{
		m_pNumberUI->SetPosition(pos);
	}
}

//===========================================================================================
// 向き設定
//===========================================================================================
void CNumber::SetRotation(D3DXVECTOR3 rot)
{
	if (m_pNumberUI != nullptr)
	{
		m_pNumberUI->SetRotation(rot);
	}
}

//===========================================================================================
// 色の設定
//===========================================================================================
void CNumber::SetColor(D3DXCOLOR col)
{
	if (m_pNumberUI != nullptr)
	{
		m_pNumberUI->SetColor(col);
	}
}

//===========================================================================================
// サイズ設定
//===========================================================================================
void CNumber::SetSize(float fWidth, float fHeight)
{
	if (m_pNumberUI != nullptr)
	{
		m_pNumberUI->SetSize(fWidth, fHeight);
	}
}

//===========================================================================================
// テクスチャの設定
//===========================================================================================
void CNumber::SetTex(D3DXVECTOR2 left, D3DXVECTOR2 right)
{
	if (m_pNumberUI != nullptr)
	{
		m_pNumberUI->SetTex(left, right);
	}
}

//===========================================================================================
// テクスチャの割り当て
//===========================================================================================
void CNumber::BindTexture(const char* pTexPath)
{
	CTexture* pTexture = CTexture::GetInstance();
	int nIdxTex = -1;

	if (pTexture != nullptr)
	{
		nIdxTex = pTexture->Regist(pTexPath);
	}

	if (m_pNumberUI != nullptr)
	{
		m_pNumberUI->BindTexture(nIdxTex);
	}
}

//===========================================================================================
// バッファの取得
//===========================================================================================
LPDIRECT3DVERTEXBUFFER9 CNumber::GetVtxBuff(void)
{
	if (m_pNumberUI == nullptr)
	{
		return nullptr;
	}

	return m_pNumberUI->GetVtxBuff();
}
