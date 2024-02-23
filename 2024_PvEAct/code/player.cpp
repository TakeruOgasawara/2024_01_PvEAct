//===========================================================================================
//
// [player.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputJoypad.h"
#include "inputMouse.h"
#include "debugproc.h"
#include "camera.h"
#include "motionDiv.h"
#include "general.h"

#include "lockOn.h"
#include "enemyBoss.h"
#include "janken.h"
#include "colliderSphere.h"
#include "characterDiv.h"
#include "character.h"
#include "motion.h"

#include "wepon.h"
#include "modelParts.h"

#include "managerSlow.h"

#include "managerEvent.h"
#include "managerInput.h"

// �萔��`
namespace
{
	static const float MOVE_SPEED = 4.5f;	// �ړ���
	static const float ATT = 0.6f;	// �ړ��ʂ̌���
	static const float ADJUST_ROT = 0.08f;	// �����̕␳
	static const float GRAVITY = 0.0f;	// �d��
	static const float LIFE = 10.0f;
	static const float ATTACK_LENGTH = 200.0f;	// �U������̋���
	static const float ATTACK_RANGE = 0.6f;	// �U������͈̔�
	static const float BLOWN_AWAY = 10.0f;	// ������ї�

	static const float AVOIDANCE_TIME = 5.0f;	// �������
	static const float AVOIDANCE_SPEED = 3.0f;	// ��𑬓x (1.0f * AVOIDANCE_SPEED)

	static const D3DXVECTOR3 LIMIT_POS = { 1500.0f, 0.0f, 1500.0f };	// �s���͈�(���R���œ���)
}

// �ÓI�����o�ϐ��錾
CPlayer* CPlayer::m_pPlayer = nullptr;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CPlayer::CPlayer(int nPriority)
{
	// �l�̃N���A
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_motionInfo, sizeof(m_motionInfo));
	ZeroMemory(&m_isTriger, sizeof(m_isTriger));
	ZeroMemory(&m_avoid, sizeof(m_avoid));
	m_bLockon = false;
	m_fCntDash = 0.0f;
	m_fTempLength = 0.0f;
	m_pSphereCollider = nullptr;
	m_pLockon = nullptr;
	m_pJanken = nullptr;
	m_pWepon = nullptr;
	m_pAfterImage = nullptr;
	m_pSlow = nullptr;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CPlayer::~CPlayer()
{

}

//===========================================================================================
// ����
//===========================================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CPlayer;

		if (m_pPlayer != nullptr)
		{
			m_pPlayer->Init(pos, rot);

			return m_pPlayer;
		}
	}

	return m_pPlayer;
}

//===========================================================================================
// ����
//===========================================================================================
CPlayer* CPlayer::GetInstance(void)
{
	if (m_pPlayer == nullptr)
	{
		return nullptr;
		assert(("�v���C���[���̎擾�Ɏ��s", false));
	}

	return m_pPlayer;
}

//===========================================================================================
// �j��
//===========================================================================================
void CPlayer::Release(void)
{
	if (m_pPlayer == nullptr)
	{
		return;
	}
	else
	{
		m_pPlayer->Uninit();

		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CPlayer::Init(void)
{
	return S_OK;
}

//===========================================================================================
// ����������(�I�[�o�[���[�h)
//===========================================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	SetPosition(pos);
	SetRotation(rot);

	// �v���C���[���f��&���[�V�����̓ǂݍ���
	CCharacterDiv::Init("data\\TEXT\\motion\\player\\motion_Upper.txt", "data\\TEXT\\motion\\player\\motion_Lower.txt");

	// �����蔻��𐶐�
	m_pSphereCollider = CSphereCollider::Create(CCollision::TAG_PLAYER, CCollision::TYPE_SPHERE, this);
	if (m_pSphereCollider != nullptr)
	{
		m_pSphereCollider->SetRadius(80.0f);
	}

	// �X���[
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PAYER, CSlowManager::TAG_PLAYER);

	// �̗͐ݒ�
	m_info.fLife = LIFE;

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CPlayer::Uninit(void)
{
	m_pPlayer = nullptr;

	// �e�N���X�̏I��
	CCharacterDiv::Uninit();

	// �������g�̔j��
	CObject::Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CPlayer::Update(void)
{
	if (GetBody() == nullptr)
	{
		return;
	}

	// ���̎擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();	// �L�[�{�[�h
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();	// �W���C�p�b�h
	CManagerEvent* pEvent = CManagerEvent::GetInstance();	// �Q�[�����
	float fSlow = 1.0f;	// �X���[�̊�l
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �e�N���X�̍X�V
	CCharacterDiv::Update();

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
	}

	// �g�����X�t�H�[���̎擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 move = GetMovement();

	// ���[�V����
	ManageMotion();

	if (m_pSlow != nullptr)
	{// �X���[�l�̎擾
		fSlow = m_pSlow->GetValue();
	}

	// �s������
	RimitPos(&pos);

	// �ړ��ʂ��X�V(����������)
	move.x += (0.0f - move.x) * ATT;
	move.z += (0.0f - move.z) * ATT;

	// �ړ��ʂ̑��
	pos += move * fSlow;

	// �J�����̒Ǐ]
	if (pCamera != nullptr)
	{
		pCamera->PlayerFollow();
	}

	if (m_pSphereCollider != nullptr)
	{
		// �ʒu�ݒ�
		m_pSphereCollider->SetPosition(pos);
		// �~�`�̉����o������
		m_pSphereCollider->PushSphere(&pos, CCollision::TAG_OBJECT);
	}

	// �g�����X�t�H�[���̔��f
	SetPosition(pos);
	SetMovement(move);
	SetRotation(rot);
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CPlayer::Draw(void)
{
	// �e�N���X�̕`��
	CCharacterDiv::Draw();

	// �f�o�b�O�\��
	Debug();
}

//===========================================================================================
// �q�b�g����
//===========================================================================================
void CPlayer::Hit(float fDamage)
{
	if (m_info.isMotionList.bAvoidance)
	{
		if (m_avoid.fTime > 1.8f)
		{
			// �Q�[�����[�h���W���X�g���
			CManagerEvent::GetInstance()->SetMode(CManagerEvent::MODE_JUST);
		}
	}

	m_info.fLife -= fDamage;

	if (m_info.fLife < 0.0f)
	{
		m_info.fLife = 0.0f;

		Uninit();
	}
}

//===========================================================================================
// �ʏ�퓬�̎�
//===========================================================================================
void CPlayer::BattleNormal(void)
{
	// ���̎擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// ���͊֌W
	ManageInput();

	// ���[�V����
	ManageMotion();

	// �U��
	Attack();

	// ���b�N�I��
	Lockon();

	// ���
	//Evasion();

	if (pCamera != nullptr)
	{
		// �J�����̒Ǐ]
		pCamera->PlayerFollow();

		if (!m_bLockon)
		{
			// �J����
			pCamera->MouseManage();

			// �p�b�h�ɂ��J��������
			pCamera->JoypadViewOperation(0.03f, 0.06f);
		}
	}
}

//===========================================================================================
// �W���X�g�̎�
//===========================================================================================
void CPlayer::BattleJust(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	InputMove();

	m_info.isMotionList.bAvoidance = false;

	// �w��ɃJ������ݒ�
	pCamera->PlayerLockon(0.4f);

	CManagerEvent::GetInstance()->SetMode(CManagerEvent::MODE_RUSH);
}

//===========================================================================================
// ���b�V���퓬��
//===========================================================================================
void CPlayer::BattleRush(void)
{
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();
	CInputMouse* pInputMouse = CInputMouse::GetInstance();

	if (pInpuJoyPad->GetTrigger(CInputJoypad::BUTTON_RB, 0))
	{
		InputAttack();
	}

	// �U��
	Attack();
}

//===========================================================================================
// ����񂯂�퓬�̎�
//===========================================================================================
void CPlayer::BattleJanken(void)
{
	if (m_pJanken == nullptr)
	{// ����񂯂�̐���
		m_pJanken = CJanken::Create();
		m_pJanken->IsJanken(true);
	}

	if (m_pJanken != nullptr)
	{
		// ����񂯂�p�̌Œ�J����
		CManager::GetInstance()->GetCamera()->FixedJanken();

		// �J�����̒Ǐ]
		CManager::GetInstance()->GetCamera()->PlayerFollow();

		// �ړI�̌����������i����̓{�X�j
		RotDest();

		// ����񂯂�̓���
		InputJanken();

		if (m_pJanken->GetResult(m_pJanken->PLAYER_PLAYER) == m_pJanken->RESULT_WIN)
		{// ���ʂ������������ꍇ
			Attack();	// �U��
		}

		if (!m_pJanken->IsJanken())
		{// �I�����Ă�����

			if (m_pJanken != nullptr)
			{// �j��
				m_pJanken->Uninit();
				m_pJanken = nullptr;
			}

			// �ʏ�̐퓬��Ԃ�
			//CManagerEvent::GetInstance()->SetMode(CManagerEvent::MODE_NORMAL);
		}
	}
}

//===========================================================================================
// ����񂯂�U��
//===========================================================================================
void CPlayer::InputJanken(void)
{
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();
	CInputMouse* pInputMouse = CInputMouse::GetInstance();

	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_M) || pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_Y, 0))
	{
		m_pJanken->SetJanken(m_pJanken->JANKEN_GU);
	}
	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_N) || pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_A, 0))
	{
		m_pJanken->SetJanken(m_pJanken->JANKEN_CHOKI);
	}
	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_B) || pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_X, 0))
	{
		m_pJanken->SetJanken(m_pJanken->JANKEN_PA);
	}
}

//===========================================================================================
// ���̓}�l�[�W��
//===========================================================================================
void CPlayer::ManageInput(void)
{
	// �U��
	InputAttack();

	// ���
	//InputAvoidance();

	if (!m_info.isMotionList.bAttack && !m_info.isMotionList.bAttack1 && !m_info.isMotionList.bAvoidance)
	{
		// �ړ�����
		InputMove();
	}
}

//------------------------------------------------------------------------
// �ړ�����
//------------------------------------------------------------------------
void CPlayer::InputMove(void)
{
	D3DXVECTOR3 rotCamera = CManager::GetInstance()->GetCamera()->GetRotation();
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad *pInpuJoyPad = CInputJoypad::GetInstance();
	float fTick = (CManager::GetInstance()->GetTick() * 100.0f);


	D3DXVECTOR3 move = GetMovement();

	// �ړ�����
	if (pInputKeyboard->GetPress(DIK_A)|| pInpuJoyPad->GetJoyStickLX(0) < 0)
	{
		if (pInputKeyboard->GetPress(DIK_W) == true || pInpuJoyPad->GetJoyStickLY(0) > 0)
		{// ����ړ�
			m_info.fRotDest = rotCamera.y + D3DX_PI * 0.75f;

			move.x += sinf(rotCamera.y + -D3DX_PI * 0.25f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + -D3DX_PI * 0.25f) * fTick * MOVE_SPEED;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInpuJoyPad->GetJoyStickLY(0) < 0)
		{// �����ړ�
			m_info.fRotDest = rotCamera.y + D3DX_PI * 0.25f;

			move.x += sinf(rotCamera.y + -D3DX_PI * 0.75f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + -D3DX_PI * 0.75f) * fTick * MOVE_SPEED;
		}
		else
		{// ���ړ�
			m_info.fRotDest = rotCamera.y + D3DX_PI * 0.5f;

			move.x += sinf(rotCamera.y + -D3DX_PI * 0.5f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + -D3DX_PI * 0.5f) * fTick * MOVE_SPEED;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true || pInpuJoyPad->GetJoyStickLX(0) > 0)
	{
		if (pInputKeyboard->GetPress(DIK_W) == true || pInpuJoyPad->GetJoyStickLY(0) > 0)
		{//�@�E��ړ�
			m_info.fRotDest = rotCamera.y + -D3DX_PI * 0.75f;

			move.x += sinf(rotCamera.y + D3DX_PI * 0.25f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + D3DX_PI * 0.25f) * fTick * MOVE_SPEED;

		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInpuJoyPad->GetJoyStickLY(0) < 0)
		{//�@�E���ړ�
			m_info.fRotDest = rotCamera.y + -D3DX_PI * 0.25f;

			move.x += sinf(rotCamera.y + D3DX_PI * 0.75f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + D3DX_PI * 0.75f) * fTick * MOVE_SPEED;
		}
		else
		{// �E�ړ�
			m_info.fRotDest = rotCamera.y + -D3DX_PI * 0.5f;

			move.x += sinf(rotCamera.y + D3DX_PI * 0.5f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + D3DX_PI * 0.5f) * fTick * MOVE_SPEED;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true || pInpuJoyPad->GetJoyStickLY(0) > 0)
	{// �O�ړ�
		m_info.fRotDest = rotCamera.y + -D3DX_PI;

		move.x += sinf(rotCamera.y + 0.0f) * fTick * MOVE_SPEED;
		move.z += cosf(rotCamera.y + 0.0f) * fTick * MOVE_SPEED;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true || pInpuJoyPad->GetJoyStickLY(0) < 0)
	{// ���ړ�
		m_info.fRotDest = rotCamera.y;

		move.x += sinf(rotCamera.y + D3DX_PI) * fTick * MOVE_SPEED;
		move.z += cosf(rotCamera.y + D3DX_PI) * fTick * MOVE_SPEED;
	}

	SetMovement(move);
	
	// �����̕␳
	RotAor();
}

//------------------------------------------------------------------------
// �U������
//------------------------------------------------------------------------
void CPlayer::InputAttack(void)
{
	// ���̎擾
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();
	CInputMouse* pInputMouse = CInputMouse::GetInstance();

	int nKey = GetBody()->GetKey(CCharacterDiv::DIV_UPPER);	// ���݂̃L�[
	int nNumKey = GetBody()->GetMotionInfo(CCharacterDiv::DIV_UPPER, GetType(CCharacterDiv::DIV_UPPER)).nNumKey;	// �L�[�̑���
	int nMotionType = GetBody()->GetType(CCharacterDiv::DIV_UPPER);

	// �U�����1��
	m_info.isMotionList.bAttack = pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_RB, 0);
	
	// �U�����2��
	/*if (nMotionType == UPPER_MOTION_ATTACK00 && nKey > 3 && nKey <= nNumKey)
	{
		if (pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_RB, 0))
		{
			m_info.isMotionList.bAttack1 = true;
		}
	}*/
}

//------------------------------------------------------------------------
// ������
//------------------------------------------------------------------------
void CPlayer::InputAvoidance(void)
{
	// ���̎擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();

	if (m_info.isMotionList.bAvoidance)
	{
		return;
	}

	if (pInputKeyboard->GetTrigger(DIK_LSHIFT) || pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_LB, 0))
	{
		float stick = sinf(pInpuJoyPad->GetJoyStickLX(0)) * cosf(pInpuJoyPad->GetJoyStickLY(0));

		// �ϐ��錾
		CEnemyBoss* pBoss = CEnemyBoss::GetInstance();
		D3DXVECTOR3 posBoss = {};
		D3DXVECTOR3 pos = GetPosition();

		if (pBoss != nullptr)
		{
			posBoss = pBoss->GetPosition();
		}

		// �p�x�����߂�
		float fAngle = atan2f((posBoss.x - pos.x), (posBoss.z - pos.z));

		if (stick < 0.0f)
		{
			m_avoid.fAngleMove = 1.0f;
 		}
		else
		{
			m_avoid.fAngleMove = -1.0f;
		}

		m_info.isMotionList.bAvoidance = true;
	}
}

//===========================================================================================
// �U��
//===========================================================================================
void CPlayer::Attack(void)
{
	// ���̎擾
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();
	CInputMouse* pInputMouse = CInputMouse::GetInstance();

	D3DXVECTOR3 rot = GetPosition();
	D3DXVECTOR3 move = GetMovement();

	int nKey = GetBody()->GetKey(CCharacterDiv::DIV_UPPER);	// ���݂̃L�[
	int nNumKey = GetBody()->GetMotionInfo(CCharacterDiv::DIV_UPPER, GetType(CCharacterDiv::DIV_UPPER)).nNumKey;	// �L�[�̑���
	int nTypeMotion = GetBody()->GetType(CCharacterDiv::DIV_UPPER);
	float fTick = CManager::GetInstance()->GetTick() * 100.0f;

	if (nTypeMotion == UPPER_MOTION_ATTACK00)
	{
		if (nKey == 3)
		{
			IsAttack(true);
		}
		if (nKey >= 1)
		{
			// �ړ�
			move.x = sinf(rot.y + D3DX_PI) * fTick * 5.0f;
			move.z = cosf(rot.y + D3DX_PI) * fTick * 5.0f;
		}
	}

	if (nTypeMotion == UPPER_MOTION_ATTACK01)
	{
		if (nKey == 2)
		{
			IsAttack(true);
		}
		if (nKey == 1)
		{
			// �ړ�
			move.x = sinf(rot.y + D3DX_PI) * fTick * 5.0f;
			move.z = cosf(rot.y + D3DX_PI) * fTick * 5.0f;

			SetMovement(move);
		}
	}
}

//===========================================================================================
// �U������(��`)
//===========================================================================================
bool CPlayer::IsAttack(bool bAway)
{
	CEnemyBoss* pBoss = CEnemyBoss::GetInstance();

	if (pBoss == nullptr)
	{
		return false;
	}

	D3DXVECTOR3 posBoss = pBoss->GetPosition();
	D3DXVECTOR3 moveBoss = pBoss->GetMovement();

	float vec1 = 0.0f, vec2 = 0.0f;	// �x�N�g��
	D3DXVECTOR3 pos0 = {};	// �n�_
	D3DXVECTOR3 pos1 = {}, pos2 = {};	// �I�_1/�I�_2
	D3DXVECTOR3 vecLine = {}, vecToPos = {};	// �n�_�ƏI�_�̋���/�n�_����Ώۂ̋���
	D3DXVECTOR3 vecLine1 = {};	// �n�_�ƏI�_�̋���

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 move = GetMovement();

	// �U���͈͂̐ݒ�
	float fLength = sqrtf
	(
		(posBoss.x - pos.x) * (posBoss.x - pos.x) +
		(posBoss.z - pos.z) * (posBoss.z - pos.z)
	);

	// �p�x�����߂�
	//float fAngle = atan2f((pos.x - posBoss.x), (pos.z - posBoss.z));
	float fAngle = atan2f((posBoss.x - pos.x), (posBoss.z - pos.z));

	// �͈͓�����
	if (fLength < ATTACK_LENGTH)
	{
		// �n�_�̌v�Z
		pos0.x = pos.x;
		pos0.y = rot.y;
		pos0.z = pos.z;
		// �I�_1�̌v�Z
		pos1.x = pos0.x + (sinf(rot.y + D3DX_PI * ATTACK_RANGE));
		pos1.y = 0.0f;
		pos1.z = pos0.z + (cosf(rot.y + D3DX_PI * ATTACK_RANGE));
		// �I�_2�̌v�Z
		pos2.x = pos0.x + (sinf(rot.y + -D3DX_PI * ATTACK_RANGE));
		pos2.y = 0.0f;
		pos2.z = pos0.z + (cosf(rot.y + -D3DX_PI * ATTACK_RANGE));

		// �n�_�ƏI�_�Ƃ̋�����
		vecLine = pos1 - pos0;
		vecLine1 = pos2 - pos0;

		// �n�_����Ώۂ܂ł̈ʒu
		vecToPos = posBoss - pos0;

		// �x�N�g���̌v�Z
		vec1 = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
		vec2 = (vecLine1.z * vecToPos.x) - (vecLine1.x * vecToPos.z);

		if (vec1 > 0 && vec2 < 0)
		{
			CManager::GetInstance()->GetDebugProc()->Print("\n�y�y������z�z\n");

			if (bAway)
			{
				pBoss->SetMovement(
					{
						sinf(fAngle) * BLOWN_AWAY,
						0.0f,
						cosf(fAngle) * BLOWN_AWAY,
					});
			}

			// �_���[�W��^����
			pBoss->Hit(0.1f);

			return true;
		}
	}

	/*CManager::GetInstance()->GetDebugProc()->Print("�n�_:%f %f %f\n", pos0.x, pos0.y, pos0.z);
	CManager::GetInstance()->GetDebugProc()->Print("�I�_1:%f %f %f\n", pos1.x, pos1.y, pos1.z);
	CManager::GetInstance()->GetDebugProc()->Print("�I�_2:%f %f %f\n", pos2.x, pos2.y, pos2.z);
	CManager::GetInstance()->GetDebugProc()->Print("�n�_�Ƃ̋���1:%f %f %f\n", vecLine.x, vecLine.y, vecLine.z);
	CManager::GetInstance()->GetDebugProc()->Print("�n�_�Ƃ̋���2:%f %f %f\n", vecLine1.x, vecLine1.y, vecLine1.z);
	CManager::GetInstance()->GetDebugProc()->Print("�x�N�g��1:%f\n", vec1);
	CManager::GetInstance()->GetDebugProc()->Print("�x�N�g��2:%f\n\n", vec2);
	CManager::GetInstance()->GetDebugProc()->Print("����:%f\n\n", fAngle);*/

	return false;
}

//===========================================================================================
// ���b�N�I��
//===========================================================================================
void CPlayer::Lockon(void)
{
	CInputMouse *pInputMouse = CInputMouse::GetInstance();
	CInputJoypad *pInpuJoyPad = CInputJoypad::GetInstance();
	CEnemyBoss *pBoss = CEnemyBoss::GetInstance();
	CCamera *pCamera =CManager::GetInstance()->GetCamera();

	if (pBoss != nullptr)
	{
		D3DXVECTOR3 posBoss = pBoss->GetPosition();

		if (pInputMouse->GetTrigger(pInputMouse->MOUSE_RIGHT) || pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_RS, 0))
		{
			m_bLockon = m_bLockon ? false : true;
		}

		if (!m_bLockon)
		{
			if (m_pLockon != nullptr)
			{
				m_pLockon->Uninit();
				m_pLockon = nullptr;
			}

			return;
		}

		if (m_pLockon == nullptr)
		{
			CMotion* pMotion = pBoss->GetBody();

			D3DXVECTOR3 pos = pMotion->GetParts(0)->pParts->GetPosition();

			m_pLockon = CLockon::Create({ posBoss.x, pos.y + 20.0f, posBoss.z });
		}

		if (m_pLockon != nullptr)
		{
			m_pLockon->SetPosition({ posBoss.x, 150.0f, posBoss.z });
			m_pLockon->SetSize({ 10.0f, 10.0f });
		}

		// �ړI�̕�����
		RotDest();
	
		if (pCamera != nullptr)
		{
			//CMotion* pMotion = pBoss->GetBody();

			//D3DXVECTOR3 pos = pMotion->GetParts(0)->pParts->GetPosition();

			//// �U���͈͂̐ݒ�
			//float fLength = sqrtf
			//(
			//	(posBoss.x - pos.x) * (posBoss.x - pos.x) +
			//	(posBoss.z - pos.z) * (posBoss.z - pos.z)
			//);

			//// �����_�̐ݒ�
			//pCamera->SetPosR({ posBoss.x, pos.y + 50.0f, posBoss.z });
			////pCamera->SetLength(fLength);

			//// ���_�̐ݒ�
			//pCamera->SetPosV(
			//	{ 
			//		pos.x + sinf(m_info.rot.y) * 200.0f,
			//		200.0f,
			//		pos.z + cosf(m_info.rot.y) * 200.0f 
			//	}
			//);

			// �w��ɃJ������ݒ�
			pCamera->PlayerLockon(0.4f);
		}
	}
}

//===========================================================================================
// ���
//===========================================================================================
void CPlayer::Evasion(void)
{
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();
	CInputMouse* pInputMouse = CInputMouse::GetInstance();
	
	// �ϐ��錾
	bool bJust = false;
	CEnemyBoss* pBoss = CEnemyBoss::GetInstance();
	D3DXVECTOR3 posBoss = pBoss->GetPosition();
	float fTickAvoidance = (CManager::GetInstance()->GetTick() * 10.0f);	// ������ԗp

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetPosition();
	D3DXVECTOR3 move = GetMovement();

	////float fAngle = atan2f((posBoss.x - pos.x), (posBoss.z - pos.z));
	//if (!m_info.isMotionList.bAvoidance)
	//{
	//	// ���������߂�
	//	m_fTempLength = sqrtf((m_info.pos.x - posBoss.x) * (m_info.pos.x - posBoss.x) + (m_info.pos.z - posBoss.z) * (m_info.pos.z - posBoss.z));
	//}

	//if (!m_info.isMotionList.bAvoidance)
	//{
	//	// ���������߂�
	//	m_avoid.fTempLength = sqrtf((pos.x - posBoss.x) * (pos.x - posBoss.x) + (pos.z - posBoss.z) * (pos.z - posBoss.z));

	//	m_avoid.move = move;
	//}

	//if (m_info.isMotionList.bAvoidance)
	//{
	//	float fTickMove = (CManager::GetInstance()->GetTick() * 100.0f);	// �ړ��p

	//	//SetMovement(m_avoid.move);
	//	fAngle += 0.1f;

	//	m_info.pos.x = posBoss.x + sinf(fAngle) * m_fTempLength;
	//	m_info.pos.z = posBoss.z + cosf(fAngle) * m_fTempLength;

	//	// ������Ԃ����炷
	//	m_avoid.fTime -= fTickAvoidance;

	//	if (m_avoid.fTime < 0.0f)
	//	{// ���Ԃ��I�������
	//		m_avoid.fTime = AVOIDANCE_TIME;
	//		m_info.isMotionList.bAvoidance = false;
	//	}
	//}

	CManager::GetInstance()->GetDebugProc()->Print("%f\n", m_avoid.fDiff);
	//CManager::GetInstance()->GetDebugProc()->Print("1:%f 2:%f\n\n", DestPos1, DestPos2);
}

//===========================================================================================
// �c��
//===========================================================================================
void CPlayer::AfterImage(void)
{

}

//===========================================================================================
// �ړI�̕����֌����i����͓G�̕����j
//===========================================================================================
void CPlayer::RotDest(void)
{
	// ���̎擾	
	CEnemyBoss* pBoss = CEnemyBoss::GetInstance();

	if (pBoss == nullptr)
	{
		return;
	}

	// �擾
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// �p�x�����߂�
	float fAngle = atan2f((pos.x - pBoss->GetPosition().x), (pos.z - pBoss->GetPosition().z));

	// �{�X�̕���������
	rot.y = fAngle;

	SetRotation(rot);
}

//===========================================================================================
// ���[�V����
//===========================================================================================
void CPlayer::ManageMotion(void)
{
	// �����g���[�V����
	LowerMotion();
	// �㔼�g���[�V����
	UpperMotion();

	if (m_pSlow != nullptr)
	{
		GetBody()->SetMmultiply(m_pSlow->GetValue());
	}
}

//===========================================================================================
// �����g���[�V����
//===========================================================================================
void CPlayer::LowerMotion(void)
{
	// ���[�V�����̔ԍ����擾
	int nDiv = CCharacterDiv::DIV_LOWER;
	int nMotionLower = GetType(nDiv);
	int nKey = GetBody()->GetKey(nDiv);
	int nNumKey = GetBody()->GetMotionInfo(nDiv, nMotionLower).nNumKey;
	D3DXVECTOR3 move = GetMovement();

	bool bFinish = GetBody()->IsFinish(DIV_LOWER);	// �I�����

	float fSpeed = sqrtf(move.x * move.x + move.z * move.z);

	if (m_info.isMotionList.bAvoidance || (nMotionLower == LOWER_MOTION_DASH && !bFinish))
	{// ������
		if (nMotionLower != LOWER_MOTION_DASH)
		{
			//SetMotion(nDiv, LOWER_MOTION_DASH);
		}
	}
	else if (m_info.isMotionList.bAttack1 || (nMotionLower == LOWER_MOTION_ATTACK01 && !bFinish))
	{// �Q��ڂ̍U�����
		if (nMotionLower != LOWER_MOTION_ATTACK01)
		{// ���[�V�����^�C�v�������ł͂Ȃ��ꍇ
			SetMotion(nDiv, LOWER_MOTION_ATTACK01);
		}
		//m_info.isMotionList.bAttack1 = false;
	}
	else if (m_info.isMotionList.bAttack || (nMotionLower == LOWER_MOTION_ATTACK00 && !bFinish))
	{// �P��ڂ̍U�����
		if (nMotionLower != LOWER_MOTION_ATTACK00)
		{
			SetMotion(nDiv, LOWER_MOTION_ATTACK00);
		}
	}
	else
	{
		if (fSpeed > 0.2f)
		{// ����
			if (nMotionLower != LOWER_MOTION_RUN)
			{
				SetMotion(nDiv, LOWER_MOTION_RUN);
			}
		}
		else
		{// 
			if (nMotionLower != LOWER_MOTION_NORMAL)
			{
				SetMotion(nDiv, LOWER_MOTION_NORMAL);
			}
		}
	}
}

//===========================================================================================
// �㔼�g���[�V����
//===========================================================================================
void CPlayer::UpperMotion(void)
{
	// ���[�V�������̎擾
	int nDiv = CCharacterDiv::DIV_UPPER;
	int nMotionLower = GetType(nDiv);	// ���[�V�����̎��
	int nKey = GetBody()->GetKey(nDiv);	// ���݂̃L�[��
	int nNumKey = GetBody()->GetMotionInfo(nDiv, nMotionLower).nNumKey;	// �L�[�̑���
	D3DXVECTOR3 move = GetMovement();

	bool bFinish = GetBody()->IsFinish(DIV_UPPER);	// �I�����

	float fSpeed = sqrtf(move.x * move.x + move.z * move.z);

	if (m_info.isMotionList.bAvoidance && bFinish)
	{// �����Ԃ��I�����Ă�ꍇ
		if (nMotionLower != UPPER_MOTION_DASH)
		{// ���[�V�����^�C�v�������ł͂Ȃ��ꍇ
			//SetMotion(nDiv, UPPER_MOTION_DASH);
		}
		//m_info.isMotionList.bAvoidance = false;
	}
	else if (m_info.isMotionList.bAttack1 || (nMotionLower == UPPER_MOTION_ATTACK01 && !bFinish))
	{// �Q��ڂ̍U�����
		if (nMotionLower != UPPER_MOTION_ATTACK01)
		{// ���[�V�����^�C�v�������ł͂Ȃ��ꍇ
			SetMotion(nDiv, UPPER_MOTION_ATTACK01);
		}
		m_info.isMotionList.bAttack1 = false;
	}
	else if (m_info.isMotionList.bAttack || (nMotionLower == UPPER_MOTION_ATTACK00 && !bFinish))
	{// �P��ڂ̍U����Ԃ܂��̓^�C�v���ꏏ�ŏI�����ĂȂ��ꍇ

		if (nMotionLower != UPPER_MOTION_ATTACK00)
		{// ���[�V�����^�C�v�������ł͂Ȃ��ꍇ
			SetMotion(nDiv, UPPER_MOTION_ATTACK00);
		}
	}
	else
	{
		if (fSpeed > 0.2f)
		{// ����
			if (nMotionLower != UPPER_MOTION_RUN)
			{// ���[�V�����^�C�v�������ł͂Ȃ��ꍇ
				SetMotion(nDiv, UPPER_MOTION_RUN);
			}
		}
		else
		{// 
			if (nMotionLower != UPPER_MOTION_NORMAL)
			{// ���[�V�����^�C�v�������ł͂Ȃ��ꍇ
				SetMotion(nDiv, UPPER_MOTION_NORMAL);
			}
		}
	}
}

//===========================================================================================
// �ړ��͈͈͎͂̔w��
//===========================================================================================
void CPlayer::RimitPos(D3DXVECTOR3* pPos)
{
	if (pPos->x <= -LIMIT_POS.x)
	{
		pPos->x = -LIMIT_POS.x;
	}
	if (pPos->x >= LIMIT_POS.x)
	{
		pPos->x = LIMIT_POS.x;
	}
	if (pPos->z <= -LIMIT_POS.z)
	{
		pPos->z = -LIMIT_POS.z;
	}
	if (pPos->z >= LIMIT_POS.z)
	{
		pPos->z = LIMIT_POS.z;
	}
}

//===========================================================================================
// �����␳
//===========================================================================================
void CPlayer::RotAor(void)
{
	D3DXVECTOR3 rot = GetRotation();

	m_info.fRotDiff = m_info.fRotDest - rot.y;	// �ڕW�̊p�x�܂ł̍��������߂�

	if (m_info.fRotDiff > D3DX_PI)
	{
		m_info.fRotDiff -= (D3DX_PI * 2);
	}
	if (m_info.fRotDiff < -D3DX_PI)
	{
		m_info.fRotDiff += (D3DX_PI * 2);
	}

	rot.y += m_info.fRotDiff * ADJUST_ROT;	// �␳����

	// �����̏C��
	general::RotLimit(&rot);

	SetRotation(rot);
}

//===========================================================================================
// �f�o�b�O
//===========================================================================================
void CPlayer::Debug(void)
{
#ifndef _DEBUG
	return;
#endif

	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 move = GetMovement();

	int nKey1 = GetBody()->GetKey(0);	// ���݂̃L�[��
	int nKey2 = GetBody()->GetKey(1);	// ���݂̃L�[��
	int nNumKey1 = GetBody()->GetMotionInfo(0, GetType(0)).nNumKey;	// �L�[�̑���
	int nNumKey2 = GetBody()->GetMotionInfo(1, GetType(1)).nNumKey;	// �L�[�̑���

	//�f�o�b�O�\��
	pDebug->Print("�y�v���C���[���z\n");
	pDebug->Print("�ʒu�Fx:%f y:%f z:%f\n", pos.x, pos.y, pos.z);
	pDebug->Print("�����Fx:%f y:%f z:%f\n", rot.x, rot.y, rot.z);
	pDebug->Print("�ړ��ʁF x:%f y:%f z:%f\n", move.x, move.y, move.z);
	pDebug->Print("�̗́F%f\n", m_info.fLife);
	pDebug->Print("���[�V�����ԍ��F��%d ��%d\n", GetBody()->GetType(0), GetBody()->GetType(1));
	pDebug->Print("�L�[�ԍ��F��%d ��%d\n", nKey1, nKey2);
	pDebug->Print("�L�[�����F��%d ��%d\n\n", nNumKey1, nNumKey2);
	pDebug->Print("�e�B�b�N:%f\n", CManager::GetInstance()->GetTick());
	pDebug->Print("LXstick:%f\n", CInputJoypad::GetInstance()->GetJoyStickLX(0));
	pDebug->Print("LYstick:%f\n", CInputJoypad::GetInstance()->GetJoyStickLY(0));
}
