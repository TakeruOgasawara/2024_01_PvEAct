//======================================================================================
//
// ���b�V���V�����_�[�̏���[meshcylindwe.h]
// Author;���}���@�V
//
//======================================================================================
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//�}�N����`
#define BG_TEX_FULE		"data\\TEXTURE\\bg00.png"			//���w�i�̉摜��ǂݍ���
#define MESH_LENGTH		(1700.0f)			//�V�����_�[�̔��a�̒���
#define MESH_HEIGHT		(1500.0f)			//�V�����_�[�̍���

//====================================================================
// �R���X�g���N�^
//====================================================================
CMeshCylinder::CMeshCylinder()
{
	// �l�̃N���A
	ZeroMemory(&m_info, sizeof(m_info));
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CMeshCylinder::~CMeshCylinder()
{

}

//====================================================================
// ����������
//====================================================================
HRESULT CMeshCylinder::Init(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	m_info.nIdxTexture = CTexture::GetInstance()->Regist(BG_TEX_FULE);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&m_info.pVtxBuff,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.0f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 0.0f) * MESH_LENGTH);
	pVtx[1].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.25f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 0.25f) * MESH_LENGTH);
	pVtx[2].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.5f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 0.5f) * MESH_LENGTH);
	pVtx[3].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.75f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 0.75f) * MESH_LENGTH);
	pVtx[4].pos = D3DXVECTOR3(cosf(D3DX_PI * 1.0f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 1.0f) * MESH_LENGTH);
	pVtx[5].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.75f) * MESH_LENGTH, MESH_HEIGHT, sinf(-D3DX_PI * 0.75f) * MESH_LENGTH);
	pVtx[6].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.5f) * MESH_LENGTH, MESH_HEIGHT, sinf(-D3DX_PI * 0.5f) * MESH_LENGTH);
	pVtx[7].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.25f) * MESH_LENGTH, MESH_HEIGHT, sinf(-D3DX_PI * 0.25f) * MESH_LENGTH);
	////�d�Ȃ��Ă��鏊(�㑤)
	pVtx[8].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.0f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 0.0f) * MESH_LENGTH);
	//����
	pVtx[9].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.0f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 0.0f) * MESH_LENGTH);
	pVtx[10].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.25f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 0.25f) * MESH_LENGTH);
	pVtx[11].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.5f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 0.5f) * MESH_LENGTH);
	pVtx[12].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.75f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 0.75f) * MESH_LENGTH);
	pVtx[13].pos = D3DXVECTOR3(cosf(D3DX_PI * 1.0f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 1.0f) * MESH_LENGTH);
	pVtx[14].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.75f) * MESH_LENGTH, 0.0f, sinf(-D3DX_PI * 0.75f) * MESH_LENGTH);
	pVtx[15].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.5f) * MESH_LENGTH, 0.0f, sinf(-D3DX_PI * 0.5f) * MESH_LENGTH);
	pVtx[16].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.25f) * MESH_LENGTH, 0.0f, sinf(-D3DX_PI * 0.25f) * MESH_LENGTH);
	//�d�Ȃ��Ă��鏊(����)
	pVtx[17].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.0f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 0.0f) * MESH_LENGTH);

	for (int nCnt = 0; nCnt < 18; nCnt++)
	{
		//�@���x�N�g���̐ݒ�
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[(0.0f�`1.0f���Őݒ�)
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.375f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.625f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.75f, 0.0f);
	pVtx[7].tex = D3DXVECTOR2(0.875f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 0.0f);
	//����
	pVtx[9].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[10].tex = D3DXVECTOR2(0.125f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(0.25f, 1.0f);
	pVtx[12].tex = D3DXVECTOR2(0.375f, 1.0f);
	pVtx[13].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[14].tex = D3DXVECTOR2(0.625f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(0.75f, 1.0f);
	pVtx[16].tex = D3DXVECTOR2(0.875f, 1.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_info.pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���(�ԍ����o���邽�߂̃�����)
	pDevice->CreateIndexBuffer(sizeof(WORD) * 20,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_info.pIdxBuff, NULL);

	WORD* pIdx;			//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	m_info.pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//���_�ԍ��f�[�^�̐ݒ�
	pIdx[0] = 0;
	pIdx[1] = 9;
	pIdx[2] = 1;
	pIdx[3] = 10;

	pIdx[4] = 2;
	pIdx[5] = 11;
	pIdx[6] = 3;
	pIdx[7] = 12;

	pIdx[8] = 4;
	pIdx[9] = 13;
	pIdx[10] = 5;
	pIdx[11] = 14;

	pIdx[12] = 6;
	pIdx[13] = 15;
	pIdx[14] = 7;
	pIdx[15] = 16;

	pIdx[16] = 8;
	pIdx[17] = 17;

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	m_info.pIdxBuff->Unlock();

	return E_NOTIMPL;
}

//====================================================================
// �I������
//====================================================================
void CMeshCylinder::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_info.pVtxBuff != NULL)
	{
		m_info.pVtxBuff->Release();
		m_info.pVtxBuff = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (m_info.pIdxBuff != NULL)
	{
		m_info.pIdxBuff->Release();
		m_info.pIdxBuff = NULL;
	}
}

//====================================================================
// �X�V����
//====================================================================
void CMeshCylinder::Update(void)
{

}

//====================================================================
// �`�揈��
//====================================================================
void CMeshCylinder::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_info.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_info.rot.y, m_info.rot.x, m_info.rot.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_info.pos.x, m_info.pos.y, m_info.pos.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_info.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_info.pVtxBuff, 0,
		sizeof(VERTEX_3D));			//���_���\���̂̃T�C�Y

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_info.pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAddress(m_info.nIdxTexture));

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,
		9,			//�p�ӂ������_�̐�
		0,
		20);		//�`�悷��v���~�e�B�u�̐�
}
