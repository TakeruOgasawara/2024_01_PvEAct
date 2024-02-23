//=============================================================================
// 
// ���[�V��������[character.cpp]
// Author : Takeru Ogasawara
// 
//=============================================================================

// �C���N���[�h�錾
#include "motion.h"
#include "renderer.h"
#include "manager.h"
#include "xfile.h"
#include "texture.h"
#include "modelParts.h"

#include "general.h"
#include "debugproc.h"

#define SPEED	(0.5f);

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMotion::CMotion()
{
	// �l�̃N���A
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_apModelPart[0], sizeof(m_apModelPart));
	ZeroMemory(&m_motion, sizeof(m_motion));
	ZeroMemory(&m_aMotionInfo[0], sizeof(m_aMotionInfo));
	m_fCntMmultiply = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
//	����
//=============================================================================
CMotion* CMotion::Create(const char* pTextPath)
{
	CMotion* pMotion = nullptr;

	if (pMotion == nullptr)
	{
		pMotion = new CMotion;

		if (pMotion != nullptr)
		{
			pMotion->Init(pTextPath);

			return pMotion;
		}
	}

	return pMotion;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMotion::Init()
{
	return S_OK;
}

//=============================================================================
// ����������(�I�[�o�[���[�h)
//=============================================================================
HRESULT CMotion::Init(const char* pTextPath)
{
	m_info.pos = { 0.0f, 0.0f, 0.0f };
	m_info.rot = { 0.0f, 0.0f, 0.0f };

	Load(pTextPath);

	m_motion.nType = 0;
	m_fCntMmultiply = 1.0f;

	InitMotion(0);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMotion::Uninit()
{
	for (int i = 0; i < m_motion.nNumMotion; i++)
	{
		if (m_aMotionInfo[i] != NULL)
		{
			delete m_aMotionInfo[i];
			m_aMotionInfo[i] = nullptr;
		}
	}

	for (int i = 0; i < m_info.nNumModelparts; i++)
	{
		if (m_apModelPart[i]->pParts != nullptr)
		{
			m_apModelPart[i]->pParts->Uninit();
			m_apModelPart[i]->pParts = nullptr;
		}
		if (m_apModelPart[i] != nullptr)
		{
			delete m_apModelPart[i];
			m_apModelPart[i]->pParts = nullptr;
		}
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CMotion::Update()
{
	// ���̎擾
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CXfile* pXfile = CXfile::GetInstance();

	if (m_aMotionInfo[m_motion.nType] == nullptr)
	{
		return;
	}

	// �ϐ��錾
	D3DXVECTOR3 posPart = {}, rotPart = {};	// �p�[�c�̈ʒu�E����
	D3DXVECTOR3 nextPos = {};	// ���̈ʒu
	D3DXVECTOR3 nextRot = {};	// ���̌���
	SKey diff = {}, dest = {};	// �����E�ڕW
	SKey CurKey = {}, nextKey = {};	// ���݁E���̃L�[
	float fFrame = 0.0f;

	int nType = m_motion.nType;

	// ���̃L�[�̐ݒ�
	int nNextKey = (m_motion.nKey + 1 + m_aMotionInfo[nType]->nNumKey) % m_aMotionInfo[nType]->nNumKey;

	for (int nCntPart = 0; nCntPart < m_info.nNumModelparts; nCntPart++)
	{
		// �p�[�c���̎擾
		posPart = m_apModelPart[nCntPart]->pParts->GetPosition();
		rotPart = m_apModelPart[nCntPart]->pParts->GetRotation();

		// ���݂̃L�[������
		CurKey.fPosX = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fPosX;
		CurKey.fPosY = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fPosY;
		CurKey.fPosZ = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fPosZ;
		CurKey.fRotX = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fRotX;
		CurKey.fRotY = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fRotY;
		CurKey.fRotZ = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fRotZ;
		// ���̃L�[������
		nextKey.fPosX = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fPosX;
		nextKey.fPosY = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fPosY;
		nextKey.fPosZ = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fPosZ;
		nextKey.fRotX = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fRotX;
		nextKey.fRotY = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fRotY;
		nextKey.fRotZ = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fRotZ;
		// �p�[�c���̎擾
		posPart = m_apModelPart[nCntPart]->pParts->GetPosOrg();
		// �t���[�����̑��
		fFrame = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].fFrame;

		// �L�[�̏�񂩂�ʒu�E�������Z�o
		diff.fPosX = nextKey.fPosX - CurKey.fPosX;
		diff.fPosY = nextKey.fPosY - CurKey.fPosY;
		diff.fPosZ = nextKey.fPosZ - CurKey.fPosZ;
		diff.fRotX = nextKey.fRotX - CurKey.fRotX;
		diff.fRotY = nextKey.fRotY - CurKey.fRotY;
		diff.fRotZ = nextKey.fRotZ - CurKey.fRotZ;

		// �����̏C��
		general::RotLimit(D3DXVECTOR3(diff.fRotX, diff.fRotY, diff.fRotZ));

		// �p�[�c�̈ʒu�E������ݒ�
		dest.fPosX = posPart.x + CurKey.fPosX + diff.fPosX * (m_motion.fCounter / fFrame);
		dest.fPosY = posPart.y + CurKey.fPosY + diff.fPosY * (m_motion.fCounter / fFrame);
		dest.fPosZ = posPart.z + CurKey.fPosZ + diff.fPosZ * (m_motion.fCounter / fFrame);

		dest.fRotX = CurKey.fRotX + diff.fRotX * (m_motion.fCounter / fFrame);
		dest.fRotY = CurKey.fRotY + diff.fRotY * (m_motion.fCounter / fFrame);
		dest.fRotZ = CurKey.fRotZ + diff.fRotZ * (m_motion.fCounter / fFrame);

		// �����̏C��
		general::RotLimit(D3DXVECTOR3(dest.fRotX, dest.fRotY, dest.fRotZ));

		// �ʒu�E�����̔��f
		if (m_apModelPart[nCntPart] != nullptr)
		{
			m_apModelPart[nCntPart]->pParts->SetPosition(D3DXVECTOR3(dest.fPosX, dest.fPosY, dest.fPosZ));
			m_apModelPart[nCntPart]->pParts->SetRotation(D3DXVECTOR3(dest.fRotX, dest.fRotY, dest.fRotZ));
		}
	}

	if (!m_motion.bFinish)
	{
		// ���[�V�����J�E���^�[��i�߂�
		m_motion.fCounter += (1.0f * m_fCntMmultiply);
	}

	if (m_motion.nKey > m_aMotionInfo[nType]->nNumKey - 1)
	{// �L�[�����𒴂�����

		// ���݂̃��[�V�����^�C�v�̕ۑ�
		//m_aMotion[nCntDiv].nTypeOld = m_aMotion[nCntDiv].nType;

		// �I��������Ԃ�
		m_motion.bFinish = true;

		m_motion.fCounter = 0.0f;

		if (m_aMotionInfo[nType]->bLoop)
		{
			// ���[�V�����ݒ�
			SetMotion(nType);
		}
	}

	if (m_motion.fCounter > m_aMotionInfo[m_motion.nType]->aKeyInfo[m_motion.nKey].fFrame)
	{// �L�[�̃t���[�����ɂȂ�����
		
		// ���݂̃L�[��i�߂�
		m_motion.nKey = (m_motion.nKey + 1 + m_aMotionInfo[m_motion.nType]->nNumKey) % m_aMotionInfo[m_motion.nType]->nNumKey;

		m_motion.fCounter = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMotion::Draw()
{
	// �}�g���b�N�X�̐ݒ�
	general::SetMatrix(&m_info.mtxWorld, m_info.pos, m_info.rot);

	// �}�g���b�N�X�|�����킹
	MultiplyMtx();
}

//=============================================================================
// ���[�V�����ݒ�
//=============================================================================
void CMotion::SetMotion(int nType)
{
	m_motion.nType = nType;
	m_motion.bFinish = false;
	m_motion.nKey = 0;
	m_motion.fCounter = 0.0f;
}

//=============================================================================
// �}�g���b�N�X�|�����킹
//=============================================================================
void CMotion::MultiplyMtx(void)
{
	// ���擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CTexture* pTexture = CTexture::GetInstance();

	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX* pMtx, *pMtxParent;	// �e�̃}�g���b�N�X

	for (int nCntParts = 0; nCntParts < m_info.nNumModelparts; nCntParts++)
	{
		// �}�g���b�N�X�̎擾
		pMtx = m_apModelPart[nCntParts]->pParts->GetMatrix();

		// ���[���h�}�g���b�N�X������
		D3DXMatrixIdentity(pMtx);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,m_apModelPart[nCntParts]->pParts->GetRotation().y, m_apModelPart[nCntParts]->pParts->GetRotation().x, m_apModelPart[nCntParts]->pParts->GetRotation().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			m_apModelPart[nCntParts]->pParts->GetPosition().x, m_apModelPart[nCntParts]->pParts->GetPosition().y, m_apModelPart[nCntParts]->pParts->GetPosition().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);

		if (m_apModelPart[nCntParts]->nIdxParent != -1)
		{//�e�p�[�c������ꍇ

			// �e�}�g���b�N�X�̎擾
			pMtxParent = m_apModelPart[m_apModelPart[nCntParts]->nIdxParent]->pParts->GetMatrix();
		}
		else
		{
			pMtxParent = &m_info.mtxWorld;
		}

		// �e�p�[�c�ƃp�[�c�̃��[���h�}�g���b�N�X���������킹��
		D3DXMatrixMultiply(pMtx, pMtx, pMtxParent);

		// ���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, pMtx);

		m_apModelPart[nCntParts]->pParts->Draw();
	}
}

//=============================================================================
// �ŏ��̃��[�V����
//=============================================================================
void CMotion::InitMotion(int nType)
{
	for (int nCntKey = 0; nCntKey < m_aMotionInfo[nType]->nNumKey; nCntKey++)
	{
		for (int nCntPart = 0; nCntPart < m_motion.nNumParts; nCntPart++)
		{
			m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart];
			

			m_apModelPart[nCntPart]->pParts->SetPosition(D3DXVECTOR3
			(
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fPosX,
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fPosY,
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fPosZ)
			);
			m_apModelPart[nCntPart]->pParts->SetRotation(D3DXVECTOR3
			(
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fRotX,
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fRotY,
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fRotZ)
			);
		}
	}
}

//=============================================================================
// �ǂݍ���
//=============================================================================
void CMotion::Load(const char* cTextPath)
{
	// �ϐ��錾
	char cTemp[254] = {};

	D3DXVECTOR3 pos = {};

	// �t�@�C������ǂݍ���
	FILE* pFile = fopen(cTextPath, "r");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ

		// �L�����N�^���̓ǂݍ���
		LoadCharacter(pFile);

		// ���[�V�����ݒ�
		LoadMotion(pFile);

		// �t�@�C�������
		fclose(pFile);
	}
}

//=============================================================================
// �L�����N�^�[���ǂݍ���
//=============================================================================
void CMotion::LoadCharacter(FILE* pFile)
{
	// �ϐ��錾
	char cTemp[254] = {};	// �ꎞ�ۑ��p
	char aPath[254] = {};	// �t�@�C���p�X�p
	int nCntModel = 0;
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

			// ���f�����Ǎ���
			(void)fscanf(pFile, "%d", &m_info.nNumModelparts);

			break;
		}

		// �G���[�\��
		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			assert(("�L�����N�^���̓ǂݍ��݂Ɏ��s", false));
		}
	}

	for (int nCntModel = 0; nCntModel < m_info.nNumModelparts;)
	{
		// ������Ǎ���
		(void)fscanf(pFile, "%s", &cTemp[0]);

		if (strcmp(cTemp, "MODEL_FILENAME") == 0)
		{// �t�@�C�����Ǎ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			// ���f���p�X�Ǎ���
			(void)fscanf(pFile, "%s", &aPath[0]);

			// �C���X�^���X����
			m_apModelPart[nCntModel] = new SModelParts;
			m_apModelPart[nCntModel]->pParts = CModelParts::Create();

			// ���f���ԍ��擾
			int nIdx = CXfile::GetInstance()->Load(&aPath[0]);

			// ���f���Ǎ���
			m_apModelPart[nCntModel]->pParts->SetIdxModel(nIdx);
			m_apModelPart[nCntModel]->pParts->BindModel(m_apModelPart[nCntModel]->pParts->GetIdxModel());

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

		if (strcmp(cTemp, "CHARACTERSET") != 0)
		{// �L�����N�^�[�g�ݗ��Ă̔��f
			continue;
		}

		while (1)
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
					// ������ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "INDEX") == 0)
					{// �ԍ��ǂݍ���
						// "="�Ǎ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apModelPart[nCntModel]->nIdx);
					}
					if (strcmp(cTemp, "PARENT") == 0)
					{// �e�ԍ��ǂݍ���
						// "="�Ǎ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apModelPart[nCntModel]->nIdxParent);
					}
					if (strcmp(cTemp, "POS") == 0)
					{// �ʒu�ǂݍ���
						// "="�Ǎ���
						(void)fscanf(pFile, "%s", &cTemp[0]);
						
						D3DXVECTOR3 pos = {};

						(void)fscanf(pFile, "%f", &pos.x);
						(void)fscanf(pFile, "%f", &pos.y);
						(void)fscanf(pFile, "%f", &pos.z);
							
						m_apModelPart[nCntModel]->pParts->SetPosition(pos);
						m_apModelPart[nCntModel]->pParts->SetPosOrg(pos);
					}
					if (strcmp(cTemp, "ROT") == 0)
					{//�����ǂݍ���
						// "="�Ǎ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						D3DXVECTOR3 rot = {};

						(void)fscanf(pFile, "%f", &rot.x);
						(void)fscanf(pFile, "%f", &rot.y);
						(void)fscanf(pFile, "%f", &rot.z);

						m_apModelPart[nCntModel]->pParts->SetRotation(rot);
					}
				}
				nCntModel++;
				m_motion.nNumParts++;
			}
		}
	}
}

//=============================================================================
// ���[�V�������̓ǂݍ���
//=============================================================================
void CMotion::LoadMotion(FILE* pFile)
{			
	// �ϐ��錾
	char cTemp[254] = {};
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntPart = 0;

	while (1)
	{
		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			return;
		}

		// ������Ǎ���
		(void)fscanf(pFile, "%s", &cTemp[0]);

		// ���[�V�����ݒ�
		if (strcmp(cTemp, "MOTIONSET") == 0)
		{
			// �C���X�^���X����
			m_aMotionInfo[nCntMotion] = new SMotionInfo;

			while (strcmp(cTemp, "END_MOTIONSET") != 0)
			{
				// ������̓Ǎ���
				(void)fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "LOOP") == 0)
				{// ���[�v���f

					(void)fscanf(pFile, "%s", &cTemp[0]);	// "="�Ǎ���
					(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntMotion]->bLoop);
				}
				if (strcmp(cTemp, "NUM_KEY") == 0)
				{// �L�[��

					(void)fscanf(pFile, "%s", &cTemp[0]);	// "="�Ǎ���
					(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion]->nNumKey);
				}

				if (strcmp(cTemp, "KEYSET") == 0)
				{
					// ������̓Ǎ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// �t���[�����擾
					while (1)
					{
						// ������̓Ǎ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "FRAME") == 0)
						{
							(void)fscanf(pFile, "%s", &cTemp[0]);	// "="�Ǎ���
							(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].fFrame);

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
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
								}
								if (strcmp(cTemp, "ROT") == 0)
								{// �����擾

									(void)fscanf(pFile, "%s", &cTemp[0]);	// "="�Ǎ���
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
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
			m_motion.nNumMotion++;	// ���[�V�������̃J�E���g�A�b�v

			if (m_motion.nNumMotion > motion::MOTION_MAX)
			{
				assert(("���[�V�����̍ő吔�𒴂��܂���", false));
			}
		}
	}
}

//=============================================================================
// �f�o�b�O�\��
//=============================================================================
void CMotion::Debug(void)
{
#ifndef _DEBUG
	return;
#endif

	CManager::GetInstance()->GetDebugProc()->Print("�y���[�V�������z\n");
	CManager::GetInstance()->GetDebugProc()->Print("�t���[�� : %d/%d\n", m_aMotionInfo[m_motion.nType]->aKeyInfo[m_motion.nKey].fFrame);
	CManager::GetInstance()->GetDebugProc()->Print("�p�[�c�� : %d\n", m_motion.nNumParts);
	CManager::GetInstance()->GetDebugProc()->Print("���[�V������ : %d\n", m_motion.nNumMotion);
	CManager::GetInstance()->GetDebugProc()->Print("�L�[�� : %d\n", m_aMotionInfo[m_motion.nType]->nNumKey);
	CManager::GetInstance()->GetDebugProc()->Print("���݃L�[ : %d\n", m_motion.nKey);
	//CManager::GetInstance()->GetDebugProc()->Print("���̃L�[ : %d\n", nNextKey);
}
