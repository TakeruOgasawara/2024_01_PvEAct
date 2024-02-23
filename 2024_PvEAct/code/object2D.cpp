//===========================================================================================
//
// [object2D.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "texture.h"

//�}�N����`
#define SIZE_X		(100.0f)	//���̒���
#define SIZE_Y		(100.0f)	//�c�̒���

//�ÓI�����o�ϐ��錾
int CObject2D::m_nNumObj2D = 0;

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(m_info));

	m_nNumObj2D++;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CObject2D::~CObject2D()
{
	m_nNumObj2D--;
}

//===========================================================================================
// ��������(�����l)
//===========================================================================================
CObject2D *CObject2D::Create(void)
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pObject2D = new CObject2D;

		if (pObject2D != nullptr)
		{
			//����������
			pObject2D->Init();

			return pObject2D;
		}
		else
		{
			return nullptr;
		}
	}

	return nullptr;
}

//===========================================================================================
// ��������(�ʒu)
//===========================================================================================
CObject2D *CObject2D::Create(D3DXVECTOR3 pos)
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pObject2D = new CObject2D;

		if (pObject2D != nullptr)
		{
			//����������
			pObject2D->Init();

			//�ʒu�̐ݒ菈��
			pObject2D->SetPosition(pos);

			return pObject2D;
		}
		else
		{
			return nullptr;
		}
	}

	return nullptr;
}

//===========================================================================================
// ��������(�ʒu�A��])
//===========================================================================================
CObject2D *CObject2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pObject2D = new CObject2D;

		if (pObject2D != nullptr)
		{
			//����������
			pObject2D->Init();

			pObject2D->SetPosition(pos);
			pObject2D->SetRotation(rot);

			return pObject2D;
		}
		else
		{
			return nullptr;
		}
	}

	return pObject2D;
}

//===========================================================================================
// ��������(�ʒu�A�e�N�X�`����)
//===========================================================================================
CObject2D * CObject2D::Create(D3DXVECTOR3 pos, const char *name)
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pObject2D = new CObject2D;

		if (pObject2D != nullptr)
		{
			//����������
			pObject2D->Init();

			pObject2D->SetPosition(pos);

			pObject2D->BindTexture(CTexture::GetInstance()->Regist(name));

			return pObject2D;
		}
		else
		{
			return nullptr;
		}
	}

	return pObject2D;
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CObject2D::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �ʒu(�f�t�H���g)
	m_info.pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5, 0.0f);

	// �F(�f�t�H���g)
	m_info.col = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	// �T�C�Y�ݒ�(�f�t�H���g)
	m_info.fWidth = SIZE_X;
	m_info.fHeight = SIZE_Y;

	// �Ίp���̒������Z�o(�f�t�H���g)
	m_info.fLength = sqrtf(SIZE_X * SIZE_X + SIZE_Y * SIZE_Y) * 0.5f;

	// �Ίp���̊p�x���Z�o����(�f�t�H���g)
	m_info.fAngle = atan2f(SIZE_X, SIZE_Y);

	// �e�N�X�`���ԍ�
	m_info.nIdxTexture = -1;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED,
		&m_info.pVtxBuff,
		NULL);

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CObject2D::Uninit(void)
{
	// �o�b�t�@�̔j��
	if (m_info.pVtxBuff != nullptr)
	{
		m_info.pVtxBuff->Release();
		m_info.pVtxBuff = nullptr;
	}

	// �j��
	Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CObject2D::Update(void)
{
	CManager::GetInstance()->GetDebugProc()->Print("a\n");
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CObject2D::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �e�N�X�`�����̎擾
	CTexture *pTexture = CTexture::GetInstance();
	
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_info.pVtxBuff, 0, sizeof(VERTEX_2D));

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(m_info.nIdxTexture));
	
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,			// �`�悷��ŏ��̃C���f�b�N�X(�厖)
		2);			// �v���~�e�B�u(�|���S��)��
}

//=======================================================
// ���_���̐ݒ�(���S����)
//=======================================================
void CObject2D::SetVertex_center(void)
{
	if (m_info.pVtxBuff == nullptr)
	{
		return;
	}

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = m_info.pos.x + sinf(m_info.rot.z + -D3DX_PI + m_info.fAngle) * m_info.fLength;
	pVtx[0].pos.y = m_info.pos.y + cosf(m_info.rot.z + -D3DX_PI + m_info.fAngle) * m_info.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_info.pos.x + sinf(m_info.rot.z + D3DX_PI - m_info.fAngle) * m_info.fLength;
	pVtx[1].pos.y = m_info.pos.y + cosf(m_info.rot.z + D3DX_PI - m_info.fAngle) * m_info.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_info.pos.x + sinf(m_info.rot.z - m_info.fAngle) * m_info.fLength;
	pVtx[2].pos.y = m_info.pos.y + cosf(m_info.rot.z - m_info.fAngle) * m_info.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_info.pos.x + sinf(m_info.rot.z + m_info.fAngle) * m_info.fLength;
	pVtx[3].pos.y = m_info.pos.y + cosf(m_info.rot.z + m_info.fAngle) * m_info.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = m_info.col;
	pVtx[1].col = m_info.col;
	pVtx[2].col = m_info.col;
	pVtx[3].col = m_info.col;

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		
	//���_�o�b�t�@���A�����b�N����
	m_info.pVtxBuff->Unlock();
}

//=======================================================
// ���_���̐ݒ�(���[����)
//=======================================================
void CObject2D::SetVertex_left(void)
{
	if (m_info.pVtxBuff == nullptr)
	{
		return;
	}

	//���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	// ����
	pVtx[0].pos.x = m_info.pos.x + sinf(m_info.rot.z + -D3DX_PI + m_info.fAngle);
	pVtx[0].pos.y = m_info.pos.y + cosf(m_info.rot.z + -D3DX_PI + m_info.fAngle) * m_info.fLength;
	pVtx[0].pos.z = 0.0f;
	// �E��
	pVtx[1].pos.x = m_info.pos.x + sinf(m_info.rot.z + D3DX_PI - m_info.fAngle) * m_info.fLength;
	pVtx[1].pos.y = m_info.pos.y + cosf(m_info.rot.z + D3DX_PI - m_info.fAngle) * m_info.fLength;
	pVtx[1].pos.z = 0.0f;
	// ����
	pVtx[2].pos.x = m_info.pos.x + sinf(m_info.rot.z - m_info.fAngle);
	pVtx[2].pos.y = m_info.pos.y + cosf(m_info.rot.z - m_info.fAngle) * m_info.fLength;
	pVtx[2].pos.z = 0.0f;
	// �E��
	pVtx[3].pos.x = m_info.pos.x + sinf(m_info.rot.z + m_info.fAngle) * m_info.fLength;
	pVtx[3].pos.y = m_info.pos.y + cosf(m_info.rot.z + m_info.fAngle) * m_info.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = m_info.col;
	pVtx[1].col = m_info.col;
	pVtx[2].col = m_info.col;
	pVtx[3].col = m_info.col;

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_info.pVtxBuff->Unlock();
}

//=======================================================
// �T�C�Y�̐ݒ�
//=======================================================
void CObject2D::SetSize(float fWidth, float fHeight)
{
	m_info.fWidth = fWidth;
	m_info.fHeight = fHeight;

	// �Ίp���̒������Z�o����(�������g�p����ꍇ)
	m_info.fLength = sqrtf(m_info.fWidth * m_info.fWidth + m_info.fHeight * m_info.fHeight) * 0.5f;

	// �Ίp���̊p�x���Z�o����(�������g�p����ꍇ)
	m_info.fAngle = atan2f(m_info.fWidth, m_info.fHeight);
}

//=======================================================
// �e�N�X�`���̐ݒ�
//=======================================================
void CObject2D::SetTex(D3DXVECTOR2 left, D3DXVECTOR2 right)
{
	if (m_info.pVtxBuff == nullptr)
	{
		return;
	}

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = left;
	pVtx[1].tex = D3DXVECTOR2(right.x, left.y);
	pVtx[2].tex = D3DXVECTOR2(left.x, right.y);
	pVtx[3].tex = right;

	// ���_�o�b�t�@���A�����b�N����
	m_info.pVtxBuff->Unlock();
}

//=======================================================
// ���_���̎擾
//=======================================================
LPDIRECT3DVERTEXBUFFER9 CObject2D::GetVtxBuff(void)
{
	return m_info.pVtxBuff;
}
