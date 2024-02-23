//===========================================================================================
//
// [object.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"

//***************************************************
// �ÓI�����o�ϐ��̐錾
//***************************************************
CObject* CObject::m_apTop[object::NUM_PRIORITY] = {};	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CObject* CObject::m_apEnd[object::NUM_PRIORITY] = {};	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
int CObject::m_nNumAll = 0;
int CObject::m_nNumEffectAll = 0;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CObject::CObject(int nPriority)
{
	//�l�̏�����
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_bDeath = false;
	m_type = TYPE_NONE;

	if (nPriority < 0)
	{
		nPriority = 0;
	}
	else if (nPriority > object::NUM_PRIORITY)
	{
		nPriority = object::NUM_PRIORITY - 1;
	}

	m_nPriority = nPriority;

	if (m_apTop[m_nPriority] == nullptr && m_apEnd[m_nPriority] == nullptr)
	{//�擪�A�Ō���ɏ�񂪓����Ă��Ȃ������ꍇ
		m_apTop[m_nPriority] = this;	//�擪�A�h���X�Ɏ������g����
		m_apEnd[m_nPriority] = this;	//�Ō���A�h���X�Ɏ������g����
	}
	else
	{
		m_pPrev = m_apEnd[m_nPriority];	//�O�̃I�u�W�F�N�g��ݒ�
		m_apEnd[m_nPriority] = this;	//�Ō���A�h���X�Ɏ������g����
		m_pPrev->m_pNext = this;	//���̃I�u�W�F�N�g��ݒ�
	}

	m_nNumAll++;	//�I�u�W�F�N�g�����̃J�E���g�A�b�v
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CObject::~CObject()
{
	m_nNumAll--;	//�I�u�W�F�N�g�����̃J�E���g�_�E��
}

//===========================================================================================
// �S�ẴI�u�W�F�N�g�̔j��
//===========================================================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < object::NUM_PRIORITY; nCntPriority++)
	{
		CObject* pObject = m_apTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			pObject->Uninit();	//�I��

			pObject = pObjectNext;	//���̃I�u�W�F�N�g����
		}
	}

	for (int nCntPri = 0; nCntPri < object::NUM_PRIORITY; nCntPri++)
	{
		// �擪�I�u�W�F�N�g����
		CObject* pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// ���̃A�h���X��ۑ�
			CObject* pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath == true)
			{
				// �폜
				pObject->Death();
			}

			// ���̃A�h���X����
			pObject = pObjectNext;
		}
	}
}

//===========================================================================================
// �S�ẴI�u�W�F�N�g�̍X�V
//===========================================================================================
void CObject::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < object::NUM_PRIORITY; nCntPriority++)
	{
		CObject* pObject = m_apTop[nCntPriority];	// �擪�I�u�W�F�N�g����

		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext;	// ���̃I�u�W�F�N�g��ۑ�

			pObject->Update();	// �X�V

			pObject = pObjectNext;	// ���̃I�u�W�F�N�g����
		}
	}

	for (int nCntPri = 0; nCntPri < object::NUM_PRIORITY; nCntPri++)
	{
		// �擪�I�u�W�F�N�g����
		CObject* pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// ���̃A�h���X��ۑ�
			CObject* pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath == true)
			{
				// �폜
				pObject->Death();
			}

			// ���̃A�h���X����
			pObject = pObjectNext;
		}
	}
}

//===========================================================================================
// �S�ẴI�u�W�F�N�g�̕`��
//===========================================================================================
void CObject::DrawAll(void)
{
	//�J�������̎擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	//�J�����̃Z�b�g
	if (pCamera != nullptr)
	{
		pCamera->SetCamera();
	}

	for (int nCntPriority = 0; nCntPriority < object::NUM_PRIORITY; nCntPriority++)
	{
		CObject* pObject = m_apTop[nCntPriority];	//�擪�I�u�W�F�N�g����

		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�N�g��ۑ�

			pObject->Draw();	//�`��

			pObject = pObjectNext;	//���̃I�u�W�F�N�g����
		}
	}

	for (int nCntPri = 0; nCntPri < object::NUM_PRIORITY; nCntPri++)
	{
		// �擪�I�u�W�F�N�g����
		CObject* pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// ���̃A�h���X��ۑ�
			CObject* pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath == true)
			{
				// �폜
				pObject->Death();
			}

			// ���̃A�h���X����
			pObject = pObjectNext;
		}
	}
}

//===================================================
// �I�u�W�F�N�g�̔j��
//===================================================
void CObject::Release(void)
{
	m_bDeath = true;	//���S��Ԃ�
}

//===================================================
// ���S�t���O�������Ă���I�u�W�F�N�g��j������
//===================================================
void CObject::Death(void)
{
	if (m_apEnd[m_nPriority] != this && m_apTop[m_nPriority] != this)
	{// �^�񒆂̃A�h���X�̔j��
		if (m_pPrev != nullptr)
		{
			// �O�̃A�h���X���玟�̃A�h���X���Ȃ�
			m_pPrev->m_pNext = m_pNext;
		}

		if (m_pNext != nullptr)
		{
			// ���̃A�h���X����O�̃A�h���X���Ȃ�
			m_pNext->m_pPrev = m_pPrev;
		}
	}

	if (m_apTop[m_nPriority] == this)
	{// �擪�A�h���X�̔j��
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = nullptr;
		}

		// �擪�A�h���X�����̃A�h���X�Ɉ����p��
		m_apTop[m_nPriority] = m_pNext;
	}

	if (m_apEnd[m_nPriority] == this)
	{// �Ō���A�h���X�̔j��
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = nullptr;
		}

		// �Ō���A�h���X��O�̃A�h���X�Ɉ����p��
		m_apEnd[m_nPriority] = m_pPrev;
	}

	delete this;
}

