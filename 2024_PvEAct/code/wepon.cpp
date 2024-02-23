//===========================================================================================
//
// ����[wepon.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "wepon.h"
#include "manager.h"
#include "renderer.h"
#include "general.h"
#include "xfile.h"
#include "texture.h"

#include "player.h"
#include "characterDiv.h"
#include "motionDiv.h"
#include "modelParts.h"
#include "debugproc.h"

//===========================================================================================
//  �R���X�g���N�^
//===========================================================================================
CWepon::CWepon()
{
	// �}�g���b�N�X�񏉊���
	ZeroMemory(m_mtxOwner, sizeof(m_mtxOwner));
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CWepon::~CWepon()
{
}

//===========================================================================================
// ����
//===========================================================================================
CWepon* CWepon::Create(const char* pPath, D3DXVECTOR3 pos)
{
	CWepon* pWepon = nullptr;

	if (pWepon == nullptr)
	{
		pWepon = new CWepon;

		pWepon->Init(pPath, pos);

		return pWepon;
	}

	return nullptr;
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CWepon::Init(const char* pPath, D3DXVECTOR3 pos)
{
	// �e�N���X�̏�����
	CObjectX::Init(pPath, pos);

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CWepon::Uninit(void)
{
	// �e�N���X�̏I��
	CObjectX::Uninit();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CWepon::Update(void)
{
	// �e�N���X�̍X�V
	CObjectX::Update();

	Follow();

	SetPosition(GetPosition());
	SetRotation(GetRotation());
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CWepon::Draw(void)
{
	// �e�N���X�̕`��
	CObjectX::DrawOnly();
}

//===========================================================================================
// �Ǐ]����
//===========================================================================================
void CWepon::Follow(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	CMotionDiv* pBody = pPlayer->GetBody();

	if (pBody == nullptr)
	{
		return;
	}

	D3DXMATRIX* pMtx = GetMatrix();
	D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f };

	CModelParts* pParts = pBody->GetParts(0, 0);

	if (pParts != nullptr)
	{
		D3DXMATRIX* pMtx = GetMatrix();
		D3DXMATRIX* pMtxPart = pParts->GetMatrix();
		D3DXVECTOR3 offset = { -10.0f,0.0f,0.0f };

		//universal::SetOffSet(pMtx, *pMtxPart, offset);
		general::SetMatrix(pMtx, *pMtxPart, pos);
	}
}
