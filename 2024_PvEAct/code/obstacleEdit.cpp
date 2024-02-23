//===========================================================================================
//
// ��Q���G�f�B�b�g[ObstacleEdit.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "obstacleEdit.h"
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "object.h"
#include "xfile.h"
#include "inputKeyboard.h"
#include "debugproc.h"

#include <stdio.h>

// �萔��`
namespace
{
	// x�t�@�C���p�X
	const char* apPathList[CObstacleEdit::TYPE_MAX] =
	{
		"data\\MODEL\\object\\TanukiShop.x",
		"data\\MODEL\\object\\mitumisi.x",
	};

	// ����
	const float SPEED = 20.0f;
}

// �ÓI�����o�ϐ��錾
int CObstacleEdit::m_nNumAll = 0;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CObstacleEdit::CObstacleEdit(int nPriority) /*: CObject(nPriority)*/
{
	m_bUse = false;
	m_type = TYPE_HOUSE;

	m_nNumAll++;	// �����J�E���g�A�b�v
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CObstacleEdit::~CObstacleEdit()
{
	m_nNumAll--;	// �����̃J�E���g�_�E��
}

//===========================================================================================
// ����
//===========================================================================================
CObstacleEdit* CObstacleEdit::Create(void)
{
	CObstacleEdit* pObjX = nullptr;

#ifdef _DEBUG	// �f�o�b�N�����s

	if (pObjX == nullptr)
	{
		pObjX = new CObstacleEdit;

		if (pObjX != NULL)
		{
			pObjX->Init();

			return pObjX;
		}
	}

#endif

	return pObjX;
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CObstacleEdit::Init(void)
{
	// �I�����f���ԍ��̐ݒ�
	m_nTypeIdx = TYPE_HOUSE;

	// 
	m_type = (EType)m_nTypeIdx;

	// ����������
	CObstacle::Init(apPathList[0], { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f, false);

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CObstacleEdit::Uninit(void)
{
	Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CObstacleEdit::Update(void)
{
	// ���̎擾
	CInputKeyboard* pInputKey = CInputKeyboard::GetInstance();
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CXfile* pXfile = CXfile::GetInstance();

	D3DXVECTOR3 scale = GetScale();
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// ����
	Operation(&pos, &rot);

	// �傫���̔��f
	SetScale(scale);

	// �ʒu�̔��f
	SetPosition(pos);

	// �����̔��f
	SetRotation(rot);

	// �f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print("\n\n�y�G�f�B�b�g���[�h���z\n\n");
	CManager::GetInstance()->GetDebugProc()->Print("�ʒu�F x:%f y:%f z:%f\n", pos.x, pos.y, pos.z);
	CManager::GetInstance()->GetDebugProc()->Print("�����F x:%f y:%f z:%f\n", rot.x, rot.y, rot.z);
	CManager::GetInstance()->GetDebugProc()->Print("��ށF %d\n\n", m_type);

	CManager::GetInstance()->GetDebugProc()->Print("�ݒu�F ENTER\n");
	CManager::GetInstance()->GetDebugProc()->Print("��ޕύX�F 1 or 2\n");
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CObstacleEdit::Draw(void)
{
	CObstacle::Draw();
}

//===========================================================================================
// ����
//===========================================================================================
void CObstacleEdit::Operation(D3DXVECTOR3* pos, D3DXVECTOR3* rot)
{
	// ���̎擾
	CInputKeyboard* pInputKey = CInputKeyboard::GetInstance();
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CXfile* pXfile = CXfile::GetInstance();

	// �ړ�
	if (pInputKey->GetTrigger(DIK_Q))
	{
		pos->y += SPEED;
	}
	if (pInputKey->GetTrigger(DIK_E))
	{
		pos->y -= SPEED;
	}
	if (pInputKey->GetTrigger(DIK_A))
	{
		pos->x -= SPEED;
	}
	if (pInputKey->GetTrigger(DIK_D))
	{
		pos->x += SPEED;
	}
	if (pInputKey->GetTrigger(DIK_W))
	{
		pos->z += SPEED;
	}
	if (pInputKey->GetTrigger(DIK_S))
	{
		pos->z -= SPEED;
	}

	// �I�u�W�F�N�g��ނ̕ύX
	if (pInputKey->GetTrigger(DIK_1))
	{
		// �J�E���g�A�b�v
		m_nTypeIdx = (m_nTypeIdx - 1 + TYPE_MAX) % TYPE_MAX;

		m_type = (EType)m_nTypeIdx;

		SetModel(apPathList[m_nTypeIdx]);
	}
	else if (pInputKey->GetTrigger(DIK_2))
	{
		// �J�E���g�_�E��
		m_nTypeIdx = (m_nTypeIdx + 1 + TYPE_MAX) % TYPE_MAX;

		m_type = (EType)m_nTypeIdx;

		SetModel(apPathList[m_nTypeIdx]);
	}

	// �I�u�W�F�N�g�̐ݒu
	if (pInputKey->GetTrigger(DIK_RETURN))
	{
		CObstacle::Create(apPathList[m_nTypeIdx], *pos);
	}
	
	// �t�@�C����������(�Z�[�u)
	if (pInputKey->GetTrigger(DIK_F9))
	{
		//Save();
	}
}

//===========================================================================================
// ��������
//===========================================================================================
void CObstacleEdit::Save(void)
{
	////�|�C���^�錾
	//FILE* pFile;

	////�t�@�C�����J��
	//pFile = fopen("", "wb");

	//if (pFile != nullptr)
	//{//�t�@�C�����J�����ꍇ
	//	fwrite(&m_nNumAll, sizeof(int), 1, pFile);

	//	for (int nCntBlock = 0; nCntBlock < m_nNumAll; nCntBlock++)
	//	{
	//		if (m_apBlock[nCntBlock] != nullptr)
	//		{
	//			memBlock.pos = m_apBlock[nCntBlock]->GetPosition();
	//			memBlock.rot = m_apBlock[nCntBlock]->GetRot();
	//			memBlock.type = m_apBlock[nCntBlock]->m_type;

	//			//�o�C�i���t�@�C���ɏ�������
	//			fwrite(&memBlock, sizeof(MemBlock), 1, pFile);
	//		}
	//	}

	//	//�t�@�C�������
	//	fclose(pFile);
	//}
	//else
	//{//�t�@�C�����J���Ȃ������ꍇ
	//	assert(("�u���b�N�f�[�^�̕ۑ����s", false));
	//}
}
