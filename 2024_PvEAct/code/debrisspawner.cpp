////*****************************************************
////
//// ��юU��j�Џ���[debrisSpawner.cpp]
//// Author:���}���V
////
////*****************************************************
//
////*****************************************************
//// �C���N���[�h
////*****************************************************
//#include "debris.h"
//#include "debrisSpawner.h"
//#include "collision.h"
//#include <stdio.h>
//
////*****************************************************
//// �ÓI�����o�ϐ��錾
////*****************************************************
//CDebrisSpawner::PARTICLE_INFO* CDebrisSpawner::m_apDebris[CDebrisSpawner::TYPE_MAX + 1] = {};
//
////=====================================================
//// �D�揇�ʂ����߂�R���X�g���N�^
////=====================================================
//CDebrisSpawner::CDebrisSpawner(int nPriority) : CObject(nPriority)
//{
//	m_pos = { 0.0f,0.0f,0.0f };
//	m_pPosOwner = nullptr;
//	m_rot = { 0.0f,0.0f,0.0f };
//	m_type = TYPE_NONE;
//	m_nPriorityDebris = 0;
//	m_nLife = 0;
//	m_ModelIdx = 0;
//
//	SetType(TYPE_PARTICLE);
//}
//
////=====================================================
////	�f�X�g���N�^
////=====================================================
//CDebrisSpawner::~CDebrisSpawner()
//{
//
//}
//
////=====================================================
////	����������
////=====================================================
//HRESULT CDebrisSpawner::Init(void)
//{
//	Load();
//
//	return S_OK;
//}
//
////=====================================================
////	�I������
////=====================================================
//void CDebrisSpawner::Uninit(void)
//{
//	Release();
//}
//
////=====================================================
////	�j������
////=====================================================
//void CDebrisSpawner::Unload(void)
//{
//	for (int nCntDebris = 0; nCntDebris < TYPE_MAX; nCntDebris++)
//	{
//		if (m_apDebris[nCntDebris] != nullptr)
//		{
//			delete m_apDebris[nCntDebris];
//			m_apDebris[nCntDebris] = nullptr;
//		}
//	}
//}
//
////=====================================================
////	�ǂ̔j��
////=====================================================
//void CDebrisSpawner::DebrisWall(void)
//{
//}
//
////=====================================================
////	�X�V����
////=====================================================
//void CDebrisSpawner::Update(void)
//{
//	// �ϐ��錾
//	float fRot, fRot2;
//	float fMove = 0.0f;
//	float fRadius = 0.0f;
//	int nLife = 0;
//	int nModelIdx = 0;
//	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };
//	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
//	CObjectX* pObjectX = nullptr;
//
//	for (int nCntEffect = 0; nCntEffect < m_apDebris[m_type]->nNumDebris; nCntEffect++)
//	{
//		if (m_apDebris[m_type]->nRot)
//		{// �����̔��f
//			if (m_apDebris[m_type]->fRangeRot > 0.1f)
//			{
//				fRot = m_rot.x + (float)(rand() % (int)(m_apDebris[m_type]->fRangeRot * 100.0f) - m_apDebris[m_type]->fRangeRot * 50.0f) / 100.0f;
//				fRot2 = m_rot.y + (float)(rand() % (int)(m_apDebris[m_type]->fRangeRot * 100.0f) - m_apDebris[m_type]->fRangeRot * 50.0f) / 100.0f * 2;
//			}
//			else
//			{
//				fRot = m_rot.x + (rand() % (int)(0.1f * 100.0f) - 0.1f * 50.0f) / 100.0f;
//				fRot2 = m_rot.y + (rand() % (int)(0.1f * 100.0f) - 0.1f * 50.0f) / 100.0f * 2;
//			}
//		}
//		else
//		{// �S����
//			fRot = rand() % 628 - 314 / 100.0f;
//			fRot2 = rand() % 628 - 314 / 100.0f;
//		}
//
//		if (m_apDebris[m_type]->fSpeed > 0.1f)
//		{
//			fMove = (float)(rand() % (int)(m_apDebris[m_type]->fSpeed * 10)) * 0.1f + m_apDebris[m_type]->fSpeed * 0.5f;
//		}
//		if (m_apDebris[m_type]->nLifeDebris != 0)
//		{
//			nLife = rand() % m_apDebris[m_type]->nLifeDebris + m_apDebris[m_type]->nLifeDebris / 2;
//		}
//
//		move.x = sinf(fRot) * sinf(fRot2) * fMove;
//		move.y = cosf(fRot) * fMove;
//		move.z = sinf(fRot) * cosf(fRot2) * fMove;
//
//		rot.x = (float)(rand() % 629 - 314) / 100.0f;
//		rot.y = (float)(rand() % 629 - 314) / 100.0f;
//		rot.z = (float)(rand() % 629 - 314) / 100.0f;
//
//		// �G�t�F�N�g����
//		pObjectX = CDebris::Create(m_pos, nLife, m_apDebris[m_type]->nModelIdx , move, m_apDebris[m_type]->fGravity, m_apDebris[m_type]->bBounce);
//		pObjectX->SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));
//	}
//
//	m_nLife--;
//
//	if (m_nLife < 0)
//	{
//		// ���g�̔j��
//		Uninit();
//	}
//}
//
////=====================================================
////	�ʒu�ݒ菈��
////=====================================================
//void CDebrisSpawner::SetPosition(D3DXVECTOR3 pos)
//{
//	m_pos = pos;
//}
//
////=====================================================
////	��������
////=====================================================
//CDebrisSpawner* CDebrisSpawner::Create(D3DXVECTOR3 pos, TYPE type, D3DXVECTOR3 rot, int nPriority)
//{
//	CDebrisSpawner* pParticle = nullptr;
//
//	if (pParticle == nullptr)
//	{// �C���X�^���X����
//		pParticle = new CDebrisSpawner;
//
//		pParticle->Init();
//
//		pParticle->m_type = type;
//
//		pParticle->m_pos = pos;
//
//		pParticle->m_rot = rot;
//
//		pParticle->m_nLife = m_apDebris[type]->nLife;
//
//		pParticle->m_nPriorityDebris = nPriority;
//	}
//
//	return pParticle;
//}
//
////=====================================================
////	�Ǎ�����
////=====================================================
//void CDebrisSpawner::Load(void)
//{
//	// �ϐ��錾
//	char cTemp[256];
//	int nCntDebris = 1;
//	PARTICLE_INFO* pInfo = nullptr;
//
//	// �t�@�C������ǂݍ���
//	FILE* pFile = fopen("data\\TEXT\\debris.txt", "r");
//
//	if (pFile != nullptr)
//	{// �t�@�C�����J�����ꍇ
//		while (true)
//		{
//			// �����ǂݍ���
//			(void)fscanf(pFile, "%s", &cTemp[0]);
//
//			if (strcmp(cTemp, "DEBRISSET") == 0)
//			{// �Ǎ��J�n
//				if (m_apDebris[nCntDebris] == nullptr)
//				{
//					// �C���X�^���X����
//					m_apDebris[nCntDebris] = new PARTICLE_INFO;
//				}
//
//				pInfo = m_apDebris[nCntDebris];
//
//				while (true)
//				{
//					// �����ǂݍ���
//					(void)fscanf(pFile, "%s", &cTemp[0]);
//
//					if (strcmp(cTemp, "END_DEBRISSET") == 0)
//					{// �p�[�e�B�N�����I������
//						break;
//					}
//
//					if (strcmp(cTemp, "MODEL_TYPE") == 0)
//					{// �p�[�e�B�N�������擾
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", &pInfo->nModelIdx);
//					}
//
//					if (strcmp(cTemp, "LIFE_DEBRISSPAWNER") == 0)
//					{// �p�[�e�B�N�������擾
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", &pInfo->nLife);
//					}
//
//					if (strcmp(cTemp, "NUM_DEBRIS") == 0)
//					{// �G�t�F�N�g���擾
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", &pInfo->nNumDebris);
//					}
//
//					if (strcmp(cTemp, "LIFE_DEBRIS") == 0)
//					{// �G�t�F�N�g�����擾
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", &pInfo->nLifeDebris);
//					}
//
//					if (strcmp(cTemp, "SPEED_DEBRIS") == 0)
//					{// �G�t�F�N�g���x�擾
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%f", &pInfo->fSpeed);
//					}
//
//					if (strcmp(cTemp, "GRAVITY") == 0)
//					{// �d�͎擾
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%f", &pInfo->fGravity);
//					}
//
//					if (strcmp(cTemp, "RANGEROT") == 0)
//					{// �����̃����_�����擾
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%f", &pInfo->fRangeRot);
//					}
//
//					if (strcmp(cTemp, "IS_ROT") == 0)
//					{// ���Z�������ǂ����擾
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%d", &pInfo->nRot);
//					}
//
//					if (strcmp(cTemp, "IS_TURN") == 0)
//					{// ���Z�������ǂ����擾
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						if (strcmp(cTemp, "1") == 0)
//						{
//							pInfo->bTurn = true;
//						}
//						else
//						{
//							pInfo->bTurn = false;
//						}
//					}
//					if (strcmp(cTemp, "IS_BAUNCE") == 0)
//					{// ���Z�������ǂ����擾
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						(void)fscanf(pFile, "%s", &cTemp[0]);
//
//						if (strcmp(cTemp, "1") == 0)
//						{
//							pInfo->bBounce = true;
//						}
//						else
//						{
//							pInfo->bBounce = false;
//						}
//					}
//				}
//
//				// �p�[�e�B�N�������Z
//				nCntDebris++;
//			}
//
//			if (strcmp(cTemp, "END_SCRIPT") == 0)
//			{// �I������
//				break;
//			}
//		}
//
//		// �t�@�C�������
//		fclose(pFile);
//	}
//	else
//	{
//		assert(("�p�[�e�B�N�����ǂݍ��݂Ɏ��s", false));
//	}
//}