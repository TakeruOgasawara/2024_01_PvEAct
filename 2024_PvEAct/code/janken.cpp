//===========================================================================================
// 
// ����񂯂񏈗�[janken.cpp]
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

// �萔�錾
namespace
{
	const static int  MAX_JANKEN = 5;
}

// �ÓI�����o�ϐ��錾
CJanken* CJanken::m_pJanken = nullptr;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CJanken::CJanken(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	ZeroMemory(&m_aPlayerInfo[0], sizeof(m_aPlayerInfo));
	m_bJanken = false;
	m_nNum = 0;
	m_flow = FLOW_NONE;
	m_nNumAllWin = 0;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CJanken::~CJanken()
{

}

//===========================================================================================
// ����
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
// �C���X�^���X�̎擾
//===========================================================================================
CJanken* CJanken::GetInstace(void)
{
	if (m_pJanken == nullptr)
	{
		return nullptr;
		assert(("�C���X�^���X�̎擾�Ɏ��s", false));
	}

	return m_pJanken;
}

//===========================================================================================
// �j��
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
// ����������
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
// �I������
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
// �X�V����
//===========================================================================================
void CJanken::Update(void)
{
	// ����
	Flow();

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print("\n\nCOM : %d\n", (int)m_aPlayerInfo[PLAYER_COM].janken);
	CManager::GetInstance()->GetDebugProc()->Print("�v���C���[ : %d\n", (int)m_aPlayerInfo[PLAYER_PLAYER].janken);
	if (m_aPlayerInfo[PLAYER_PLAYER].result == RESULT_WIN)
	{
		CManager::GetInstance()->GetDebugProc()->Print("�v���C���[�̏���\n");
	}
	else if (m_aPlayerInfo[PLAYER_PLAYER].result == RESULT_LOSE)
	{
		CManager::GetInstance()->GetDebugProc()->Print("�v���C���[�̕���\n");
	}
	else  if (m_aPlayerInfo[PLAYER_PLAYER].result == RESULT_DRAW)
	{
		CManager::GetInstance()->GetDebugProc()->Print("������\n");
	}
	else
	{
		CManager::GetInstance()->GetDebugProc()->Print("�Ȃ�\n");
	}
	CManager::GetInstance()->GetDebugProc()->Print("�A�������� : %d\n\n", m_nNumAllWin);
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CJanken::Draw(void)
{

}

//===========================================================================================
// ����
//===========================================================================================
void CJanken::Flow(void)
{
	// ����񂯂�̗���
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
		assert(("�͈͊O�̐��l�������Ă��܂�", false));
		break;
	}
}

//===========================================================================================
// �R���s���[�^�̓���
//===========================================================================================
void CJanken::ComAnswer(void)
{
	if (m_bJanken && !m_aPlayerInfo[PLAYER_COM].bAnswer)
	{
		int nResult = rand() % (JANKE_MAX - 1) + 1;

		m_aPlayerInfo[PLAYER_COM].janken = (EJanken)nResult;

		// ������
		m_aPlayerInfo[PLAYER_COM].bAnswer = true;
	}
}

//===========================================================================================
// ����񂯂񌋉�
//===========================================================================================
void CJanken::CheckAnswer(void)
{
	switch (m_aPlayerInfo[PLAYER_COM].janken)
	{// �v���C���[�̑I����������񂯂�

	case JANKEN_NONE:	// ���s����
		m_aPlayerInfo[PLAYER_COM].result = RESULT_NONE;
		m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_NONE;
		break;

	case JANKEN_GU:
		if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_GU)
		{// ������
			m_aPlayerInfo[PLAYER_COM].result = RESULT_DRAW;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_DRAW;
		}
		else if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_CHOKI)
		{// �v���C���[�̕���
			m_aPlayerInfo[PLAYER_COM].result = RESULT_WIN;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_LOSE;
		}
		else
		{// �v���C���[�̏���
			m_aPlayerInfo[PLAYER_COM].result = RESULT_LOSE;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_WIN;
		}
		break;

	case JANKEN_CHOKI:
		if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_GU)
		{// �v���C���[�̏���
			m_aPlayerInfo[PLAYER_COM].result = RESULT_LOSE;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_WIN;
		}
		else if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_CHOKI)
		{// ������
			m_aPlayerInfo[PLAYER_COM].result = RESULT_DRAW;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_DRAW;
		}
		else
		{// �v���C���[�̕���
			m_aPlayerInfo[PLAYER_COM].result = RESULT_WIN;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_LOSE;
		}
		break;

	case JANKEN_PA:
		if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_GU)
		{// �v���C���[�̕���
			m_aPlayerInfo[PLAYER_COM].result = RESULT_WIN;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_LOSE;
		}
		else if (m_aPlayerInfo[PLAYER_PLAYER].janken == JANKEN_CHOKI)
		{// �v���C���[�̏���
			m_aPlayerInfo[PLAYER_COM].result = RESULT_LOSE;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_WIN;
		}
		else
		{// ������
			m_aPlayerInfo[PLAYER_COM].result = RESULT_DRAW;
			m_aPlayerInfo[PLAYER_PLAYER].result = RESULT_DRAW;
		}
		break;

	default:
		assert(("�͈͊O�̐��l������܂���", false));
		break;
	}

	if (m_aPlayerInfo[PLAYER_PLAYER].result == RESULT_LOSE)
	{
		m_nNumAllWin = 0;
	}

	// �I��
	m_flow = FLOW_END;
}

//===========================================================================================
// �A���ōs�����ǂ���
//===========================================================================================
void CJanken::continuity(void)
{
	m_nNum++;	// ����񂯂��

	if (m_aPlayerInfo[PLAYER_PLAYER].result == RESULT_WIN)
	{
		// �ŏ�����ɂ���
		SetFlow(FLOW_INIT);

		// �����Ă��Ȃ����
		m_aPlayerInfo[PLAYER_COM].bAnswer = false;
		m_aPlayerInfo[PLAYER_PLAYER].bAnswer = false;

		m_nNumAllWin++;	// ������
	}
	else
	{
		m_bJanken = false;
		//Uninit();
	}
}

//===========================================================================================
// ����񂯂�ݒ�
//===========================================================================================
void CJanken::SetJanken(EJanken janken)
{
	if (m_aPlayerInfo[PLAYER_PLAYER].bAnswer)
	{
		return;
	}

	if (janken < JANKE_MAX && janken > JANKEN_NONE && !m_aPlayerInfo[PLAYER_PLAYER].bAnswer)
	{
		m_aPlayerInfo[PLAYER_PLAYER].janken = janken;	// �ݒ肳�ꂽ����񂯂�
		m_aPlayerInfo[PLAYER_PLAYER].bAnswer = true;	// ������
		m_flow = FLOW_CHECK_ANSWER;	// �������킹
	}
	else
	{
		assert(("����񂯂�̐ݒ�Ɏ��s", false));
	}
}

//===========================================================================================
// ����ݒ�
//===========================================================================================
void CJanken::SetFlow(EFlow flow)
{
	if (flow > FLOW_NONE && flow < FLOW_MAX)
	{
		m_flow = flow;
	}
	else
	{
		assert(("����̐ݒ�Ɏ��s���܂���", false));
	}
}

//===========================================================================================
// ����񂯂�J�n�̔��f
//===========================================================================================
void CJanken::IsJanken(bool bJanken)
{
	m_bJanken = bJanken;
	m_flow = FLOW_INIT;
}

