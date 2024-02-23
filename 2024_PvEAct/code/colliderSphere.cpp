//===========================================================================================
//
// �~�`�����蔻�菈��[.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================

// �C���N���[�h�錾
#include "colliderSphere.h"
#include "collision.h"
#include "xfile.h"
#include "object3D.h"

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CSphereCollider::CSphereCollider() : m_fRadius(0.0f), m_pObject3D(nullptr)
{

}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CSphereCollider::~CSphereCollider()
{

}

//===========================================================================================
// ����
//===========================================================================================
CSphereCollider* CSphereCollider::Create(ETag tag, EType type, CObject* pObject)
{
	CSphereCollider* pSpere = nullptr;

	if (pSpere == nullptr)
	{
		pSpere = new CSphereCollider;

		pSpere->Init();

		pSpere->SetTag(tag);
		pSpere->SetType(type);
		pSpere->SetOwner(pObject);

		return pSpere;
	}

	return nullptr;
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CSphereCollider::Init(void)
{
#ifdef _DEBUG
	if (m_pObject3D == nullptr)
	{// �r���{�[�h�̐���
		m_pObject3D = CObject3D::Create(GetPosition(), "data\\TEXTURE\\col.png");

		if (m_pObject3D != nullptr)
		{
			m_pObject3D->SetSize(m_fRadius, m_fRadius);
		}
	}
#endif


	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CSphereCollider::Uninit(void)
{
	if (m_pObject3D != nullptr)
	{// �r���{�[�h�̍폜
		m_pObject3D->Uninit();
		m_pObject3D = nullptr;
	}

	// �e�N���X�̏I������
	CCollision::Uninit();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CSphereCollider::Update(void)
{
	if (m_pObject3D != nullptr)
	{// 
		D3DXVECTOR3 pos = GetPosition();
		m_pObject3D->SetPosition(pos);
		m_pObject3D->SetSize(m_fRadius, m_fRadius);
	}
}

//===========================================================================================
// �~�̉����o������
//===========================================================================================
void CSphereCollider::PushSphere(D3DXVECTOR3* pPos, ETag tag)
{
	if (tag <= TAG_NONE && tag >= TAG_MAX)
	{
		return;
	}

	CCollision** ppCollision = GetCollision();

	for (int nCnt = 0; nCnt < collision::MAX_OBJECT; nCnt++)
	{
		if (ppCollision[nCnt] == nullptr)
		{
			continue;
		}

		if (ppCollision[nCnt]->GetType() != TYPE_SPHERE)
		{
			continue;
		}

		if (ppCollision[nCnt]->GetTag() == tag || ppCollision[nCnt] == this)
		{
			continue;
		}

		// ���ނƂ̑Ίp���̒������Z�o����
		float fLengthDiff = sqrtf
		(
			(ppCollision[nCnt]->GetPosition().x - pPos->x) * (ppCollision[nCnt]->GetPosition().x - pPos->x) +
			(ppCollision[nCnt]->GetPosition().z - pPos->z) * (ppCollision[nCnt]->GetPosition().z - pPos->z)
		);

		// �ʒu�̍���
		D3DXVECTOR3 posDiff =
		{
			ppCollision[nCnt]->GetPosition().x - pPos->x,
			0.0f,
			ppCollision[nCnt]->GetPosition().z - pPos->z,
		};
		
		// �Ԃ��鎞�̋���
		float fLength = ppCollision[nCnt]->GetRadius() + GetRadius();

		if (fLengthDiff < fLength)
		{
			D3DXVECTOR3 posAfter = {0.0f, 0.0f, 0.0f};

			// �����x�N�g�����牟���o����̈ʒu���v�Z
			D3DXVec3Normalize(&posAfter, &posDiff);

			posAfter *= -fLength;

			posAfter += ppCollision[nCnt]->GetPosition();

			*pPos = posAfter;
		}
	}
}

//===========================================================================================
// �~�̃g���K�[����
//===========================================================================================
bool CSphereCollider::OnEnter(D3DXVECTOR3* pPos, ETag tag)
{
	if (tag <= TAG_NONE && tag >= TAG_MAX)
	{
		return false;
	}

	CCollision** ppCollision = GetCollision();

	for (int nCnt = 0; nCnt < collision::MAX_OBJECT; nCnt++)
	{
		if (ppCollision[nCnt] == nullptr)
		{
			continue;
		}

		if (ppCollision[nCnt]->GetType() != TYPE_SPHERE)
		{
			continue;
		}

		if (ppCollision[nCnt]->GetTag() == tag || ppCollision[nCnt] == this)
		{
			continue;
		}

		// �Ίp���̒������Z�o����
		float fLengthDiff = sqrtf
		(
			(ppCollision[nCnt]->GetPosition().x - pPos->x) * (ppCollision[nCnt]->GetPosition().x - pPos->x) +
			(ppCollision[nCnt]->GetPosition().z - pPos->z) * (ppCollision[nCnt]->GetPosition().z - pPos->z)
		);

		// �ʒu�̍���
		D3DXVECTOR3 posDiff = ppCollision[nCnt]->GetPosition() - *pPos;

		// �Ԃ��鎞�̋���
		float fLength = ppCollision[nCnt]->GetRadius() + GetRadius();

		if (fLengthDiff < fLength)
		{
			return true;
		}
	}

	return false;
}
