//===========================================================================================
// 
// [texture.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <stdlib.h>
#include "xfile.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//********************************************************************
// �ÓI�����o�ϐ��錾
//********************************************************************
CXfile* CXfile::m_pXfile = nullptr;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CXfile::CXfile()
{
	// �l�̃N���A
	ZeroMemory(&m_apXFile[0], sizeof(m_apXFile));
	m_nNumAll = 0;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CXfile::~CXfile()
{

}

//===========================================================================================
// ����
//===========================================================================================
CXfile* CXfile::Create(void)
{
	if (m_pXfile == nullptr)
	{
		m_pXfile = new CXfile;

		m_pXfile->InitLoad();

		return m_pXfile;
	}

	return m_pXfile;
}

//===========================================================================================
//	�C���X�^���X�̎擾
//===========================================================================================
CXfile* CXfile::GetInstance(void)
{
	if (m_pXfile == nullptr)
	{
		assert(("x�t�@�C�����̎擾�Ɏ��s", false));
	}

	return m_pXfile;
}

//===========================================================================================
// �C���X�^���X�̔j��
//===========================================================================================
void CXfile::Release(void)
{
	if (m_pXfile == nullptr)
	{
		return;
	}
	else
	{
		m_pXfile->Unload();

		delete m_pXfile;
		m_pXfile = nullptr;
	}
}


//===========================================================================================
// X�t�@�C����ǂݍ���(����������)
//===========================================================================================
void CXfile::InitLoad(void)
{
	// �ŏ��ɓǂݍ���x�t�@�C��
	const char *aFileList[xfile::MAX_FILE] =
	{
		
	};

	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (aFileList[i] != nullptr)
		{
			// ���f�����̓o�^
			Load(aFileList[i]);
		}
		else
		{
			break;
		}
	}
}

//===========================================================================================
// �S�Ĕj������(�I������)
//===========================================================================================
void CXfile::Unload(void)
{
	for (int i = 0; i < xfile::MAX_FILE; i++)
	{
		if (m_apXFile[i] != nullptr)
		{
			//�t�@�C�������N���A
			ZeroMemory(&m_apXFile[i]->aXfileName, sizeof(m_apXFile[i]->aXfileName));
			
			if (m_apXFile[i]->pBuffMat != nullptr)
			{
				m_apXFile[i]->pBuffMat->Release();
				m_apXFile[i]->pBuffMat = nullptr;
			}

			if (m_apXFile[i]->pMesh != nullptr)
			{
				m_apXFile[i]->pMesh->Release();
				m_apXFile[i]->pMesh = nullptr;
			}

			if (m_apXFile[i]->pIdxTexture != nullptr)
			{
				delete m_apXFile[i]->pIdxTexture;
				m_apXFile[i]->pIdxTexture = nullptr;
			}

			// �������̉��
			delete m_apXFile[i];
			m_apXFile[i] = nullptr;

			m_nNumAll--;
		}
	}
}

//===========================================================================================
// X�t�@�C���Ɋ��蓖�Ă�ꂽ�ԍ���Ԃ�
//===========================================================================================
int CXfile::Load(const char *pXfilePath)
{
	for (int i = 0; i < xfile::MAX_FILE; i++)
	{
		if (m_apXFile[i] != nullptr)
		{
			if (strcmp(&m_apXFile[i]->aXfileName[0], pXfilePath) == 0)
			{// �o�^���ꂽ�t�@�C�����ƈ����̃t�@�C�����������������ꍇ

				return i;	// �ԍ���Ԃ�
			}

			continue;
		}

		// ���̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
		D3DXMATERIAL* pMat;	// �}�e���A���|�C���^

		// ����
		m_apXFile[i] = new SXFile;

		// X�t�@�C�����̏�����
		ZeroMemory(m_apXFile[i], sizeof(SXFile));

		//  X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX
		(
			pXfilePath,	// �t�@�C����
			D3DXMESH_SYSTEMMEM, pDevice,
			NULL,
			&m_apXFile[i]->pBuffMat,	// �}�e���A���f�[�^
			NULL,
			&m_apXFile[i]->dwNumMat,	// �}�e���A����
			&m_apXFile[i]->pMesh)))	// ���b�V���f�[�^
		{
			return E_FAIL;	// �ǂݍ��ݎ��s
		}

		// �K�v�����e�N�X�`���̃|�C���^���m�ۂ���
		m_apXFile[i]->pIdxTexture = new int[m_apXFile[i]->dwNumMat];

		// �}�e���A���ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_apXFile[i]->pBuffMat->GetBufferPointer();

		for (int nCntIdxMat = 0; nCntIdxMat < (int)m_apXFile[i]->dwNumMat; nCntIdxMat++)
		{
			if (pMat[nCntIdxMat].pTextureFilename != nullptr)
			{
				// �t�@�C������e�N�X�`����ǂݍ���
				m_apXFile[i]->pIdxTexture[nCntIdxMat] = CTexture::GetInstance()->Regist(pMat[nCntIdxMat].pTextureFilename);
			}
			else
			{
  				m_apXFile[i]->pIdxTexture[nCntIdxMat] = -1;
			}
		}

		// �e�N�X�`��������������
		strcpy(&m_apXFile[i]->aXfileName[0], pXfilePath);

		// �I�u�W�F�N�g�T�C�Y���Z�o����֐�
		VtxMaxMin(i);

		m_nNumAll++;	// �����J�E���g�A�b�v

		return i;
	}

	return -1;
}

//===========================================================================================
// �I�u�W�F�N�g�̃T�C�Y���Z�o
//===========================================================================================
void CXfile::VtxMaxMin(int nNumCnt)
{
	DWORD dwSizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	int nNumVtx;	// ���_��

	// ���_�����擾
	nNumVtx = m_apXFile[nNumCnt]->pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(m_apXFile[nNumCnt]->pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_apXFile[nNumCnt]->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

		if (m_apXFile[nNumCnt]->vtxMax.x < vtx.x)
		{// x���傫��������
			m_apXFile[nNumCnt]->vtxMax.x = vtx.x;
		}
		if (m_apXFile[nNumCnt]->vtxMax.z < vtx.z)
		{// z�̒l���傫��������
			m_apXFile[nNumCnt]->vtxMax.z = vtx.z;
		}

		if (m_apXFile[nNumCnt]->vtxMin.x > vtx.x)
		{// x��������������
			m_apXFile[nNumCnt]->vtxMin.x = vtx.x;
		}
		if (m_apXFile[nNumCnt]->vtxMin.z > vtx.z)
		{// z�̒l���傫��������
			m_apXFile[nNumCnt]->vtxMin.z = vtx.z;
		}

		if (m_apXFile[nNumCnt]->vtxMax.y < vtx.y)
		{// x���傫��������
			m_apXFile[nNumCnt]->vtxMax.y = vtx.y;
		}
		if (m_apXFile[nNumCnt]->vtxMin.y > vtx.y)
		{// x��������������
			m_apXFile[nNumCnt]->vtxMin.y = vtx.y;
		}

		pVtxBuff += dwSizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}
	
	// ���_�o�b�t�@���A�����b�N
	m_apXFile[nNumCnt]->pMesh->UnlockVertexBuffer();
}

//===========================================================================================
// �A�h���X�̎擾
//===========================================================================================
CXfile::SXFile* CXfile::GetXfile(int nIdx)
{
	if (nIdx < 0 && nIdx >= m_nNumAll)
	{
		return nullptr;
	}

	return m_apXFile[nIdx];
}

//===========================================================================================
// ���b�V���̎擾
//===========================================================================================
LPD3DXMESH CXfile::GetMesh(int nIdx)
{
	if (nIdx > m_nNumAll || nIdx < 0)
	{
		assert(("���f���̃��b�V��������܂���", false));
	}
	return m_apXFile[nIdx]->pMesh;
}

//===========================================================================================
// �o�b�t�@�[�̎擾
//===========================================================================================
LPD3DXBUFFER CXfile::GetBuffMat(int nIdx)
{
	if (nIdx > m_nNumAll || nIdx < 0)
	{
		assert(("���f���̃o�b�t�@������܂���", false));
	}

	return m_apXFile[nIdx]->pBuffMat;
}

//===========================================================================================
// �}�e���A���̐��擾
//===========================================================================================
DWORD CXfile::GetDwNumMat(int nIdx)
{
	if (nIdx > m_nNumAll || nIdx < 0)
	{
		assert(("���f���̃��b�V��������܂���", false));
	}
	return m_apXFile[nIdx]->dwNumMat;
}

