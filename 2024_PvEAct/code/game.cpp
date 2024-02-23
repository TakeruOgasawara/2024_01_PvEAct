//===========================================================================================
//
// �Q�[�����[game.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <istream>

#include "game.h"

#include "managerEvent.h"
#include "managerSlow.h"
#include "managerUI.h"

#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "objectX.h"

#include "debugproc.h"
#include "sound.h"
#include "camera.h"
#include "billboard.h"

#include "inputKeyboard.h"
#include "fade.h"
#include "pause.h"

#include "player.h"
#include "enemyBoss.h"
#include "janken.h"

#include "player.h"

#include "meshWall.h"
#include "wepon.h"

#include "frame.h"
#include "gauge.h"

#include "lifeGauge.h"
#include "managerSlow.h"
#include "managerUI.h"

//�ÓI�����o�ϐ��錾
CPause *CGame::m_pPause = nullptr;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CGame::CGame()
{
	m_pSound = nullptr;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CGame::~CGame()
{

}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CGame::Init()
{
	// �Q�[����ԃ}�l�[�W���̐���
	CManagerEvent::Create();

	// ���̐���
	CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXTURE\\floor.png");

	// �G�̐���
	//CEnemyBoss::Create({ 100.0f, 0.0f, 0.0f });

	// �v���C���[�̐���
	CPlayer::Create({ 0.0f, 0.0f, -200.0f });

	//CWepon::Create("data\\MODEL\\wepon\\sword.x");

	// UI�}�l�[�W���̐���
	CManagerUI::Create();

	CObjectX::Create("data\\MODEL\\player1\\body.x");


	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CGame::Uninit()
{
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	// �Q�[����Ԃ̔j��
	//CManagerEvent::Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CGame::Update()
{
	// �|�C���^�̎擾
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	CInputKeyboard *pInputKey = CInputKeyboard::GetInstance();
	CFade *pFade = CManager::GetInstance()->GetFade();
	CEnemyBoss* pBoss = CEnemyBoss::GetInstance();

	//CManagerGameState::GetInstance()->Update();

	// �V�[���̍X�V
	CScene::Update();

	if (pBoss != nullptr)
	{
		if (pBoss->GetState() == pBoss->STATE_DEATH)
		{
			CManager::GetInstance()->GetFade()->Set(MODE_RESULT);
		}
	}
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CGame::Draw()
{
	CScene::Draw();

	if (m_pPause != nullptr)
	{
		m_pPause->Draw();
	}
}
