//=============================================================================
// 
//  �L�����N�^�[����[character.cpp]
// Author : Takeru Ogasawara
// 
//=============================================================================

// �C���N���[�h�錾
#include "characterDiv.h"
#include "motionDiv.h"
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
CCharacterDiv::CCharacterDiv()
{
	// �l�̃N���A
	ZeroMemory(&m_info, sizeof(SCharacter));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCharacterDiv::~CCharacterDiv()
{

}

//=============================================================================
//	����
//=============================================================================
CCharacterDiv* CCharacterDiv::Create(const char* pUpperPath, const char* pLowerPath)
{
	CCharacterDiv* pChar = nullptr;

	if (pChar == nullptr)
	{
		pChar = new CCharacterDiv;

		if (pChar != nullptr)
		{
			pChar->Init(pUpperPath, pLowerPath);

			return pChar;
		}
	}

	return pChar;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCharacterDiv::Init()
{
	return S_OK;
}

//=============================================================================
// ����������(�I�[�o�[���[�h)
//=============================================================================
HRESULT CCharacterDiv::Init(const char* pUpperPath, const char* pLowerPath)
{
	if (m_info.pBody == nullptr && pUpperPath != nullptr && pLowerPath != nullptr)
	{
		m_info.pBody = CMotionDiv::Create(pUpperPath, pLowerPath);

		if (m_info.pBody != nullptr)
		{
			m_info.pBody->SetPosition(GetPosition());
			m_info.pBody->SetRotation(GetRotation());
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCharacterDiv::Uninit()
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
void CCharacterDiv::Update()
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
void CCharacterDiv::Draw()
{
	// �}�g���b�N�X�̊|�����킹
	general::SetMatrix(&m_info.mtxWorld, m_info.pos, m_info.rot);
}

//=============================================================================
// �ʒu�ݒ�
//=============================================================================
void CCharacterDiv::SetPosition(D3DXVECTOR3 pos)
{
	m_info.pos = pos;

	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetPosition(m_info.pos);
	}
}

//=============================================================================
// �����ݒ�
//=============================================================================
void CCharacterDiv::SetRotation(D3DXVECTOR3 rot)
{
	m_info.rot = rot;

	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetRotation(m_info.rot);
	}
}

//=====================================================
// ���[�V�����ݒ�
//=====================================================
void CCharacterDiv::SetMotion(int nNum, int nMotion)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody->SetMotion(nNum, nMotion);
	}
}

//=====================================================
// �̂̐ݒ�
//=====================================================
void CCharacterDiv::SetBody(CMotionDiv* pBody)
{
	if (m_info.pBody != nullptr)
	{
		m_info.pBody = pBody;
	}
}

//=====================================================
// ���[�V�����擾
//=====================================================
int CCharacterDiv::GetType(int nNumDiv)
{
	int nMotion = 0;

	if (m_info.pBody != nullptr)
	{
		nMotion = m_info.pBody->GetType(nNumDiv);
	}

	return nMotion;
}

//=====================================================
// �̂̎擾
//=====================================================
CMotionDiv* CCharacterDiv::GetBody(void)
{
	if (m_info.pBody != nullptr)
	{
		return m_info.pBody;
	}

	return nullptr;
}
