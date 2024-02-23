#include "lifeGauge.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "UI.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLifeGauge::CLifeGauge()
{
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_ui, sizeof(m_ui));
	m_state = STATE_LIVE;
	m_bDamage = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLifeGauge::~CLifeGauge()
{

}

//=============================================================================
// ����
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
// ����������
//=============================================================================
HRESULT CLifeGauge::Init()
{
	// �C���X�^���X����
	m_ui.pFrame = CUI::Create();
	//m_ui.pAfterImage = CUI::Create();
	m_ui.pGauge = CUI::Create();

	m_info.fLife = 200.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLifeGauge::Uninit()
{
	m_ui.pGauge = nullptr;
	m_ui.pAfterImage = nullptr;
	m_ui.pFrame = nullptr;

	// ���g�̔j��
	Release();
}

//=============================================================================
// �X�V����
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
// �`�揈��
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
// �ʒu�ݒ�
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
// �Q�[�W�T�C�Y�ݒ�
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
// �t���[���T�C�Y�ݒ�
//=============================================================================
void CLifeGauge::SetSizeFrame(float width, float height)
{
	if (m_ui.pFrame != nullptr)
	{
		m_ui.pFrame->SetSize(width, height);
	}
	
}

//=============================================================================
// �F�ݒ�
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
// �e�N�X�`���ݒ�
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
// �_���[�W
//=============================================================================
void CLifeGauge::Damage(float fDamage)
{
	if (m_ui.pGauge != nullptr)
	{// �Q�[�W�̌��������v�Z���Ĕ��f

		D3DXVECTOR2 size = m_ui.pGauge->GetSize();

		// ���̍ő�̗͂ɑ΂���_���[�W�̊��������߂�
		float fRatio = fDamage / m_info.fLife;
		// �Q�[�W�ɑ΂��錸���l�����߂�
		fRatio = size.x * fRatio;
		// �Q�[�W������
		size.x = size.x - fRatio;

		if (size.x < 0.0f)
		{
			size.x = 0.0f;
		}
		
		m_ui.pGauge->SetSize(size.x, size.y);
	}

	// �̗͂����炷
	m_info.fLife -= fDamage;

	if (m_info.fLife < 0.0f)
	{
		m_info.fLife = 0.0f;
	}
}
