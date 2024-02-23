#include "managerUI.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "texture.h"
#include "managerEvent.h"
#include "lifeGauge.h"
#include "time.h"

#include "player.h"
#include "enemyBoss.h"

// �萔�錾
namespace
{
	// �̗�UI =====================
	// �v���C���[
	// �Q�[�W
	static const D3DXVECTOR3 PLAYER_LIFE_GAUGE_POS = { 20.0f, 650.0f, 0.0f };	// �ʒu
	static const D3DXVECTOR2 PLAYER_LIFE_GAUGE_SIZE = { 600.0f, 20.0f };		// �T�C�Y
	static const D3DXCOLOR PLAYER_LIFE_GAUGE_COL = { 0.0f, 1.0f, 0.0f, 1.0f };	// �F
	static const char* PLAYER_LIFE_GAUGE_PATH = "";
	// �t���[��
	static const D3DXVECTOR3 PLAYER_LIFE_FRAME_POS = { 20.0f, 650.0f, 0.0f };	// �ʒu
	static const D3DXVECTOR2 PLAYER_LIFE_FRAME_SIZE = { 600.0f, 20.0f };		// �T�C�Y
	static const D3DXCOLOR PLAYER_LIFE_FRAME_COL = { 0.5f, 0.5f, 0.5f, 1.0f };	// �F
	static const char* PLAYER_LIFE_FRAME_PATH = "";

	// �G(boss)
	// �Q�[�W
	static const D3DXVECTOR3 ENEMY_LIFE_GAUGE_POS = { 20.0f, 20.0f, 0.0f };		// �ʒu
	static const D3DXVECTOR2 ENEMY_LIFE_GAUGE_SIZE = { 900.0f, 20.0f };			// �T�C�Y
	static const D3DXCOLOR ENEMY_LIFE_GAUGE_COL = { 0.0f, 1.0f, 0.0f, 1.0f };	// �F
	static const char* ENEMY_LIFE_GAUGE_PATH = "";
	// �t���[��
	static const D3DXVECTOR3 ENEMY_LIFE_FRAME_POS = { 20.0f, 20.0f, 0.0f };		// �ʒu
	static const D3DXVECTOR2 ENEMY_LIFE_FRAME_SIZE = { 900.0f, 20.0f };			// �T�C�Y
	static const D3DXCOLOR ENEMY_LIFE_FRAME_COL = { 0.5f, 0.5f, 0.5f, 1.0f };	// �F
	static const char* ENEMY_LIFE_FRAME_PATH = "";
}

// �ÓI�����o�ϐ�
CManagerUI* CManagerUI::m_pUiManager = nullptr;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CManagerUI::CManagerUI()
{
	// �l�̃N���A
	ZeroMemory(&m_ui, sizeof(m_ui));
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CManagerUI::~CManagerUI()
{
	
}

//===========================================================================================
// ����
//===========================================================================================
CManagerUI* CManagerUI::Create(void)
{
	if (m_pUiManager == nullptr)
	{
		m_pUiManager = new CManagerUI;

		m_pUiManager->Init();

		return m_pUiManager;
	}

	return m_pUiManager;
}

//===========================================================================================
// �C���X�^���X����
//===========================================================================================
CManagerUI* CManagerUI::GetInstance(void)
{
	if (m_pUiManager == nullptr)
	{
		assert(("�}�l�[�W��UI�̎擾�Ɏ��s", false));
	}

	return m_pUiManager;
}

//===========================================================================================
// �C���X�^���X�̔j��
//===========================================================================================
void CManagerUI::Release(void)
{
	if (m_pUiManager != nullptr)
	{
		m_pUiManager->Uninit();

		delete m_pUiManager;
		m_pUiManager = nullptr;
	}
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CManagerUI::Init(void)
{
	// �̗�UI�̐���
	CreateLife();

	CreateTime();

	SetLifeEnemy();

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CManagerUI::Uninit(void)
{
	if (m_ui.pRushUi != nullptr)
	{
		m_ui.pRushUi = nullptr;
	}

	CObject::Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CManagerUI::Update(void)
{
	// �Q�[����Ԃ̎擾
	CManagerEvent* pEvent = CManagerEvent::GetInstance();	// �Q�[�����

	if (pEvent != nullptr)
	{
		// �퓬�̏��
		switch (pEvent->GetMode())
		{
		case pEvent->MODE_NORMAL:

			break;
		case pEvent->MODE_JUST:

			break;
		case pEvent->MODE_RUSH:
			if (m_ui.pRushUi == nullptr)
			{
				m_ui.pRushUi = CObject2D::Create({ 0.0f, 0.0f, 0.0f });
			}
			else
			{
				m_ui.pRushUi->BindTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\ui_attackLB.png"));
				m_ui.pRushUi->SetPosition({ SCREEN_WIDTH * 0.5f, 600.0f, 0.0f });
			}
			break;

		case pEvent->MODE_JANKEN:
			break;
		default:
			break;
		}
	}

	UpdateLife();
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CManagerUI::Draw(void)
{

}

//===========================================================================================
// �̗�UI�̐���
//===========================================================================================
void CManagerUI::CreateLife(void)
{
	// �v���C���[���C�tUI
	m_ui.pGaugePlayer = CLifeGauge::Create();

	if (m_ui.pGaugePlayer != nullptr)
	{
		m_ui.pGaugePlayer->SetPosition(PLAYER_LIFE_GAUGE_POS, PLAYER_LIFE_FRAME_POS);
		m_ui.pGaugePlayer->SetSizeFrame(PLAYER_LIFE_FRAME_SIZE.x, PLAYER_LIFE_FRAME_SIZE.y);
		m_ui.pGaugePlayer->SetSizeGauge(PLAYER_LIFE_GAUGE_SIZE.x, PLAYER_LIFE_GAUGE_SIZE.y);
		m_ui.pGaugePlayer->SetColor(PLAYER_LIFE_GAUGE_COL, PLAYER_LIFE_FRAME_COL);
		m_ui.pGaugePlayer->BindTexture(PLAYER_LIFE_GAUGE_PATH, PLAYER_LIFE_FRAME_PATH);
	}

	// �G���C�tUI
	m_ui.pGaugeEnemy = CLifeGauge::Create();

	if (m_ui.pGaugeEnemy != nullptr)
	{
		m_ui.pGaugeEnemy->SetPosition(ENEMY_LIFE_GAUGE_POS, ENEMY_LIFE_FRAME_POS);
		m_ui.pGaugeEnemy->SetSizeFrame(ENEMY_LIFE_FRAME_SIZE.x, ENEMY_LIFE_FRAME_SIZE.y);
		m_ui.pGaugeEnemy->SetSizeGauge(ENEMY_LIFE_GAUGE_SIZE.x, ENEMY_LIFE_GAUGE_SIZE.y);
		m_ui.pGaugeEnemy->SetColor(ENEMY_LIFE_GAUGE_COL, ENEMY_LIFE_FRAME_COL);
		m_ui.pGaugeEnemy->BindTexture(ENEMY_LIFE_GAUGE_PATH, ENEMY_LIFE_FRAME_PATH);

	}
}

//===========================================================================================
// �̗͂̐ݒ�
//===========================================================================================
void CManagerUI::SetLifePlayer(void)
{
	/*CPlayer* pPlayer = nullptr;
	pPlayer = CPlayer::GetInstance();

	if (m_ui.pGaugePlayer != nullptr && pPlayer != nullptr)
	{
		m_ui.pGaugePlayer->SetLife(pPlayer->GetL)
	}*/
}

//===========================================================================================
// �̗͂̐ݒ�
//===========================================================================================
void CManagerUI::SetLifeEnemy(void)
{
	CEnemyBoss* pEnemy = nullptr;
	pEnemy = CEnemyBoss::GetInstance();

	if (m_ui.pGaugeEnemy != nullptr && pEnemy != nullptr)
	{
		m_ui.pGaugeEnemy->SetLife(pEnemy->GetLife());
	}
}

//===========================================================================================
// �_���[�W�̐ݒ�
//===========================================================================================
void CManagerUI::UpdateLife(void)
{
	CEnemyBoss* pEnemy = nullptr;
	pEnemy = CEnemyBoss::GetInstance();

	if (m_ui.pGaugeEnemy != nullptr && pEnemy != nullptr)
	{
		float fDamage = pEnemy->GetDamage();

		m_ui.pGaugeEnemy->Damage(fDamage);

		pEnemy->SetDamage(0.0f);
	}
}

//===========================================================================================
// �^�C���̐���
//===========================================================================================
void CManagerUI::CreateTime(void)
{
	m_ui.pTime = CTime::Create({ 1090.0f, 30.0f, 1.0f });

	if (m_ui.pTime != nullptr)
	{
		m_ui.pTime->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
		m_ui.pTime->SetSize(20.0f, 40.0f);
	}
}
