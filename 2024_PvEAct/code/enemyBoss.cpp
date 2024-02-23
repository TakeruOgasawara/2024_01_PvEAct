//===========================================================================================
//
// [player3D.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <stdio.h>
#include "enemyBoss.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputJoypad.h"
#include "debugproc.h"
#include "camera.h"
#include "motion.h"
#include "general.h"
#include "colliderSphere.h"
#include "managerSlow.h"

#include "objectX.h"
#include "modelParts.h"
#include "player.h"
#include "fade.h"
#include "scene.h"
#include "managerEvent.h"
#include "object3D.h"

// �萔��`
namespace
{
	static const float MOVE_SPEED = 5.5f;	// �ړ���
	static const float ATT = 0.5f;	// �ړ��ʂ̌���
	static const float ADJUST_ROT = 0.08f;	// �����̕␳
	static const float GRAVITY = 1.0f;	// �d��
	static const float LIFE = 0.3f;

	static const float ATTACK_LENGTH = 300.0f;	// �U������̋���
	static const float ATTACK_RANGE = 0.6f;	// �U������͈̔�
	static const float BLOWN_AWAY = 10.0f;	// ������ї�

}

// �ÓI�����o�ϐ��錾
CEnemyBoss* CEnemyBoss::m_pEnemy = nullptr;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CEnemyBoss::CEnemyBoss(int nPriority)
{
	// �l�̃N���A
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_motionInfo, sizeof(m_motionInfo));
	m_bAttack = false;
	m_pSphereCollider = nullptr;
	m_pObjectX = nullptr;
	m_pAttackRange = nullptr;
	m_pSlow = nullptr;
	m_DiffLife = 0.0f;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CEnemyBoss::~CEnemyBoss()
{

}

//===========================================================================================
// ����
//===========================================================================================
CEnemyBoss *CEnemyBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pEnemy == nullptr)
	{
		m_pEnemy = new CEnemyBoss;

		if (m_pEnemy != nullptr)
		{
			m_pEnemy->Init(pos, rot);

			return m_pEnemy;
		}
	}

	return m_pEnemy;
}

//===========================================================================================
// �C���X�^���X�̎擾
//===========================================================================================
CEnemyBoss* CEnemyBoss::GetInstance(void)
{
	if (m_pEnemy == nullptr)
	{
		return nullptr;
		//assert(("�G�l�~�[���̎擾�Ɏ��s", false));
	}

	return m_pEnemy;
}

//===========================================================================================
// �j��
//===========================================================================================
void CEnemyBoss::Release(void)
{
	if (m_pEnemy == nullptr)
	{
		return;
	}
	else
	{
		//m_pEnemy->Uninit();

		delete m_pEnemy;
		m_pEnemy = nullptr;
	}
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CEnemyBoss::Init(void)
{
	return S_OK;
}

//===========================================================================================
// ����������(�I�[�o�[���[�h)
//===========================================================================================
HRESULT CEnemyBoss::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_info.fLife = LIFE;

	CCharacter::Init("data\\TEXT\\motion\\enemyBoss\\motion.txt");

	SetMotion(MOTION_NORMAL);

	// �X���[����
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_ENEMY, CSlowManager::TAG_ENEMY);

	// �����蔻��̐���
	m_pSphereCollider = CSphereCollider::Create(CSphereCollider::TAG_ENEMY_BOSS, CSphereCollider::TYPE_SPHERE, this);

	if (m_pSphereCollider != nullptr)
	{
		m_pSphereCollider->SetRadius(40.0f);
	}

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CEnemyBoss::Uninit(void)
{
	m_pEnemy = nullptr;

	CCharacter::Uninit();

	// �������g�̔j��
	CObject::Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CEnemyBoss::Update(void)
{
	if (GetBody() == nullptr)
	{
		return;
	}

	// �ϐ��錾
	float fSlow = 1.0f;

	CCharacter::Update();

	// �Q�[�����[�h�̎擾
	CManagerEvent* pEvent = CManagerEvent::GetInstance();

	if (pEvent != nullptr)
	{
		// �퓬�̏��
		switch (pEvent->GetMode())
		{
		case pEvent->MODE_NORMAL:
			BattleNormal();
			break;
		case pEvent->MODE_JUST:
			BattleJust();
			break;
		case pEvent->MODE_RUSH:
			BattleRush();
			break;
		case pEvent->MODE_JANKEN:
			BattleJanken();
			break;
		default:
			break;
		}

		// ���[�V�����ݒ�
		//Motion();

		// �ړ��ʂ��X�V(����������)
		m_info.move.x += (0.0f - m_info.move.x) * ATT;
		m_info.move.z += (0.0f - m_info.move.z) * ATT;

		if (m_pSlow != nullptr)
		{
			fSlow = m_pSlow->GetValue();
		}

		// ���[�V�����̃X���[
		GetBody()->SetMmultiply(fSlow);

		// �ړ��ʂ̑��
		m_info.pos += m_info.move * fSlow;

		if (m_pSphereCollider != nullptr)
		{
			m_pSphereCollider->SetPosition(m_info.pos);
		}

		// ���f
		if (m_motionInfo.pMotion != nullptr)
		{
			m_motionInfo.pMotion->SetPosition(m_info.pos);
			m_motionInfo.pMotion->SetRotation(m_info.rot);
		}

		if (m_info.fLife == 0.0f)
		{
			CManager::GetInstance()->GetFade()->Set(CScene::MODE_RESULT);
		}
	}
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CEnemyBoss::Draw(void)
{
	CCharacter::Draw();

	//if (m_motionInfo.pMotion != nullptr)
	//{// �}�g���b�N�X�̊|�����킹
	//	general::SetMatrix(&m_info.mtxWorld, m_motionInfo.pMotion->GetPosition());
	//}

	// �f�o�b�O�\��
	Debug();
}

//===========================================================================================
// �q�b�g����
//===========================================================================================
void CEnemyBoss::Hit(float fDamage)
{
	m_DiffLife = fDamage;
	m_info.fLife -= fDamage;

	if (m_info.fLife <= 0.0f)
	{
		m_info.fLife = 0.0f;

		m_info.state = STATE_DEATH;
	}
}

//===========================================================================================
// �ʏ�
//===========================================================================================
void CEnemyBoss::BattleNormal(void)
{
	//Move();
	Rot();
	//Attack();
}

//===========================================================================================
// �W���X�g���
//===========================================================================================
void CEnemyBoss::BattleJust(void)
{

}

//===========================================================================================
// ���b�V��
//===========================================================================================
void CEnemyBoss::BattleRush(void)
{

}

//===========================================================================================
// ����񂯂�
//===========================================================================================
void CEnemyBoss::BattleJanken(void)
{

}

//===========================================================================================
// �����␳
//===========================================================================================
void CEnemyBoss::RotAor(void)
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

//===========================================================================================
// �U��
//===========================================================================================
void CEnemyBoss::Attack(void)
{
	int nKey = GetBody()->GetKey();	// ���݂̃L�[
	int nNumKey = GetBody()->GetMotionInfo(GetType())->nNumKey;	// �L�[�̑���
	int nTypeMotion = GetBody()->GetType();
	float fTick = CManager::GetInstance()->GetTick() * 10.0f;
	bool bFinish = GetBody()->GetMotion().bFinish;

	if (nTypeMotion == MOTION_ATTACK00)
	{
		if (bFinish)
		{
			m_bAttack = true;
		}

		if (nKey == 2)
		{
			IsAttack();
		}
		if (nKey >= 1)
		{
			// �ړ�
			m_info.move.x = sinf(m_info.rot.y + D3DX_PI) * fTick * 10.0f;
			m_info.move.z = cosf(m_info.rot.y + D3DX_PI) * fTick * 10.0f;
		}
	}
}

//===========================================================================================
// �U������
//===========================================================================================
bool CEnemyBoss::IsAttack(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return false;
	}

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 movePlayer = pPlayer->GetMovement();

	float vec1 = 0.0f, vec2 = 0.0f;	// �x�N�g��
	D3DXVECTOR3 pos0 = {};	// �n�_
	D3DXVECTOR3 pos1 = {}, pos2 = {};	// �I�_1/�I�_2
	D3DXVECTOR3 vecLine = {}, vecToPos = {};	// �n�_�ƏI�_�̋���/�n�_����Ώۂ̋���
	D3DXVECTOR3 vecLine1 = {};	// �n�_�ƏI�_�̋���

	// �U���͈͂̐ݒ�
	float fLength = sqrtf
	(
		(posPlayer.x - m_info.pos.x) * (posPlayer.x - m_info.pos.x) +
		(posPlayer.z - m_info.pos.z) * (posPlayer.z - m_info.pos.z)
	);

	// �p�x�����߂�
	float fAngle = atan2f((posPlayer.x - m_info.pos.x), (posPlayer.z - m_info.pos.z));

	// �͈͓�����
	if (fLength < ATTACK_LENGTH)
	{
		// �n�_�̌v�Z
		pos0.x = m_info.pos.x;
		pos0.y = m_info.rot.y;
		pos0.z = m_info.pos.z;
		// �I�_�̌v�Z
		pos1.x = pos0.x + (sinf(m_info.rot.y + D3DX_PI * ATTACK_RANGE));
		pos1.y = 0.0f;
		pos1.z = pos0.z + (cosf(m_info.rot.y + D3DX_PI * ATTACK_RANGE));
		// �I�_1�̌v�Z
		pos2.x = pos0.x + (sinf(m_info.rot.y + -D3DX_PI * ATTACK_RANGE));
		pos2.y = 0.0f;
		pos2.z = pos0.z + (cosf(m_info.rot.y + -D3DX_PI * ATTACK_RANGE));
		
		// �n�_�ƏI�_�Ƃ̋�����
		vecLine = pos1 - pos0;
		vecLine1 = pos2 - pos0;

		// �n�_����Ώۂ܂ł̈ʒu
		vecToPos = posPlayer - pos0;

		// �x�N�g������
		vec1 = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
		vec2 = (vecLine1.z * vecToPos.x) - (vecLine1.x * vecToPos.z);

		if (vec1 > 0 && vec2 < 0)
		{
			CManager::GetInstance()->GetDebugProc()->Print("\n�y�y������z�z\n");

			//if (m_bAttack)
			// �W���X�g��𔻒�
			if (pPlayer->IsMotion().bAvoidance)
			{
				//float time = pPlayer->GetAvoidanceTime();

				//if (time > 1.8f)
				//{
				//	//pPlayer->SetIsJust(true);
				//	//CManagerGameState::GetInstance()->SetMode(CManagerGameState::MODE_JUST);
				//}

				m_bAttack = false;
				return false;
			}
			else if (m_bAttack)
			{
				// ������΂�
				pPlayer->SetMovement(
					{
						sinf(fAngle) * BLOWN_AWAY,
						0.0f,
						cosf(fAngle) * BLOWN_AWAY,
					});

				pPlayer->Hit(0.1f);

				return true;
			}
		}
	}
	return false;
}

//===========================================================================================
// ����
//===========================================================================================
void CEnemyBoss::Rot(void)
{
	// ���̎擾
	CPlayer* pPlayer = CPlayer::GetInstance();	// �{�X
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// �{�X�ʒu

	// �p�x�����߂�
	float fAngle = atan2f((m_info.pos.x - posPlayer.x), (m_info.pos.z - posPlayer.z));

	// �{�X�̕���������
	m_info.rot.y = fAngle;
}

//===========================================================================================
// �s��
//===========================================================================================
void CEnemyBoss::Move(void)
{
	// ���̎擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();	// �J����
	CPlayer* pPlayer = CPlayer::GetInstance();	// �{�X
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// �{�X�ʒu

	// �p�x�����߂�
	float fAngle = atan2f((m_info.pos.x - posPlayer.x), (m_info.pos.z - posPlayer.z));

	// ���������߂�
	float fLength = sqrtf((m_info.pos.x - posPlayer.x) * (m_info.pos.x - posPlayer.x) + (m_info.pos.z - posPlayer.z) * (m_info.pos.z - posPlayer.z));

	//�G�ƃv���C���[�̋��������߂�
	D3DXVECTOR3 e = 
	{
		(posPlayer.x - m_info.pos.x),
		0.0f,
		(posPlayer.z - m_info.pos.z)
	};

	//�ړ��ʂɑ��
	m_info.move.x += e.x;
	m_info.move.z += e.z;

	//�ړ��ʂ𐳋K������
	D3DXVec3Normalize(&m_info.move, &m_info.move);

	// ���x
	m_info.move.x *= MOVE_SPEED;
	m_info.move.z *= MOVE_SPEED;
}

//===========================================================================================
// �ːi
//===========================================================================================
void CEnemyBoss::Rush(void)
{

}

//===========================================================================================
// ���[�V����
//===========================================================================================
void CEnemyBoss::Motion(void)
{
	// ���̎擾
	CPlayer* pPlayer = CPlayer::GetInstance();	// �{�X
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// �{�X�ʒu
	int nTypeMotion = GetBody()->GetType();


	// �p�x�����߂�
	//float fAngle = atan2f((m_info.pos.x - posPlayer.x), (m_info.pos.z - posPlayer.z));

	// ���������߂�
	float fLength = sqrtf((m_info.pos.x - posPlayer.x) * (m_info.pos.x - posPlayer.x) + (m_info.pos.z - posPlayer.z) * (m_info.pos.z - posPlayer.z));

	if (fLength < ATTACK_LENGTH)
	{// �͈͓��ɂ�����U�����[�V����
		if (nTypeMotion != MOTION_ATTACK00)
		{
			SetMotion(MOTION_ATTACK00);
		}
	}

	if (m_bAttack && GetBody()->GetMotion().bFinish)
	{
		if (nTypeMotion != MOTION_NORMAL)
		{
			SetMotion(MOTION_NORMAL);
		}
	}
}

//===========================================================================================
// �f�o�b�O�\��
//===========================================================================================
void CEnemyBoss::Debug(void)
{
#ifndef _DEBUG
	return;
#endif

	//�f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print("�y�G���z\n");
	CManager::GetInstance()->GetDebugProc()->Print("�ʒu�F x:%f y:%f z:%f\n", m_info.pos.x, m_info.pos.y, m_info.pos.z);
	CManager::GetInstance()->GetDebugProc()->Print("�����F x:%f y:%f z:%f\n", m_info.rot.x, m_info.rot.y, m_info.rot.z);
	CManager::GetInstance()->GetDebugProc()->Print("�ړ��ʁF x:%f y:%f z:%f\n", m_info.move.x, m_info.move.y, m_info.move.z);
	CManager::GetInstance()->GetDebugProc()->Print("�̗́F %f\n\n", m_info.fLife);
}
