//===========================================================================================
//
// �����蔻�菈��[Enemy.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "colliderBox.h"
#include "colliderSphere.h"

// �ÓI�����o�ϐ��錾
CCollision* CCollision::m_apCollision[collision::MAX_OBJECT] = {};
int CCollision::m_nNumAll = 0;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CCollision::CCollision() :
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), m_tag(TAG_NONE), m_type(TYPE_NONE), m_bTrigger(false), m_nID(-1), m_pObject(nullptr)
{
	for (int nCntObject = 0; nCntObject < collision::MAX_OBJECT; nCntObject++)
	{
		if (m_apCollision[nCntObject] == nullptr)
		{
			m_apCollision[nCntObject] = this;	//�������g����
			m_nID = nCntObject;	//�������g��ID��ۑ�
			m_nNumAll++;	//�������J�E���g�A�b�v

			break;
		}
	}
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CCollision::~CCollision()
{
	
}

//===========================================================================================
// ����(�f�t�H���g)
//===========================================================================================
CCollision* CCollision::Create(void)
{
	CCollision* pCls = nullptr;

	if (pCls == nullptr)
	{
		pCls = new CCollision;

		pCls->Init();
	}

	return pCls;

}

//===========================================================================================
// ����(�I�[�o�[���C�h)
//===========================================================================================
CCollision* CCollision::Create(ETag tag, EType type, CObject* pObject)
{
	CCollision* pCls = nullptr;

	if (pCls == nullptr)
	{
		switch (type)
		{
		case TYPE_NONE:
			pCls = new CCollision;
			break;
		case TYPE_BOX:
			pCls = new CBoxCollider;
			break;
		case TYPE_SPHERE:
			pCls = new CSphereCollider;
			break;
		
		default:
			break;
		}

		pCls->Init();

		pCls->m_tag = tag;
		pCls->m_type = type;
	}

	return pCls;
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CCollision::Init()
{
	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CCollision::Uninit()
{
	if (m_apCollision[m_nID] != nullptr)
	{
		m_apCollision[m_nID] = nullptr;
	}

	Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CCollision::Update()
{

}

//===========================================================================================
// �S�j��
//===========================================================================================
void CCollision::ReleaseAll(void)
{
	for (int i = 0; i < collision::MAX_OBJECT; i++)
	{
		if (m_apCollision[i] != nullptr)
		{
			m_apCollision[i]->Uninit();
		}
	}
}

//===========================================================================================
// �I�u�W�F�N�g���̎擾
//===========================================================================================
CObject* CCollision::GetOwner(void) const
{
	if (m_pObject == nullptr)
	{
		return nullptr;
	}

	return m_pObject;
}
