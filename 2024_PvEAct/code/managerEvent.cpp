#include "managerEvent.h"
#include "object.h"

#include "debugproc.h"

#include "inputKeyboard.h"
#include "fade.h"
#include "pause.h"

#include "player.h"
#include "enemyBoss.h"

#include "frame.h"
#include "managerSlow.h"
#include "texture.h"
#include "managerUI.h"

// �萔��`
namespace
{
	static const float SLOW_TIME = 10.0f;	// �X���[����
	static const int RUSH_TIME = 10;	// ���b�V������
}

// �ÓI�����o�ϐ�
CManagerEvent* CManagerEvent::m_pManager = nullptr;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CManagerEvent::CManagerEvent()
{
	m_mode = MODE_NORMAL;
	m_fSlowTime = 0.0f;
	m_nRushTime = 0;
	m_pSlowManager = nullptr;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CManagerEvent::~CManagerEvent()
{

}

//===========================================================================================
// �C���X�^���X����
//===========================================================================================
CManagerEvent* CManagerEvent::Create(void)
{
	if (m_pManager == nullptr)
	{
		m_pManager = new CManagerEvent;

		m_pManager->Init();

		return m_pManager;
	}

	return m_pManager;
}

//===========================================================================================
// �C���X�^���X�擾
//===========================================================================================
CManagerEvent* CManagerEvent::GetInstance(void)
{
	if (m_pManager == nullptr)
	{
		//assert(("�C�x���g�}�l�[�W���̎擾�Ɏ��s", false));
	}

	return m_pManager;
}

//===========================================================================================
// �C���X�^���X�̔j��
//===========================================================================================
void CManagerEvent::Release(void)
{
	if (m_pManager != nullptr)
	{
		m_pManager->Uninit();
		delete m_pManager;
		m_pManager = nullptr;
	}
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CManagerEvent::Init(void)
{
	// �X���[�}�l�[�W���̐���
	m_pSlowManager = CSlowManager::Create();

	m_fSlowTime = SLOW_TIME;
	m_nRushTime = RUSH_TIME;
	m_mode = MODE_NORMAL;

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CManagerEvent::Uninit(void)
{
	if (m_pSlowManager != nullptr)
	{
		m_pSlowManager->Uninit();
		m_pSlowManager = nullptr;
	}

 	CObject::Release();

	// UI�}�l�[�W���̔j��
	//CManagerUI::Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CManagerEvent::Update(void)
{
	// �X���[�̍X�V
	SlowUpdate();

	// �퓬�̏��
	switch (m_mode)
	{
	case CManagerEvent::EGameMode::MODE_NORMAL:

		break;
	case CManagerEvent::EGameMode::MODE_JUST:

		break;
	case CManagerEvent::EGameMode::MODE_RUSH:
		if (m_ui.pRushUI == nullptr)
		{
			m_ui.pRushUI = CObject2D::Create({ 0.0f, 0.0f, 0.0f });
		}
		else
		{
			m_ui.pRushUI->BindTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\ui_attackLB.png"));
			m_ui.pRushUI->SetPosition({ SCREEN_WIDTH * 0.5f, 600.0f, 0.0f });
		}
		break;

	case CManagerEvent::EGameMode::MODE_JANKEN:
		break;
	default:
		break;
	}
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CManagerEvent::Draw(void)
{

}

//===========================================================================================
// �X���[�X�V
//===========================================================================================
void CManagerEvent::SlowUpdate(void)
{
	if (m_pSlowManager != nullptr)
	{
		CPlayer* pPlayer = CPlayer::GetInstance();
		CEnemyBoss* pBoss = CEnemyBoss::GetInstance();

		if (pPlayer != nullptr)
		{
			if (pPlayer->GetIsJust())
			{// �W���X�g������Ă����ꍇ

				if (m_fSlowTime > 0.0f)
				{// ���ׂĂ̑��x�𗎂Ƃ�(�o�^���Ă���I�u�W�F�N�g�̂�)
					m_fSlowTime -= CManager::GetInstance()->GetTick() * 10.0f;

					m_pSlowManager->All(0.05f);
				}
				else
				{
					m_pSlowManager->Player(1.0f);
					m_pSlowManager->Enemy(0.05f);
					//m_fSlowTime = 0.0f;
				}
			}
		}
	}
}

//===========================================================================================
// �ʏ�
//===========================================================================================
void CManagerEvent::Normal(void)
{

}

//===========================================================================================
// ���b�V��
//===========================================================================================
void CManagerEvent::Rush(void)
{

}

//===========================================================================================
// ����񂯂�
//===========================================================================================
void CManagerEvent::Janken(void)
{

}
