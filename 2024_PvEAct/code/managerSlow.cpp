//======================================================================================
//
// �X���[����[slow.cpp]
// Author;���}���@�V
//
//======================================================================================
#include "managerSlow.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "inputJoypad.h"
#include "inputKeyboard.h"
#include "inputMouse.h"

// �ÓI�����o�ϐ��錾
CSlowManager* CSlowManager::m_apSlow[CAMP_MAX][MAX_OBJECT] = {};
int CSlowManager::m_nNumAll = 0;

//======================================================================================
// �R���X�g���N�^
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
	{// �͈͊O�̏ꍇ�A�����Ȃ�
		m_camp = CAMP_NONE;
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apSlow[m_camp][nCntObject] == nullptr)
		{
			m_apSlow[m_camp][nCntObject] = this;	//�������g����
			m_nID = nCntObject;	//�������g��ID��ۑ�
			m_nNumAll++;	//�������J�E���g�A�b�v

			break;
		}
	}
}

//======================================================================================
// �f�X�g���N�^
//======================================================================================
CSlowManager::~CSlowManager()
{

}

//======================================================================================
// ����
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
// ����
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
// ����������
//======================================================================================
HRESULT CSlowManager::Init()
{
	m_fTime = 1.0f;

	return S_OK;
}

//======================================================================================
// �I������
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
// �X�V����
//======================================================================================
void CSlowManager::Update()
{
	
}

//======================================================================================
// �S�j��
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
// �S�̂��X���[�ɂ���
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

			// �l��ݒ�(�S��)
			m_apSlow[nCntCamp][i]->SetValue(fValue);
		}
	}
}

//======================================================================================
// �S�̂�ʏ�֖߂�
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

			// �l���f�t�H���g(�S��)
			m_apSlow[nCntCamp][i]->SetValue(1.0f);
		}
	}
}

//======================================================================================
// �v���C���[�w�c�̐ݒ�
//======================================================================================
void CSlowManager::Player(float fValue)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (m_apSlow[CAMP_PAYER][i] == nullptr)
		{
			continue;
		}

		// �l��ݒ�(�S��)
		m_apSlow[CAMP_PAYER][i]->SetValue(fValue);
	}
}

//======================================================================================
// �G�w�c�̐ݒ�
//======================================================================================
void CSlowManager::Enemy(float fValue)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (m_apSlow[CAMP_ENEMY][i] == nullptr)
		{
			continue;
		}

		// �l��ݒ�(�S��)
		m_apSlow[CAMP_ENEMY][i]->SetValue(fValue);
	}
}

//======================================================================================
// �w�c�̃^�O�ݒ�
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
			// �l��ݒ�(�w�c�̃^�O)
			m_apSlow[camp][i]->SetValue(fValue);
		}
	}
}
