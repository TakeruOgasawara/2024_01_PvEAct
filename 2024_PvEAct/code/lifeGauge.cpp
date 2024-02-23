#include "lifeGauge.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "UI.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CLifeGauge::CLifeGauge()
{
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_ui, sizeof(m_ui));
	m_state = STATE_LIVE;
	m_bDamage = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CLifeGauge::~CLifeGauge()
{

}

//=============================================================================
// 生成
//=============================================================================
CLifeGauge* CLifeGauge::Create()
{
	CLifeGauge* pLife = nullptr;

	if (pLife == nullptr)
	{
		pLife = new CLifeGauge;

		pLife->Init();
	}

	return pLife;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLifeGauge::Init()
{
	// インスタンス生成
	m_ui.pFrame = CUI::Create();
	//m_ui.pAfterImage = CUI::Create();
	m_ui.pGauge = CUI::Create();

	m_info.fLife = 200.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLifeGauge::Uninit()
{
	m_ui.pGauge = nullptr;
	m_ui.pAfterImage = nullptr;
	m_ui.pFrame = nullptr;

	// 自身の破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLifeGauge::Update()
{
	if (m_ui.pFrame != nullptr)
	{
		m_ui.pFrame->SetVertex_left();
	}
	if (m_ui.pGauge != nullptr)
	{
		m_ui.pGauge->SetVertex_left();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CLifeGauge::Draw()
{
	if (m_ui.pFrame != nullptr)
	{
		m_ui.pFrame->Draw();
	}
	if (m_ui.pGauge != nullptr)
	{
		m_ui.pGauge->Draw();
	}
}

//=============================================================================
// 位置設定
//=============================================================================
void CLifeGauge::SetPosition(D3DXVECTOR3 posLife, D3DXVECTOR3 posFrame)
{
	if (m_ui.pGauge != nullptr)
	{
		m_ui.pGauge->SetPosition(posLife);
	}
	if (m_ui.pFrame != nullptr)
	{
		m_ui.pFrame->SetPosition(posFrame);
	}
}

//=============================================================================
// ゲージサイズ設定
//=============================================================================
void CLifeGauge::SetSizeGauge(float width, float height)
{
	if (m_ui.pGauge != nullptr)
	{
		m_ui.pGauge->SetSize(width, height);
	}
	if (m_ui.pAfterImage != nullptr)
	{
		m_ui.pAfterImage->SetSize(width, height);
	}
}

//=============================================================================
// フレームサイズ設定
//=============================================================================
void CLifeGauge::SetSizeFrame(float width, float height)
{
	if (m_ui.pFrame != nullptr)
	{
		m_ui.pFrame->SetSize(width, height);
	}
	
}

//=============================================================================
// 色設定
//=============================================================================
void CLifeGauge::SetColor(D3DXCOLOR colLife, D3DXCOLOR colFrame)
{
	if (m_ui.pGauge != nullptr && m_ui.pFrame)
	{
		m_ui.pGauge->SetColor(colLife);
		m_ui.pFrame->SetColor(colFrame);
	}
}

//=============================================================================
// テクスチャ設定
//=============================================================================
void CLifeGauge::BindTexture(const char* pTexPath_gauge, const char* pTexPath_frame)
{
	if (m_ui.pGauge != nullptr && m_ui.pFrame)
	{
		CTexture* pTexture = CTexture::GetInstance();
		int nIdx1 = -1, nIdx2 = -1;

		if (pTexture != nullptr)
		{
			nIdx1 = pTexture->Regist(pTexPath_gauge);
			nIdx2 = pTexture->Regist(pTexPath_frame);
		}
		
		m_ui.pGauge->BindTexture(nIdx1);
		m_ui.pFrame->BindTexture(nIdx2);
	}
}

//=============================================================================
// ダメージ
//=============================================================================
void CLifeGauge::Damage(float fDamage)
{
	if (m_ui.pGauge != nullptr)
	{// ゲージの減少率を計算して反映

		D3DXVECTOR2 size = m_ui.pGauge->GetSize();

		// 今の最大体力に対するダメージの割合を求める
		float fRatio = fDamage / m_info.fLife;
		// ゲージに対する減少値を求める
		fRatio = size.x * fRatio;
		// ゲージを減少
		size.x = size.x - fRatio;

		if (size.x < 0.0f)
		{
			size.x = 0.0f;
		}
		
		m_ui.pGauge->SetSize(size.x, size.y);
	}

	// 体力を減らす
	m_info.fLife -= fDamage;

	if (m_info.fLife < 0.0f)
	{
		m_info.fLife = 0.0f;
	}
}
