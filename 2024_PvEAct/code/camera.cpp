//===========================================================================================
//
// [camera.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
#include "inputJoypad.h"
#include "debugproc.h"
#include "general.h"
#include "player.h"
#include "enemyBoss.h"

//********************************************************
// �萔�錾
//********************************************************
namespace
{
	D3DXVECTOR3 POS_V = { 0.0f, 200.0f, -400.0f };	// ���_�ʒu
	D3DXVECTOR3 POS_R = { 0.0f, 100.0f, 0.0f };	// �����_�ʒu
	D3DXVECTOR3 ROT = { 0.0f, 0.0f, 0.0f };	// ����
	float LENGTH = 300.0f;	// ���_�ƒ����_�̋���
	float MAE = 0.0f;	// �Ώۂ��璍���_�Ƃ̋���
}

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CCamera::CCamera()
{
	m_posV = { 0.0f, 0.0f, 0.0f };
	m_posR = { 0.0f, 0.0f, 0.0f };
	m_vecU = { 0.0f, 0.0f, 0.0f };
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_rotOld = { 0.0f, 0.0f, 0.0f };
	m_move = { 0.0f, 0.0f, 0.0f };
	m_posVDest = { 0.0f, 0.0f, 0.0f };
	m_posRDest = { 0.0f, 0.0f, 0.0f };
	m_fLength = 0.0f;
	m_fShakeTime = 0.0f;

	ZeroMemory(&m_shakeInfo, sizeof(m_shakeInfo));
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CCamera::~CCamera()
{

}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CCamera::Init(void)
{
	m_posV = POS_V;
	m_posR = POS_R;
	m_vecU = { 0.0f, 1.0f, 0.0f };
	m_rot.x = -0.6f;
	m_rot.z = -0.6f;
	m_fLength = LENGTH;

	//SetShake(10.0f, 2.0f);

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CCamera::Uninit(void)
{

}

//===========================================================================================
// �X�V����
//===========================================================================================
void CCamera::Update(void)
{
	// �����̐���
	RotLimit();
	//MouseManage();

	//ShakeUpdate();

	CManager::GetInstance()->GetDebugProc()->Print("\n�y�J�������z\n");
	CManager::GetInstance()->GetDebugProc()->Print("���_�F x:%f y:%f z:%f\n", m_posV.x, m_posV.y, m_posV.z);
	CManager::GetInstance()->GetDebugProc()->Print("�����_�F x:%f y:%f z:%f\n", m_posR.x, m_posR.y, m_posR.z);
	CManager::GetInstance()->GetDebugProc()->Print("�����F x:%f, y:%f z:%f\n", m_rot.z, m_rot.y, m_rot.z);
	CManager::GetInstance()->GetDebugProc()->Print("�����ԁF %f\n\n", m_fLength);
}

//===========================================================================================
// �J�����ݒ�
//===========================================================================================
void CCamera::SetCamera(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),	// ����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// ��ʂ̃A�X�y�N�g��
		10.0f,				// z�l�̍ŏ��l
		10000.0f);			// z�l�̍ő�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,	// ���_
		&m_posR,	// �����_
		&m_vecU);	// ���x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//===========================================================================================
// �}�E�X����
//===========================================================================================
void CCamera::MouseManage(void)
{
	// ���̎擾
	CInputMouse* pInputMouse = CInputMouse::GetInstance();

	D3DXVECTOR3 move = pInputMouse->GetMove();

	if (pInputMouse->GetPress(pInputMouse->MOUSE_LEFT))
	{
		// ���_�̈ړ�
		m_rot.z += -move.y * 0.004f;
		m_rot.y += move.x * 0.004f;

		// ���_�̍X�V����
		ViewpointOffset();
	}
	if (pInputMouse->GetPress(pInputMouse->MOUSE_RIGHT))
	{
		// ���_�̈ړ�
		m_rot.z += -move.y * 0.004f;
		m_rot.y += move.x * 0.004f;

		// ���_�̍X�V����
		RegardpointOffset();
	}

	// ���_�E�����_�Ƃ̋����ύX
	if (move.z >= 0.001f || move.z <= -0.001f)
	{
		if (m_fLength >= 100.0f)
		{
			// ���_�̈ړ�
			m_fLength += -move.z * 0.2f;

			// ���_�̍X�V����
			ViewpointOffset();
		}
		else
		{
			m_fLength = 100.0000000001f;
		}
	}

	// �����̏C��
	general::RotLimit(&m_rot);
}

//===========================================================================================
// �W���C�p�b�h����
//===========================================================================================
void CCamera::JoypadViewOperation(float fVertSpeed, float fHorizontalSpeed)
{
	// ���̎擾
	CInputJoypad* pInputJoypad = CInputJoypad::GetInstance();

	// �J���������̕ύX
	if (pInputJoypad->GetJoyStickRX(0) > 0)
	{// �X�e�B�b�N�E����
		m_rot.y += fHorizontalSpeed;
	}
	if (pInputJoypad->GetJoyStickRX(0) < 0)
	{// �X�e�B�b�N������
		m_rot.y -= fHorizontalSpeed;
	}
	if (pInputJoypad->GetJoyStickRY(0) > 0)
	{// �X�e�B�b�N�����
		m_rot.z += fVertSpeed;
	}
	if (pInputJoypad->GetJoyStickRY(0) < 0)
	{// �X�e�B�b�N������
		m_rot.z -= fVertSpeed;
	}

	// ���_�̕␳
	ViewpointOffset();
}

//===========================================================================================
// �v���C���[�̒Ǐ]����
//===========================================================================================
void CCamera::PlayerFollow(void)
{
	// �|�C���^���̎擾
	CPlayer* pPlayer = CPlayer::GetInstance();

	// �ϐ��錾
	D3DXVECTOR3 posRDiff = {}, posVDiff = {};

	// �ړI�̎��_ / �����_��ݒ�
	m_posVDest =
	{
		m_posRDest.x + (cosf(m_rot.z) * sinf(m_rot.y)) * -m_fLength,
		pPlayer->GetPosition().y + POS_V.y,
		m_posRDest.z + (cosf(m_rot.z) * cosf(m_rot.y)) * -m_fLength
	};
	m_posRDest = 
	{
		pPlayer->GetPosition().x + sinf(pPlayer->GetRotation().y + D3DX_PI) * MAE,
		pPlayer->GetPosition().y + POS_R.y,
		pPlayer->GetPosition().z + cosf(pPlayer->GetRotation().y + D3DX_PI) * MAE
	};

	posRDiff = m_posRDest - m_posR;	// �����_�̍���
	posVDiff = m_posVDest - m_posV;	// ���_�̍���

	m_posR += posRDiff * 0.07f;	// �����_��␳����
	m_posV += posVDiff * 0.08f;	// ���_��␳����
}

//===========================================================================================
// �v���C���[�̔w�ʂ������I�Ɍ�������
//===========================================================================================
void CCamera::PlayerLockon(float fOffset)
{
	// �ϐ��錾
	float fRotDiff = 0.0f;	// �����ۑ��p

	// ���̎擾
	CPlayer* pPlayer = CPlayer::GetInstance();
	
	// �������v�Z
	fRotDiff = (pPlayer->GetRotation().y + D3DX_PI) - m_rot.y;

	// �ڕW�̊p�x�̕␳
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2;
	}
	if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}

	m_rot.y += fRotDiff * fOffset;	//�␳����

	// �p�x�̕␳
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	// �J�������X�V����
	ViewpointOffset();
}

//===========================================================================================
// �����_�␳
//===========================================================================================
void CCamera::RegardpointOffset(void)
{
	m_posR.x = m_posV.x + (cosf(m_rot.z) * sinf(m_rot.y)) * m_fLength;
	m_posR.y = m_posV.y + sinf(m_rot.z) * m_fLength;
	m_posR.z = m_posV.z + (cosf(m_rot.z) * cosf(m_rot.y)) * m_fLength;
}

//===========================================================================================
// ���_�␳
//===========================================================================================
void CCamera::ViewpointOffset(void)
{
	m_posV.x = m_posR.x + (cosf(m_rot.z) * sinf(m_rot.y)) * -m_fLength;
	m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fLength;
	m_posV.z = m_posR.z + (cosf(m_rot.z) * cosf(m_rot.y)) * -m_fLength;
}

//===========================================================================================
// �Œ�J����
//===========================================================================================
void CCamera::FixedJanken(void)
{
	// �ϐ��錾
	float fRotDest = 0.0f;	// �����ۑ��p
	float fRotDiff = 0.0f;	// �����ۑ��p

	// ���̎擾
	CPlayer* pPlayer = CPlayer::GetInstance();
	CEnemyBoss* pBoss = CEnemyBoss::GetInstance();
	D3DXVECTOR3 posPlayer = {}, posBoss = {};
	D3DXVECTOR3 rotPlayer = {}, rotBoss = {};

	if (pPlayer == nullptr || pBoss == nullptr)
	{
		return;
	}
	
	// �ʒu���̎擾
	posPlayer = pPlayer->GetPosition();
	rotPlayer = pPlayer->GetRotation();
	posBoss = pBoss->GetPosition();
	rotBoss = pBoss->GetRotation();

	// �������v�Z
	fRotDest = (rotPlayer.y + D3DX_PI * 0.85f);

	fRotDiff = fRotDest - m_rot.y;	// �ڕW�̊p�x�܂ł̍��������߂�

	general::RotRevi(&fRotDiff);

	// ���_�̐ݒ�
	//m_posV.x = m_posV.x + sinf(pPlayer->GetRotation().y + D3DX_PI/* * 2.5f*/) * 50.0f;
	//m_posV.y = 500.0f;
	//m_posV.z = m_posV.z + cosf(pPlayer->GetRotation().y + D3DX_PI/* * 2.5f*/) * 50.0f;

	// �����_�̐ݒ�
	//SetPosR(posBoss);

	// ���_�ƒ����_�̋���
	m_fLength = 300.0f;

	m_rot.y += fRotDiff * 0.2f;	//�␳����

	// �J�������X�V����
	ViewpointOffset();
	RegardpointOffset();
}

//===========================================================================================
// ���_�����̐���
//===========================================================================================
void CCamera::RotLimit(void)
{
	// ���݂̌�������
	m_rotOld = m_rot;

	if (m_rot.x < -(D3DX_PI * 0.45f) || m_rot.z < -(D3DX_PI * 0.45f))
	{
		m_rot.z = -(D3DX_PI * 0.45f);
		m_rot.x = -(D3DX_PI * 0.45f);
	}
	else if (m_rot.x > (D3DX_PI * 0.15f) || m_rot.z > (D3DX_PI * 0.15f))
	{
		m_rot.x = (D3DX_PI * 0.15f);
		m_rot.z = (D3DX_PI * 0.15f);
	}
}

//===========================================================================================
// �V�F�C�N�ݒ�
//===========================================================================================
void CCamera::SetShake(float intensity, float time)
{
	m_shakeInfo.fIntensity = intensity;
	m_shakeInfo.fTime = time;
	m_shakeInfo.fStartTime = (float)clock();
}

//===========================================================================================
// �V�F�C�N�̍X�V
//===========================================================================================
void CCamera::ShakeUpdate(void)
{
	if (m_shakeInfo.fTime > 0.0f)
	{
		// �V�F�C�N���̏���
		float fElapsed = ((float)clock() - m_shakeInfo.fStartTime) / (float)CLOCKS_PER_SEC;

		if (fElapsed < m_shakeInfo.fTime)
		{
			// �����_���Ȓl�ŃJ�������V�t�g����
			m_posV += (D3DXVECTOR3(
				(rand() / (float)RAND_MAX - 0.5f) * m_shakeInfo.fIntensity,
				(rand() / (float)RAND_MAX - 0.5f) * m_shakeInfo.fIntensity,
				(rand() / (float)RAND_MAX - 0.5f) * m_shakeInfo.fIntensity
			));
		}
		else
		{
			// �V�F�C�N���I�������烊�Z�b�g
			m_shakeInfo.fTime = 0.0f;
			m_shakeInfo.fIntensity = 0.0f;
		}
	}
}
