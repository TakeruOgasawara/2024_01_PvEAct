//===========================================================================================
//
// [billboard.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "billboard.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "texture.h"

//�}�N����`
#define SIZE			(10.0f)

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CBillboard::CBillboard(int nPriority) : CObject(nPriority)/*, m_pTexture(nullptr)*/			//���ꏈ�������������炵��
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
	m_fLength = 0.0f;
	m_bUse = false;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CBillboard::~CBillboard()
{
	
}

//===========================================================================================
// �I�u�W�F�N�g3D�̐���
//===========================================================================================
CBillboard *CBillboard::Create(void)
{
	CBillboard *pBillboard = nullptr;

	if (pBillboard == nullptr)
	{
		pBillboard = new CBillboard;

		if (pBillboard != nullptr)
		{
			pBillboard->Init();

			return pBillboard;
		}
	}

	return pBillboard;
}

//===========================================================================================
// �I�u�W�F�N�g3D�̐���
//===========================================================================================
CBillboard *CBillboard::Create(D3DXVECTOR3 pos)
{
	CBillboard *pBillboard = nullptr;

	if (pBillboard == nullptr)
	{
		pBillboard = new CBillboard;

		if (pBillboard != nullptr)
		{
			pBillboard->Init();

			pBillboard->SetPosition(pos);
			pBillboard->m_pos = pos;

			return pBillboard;
		}
		else
		{
			return nullptr;
		}
	}

	return pBillboard;
}

//===========================================================================================
// ����
//===========================================================================================
CBillboard *CBillboard::Create(const char* pTexPath, D3DXVECTOR3 pos)
{
	CBillboard *pBillboard = nullptr;
	CTexture* pTexture = CTexture::GetInstance();;
	int nIdxTex = 0;

	if (pBillboard == nullptr)
	{
		pBillboard = new CBillboard;

		if (pBillboard != nullptr)
		{
			pBillboard->Init();

			pBillboard->SetPosition(pos);

			if (pTexture != nullptr)
			{
				nIdxTex = pTexture->Regist(pTexPath);
			}
			
			pBillboard->BindTexture(nIdxTex);

			return pBillboard;
		}
		else
		{
			return nullptr;
		}
	}

	return pBillboard;
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CBillboard::Init(void)
{
	//�f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	if (m_pVtxBuff == nullptr)
	{
		return S_FALSE;
	}

	//���_�ݒ�
	SetverTex();

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CBillboard::Uninit(void)
{
	//�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�j��
	Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CBillboard::Update(void)
{

}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CBillboard::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	CTexture* pTexture = CTexture::GetInstance();

	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�̎擾�p

	//z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		//��r���@
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			//z�o�b�t�@�X�V�̗L�������ݒ�

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//�A���t�@�e�X�g�̗L��/�����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		//��r���@
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);					//��l

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0,
		sizeof(VERTEX_3D));			//���_���\���̂̃T�C�Y

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(m_nTextureIdx));

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,			//�`�悷��ŏ��̃C���f�b�N�X(�厖)
		2);			//�v���~�e�B�u(�|���S��)��

	//z�o�b�t�@��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			//z�o�b�t�@�X�V�̗L�������ݒ�

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		//�A���t�@�e�X�g�̗L��/�����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	//��r���@
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);				//��l
}

//===========================================================================================
// ���_���̐ݒ�
//===========================================================================================
void CBillboard::SetverTex(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-SIZE, +SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+SIZE, +SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-SIZE, -SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+SIZE, -SIZE, 0.0f);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[(0.0f�`1.0f���Őݒ�)
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================================================================
// �ʒu�ݒ�
//===========================================================================================
void CBillboard::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;

	SetverTex();
}

//===========================================================================================
// �����ݒ�
//===========================================================================================
void CBillboard::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;

	SetverTex();
}

//===========================================================================================
//	�T�C�Y�ݒ�
//===========================================================================================
void CBillboard::SetSize(D3DXVECTOR2 size)
{
	m_size = size;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================================================================
//	���_���a�̐ݒ�
//===========================================================================================
void CBillboard::SetRadius(D3DXVECTOR2 radius)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//�Ίp���̒������Z�o(�f�t�H���g)
	m_fLength = sqrtf(radius.x * radius.x + radius.y * radius.y) * 0.5f;

	//�Ίp���̊p�x���Z�o����(�f�t�H���g)
	m_fAngle = atan2f(radius.x, radius.y);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + -D3DX_PI + m_fAngle) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + -D3DX_PI + m_fAngle) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//===========================================================================================
//	�F�ݒ�
//===========================================================================================
void CBillboard::SetColor(D3DXCOLOR col)
{
	m_col = col;

		//���_���ւ̃|�C���^
		VERTEX_3D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[(0.0f�`1.0f���Őݒ�)
		pVtx[0].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[1].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[2].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[3].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
}

//===========================================================================================
// �e�N�X�`���̊��蓖��
//===========================================================================================
void CBillboard::BindTexture(int nIdx)
{
	m_nTextureIdx = nIdx;
}