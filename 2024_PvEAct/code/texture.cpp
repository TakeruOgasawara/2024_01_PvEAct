//===========================================================================================
// 
// [texture.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <stdlib.h>
#include "texture.h"
#include "renderer.h"
#include "manager.h"

// �ÓI�����o�ϐ��錾
CTexture* CTexture::m_pTexture = nullptr;
int CTexture::m_nNumAll = 0;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CTexture::CTexture()
{
	ZeroMemory(&m_aData, sizeof(m_aData));
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CTexture::~CTexture()
{

}

//===========================================================================================
// ����
//===========================================================================================
CTexture* CTexture::Create(void)
{
	if (m_pTexture == nullptr)
	{
		m_pTexture = new CTexture;

		m_pTexture->Load();

		return m_pTexture;
	}

	return m_pTexture;
}

//===========================================================================================
//	�C���X�^���X�̎擾
//===========================================================================================
CTexture* CTexture::GetInstance(void)
{
	if (m_pTexture == nullptr)
	{
		assert(("�e�N�X�`�����̎擾�Ɏ��s", false));
	}

	return m_pTexture;
}

//===========================================================================================
// �C���X�^���X�̔j��
//===========================================================================================
void CTexture::Release(void)
{
	if (m_pTexture == nullptr)
	{
		return;
	}
	else
	{
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

//===========================================================================================
// �e�N�X�`����S�ēǂݍ���(����������)
//===========================================================================================
HRESULT CTexture::Load(void)
{
	//// �t�@�C����
	//const char *c_apTexName[MAX_TEXTURE] =
	//{
	//	"",
	//};

	//// �f�o�C�X�̎擾
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	//{
	//	if (c_apTexName[nCntTex] != nullptr)
	//	{
	//		Regist(c_apTexName[nCntTex]);
	//	}
	//	else
	//	{
	//		return NULL;
	//	}
	//}

	return S_OK;
}

//===========================================================================================
// �e�N�X�`����S�Ĕj������(�I������)
//===========================================================================================
void CTexture::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		if (m_aData[nCntTex].pTexture != nullptr)
		{
			m_aData[nCntTex].pTexture->Release();
			m_aData[nCntTex].pTexture = nullptr;
		}	
		if (m_aData[nCntTex].aName != "")
		{	
			ZeroMemory(&m_aData[nCntTex].aName[0], sizeof(m_aData[nCntTex].aName));
		}
	}
}

//===========================================================================================
// �e�N�X�`���̊��蓖��
//===========================================================================================
int CTexture::Regist(const char * pTexPath)
{	
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{// �e�N�X�`���̍ő吔���܂��

		if (m_aData[nCntTex].pTexture != nullptr && m_aData[nCntTex].aName != "")
		{
			if (strcmp(&m_aData[nCntTex].aName[0], pTexPath) == 0)
			{// �ۑ����ꂽ�t�@�C�����ƈ����̃t�@�C�����������������ꍇ
				return nCntTex;	//�ԍ���Ԃ�
			}
		}
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	
	// �e�N�X�`���̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(pDevice, pTexPath, &m_aData[m_nNumAll].pTexture)))
	{
		return -1;
	}

	// �e�N�X�`��������������
	strcpy(&m_aData[m_nNumAll].aName[0], pTexPath);

	int nId = m_nNumAll;

	m_nNumAll++;	// �e�N�X�`�������̃J�E���g�A�b�v

	return nId;
}

//===========================================================================================
// �e�N�X�`���ԍ�
//===========================================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (m_aData[nIdx].pTexture == nullptr)
	{
		return nullptr;
	}

	return m_aData[nIdx].pTexture;
}
