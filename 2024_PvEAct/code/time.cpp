//===========================================================================================
//
// [time.cpp]m_aChain[0]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "time.h"
#include "object2D.h"
#include "number.h"
#include "manager.h"
#include "texture.h"

#include <assert.h>

//�}�N����`
#define SPACE			(30.0f)
#define START_MINUT		(3)
#define START_TIME		(0)
#define COLON	(D3DXVECTOR2())

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CTime::CTime()
{
	ZeroMemory(&m_apNumber[0], sizeof(m_apNumber));
	m_pPeriod = nullptr;
	m_pColon = nullptr;
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_nSecondCount = 0;
	m_nMinuteCount = 0;
	m_dwGameStartTime = 0;	//�Q�[���J�n����
	m_dwGameTime = 0;	//�Q�[���o�ߎ���
	m_bStop = false;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CTime::~CTime()
{

}

//===========================================================================================
// ����
//===========================================================================================
CTime* CTime::Create()
{
	CTime* pTime = nullptr;

	if (pTime == nullptr)
	{
		pTime = new CTime;

		if (pTime != nullptr)
		{
			pTime->Init();

			return pTime;
		}
	}

	return nullptr;
}

//===========================================================================================
// ����
//===========================================================================================
CTime* CTime::Create(D3DXVECTOR3 pos)
{
	CTime* pTime = nullptr;

	if (pTime == nullptr)
	{
		pTime = new CTime;

		if (pTime != nullptr)
		{
			pTime->m_pos = pos;
			pTime->Init();

			return pTime;
		}
	}

	return nullptr;
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CTime::Init()
{
	m_nMinuteCount = START_MINUT * 10000;
	m_nSecondCount = START_TIME;

	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(m_pos);

		switch (nCnt)
		{
		case 0:	//��
			m_apNumber[nCnt]->SetPosition(D3DXVECTOR3(m_pos.x + ((SPACE) * nCnt), m_pos.y, 0.0f));

			m_pColon = CObject2D::Create(D3DXVECTOR3(m_pos.x + ((SPACE) * nCnt + 27.0f), m_pos.y + 7.0f, 0.0f));
			m_pColon->SetSize(3.0f, 13.0f);
			m_pColon->BindTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\number\\colon.png"));

			break;

		case 1:	//�b
			m_apNumber[nCnt]->SetPosition(D3DXVECTOR3(m_pos.x + ((SPACE) * nCnt + 25.0f), m_pos.y, 0.0f));
			break;

		case 2:
			m_apNumber[nCnt]->SetPosition(D3DXVECTOR3(m_pos.x + ((SPACE) * nCnt + 25.0f), m_pos.y, 0.0f));
			break;

		case 3:	//�R���}�b
			m_apNumber[nCnt]->SetPosition(D3DXVECTOR3(m_pos.x + ((SPACE) * nCnt + 50.0f), m_pos.y, 0.0f));

			m_pPeriod = CObject2D::Create(D3DXVECTOR3(m_pos.x + ((SPACE)*nCnt + 25.0f), m_pos.y + 20.0f, 0.0f));
			m_pPeriod->SetSize(3.0f, 3.0f);
			m_pPeriod->BindTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\number\\period.png"));

			break;

		case 4:
			m_apNumber[nCnt]->SetPosition(D3DXVECTOR3(m_pos.x + ((SPACE) * nCnt + 50.0f), m_pos.y, 0.0f));
			break;
		default:

			break;
		}

		m_apNumber[nCnt]->SetSize(15.0f, 20.0f);
	}

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CTime::Uninit()
{
	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->Uninit();
			delete m_apNumber[nCnt];
			m_apNumber[nCnt] = nullptr;
		}
	}

	if (m_pColon != nullptr)
	{
		m_pColon = nullptr;
	}

	if (m_pPeriod != nullptr)
	{
		m_pPeriod = nullptr;
	}

	Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CTime::Update()
{
	SetTime();
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CTime::Draw()
{
	for (int nCnt = 0; nCnt < NUM_TIME; nCnt++)
	{
		m_apNumber[nCnt]->Draw();
	}
}

//===========================================================================================
// �^�C���̐ݒ�
//===========================================================================================
void CTime::SetTime(void)
{
	int aTexU[NUM_TIME] = {};	//�e���̐������i�[

	//���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	if (!m_bStop)
	{
		//�o�ߎ��Ԃ̎Z�o
		m_dwGameTime = timeGetTime() - m_dwGameStartTime;

		if (m_dwGameTime %= 1000)
		{
			m_nSecondCount--;
		}
	}

	//��
	aTexU[0] = m_nMinuteCount % 100000 / 10000;

	// �b
	aTexU[1] = m_nSecondCount % 10000 / 1000;
	aTexU[2] = m_nSecondCount % 1000 / 100;

	// �R���}�b
	aTexU[3] = m_nSecondCount % 100 / 10;
	aTexU[4] = m_nSecondCount % 10 / 1;

	for (int nCntTime = 0; nCntTime < NUM_TIME; nCntTime++)
	{
		if (m_apNumber[nCntTime] != nullptr)
		{
			m_apNumber[nCntTime]->Update();

			//���_�o�b�t�@�����b�N
			m_apNumber[nCntTime]->GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

			//�e�N�X�`���̍��W�ݒ�
			pVtx[0].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(aTexU[nCntTime] * 0.1f + 0.1f, 1.0f);

			//���_�o�b�t�@���A�����b�N����
			m_apNumber[nCntTime]->GetVtxBuff()->Unlock();
		}
	}

	// �I��
	if (!m_bStop)
	{
		if (m_nMinuteCount <= 0 && m_nSecondCount <= 0)
		{
			m_nMinuteCount = 0;
			m_nSecondCount = 0;
			m_bStop = true;
			return;
		}
		if (m_nSecondCount <= 0)
		{
			m_nSecondCount = 6000;
			m_nMinuteCount -= (1 * 10000);
		}
	}
}

//===========================================================================================
// �^�C���̉��Z(�A�C�e������������Ȃ� �����͎g���)
//===========================================================================================
void CTime::AddTime(int nMinute, int nSecond)
{
	m_nMinuteCount = nMinute * 10000;
	m_nSecondCount = nSecond /** 100*/;
}

//===========================================================================================
// �F�̐ݒ�
//===========================================================================================
void CTime::SetColor(D3DXCOLOR col)
{
	for (int nCntTime = 0; nCntTime < NUM_TIME; nCntTime++)
	{
		if (m_apNumber[nCntTime] != nullptr)
		{
			m_apNumber[nCntTime]->SetColor(col);
		}
	}
}

//===========================================================================================
// �T�C�Y�̐ݒ�
//===========================================================================================
void CTime::SetSize(float fWidth, float fHeight)
{
	for (int nCntTime = 0; nCntTime < NUM_TIME; nCntTime++)
	{
		if (m_apNumber[nCntTime] != nullptr)
		{
			m_apNumber[nCntTime]->SetSize(fWidth, fHeight);
		}
	}
}

//===========================================================================================
// �^�C���̎擾
//===========================================================================================
int CTime::GetTime(void)
{
	return m_nSecondCount;
}
