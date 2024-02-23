//===========================================================================================
//
// [player3D.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <stdio.h>
#include "enemy.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputJoypad.h"
#include "debugproc.h"
#include "camera.h"
#include "motionDiv.h"
#include "general.h"

// �萔��`
namespace
{
	static const float MOVE_SPEED = 5.5f;	// �ړ���
	static const float ATT = 0.5f;	// �ړ��ʂ̌���
	static const float ADJUST_ROT = 0.08f;	// �����̕␳
	static const float GRAVITY = 1.0f;	// �d��
}

// �ÓI�����o�ϐ��錾
CEnemy* CEnemy::m_pEnemy = nullptr;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CEnemy::CEnemy(int nPriority)
{
	// �l�̃N���A
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_motionInfo, sizeof(m_motionInfo));
	
	m_pCollision = nullptr;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CEnemy::~CEnemy()
{

}

//===========================================================================================
// ����
//===========================================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pEnemy == nullptr)
	{
		m_pEnemy = new CEnemy;

		if (m_pEnemy != nullptr)
		{
			m_pEnemy->Init(pos, rot);

			return m_pEnemy;
		}
	}

	return m_pEnemy;
}

//===========================================================================================
// ����
//===========================================================================================
CEnemy* CEnemy::GetInstance(void)
{
	if (m_pEnemy == nullptr)
	{
		assert(("�G�l�~�[���̎擾�Ɏ��s", false));
	}

	return m_pEnemy;
}

//===========================================================================================
// �j��
//===========================================================================================
void CEnemy::Release(void)
{
	if (m_pEnemy == nullptr)
	{
		return;
	}
	else
	{
		m_pEnemy->Uninit();

		delete m_pEnemy;
		m_pEnemy = nullptr;
	}
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CEnemy::Init(void)
{
	return S_OK;
}

//===========================================================================================
// ����������(�I�[�o�[���[�h)
//===========================================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//m_motionInfo.pMotionDiv = CMotionDiv::Create("data\\TEXT\\motion\\motion_playerUpper.txt", "data\\TEXT\\motion\\motion_playerLower.txt");

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CEnemy::Uninit(void)
{
	// �������g�̔j��
	CObject::Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CEnemy::Update(void)
{	
	// �ړ��ʂ��X�V(����������)
	m_info.move.x += (0.0f - m_info.move.x) * ATT;
	m_info.move.z += (0.0f - m_info.move.z) * ATT;

	// �ړ��ʂ̑��
	m_info.pos += m_info.move;

	// ���f
	if (m_motionInfo.pMotionDiv != nullptr)
	{
		m_motionInfo.pMotionDiv->SetPosition(m_info.pos);
		m_motionInfo.pMotionDiv->SetRotation(m_info.rot);
	}

	//�f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print("�y�v���C���[���z\n");
	CManager::GetInstance()->GetDebugProc()->Print("�ʒu�F x:%f y:%f z:%f\n", m_info.pos.x, m_info.pos.y, m_info.pos.z);
	CManager::GetInstance()->GetDebugProc()->Print("�����F x:%f y:%f z:%f\n", m_info.rot.x, m_info.rot.y, m_info.rot.z);
	CManager::GetInstance()->GetDebugProc()->Print("�ړ��ʁF x:%f y:%f z:%f\n\n", m_info.move.x, m_info.move.y, m_info.move.z);
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CEnemy::Draw(void)
{

}

//===========================================================================================
// �����␳
//===========================================================================================
void CEnemy::RotAor(void)
{
	m_info.fRotDiff = m_info.fRotDest - m_info.rot.y;	//�ڕW�̊p�x�܂ł̍��������߂�

	if (m_info.fRotDiff > D3DX_PI)
	{
		m_info.fRotDiff -= (D3DX_PI * 2);
	}
	if (m_info.fRotDiff < -D3DX_PI)
	{
		m_info.fRotDiff += (D3DX_PI * 2);
	}

	m_info.rot.y += m_info.fRotDiff * ADJUST_ROT;	//�␳����

	// �����̏C��
	general::RotLimit(&m_info.rot);
}