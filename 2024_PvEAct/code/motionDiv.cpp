//=============================================================================
// 
//  �L�����N�^�[����[character.cpp]
// Author : Takeru Ogasawara
// 
//=============================================================================

// �C���N���[�h�錾
#include "motionDiv.h"
#include "renderer.h"
#include "manager.h"
#include "xfile.h"
#include "texture.h"
#include "modelParts.h"

#include "input.h"
#include "general.h"
#include "debugproc.h"
#include <stdio.h>

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMotionDiv::CMotionDiv()
{
	// �l�̃N���A
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_apModelPart[0], sizeof(m_apModelPart));
	ZeroMemory(&m_aMotion[0], sizeof(m_aMotion));
	ZeroMemory(&m_aMotionInfo[0], sizeof(m_aMotionInfo));
	m_fCntMmultiply = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMotionDiv::~CMotionDiv()
{

}

//=============================================================================
//	����
//=============================================================================
CMotionDiv* CMotionDiv::Create(const char* pUpperPath, const char* pLowerPath)
{
	CMotionDiv* pMotion = nullptr;

	if (pMotion == nullptr)
	{
		pMotion = new CMotionDiv;

		if (pMotion != nullptr)
		{
			pMotion->Init(pUpperPath, pLowerPath);

			return pMotion;
		}
	}

	return pMotion;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMotionDiv::Init()
{

	return S_OK;
}

//=============================================================================
// ����������(�I�[�o�[���[�h)
//=============================================================================
HRESULT CMotionDiv::Init(const char* pUpperPath, const char* pLowerPath)
{
	m_info.pos = { 0.0f, 0.0f, 0.0f };
	m_info.rot = { 0.0f, 0.0f, 0.0f };

	// �Ǎ���
	Load(pUpperPath, pLowerPath);

	// �ŏ��ɍs�����[�V������ݒ�
	InitMotionDiv(0);
	InitPose(0);

	// �J�E���g
	m_fCntMmultiply = 1.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMotionDiv::Uninit()
{
	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{
		for (int nCntParts = 0; nCntParts < m_info.aNumLoadPartsDiv[nCntDiv]; nCntParts++)
		{
			// ���f���p�[�c�̔j��
			if (m_apModelPart[nCntDiv][nCntParts]->pParts != nullptr)
			{
				m_apModelPart[nCntDiv][nCntParts]->pParts->Uninit();
				m_apModelPart[nCntDiv][nCntParts]->pParts = nullptr;
			}
			if (m_apModelPart[nCntDiv][nCntParts] != nullptr)
			{
				delete m_apModelPart[nCntDiv][nCntParts];
				m_apModelPart[nCntDiv][nCntParts] = nullptr;
			}
		}
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMotionDiv::Update()
{
	// ���[�V�����̍X�V
	UpdateMotionDiv();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMotionDiv::Draw()
{
	// �}�g���b�N�X�̐ݒ�
	general::SetMatrix(&m_info.mtxWorld, m_info.pos, m_info.rot);

	// �}�g���b�N�X�|�����킹
	MultiplyMtx();
}

//=============================================================================
// �}�g���b�N�X�|�����킹
//=============================================================================
void CMotionDiv::MultiplyMtx(void)
{
	// ���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CTexture* pTexture = CTexture::GetInstance();

	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX* pMtx{}, * pMtxParent{};	// �e�̃}�g���b�N�X

	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{
		if (m_apModelPart[nCntDiv][0] == nullptr)
		{
			continue;
		}

		for (int nCntParts = 0; nCntParts < m_info.aNumLoadPartsDiv[nCntDiv]; nCntParts++)
		{
			// �}�g���b�N�X�̎擾
			pMtx = m_apModelPart[nCntDiv][nCntParts]->pParts->GetMatrix();

			// ���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(pMtx);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(
				&mtxRot,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetRotation().y,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetRotation().x,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetRotation().z
			);
			D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(
				&mtxTrans,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetPosition().x,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetPosition().y,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetPosition().z
			);
			D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);

			if (m_apModelPart[nCntDiv][nCntParts]->nIdxParent != -1)
			{// �e�p�[�c������ꍇ

				// �e�}�g���b�N�X�̎擾
				pMtxParent = m_apModelPart[nCntDiv][m_apModelPart[nCntDiv][nCntParts]->nIdxParent]->pParts->GetMatrix();
			}
			else
			{
				if (nCntDiv == DIV_LOWER)
				{
					pMtxParent = &m_info.mtxWorld;
				}
				else if (nCntDiv == DIV_UPPER)
				{// �J�E���g���㔼�g�̏ꍇ�e�������g�ɐݒ肷��
					pMtxParent = m_apModelPart[DIV_LOWER][0]->pParts->GetMatrix();
				}
			}

			// �e�p�[�c�ƃp�[�c�̃��[���h�}�g���b�N�X���������킹��
			D3DXMatrixMultiply(pMtx, pMtx, pMtxParent);

			// ���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, pMtx);

			// �p�[�c�̕`��
			m_apModelPart[nCntDiv][nCntParts]->pParts->Draw();
		}
	}
}

//=============================================================================
// �ŏ��̃��[�V����
//=============================================================================
void CMotionDiv::InitMotionDiv(int nType)
{
	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{
		for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntDiv][nType].nNumKey; nCntKey++)
		{
			for (int nCntPart = 0; nCntPart < m_aMotion[nCntDiv].nNumParts; nCntPart++)
			{
				m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart];


				m_apModelPart[nCntDiv][nCntPart]->pParts->SetPosition(D3DXVECTOR3
				(
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fPosX,
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fPosY,
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ)
				);
				m_apModelPart[nCntDiv][nCntPart]->pParts->SetRotation(D3DXVECTOR3
				(
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fRotX,
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fRotY,
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ)
				);
			}
		}
	}
}

//=============================================================================
// ���[�V�����̍X�V
//=============================================================================
void CMotionDiv::UpdateMotionDiv(void)
{
	// �ϐ��錾
	D3DXVECTOR3 posPart = {}, rotPart = {};	// 
	D3DXVECTOR3 nextPos = {};	// ���̈ʒu
	D3DXVECTOR3 nextRot = {};	// ���̌���
	SKey diff = {}, dest = {};	// �����E�ڕW
	SKey CurKey = {}, nextKey = {}, oldKey = {};
	int nType = 0;
	int nNextKey = 0;
	float fFrame = 0.0f;

	// ���݂̈ʒu��ۑ�
	m_info.posOld = m_info.pos;

	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{

		if (m_apModelPart[nCntDiv][0] == nullptr)
		{
			break;
		}

		nType = m_aMotion[nCntDiv].nType;
		
		if (m_aMotionInfo[nCntDiv][nType].nNumKey <= 0)
		{
			continue;
		}

		// ���̃L�[�̐ݒ�
		nNextKey = (m_aMotion[nCntDiv].nKey + 1 + m_aMotionInfo[nCntDiv][nType].nNumKey) % m_aMotionInfo[nCntDiv][nType].nNumKey;

		m_aMotion[nCntDiv].bLoop = m_aMotionInfo[nCntDiv][nType].bLoop;

		for (int nCntPart = 0; nCntPart < m_info.aNumLoadPartsDiv[nCntDiv]; nCntPart++)
		{
			// �p�[�c���̎擾
			posPart = m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosOrg();
			rotPart = m_apModelPart[nCntDiv][nCntPart]->pParts->GetRotation();

			// �t���[�����̑��
			fFrame = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].fFrame;
			// ���݂̃L�[������
			CurKey.fPosX = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fPosX;
			CurKey.fPosY = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fPosY;
			CurKey.fPosZ = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fPosZ;
			CurKey.fRotX = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fRotX;
			CurKey.fRotY = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fRotY;
			CurKey.fRotZ = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fRotZ;
			// ���̃L�[������
			nextKey.fPosX = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fPosX;
			nextKey.fPosY = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fPosY;
			nextKey.fPosZ = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fPosZ;
			nextKey.fRotX = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fRotX;
			nextKey.fRotY = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fRotY;
			nextKey.fRotZ = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fRotZ;
			// �ߋ��̃L�[
			oldKey.fPosX = m_aKeyOld[nCntDiv][nCntPart].fPosX;
			oldKey.fPosY = m_aKeyOld[nCntDiv][nCntPart].fPosY;
			oldKey.fPosZ = m_aKeyOld[nCntDiv][nCntPart].fPosZ;
			oldKey.fRotX = m_aKeyOld[nCntDiv][nCntPart].fRotX;
			oldKey.fRotY = m_aKeyOld[nCntDiv][nCntPart].fRotY;
			oldKey.fRotZ = m_aKeyOld[nCntDiv][nCntPart].fRotZ;


			// �ʒu�E�����̍������Z�o =====================================================================================
			//diff.fPosX = nextKey.fPosX - CurKey.fPosX;
			//diff.fPosY = nextKey.fPosY - CurKey.fPosY;
			//diff.fPosZ = nextKey.fPosZ - CurKey.fPosZ;
			//diff.fRotX = nextKey.fRotX - CurKey.fRotX;
			//diff.fRotY = nextKey.fRotY - CurKey.fRotY;
			//diff.fRotZ = nextKey.fRotZ - CurKey.fRotZ;

			// �u�����h
			diff.fPosX = nextKey.fPosX - oldKey.fPosX;
			diff.fPosY = nextKey.fPosY - oldKey.fPosY;
			diff.fPosZ = nextKey.fPosZ - oldKey.fPosZ;
			diff.fRotX = nextKey.fRotX - oldKey.fRotX;
			diff.fRotY = nextKey.fRotY - oldKey.fRotY;
			diff.fRotZ = nextKey.fRotZ - oldKey.fRotZ;

			// �����̏C��
			general::RotLimit(D3DXVECTOR3(diff.fRotX, diff.fRotY, diff.fRotZ));

			// �ʒu�E������ݒ� =====================================================================================
			// �u�����h
			dest.fPosX = posPart.x + oldKey.fPosX + diff.fPosX * (m_aMotion[nCntDiv].fCounter / fFrame);
			dest.fPosY = posPart.y + oldKey.fPosY + diff.fPosY * (m_aMotion[nCntDiv].fCounter / fFrame);
			dest.fPosZ = posPart.z + oldKey.fPosZ + diff.fPosZ * (m_aMotion[nCntDiv].fCounter / fFrame);
			dest.fRotX = oldKey.fRotX + diff.fRotX * (m_aMotion[nCntDiv].fCounter / fFrame);
			dest.fRotY = oldKey.fRotY + diff.fRotY * (m_aMotion[nCntDiv].fCounter / fFrame);
			dest.fRotZ = oldKey.fRotZ + diff.fRotZ * (m_aMotion[nCntDiv].fCounter / fFrame);

			// �����̏C��
			general::RotLimit(D3DXVECTOR3(dest.fRotX, dest.fRotY, dest.fRotZ));

			// �ʒu�E�����̔��f
			if (m_apModelPart[nCntPart] != nullptr)
			{
				m_apModelPart[nCntDiv][nCntPart]->pParts->SetPosition(D3DXVECTOR3(dest.fPosX, dest.fPosY, dest.fPosZ));
				m_apModelPart[nCntDiv][nCntPart]->pParts->SetRotation(D3DXVECTOR3(dest.fRotX, dest.fRotY, dest.fRotZ));
			}
		}
		
		//if (!m_aMotion[nCntDiv].bFinish)
		{
			// ���[�V�����J�E���^�[��i�߂�
			m_aMotion[nCntDiv].fCounter += (1.0f * m_fCntMmultiply);
		}

		if (m_aMotion[nCntDiv].nKey >= m_aMotionInfo[nCntDiv][nType].nNumKey - 1)
		{// �L�[�����𒴂�����

			// ���݂̃��[�V�����^�C�v�̕ۑ�
			m_aMotion[nCntDiv].nTypeOld = m_aMotion[nCntDiv].nType;

			// �I��������Ԃ�
			m_aMotion[nCntDiv].bFinish = true;

			if (m_aMotionInfo[nCntDiv][nType].bLoop)
			{
				// ���[�V�����ݒ�
				SetMotion(nCntDiv, nType);
			}
		}

		if (m_aMotion[nCntDiv].fCounter > (float)m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].fFrame)
		{// �L�[�̃t���[�����ɂȂ�����

			if (m_aMotion[nCntDiv].nKey < m_aMotionInfo[nCntDiv][nType].nNumKey)
			{
				// ���݂̃L�[��i�߂�
				m_aMotion[nCntDiv].nKey = (m_aMotion[nCntDiv].nKey + 1 + m_aMotionInfo[nCntDiv][nType].nNumKey) % m_aMotionInfo[nCntDiv][nType].nNumKey;

				// �J�E���^�[��߂�
				m_aMotion[nCntDiv].fCounter = 0.0f;

				// �ߋ��L�[�̐ݒ�
				SetKeyOld();
			}
		}

		// �f�o�b�O�\��
		//Debug(nCntDiv, nType);
	}
}

//=============================================================================
// ���[�V�����ݒ�
//=============================================================================
void CMotionDiv::SetMotion(int nNumDiv, int type)
{
	SetKeyOld();	// �ߋ��L�[�̐ݒ�
	m_aMotion[nNumDiv].bFinish = false;	// �I�����ĂȂ����
	m_aMotion[nNumDiv].nType = type;	// ���[�V�����^�C�v�̐ݒ�
	m_aMotion[nNumDiv].nKey = 0;		// ���݂̃L�[���N���A
	m_aMotion[nNumDiv].fCounter = 0.0f;	// ���[�V�����J�E���^�[���N���A
}

//=====================================================
// �|�[�Y�̏����ݒ�
//=====================================================
void CMotionDiv::InitPose(int nType)
{
	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{
		for (int i = 0; i < m_aMotionInfo[nCntDiv][nType].nNumKey; i++)
		{
			for (int nCntPart = 0; nCntPart < m_aMotion[nCntDiv].nNumParts; nCntPart++)
			{
				m_aKeyOld[nCntDiv][nCntPart] = m_aMotionInfo[nCntDiv][nType].aKeyInfo[i].aKey[nCntPart];
			}
		}
	}
}

//=====================================================
// �I������L�[�̏��ۑ�
//=====================================================
void CMotionDiv::SetKeyOld(void)
{
	for (int nCntDiv = 0; nCntDiv < motionDiv::MODEL_MAX; nCntDiv++)
	{
		for (int nCntPart = 0; nCntPart < m_aMotion[nCntDiv].nNumParts; nCntPart++)
		{
			m_aKeyOld[nCntDiv][nCntPart].fPosX = m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosition().x - m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosOrg().x;
			m_aKeyOld[nCntDiv][nCntPart].fPosY = m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosition().y - m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosOrg().y;
			m_aKeyOld[nCntDiv][nCntPart].fPosZ = m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosition().z - m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosOrg().z;

			m_aKeyOld[nCntDiv][nCntPart].fRotX = m_apModelPart[nCntDiv][nCntPart]->pParts->GetRotation().x;
			m_aKeyOld[nCntDiv][nCntPart].fRotY = m_apModelPart[nCntDiv][nCntPart]->pParts->GetRotation().y;
			m_aKeyOld[nCntDiv][nCntPart].fRotZ = m_apModelPart[nCntDiv][nCntPart]->pParts->GetRotation().z;
		}
	}
}

//=============================================================================
// �ǂݍ���
//=============================================================================
void CMotionDiv::Load(const char* pUpperPath, const char* pLowerPath)
{
	FILE* pFile = nullptr;	// �t�@�C���|�C���^�錾

	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{
		if (nCntDiv == DIV_LOWER)
		{
			// �t�@�C�����J��
			pFile = fopen(pLowerPath, "r");
		}
		else if(nCntDiv == DIV_UPPER)
		{
			// �t�@�C�����J��
			pFile = fopen(pUpperPath, "r");
		}

		if (pFile != nullptr)
		{// �t�@�C�����J�����ꍇ

			// �L�����N�^�[�̓ǂݍ���
			LoadCharacter(pFile, nCntDiv);

			// ���[�V�����̓Ǎ���
			LoadMotion(pFile, nCntDiv);

			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//	�L�����N�^�[�̓Ǎ���
//=============================================================================
void CMotionDiv::LoadCharacter(FILE* pFile, int nCntDiv)
{
	// �ϐ��錾
	char cTemp[254] = {};	// �ꎞ�ۑ��p
	char aPath[254] = {};	// �t�@�C���p�X�p
	bool bStart = false;	// �L�����N�^�̑g�ݗ��Ă��ł��Ă邩�ǂ���

	//----------------------------------------------------------------------
	// �t�@�C�����Ǎ���
	//----------------------------------------------------------------------
	while (1)
	{
		// ������Ǎ���
		(void)fscanf(pFile, "%s", &cTemp[0]);

		if (strcmp(cTemp, "NUM_MODEL") == 0)
		{
			// "="�Ǎ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			// �ǂݍ��ރ��f��(�p�[�c)���Ǎ���
			(void)fscanf(pFile, "%d", &m_info.aNumLoadPartsDiv[nCntDiv]);

			break;
		}

		// �G���[�\��
		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			assert(("�L�����N�^���̓ǂݍ��݂Ɏ��s", false));
		}
	}

	for (int nCntModel = 0; nCntModel < m_info.aNumLoadPartsDiv[nCntDiv];)
	{
		// ������Ǎ���
		(void)fscanf(pFile, "%s", &cTemp[0]);

		if (strcmp(cTemp, "MODEL_FILENAME") == 0)
		{// �t�@�C�����Ǎ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			// ���f���p�X�Ǎ���
			(void)fscanf(pFile, "%s", &aPath[0]);

			// �C���X�^���X����
			m_apModelPart[nCntDiv][nCntModel] = new SModelParts;
			m_apModelPart[nCntDiv][nCntModel]->pParts = CModelParts::Create();

			// ���f���ԍ��擾
			int nIdx = CXfile::GetInstance()->Load(&aPath[0]);

			// ���f���Ǎ���
			m_apModelPart[nCntDiv][nCntModel]->pParts->SetIdxModel(nIdx);
			m_apModelPart[nCntDiv][nCntModel]->pParts->BindModel(m_apModelPart[nCntDiv][nCntModel]->pParts->GetIdxModel());

			nCntModel++;
		}
	}

	//----------------------------------------------------------------------
	// �L�����N�^�[�̑g�ݗ���
	//----------------------------------------------------------------------
	while (1)
	{
		// ������ǂݍ���
		(void)fscanf(pFile, "%s", &cTemp[0]);

		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{// �I�����f
			return;
		}

		if (strcmp(cTemp, "END_CHARACTERSET") == 0)
		{// �g�ݗ��ďI���̔��f
			return;
		}

		if (strcmp(cTemp, "CHARACTERSET") != 0)
		{// �L�����N�^�[�g�ݗ��ĊJ�n�̔��f
			continue;
		}

		while (strcmp(cTemp, "END_SCRIPT") != 0)
		{
			if (strcmp(cTemp, "NUM_PARTS") == 0)
			{
				// "="�ǂݍ���
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_info.aNumSetPartsDiv[nCntDiv]);

				break;
			}

			// ������ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);
		}

		for (int nCntParts = 0; nCntParts < m_info.aNumSetPartsDiv[nCntDiv];)
		{
			// ������ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "END_CHARACTERSET") == 0)
			{// �g�ݗ��ďI���̔��f
				return;
			}

			if (strcmp(cTemp, "PARTSSET") == 0)
			{// �p�[�c�̐ݒ�

				while (strcmp(cTemp, "END_PARTSSET") != 0)
				{
					// ������Ǎ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "INDEX") == 0)
					{// �ԍ��Ǎ���
						// "="�Ǎ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apModelPart[nCntDiv][nCntParts]->nIdx);
					}
					if (strcmp(cTemp, "PARENT") == 0)
					{// �e�ԍ��Ǎ���
						// "="�Ǎ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apModelPart[nCntDiv][nCntParts]->nIdxParent);

						if (m_apModelPart[nCntDiv][nCntParts]->nIdxParent <= -1)
						{// �e�ԍ��̕ۑ�

							m_aMotion[nCntDiv].nIdxParent = nCntParts;
						}
					}
					if (strcmp(cTemp, "POS") == 0)
					{// �ʒu�Ǎ���
						// "="�Ǎ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						D3DXVECTOR3 pos = {};

						(void)fscanf(pFile, "%f", &pos.x);
						(void)fscanf(pFile, "%f", &pos.y);
						(void)fscanf(pFile, "%f", &pos.z);

						m_apModelPart[nCntDiv][nCntParts]->pParts->SetPosition(pos);
						m_apModelPart[nCntDiv][nCntParts]->pParts->SetPosOrg(pos);
					}
					if (strcmp(cTemp, "ROT") == 0)
					{//�����Ǎ���
						// "="�Ǎ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						D3DXVECTOR3 rot = {};

						(void)fscanf(pFile, "%f", &rot.x);
						(void)fscanf(pFile, "%f", &rot.y);
						(void)fscanf(pFile, "%f", &rot.z);

						m_apModelPart[nCntDiv][nCntParts]->pParts->SetRotation(rot);
					}
				}
				nCntParts++;
				m_aMotion[nCntDiv].nNumParts++;
			}
		}
	}	
}

//=============================================================================
// ���[�V�����̓Ǎ���
//=============================================================================
void CMotionDiv::LoadMotion(FILE* pFile, int nCntDiv)
{
	// �ϐ��錾
	char cTemp[254] = {};
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntPart = 0;

	while (1)
	{
		// ������Ǎ���
		(void)fscanf(pFile, "%s", &cTemp[0]);

		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			return;
		}

		// ���[�V�����ݒ�
		if (strcmp(cTemp, "MOTIONSET") == 0)
		{
			while (strcmp(cTemp, "END_MOTIONSET") != 0)
			{
				// ������̓Ǎ���
				(void)fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "LOOP") == 0)
				{// ���[�v���f

					(void)fscanf(pFile, "%s", &cTemp[0]);	// "="�Ǎ���
					(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntDiv][nCntMotion].bLoop);
				}
				if (strcmp(cTemp, "NUM_KEY") == 0)
				{// �L�[��

					(void)fscanf(pFile, "%s", &cTemp[0]);	// "="�Ǎ���
					(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][nCntMotion].nNumKey);
				}

				if (strcmp(cTemp, "KEYSET") == 0)
				{
					// �t���[�����擾
					while (1)
					{
						// ������̓Ǎ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "FRAME") == 0)
						{
							(void)fscanf(pFile, "%s", &cTemp[0]);	// "="�Ǎ���

							(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].fFrame);

							break;
						}
					}

					while (strcmp(cTemp, "END_KEYSET") != 0)
					{
						// ������̓Ǎ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "KEY") == 0)
						{// �L�[���̎擾

							while (strcmp(cTemp, "END_KEY") != 0)
							{// �I���܂ŃL�[�ݒ�

								// ������̓Ǎ���
								(void)fscanf(pFile, "%s", &cTemp[0]);

								if (strcmp(cTemp, "POS") == 0)
								{// �ʒu�擾

									(void)fscanf(pFile, "%s", &cTemp[0]);	// "="�Ǎ���
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
								}
								if (strcmp(cTemp, "ROT") == 0)
								{// �����擾

									(void)fscanf(pFile, "%s", &cTemp[0]);	// "="�Ǎ���
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
								}
							}
							nCntPart++;
						}
					}
					nCntKey++;
					nCntPart = 0;
				}
			}
			nCntKey = 0;
			nCntMotion++;
			m_aMotion[nCntDiv].nNumMotion++;	// ���[�V�������̃J�E���g�A�b�v
		}
	}
}

//=============================================================================
// �p�[�c���̎擾
//=============================================================================
CModelParts* CMotionDiv::GetParts(int nNumDiv, int nNumPart)
{
	if (m_apModelPart[nNumDiv][nNumPart]->pParts == nullptr)
	{
		return nullptr;
	}

	return m_apModelPart[nNumDiv][nNumPart]->pParts;
}

//=============================================================================
// �f�o�b�O�\��
//=============================================================================
void CMotionDiv::Debug(int nCntDiv, int nType)
{
#ifndef _DEBUG
	return;
#endif

	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	
	pDebug->Print("\n�y���[�V�������z\n");
	pDebug->Print("�t���[�� : %f/%f\n", m_aMotion[nCntDiv].fCounter, m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].fFrame);
	pDebug->Print("�p�[�c�� : %d\n", m_aMotion[nCntDiv].nNumParts);
	pDebug->Print("�L�[�� : %d\n", m_aMotionInfo[nCntDiv][nType].nNumKey);
	pDebug->Print("���݃L�[ : %d\n", m_aMotion[nCntDiv].nKey);
	pDebug->Print("���[�V������ : %d\n", m_aMotion[nCntDiv].nNumMotion);
}
