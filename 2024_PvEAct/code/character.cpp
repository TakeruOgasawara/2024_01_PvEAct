//=============================================================================
// 
//  �L�����N�^�[����[character.cpp]
// Author : Takeru Ogasawara
// 
//=============================================================================

// �C���N���[�h�錾
#include "character.h"
#include "motion.h"
#include "renderer.h"
#include "manager.h"
#include "xfile.h"
#include "texture.h"

#include "input.h"
#include "general.h"
#include <stdio.h>

#define SPEED	(2.0f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCharacter::CCharacter()
{
	// �l�̃N���A
	ZeroMemory(&m_info, sizeof(SCharacter));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCharacter::~CCharacter()
{

}

//=============================================================================
//	����
//=============================================================================
CCharacter* CCharacter::Create(const char* pPath)
{
	CCharacter* pChar = nullptr;

	if (pChar == nullptr)
	{
		pChar = new CCharacter;

		if (pChar != nullptr)
		{
			pChar->Init(pPath);

			return pChar;
		}
	}

	return pChar;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCharacter::Init()
{
	return S_OK;
}

//=============================================================================
// ����������(�I�[�o�[���[�h)
//=============================================================================
HRESULT CCharacter::Init(const char* pPath)
{
	m_info.pBody = CMotion::Create(pPath);

	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetPosition(GetPosition());
		m_info.pBody->SetRotation(GetRotation());
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCharacter::Uninit()
{
	/*if (m_info.pBody != nullptr)
	{
		m_info.pBody->Uninit();
		m_info.pBody = nullptr;
	}*/

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCharacter::Update()
{
	if (m_info.pBody != nullptr)
	{// �̂̒Ǐ]
		m_info.pBody->SetPosition(GetPosition());
		m_info.pBody->SetRotation(GetRotation());
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCharacter::Draw()
{
	// �}�g���b�N�X�̊|�����킹
	general::SetMatrix(&m_info.mtxWorld, m_info.pos, m_info.rot);
}

//=============================================================================
// �ʒu�ݒ�
//=============================================================================
void CCharacter::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;

	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetPosition(GetPosition());
	}
}

//=============================================================================
// �����ݒ�
//=============================================================================
void CCharacter::SetRotation(D3DXVECTOR3 rot)
{
	m_info.rot = rot;

	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetRotation(GetRotation());
	}
}

//=====================================================
// ���[�V�����ݒ�
//=====================================================
void CCharacter::SetMotion(int nMotion)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetMotion(nMotion);
	}
}

//=====================================================
// �̂̐ݒ�
//=====================================================
void CCharacter::SetBody(CMotion* pBody)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody = pBody;
	}
}

//=====================================================
// ���[�V�����擾
//=====================================================
int CCharacter::GetType(void)
{
	int nMotion = 0;

	if (m_info.pBody != nullptr)
	{
		nMotion = m_info.pBody->GetType();
	}

	return nMotion;
}

//=====================================================
// �̂̎擾
//=====================================================
CMotion* CCharacter::GetBody(void)
{
	if (m_info.pBody != nullptr)
	{
		return m_info.pBody;
	}

	return nullptr;
}
