//*****************************************************
//
// �p�[�c�̏���[parts.cpp]
// Author : Takeru Ogasawara
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "modelParts.h"

#include "debugproc.h"

//====================================================
// �R���X�g���N�^
//====================================================
CModelParts::CModelParts()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_IdxModelParts = -1;
	m_fRadius = 0.0f;
	m_bChangeCol = false;
	m_col = { 0.0f,0.0f,0.0f,0.0f };
	m_pModelParts = nullptr;
	ZeroMemory(&m_mtxWorld, sizeof(m_mtxWorld));
}

//====================================================
// �f�X�g���N�^
//====================================================
CModelParts::~CModelParts()
{

}

//=====================================================
// ��������
//=====================================================
CModelParts* CModelParts::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModelParts* pParts = nullptr;

	if (pParts == nullptr)
	{
		// �C���X�^���X����
		pParts = new CModelParts;

		pParts->m_pos = pos;
		pParts->m_rot = rot;

		// ����������
		pParts->Init();
	}

	return pParts;
}

//====================================================
// ����������
//====================================================
HRESULT CModelParts::Init(void)
{
	return S_OK;
}

//====================================================
// �I������
//====================================================
void CModelParts::Uninit(void)
{
	if (m_pModelParts != nullptr)
	{
		m_pModelParts = nullptr;
	}

	delete this;
}

//====================================================
// �X�V����
//====================================================
void CModelParts::Update(void)
{
	// �ړ��ʂ����Z
	m_pos += m_move;
}

//====================================================
// �`�揈��
//====================================================
void CModelParts::Draw(void)
{
	if (m_pModelParts == nullptr)
	{
		return;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATERIAL* pMat;	// �}�e���A���f�[�^�ւ̃|�C���^
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p
	LPDIRECT3DTEXTURE9 pTexture = nullptr;
	int nIdxTex = 0;

	// ���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModelParts->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelParts->dwNumMat; nCntMat++)
	{
		// �}�e���A���̕ۑ�
		matDef = pMat[nCntMat].MatD3D;

		if (m_bChangeCol)
		{
			// �F�̐ݒ�
			pMat[nCntMat].MatD3D.Diffuse = m_col;
		}

		// �}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		nIdxTex = m_pModelParts->pIdxTexture[nCntMat];

		// �e�N�X�`���̎擾
		pTexture = CTexture::GetInstance()->GetAddress(nIdxTex);
		
		// �e�N�X�`���ݒ�
		pDevice->SetTexture(0, pTexture);
		
		// ���f���i�p�[�c�j�`��
		m_pModelParts->pMesh->DrawSubset(nCntMat);

		// �F��߂�
		pMat[nCntMat].MatD3D = matDef;

		// �}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//====================================================
// �e�p�̕`�揈��
//====================================================
void CModelParts::DrawShadow(void)
{
	if (m_pModelParts == nullptr)
	{
		return;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATERIAL* pMat;	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	LPDIRECT3DTEXTURE9 pTexture = nullptr;	// �e�N�X�`���|�C���^

	// ���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModelParts->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelParts->dwNumMat; nCntMat++)
	{
		// �}�e���A���̕ۑ�
		matDef = pMat[nCntMat].MatD3D;

		// �F�̐ݒ�
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// �}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̎擾
		CTexture::GetInstance()->GetAddress(m_pModelParts->pIdxTexture[nCntMat]);

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, pTexture);

		//���f���i�p�[�c�j�`��
		m_pModelParts->pMesh->DrawSubset(nCntMat);

		// �F��߂�
		pMat[nCntMat].MatD3D = matDef;

		// �}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//====================================================
// �c���̕`�揈��
//====================================================
void CModelParts::DrawAfterImage(void)
{
	if (m_pModelParts == nullptr)
	{
		return;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATERIAL* pMat;	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	LPDIRECT3DTEXTURE9 pTexture = nullptr;	// �e�N�X�`���|�C���^

	// ���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModelParts->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelParts->dwNumMat; nCntMat++)
	{
		// �}�e���A���̕ۑ�
		matDef = pMat[nCntMat].MatD3D;

		// �}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̎擾
		pTexture = CTexture::GetInstance()->GetAddress(m_pModelParts->pIdxTexture[nCntMat]);

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, pTexture);

		//���f���i�p�[�c�j�`��
		m_pModelParts->pMesh->DrawSubset(nCntMat);

		// �F��߂�
		pMat[nCntMat].MatD3D = matDef;

		// �}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//=====================================================
// �}�g���b�N�X�ݒ菈��
//=====================================================
void CModelParts::SetMatrix(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=====================================================
// ���f���̍Œ����_�擾
//=====================================================
void CModelParts::SetRadius(void)
{
	// �v�Z�p�ϐ�
	int nNumVtx;			// ���_��
	DWORD dwSizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	float fLength = 0.0f;

	// ���_���̎擾
	nNumVtx = m_pModelParts->pMesh->GetNumVertices();

	// �t�H�[�}�b�g�T�C�Y����
	dwSizeFVF = D3DXGetFVFVertexSize(m_pModelParts->pMesh->GetFVF());

	// ���_�o�b�t�@�̃��b�N
	m_pModelParts->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (D3DXVec3Length(&vtx) > fLength)
		{
			fLength = D3DXVec3Length(&vtx);
		}

		pVtxBuff += dwSizeFVF;
	}

	// ���_�o�b�t�@�̃A�����b�N
	m_pModelParts->pMesh->UnlockVertexBuffer();

	// ��������
	m_fRadius = fLength;
}

//=====================================================
// ���f���̊��蓖��
//=====================================================
void CModelParts::BindModel(int nIdx)
{
	// ���f�����蓖��
	m_pModelParts = CXfile::GetInstance()->GetXfile(nIdx);
}