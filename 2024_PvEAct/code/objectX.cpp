//===========================================================================================
//
// [objectX.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "object.h"
#include "general.h"

#define SIZE (20.0f)

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pXfile = nullptr;
	m_nModelIdx = 0;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CObjectX::~CObjectX()
{

}

//===========================================================================================
// ����
//===========================================================================================
CObjectX * CObjectX::Create(void)
{
	CObjectX *pObjX = nullptr;

	if (pObjX == nullptr)
	{
		pObjX = new CObjectX;

		if (pObjX != nullptr)
		{
			pObjX->Init();

			return pObjX;
		}
	}

	return pObjX;
}

//===========================================================================================
// ����
//===========================================================================================
CObjectX *CObjectX::Create(const char *c_pFileName, D3DXVECTOR3 pos)
{
	CObjectX *pObjX = nullptr;

	if (pObjX == nullptr)
	{
		pObjX = new CObjectX;

		if (pObjX != nullptr)
		{
			// ����������
			pObjX->Init(c_pFileName, pos);

			// �ʒu�ݒ�
			pObjX->SetPosition(pos);

			// ��ސݒ�
			pObjX->SetType(TYPE_OBJECTX);

			return pObjX;
		}
		else
		{
			return NULL;
		}
	}

	return pObjX;
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CObjectX::Init(void)
{
	// ���蓖��
	m_nModelIdx = CXfile::GetInstance()->Load("data\\MODEL\\object\\block00.x");
	
	// �T�C�Y
	m_scale = { 1.0f, 1.0f, 1.0f };

	return S_OK;
}

//===========================================================================================
// ����������(�I�[�o�[���[�h)
//===========================================================================================
HRESULT CObjectX::Init(const char *c_pFileName, D3DXVECTOR3 pos)
{
	// �e�N�X�`���̊��蓖��
	int nIdx = CXfile::GetInstance()->Load(c_pFileName);

	// �������蓖�Ă�
	BindXfile(nIdx);
	
	// �T�C�Y
	m_scale = { 1.0f, 1.0f, 1.0f };

	// �ʒu
	m_pos = pos;

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CObjectX::Uninit(void)
{
	// ���g�̔j��
	Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CObjectX::Update(void)
{
	m_pos = GetPosition();
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CObjectX::Draw(void)
{
	if (m_pXfile == nullptr)
	{
		return;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;	// �}�e���A���f�[�^�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	//// ���[���h�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&m_mtxWorld);

	//// �X�P�[���̔��f
	//D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//// �����𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//// �ʒu�𔽉f
	//D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//// ���[���h�}�g���b�N�X�̐ݒ�
	//pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �}�g���b�N�X�̐ݒ�
	general::SetMatrix(&m_mtxWorld, m_pos, m_rot);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pXfile->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pXfile->dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̎擾
		pTexture = CTexture::GetInstance()->GetAddress(m_pXfile->pIdxTexture[nCntMat]);
		
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture);

		// �I�u�W�F�N�g(�p�[�c)�̕`��
		m_pXfile->pMesh->DrawSubset(nCntMat);
	}

	// �ۑ�����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===========================================================================================
// �`���p
//===========================================================================================
void CObjectX::DrawOnly(void)
{
	if (m_pXfile == nullptr)
	{
		return;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;	// �}�e���A���f�[�^�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pXfile->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pXfile->dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̎擾
		pTexture = CTexture::GetInstance()->GetAddress(m_pXfile->pIdxTexture[nCntMat]);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture);

		// �I�u�W�F�N�g(�p�[�c)�̕`��
		m_pXfile->pMesh->DrawSubset(nCntMat);
	}
}

//===========================================================================================
// ���f���̐ݒ�
//===========================================================================================
void CObjectX::SetModel(const char* cPath)
{
	if (m_pXfile != nullptr)
	{
		m_nModelIdx = CTexture::GetInstance()->Regist(cPath);
	}
}

//===========================================================================================
// �ő�l
//===========================================================================================
D3DXVECTOR3 CObjectX::GetVtxMax(void) const
{
	if (m_pXfile != nullptr)
	{
		return m_pXfile->vtxMax;
	}

	return D3DXVECTOR3();
}

//===========================================================================================
// �ŏ��l
//===========================================================================================
D3DXVECTOR3 CObjectX::GetVtxMin(void) const
{
	if (m_pXfile != nullptr)
	{
		return m_pXfile->vtxMin;
	}

	return D3DXVECTOR3();
}

//===========================================================================================
// X�t�@�C�����̎擾
//===========================================================================================
//CXfile* CObjectX::GetXfile(void)
//{
//	if (m_pXfile == nullptr)
//	{
//		return nullptr;
//	}
//
//	return m_pXfile;
//}
