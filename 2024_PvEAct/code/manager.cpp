//===========================================================================================
//
// [manager.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include"renderer.h"
#include "object.h"
#include "manager.h"
#include "managerInput.h"
#include "debugproc.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "xfile.h"

#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"

#include "object3D.h"

//�ÓI�����o�ϐ��錾
CManager* CManager::m_pManager = nullptr;
CScene::EMode CScene::m_mode = CScene::MODE_TITLE;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CManager::CManager()
{
	// �l�̃N���A
	ZeroMemory(&m_info, sizeof(m_info));
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CManager::~CManager()
{

}

//===========================================================================================
// �C���X�^���X�̎擾
//===========================================================================================
CManager* CManager::GetInstance(void)
{
	if (m_pManager == nullptr)
	{
		m_pManager = new CManager;

		return m_pManager;
	}

	return m_pManager;
}

//===========================================================================================
// �j��
//===========================================================================================
void CManager::Release(void)
{
	if (m_pManager == nullptr)
	{
		return;
	}
	else
	{
		m_pManager->Uninit();

		delete m_pManager;
		m_pManager = nullptr;
	}
}


//===========================================================================================
// ����������
//===========================================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//---------------------------
	// �C���X�^���X����
	//---------------------------
	// �����_���[�̐���
	if (m_info.pRenderer == nullptr)
	{
		m_info.pRenderer = new CRenderer;

		if (m_info.pRenderer != nullptr)
		{
			//�����_���[�̏���������
			if (FAILED(m_info.pRenderer->Init(hWnd, TRUE)))
			{
				return E_FAIL;
			}
		}
	}

	// �C���v�b�g�}�l�[�W���[�̐���
	CManagerInput::Create(hInstance, hWnd);

	// �T�E���h
	//if (m_info.m_pSound == nullptr)
	//{
	//	m_info.m_pSound = new CSound;
	//	if (m_info.m_pSound != nullptr)
	//	{
	//		// �T�E���h�̏���������
	//		m_info.m_pSound->Init(hWnd);
	//	}
	//}
	// �f�o�b�O�v���b�N�̐���
	if (m_info.pDebugProc == nullptr)
	{
		m_info.pDebugProc = new CDebugProc;

		// ����������
		if (m_info.pDebugProc != nullptr)
		{
			m_info.pDebugProc->Init();
		}
	}
	// �J�����̐���
	if (m_info.pCamera == nullptr)
	{
		m_info.pCamera = new CCamera;

		// ����������
		if (m_info.pCamera != nullptr)
		{
			m_info.pCamera->Init();
		}
	}
	// ���C�g�̐���
	if (m_info.pLight == nullptr)
	{
		m_info.pLight = new CLight;

		// ����������
		if (m_info.pLight != nullptr)
		{
			m_info.pLight->Init();
		}
	}

	// �e�N�X�`���̐���
	CTexture::Create();

	// X�t�@�C���̐���
	CXfile::Create();

	// ���[�h�ݒ�
	SetMode(GetMode());
	
	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CManager::Uninit(void)
{
	// �S�I�u�W�F�N�g�̔j��(+�I������)
	CObject::ReleaseAll();

	// �C���v�b�g�}�l�[�W���[�̔j��
	CManagerInput::Release();

	// �f�o�b�O�v���b�N�j��
	if (m_info.pDebugProc != nullptr)
	{
		// �f�o�b�O�v���b�N�̏I������
		m_info.pDebugProc->Uninit();

		//�������̊J��
		delete m_info.pDebugProc;
		m_info.pDebugProc = nullptr;
	}
	// �����_���[�̔j��
	if (m_info.pRenderer != nullptr)
	{
		// �����_���[�̏I������
		m_info.pRenderer->Uninit();

		// �������̊J��
		delete m_info.pRenderer;
		m_info.pRenderer = nullptr;
	}
	// �T�E���h�̔j��
	//if (m_info.pSound != nullptr)
	//{
	//	//�T�E���h�̏I������
	//	m_info.pSound->Stop();
	//	m_info.pSound->Uninit();

	//	delete m_info.pSound;
	//	m_info.pSound = nullptr;
	//}
	// �J�����̔j��
	if (m_info.pCamera != nullptr)
	{
		//�J�����̏I������
		m_info.pCamera->Uninit();
		m_info.pCamera->Uninit();

		delete m_info.pCamera;
		m_info.pCamera = nullptr;
	}
	// ���C�g�̔j��
	if (m_info.pLight != nullptr)
	{
		//���C�g�̏I������
		m_info.pLight->Uninit();

		delete m_info.pLight;
		m_info.pLight = nullptr;
	}

	// �e�N�X�`���̔j��
	CTexture::Release();

	// X�t�@�C���̔j��
	CXfile::Release();

	// �V�[���̏I���A�j��
	if (m_info.pScene != nullptr)
	{
		// �V�[���̏I������
		m_info.pScene->Uninit();
		delete m_info.pScene;
		m_info.pScene = nullptr;
	}
	
	if (m_info.pFade != nullptr)
	{
		m_info.pFade = nullptr;
	}
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CManager::Update(void)
{
	CManager::GetDebugProc()->Print("FPS[%d]\n", GetFps());
	CManager::GetDebugProc()->Print("�I�u�W�F�N�g����[%d]\n\n", CObject::GetNumAll());

	m_info.pDebugProc->Print("�y ������@ �z\n");
	m_info.pDebugProc->Print("F1 : �f�o�b�O�\��[ON/OFF]\nF2 : �@���\��[ON/OFF]\n");

	// �f�o�b�O�v���b�N�̍X�V����
	if (CManager::GetDebugProc() != nullptr)
	{
		CManager::GetDebugProc()->Update();
	}

	// �C���v�b�g�}�l�[�W���[�̍X�V����
	CManagerInput::GetInstance()->Update();

	// �V�[���̍X�V
	if (m_info.pScene != nullptr)
	{
		m_info.pScene->Update();
	} 
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CManager::Draw(void)
{
	//�����_���[�̕`�揈��
	m_info.pScene->Draw();
}

//===========================================================================================
// ���[�h�̐ݒ�
//===========================================================================================
void CManager::SetMode(CScene::EMode mode)
{
	//// �T�E���h�̒�~
	//if (m_pManager->m_info.pSound != nullptr)
	//{
	//	m_pManager->m_info.pSound->Stop();
	//}

	// ���݂̃��[�h��j��
	if (m_pManager->m_info.pScene != nullptr)
	{
		m_pManager->m_info.pScene->Uninit();
		delete m_pManager->m_info.pScene;
		m_pManager->m_info.pScene = nullptr;
	}

	// �S�I�u�W�F�N�g�̍폜
	CObject::ReleaseAll();

	// �e�N�X�`���̏I���A�j��
	if (CTexture::GetInstance() != nullptr)
	{
		// �e�N�X�`���̔j��
		CTexture::Release();

		// �e�N�X�`���̐���
		CTexture::Create();
	}

	// X�t�@�C���̔j��
	if (CXfile::GetInstance() != nullptr)
	{
		// X�t�@�C���̔j��
		CXfile::Release();

		// X�t�@�C���̐���
		CXfile::Create();
	}
	
	// �t�F�[�h�̍폜
	m_pManager->m_info.pFade = nullptr;

	// �V�������[�h�̐���
	m_pManager->m_info.pScene = CScene::Create(mode);
	m_pManager->m_info.pScene->SetMode(mode);

	if (m_pManager->m_info.pScene != nullptr)
	{
		m_pManager->m_info.pScene->Init();
	}

	// �t�F�[�h�̐���
	if (m_pManager->m_info.pFade == nullptr)
	{
		m_pManager->m_info.pFade = CFade::Create(mode);
	}
}