//===========================================================================================
//
// [mapEdit.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "mapEdit.h"
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "object.h"
#include "xfile.h"
#include "inputKeyboard.h"
#include "debugproc.h"

// �萔��`
namespace
{
	// x�t�@�C���p�X
	const char* apPathList[CMapEdit::TYPE_MAX] =
	{
		"data\\MODEL\\object\\TanukiShop.x"
	};

	const float SPEED = 20.0f;
}


//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CMapEdit::CMapEdit(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pXfile = nullptr;
	m_nRegistModelIdx = 0;
	m_nTypeIdx = 0;
	m_bUse = false;
	m_type = TYPE_HOUSE;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CMapEdit::~CMapEdit()
{

}

//===========================================================================================
// ����
//===========================================================================================
CMapEdit* CMapEdit::Create(void)
{
	CMapEdit* pObjX = nullptr;

	if (pObjX == nullptr)
	{
		pObjX = new CMapEdit;

		if (pObjX != NULL)
		{
			pObjX->Init();

			return pObjX;
		}
	}

	return pObjX;
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CMapEdit::Init(void)
{
	// �I�����f���ԍ��̐ݒ�
	m_nTypeIdx = TYPE_HOUSE;

	// 
	m_type = (EType)m_nTypeIdx;

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CMapEdit::Uninit(void)
{
	Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CMapEdit::Update(void)
{
#ifdef _DEBUG	//�f�o�b�N�����s

	//�L�[�{�[�h���̎擾
	CInputKeyboard* pInputKey = CInputKeyboard::GetInstance();
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CXfile* pXfile = CXfile::GetInstance();

	// �G�f�B�b�g���g�����ǂ���
	if (pInputKey->GetTrigger(DIK_F3))
	{
		m_bUse = m_bUse ? false : true;
	}

	if (m_bUse == false )
	{
		return;
	}

	// �f�o�b�O�\��
	pDebug->Print("\n\n�y�G�f�B�b�g���[�h���z\n");
	pDebug->Print("�ʒu�F x:%f y:%f z:%f\n", m_pos.x, m_pos.y, m_pos.z);
	pDebug->Print("�����F x:%f y:%f z:%f\n", m_rot.x, m_rot.y, m_rot.z);
	pDebug->Print("��ށF %d\n", m_type);

	// ����
	Operation();

	// �ʒu�̔��f
	SetPosition(m_pos);

#endif
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CMapEdit::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//LPDIRECT3DTEXTURE9 pTexture;
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;	//�}�e���A���f�[�^�ւ̃|�C���^

	if (m_pXfile->GetMesh(m_nRegistModelIdx) == nullptr)
	{
		return;
	}

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���̔��f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pXfile->GetBuffMat(m_nRegistModelIdx)->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)m_pXfile->GetDwNumMat(m_nRegistModelIdx); nCntMat++)
	//{
	//	// �}�e���A���̐ݒ�
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	// �e�N�X�`���ԍ��̎擾
	//	pTexture = CTexture::GetInstance()->GetAddress(m_pXfile->p[nCntMat]);

	//	// �e�N�X�`���̐ݒ�
	//	pDevice->SetTexture(0, CTexture::GetInstance()->GetAddress(pTexture));

	//	// �I�u�W�F�N�g(�p�[�c)�̕`��
	//	m_pXfile->GetMesh(m_nRegistModelIdx)->DrawSubset(nCntMat);
	//}

	// �ۑ�����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===========================================================================================
// ����
//===========================================================================================
void CMapEdit::Operation(void)
{
	//�L�[�{�[�h���̎擾
	CInputKeyboard* pInputKey = CInputKeyboard::GetInstance();
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CXfile* pXfile = CXfile::GetInstance();

	//�ړ�
	if (pInputKey->GetTrigger(DIK_UP))
	{
		m_pos.y += SPEED;
	}
	if (pInputKey->GetTrigger(DIK_DOWN))
	{
		m_pos.y -= SPEED;
	}
	if (pInputKey->GetTrigger(DIK_LEFT))
	{
		m_pos.x -= SPEED;
	}
	if (pInputKey->GetTrigger(DIK_RIGHT))
	{
		m_pos.x += SPEED;
	}

	if (pInputKey->GetTrigger(DIK_LSHIFT))
	{
		m_pos.z += SPEED;
	}

	if (pInputKey->GetTrigger(DIK_LCONTROL))
	{
		m_pos.z -= SPEED;
	}

	// �I�u�W�F�N�g��ނ̕ύX
	if (pInputKey->GetTrigger(DIK_1))
	{
		m_nTypeIdx++;

		m_nTypeIdx %= TYPE_MAX;			//�J��Ԃ�
	}

	//�I�u�W�F�N�g�̐ݒu
	if (pInputKey->GetTrigger(DIK_RETURN))
	{

	}

	//�t�@�C����������(�Z�[�u)
	if (pInputKey->GetTrigger(DIK_F9))
	{
		//Save();
	}
}
