//======================================================================================
//
// ���b�V���t�B�[���h�̏���[polygon.cpp]
// Author;���}���@�V
//
//======================================================================================
#include "meshwall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

#include "player.h"
#include "debugproc.h"

//�}�N����`
#define BG_TEX_FULE		""			//���w�i�̉摜��ǂݍ���
#define MESHWALL_WIDTH		(70.0f)		//��
#define MESHWALL_HEIGHT		(40.0f)			//����

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CMeshWall::CMeshWall()
{
	ZeroMemory(&m_info, sizeof(m_info));
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CMeshWall::~CMeshWall()
{

}

//===========================================================================================
// ����
//===========================================================================================
CMeshWall* CMeshWall::Create(D3DXVECTOR3 pos)
{
	CMeshWall* pMeshWall = nullptr;

	if (pMeshWall == nullptr)
	{
		pMeshWall = new CMeshWall;

		pMeshWall->Init();
		pMeshWall->m_info.pos = pos;

		return pMeshWall;
	}

	return nullptr;
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CMeshWall::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	m_nTextureIdx = CTexture::GetInstance()->Regist(BG_TEX_FULE);

	m_info.fWidth = MESHWALL_WIDTH;
	m_info.fHeight = MESHWALL_HEIGHT;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&m_info.pVtxBuff,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_info.fWidth, m_info.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, m_info.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_info.fWidth, m_info.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(-m_info.fWidth, m_info.fHeight * 0.5f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, m_info.fHeight * 0.5f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(m_info.fWidth, m_info.fHeight * 0.5f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(-m_info.fWidth, 0.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[8].pos = D3DXVECTOR3(m_info.fWidth, 0.0f, 0.0f);
	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//���_�J���[(0.0f�`1.0f���Őݒ�)
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	//���_�o�b�t�@���A�����b�N����
	m_info.pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���(�ԍ����o���邽�߂̃�����)
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_info.pIdxBuff, NULL);

	WORD* pIdx;			//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	m_info.pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//���_�ԍ��f�[�^�̐ݒ�
	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;	

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	m_info.pIdxBuff->Unlock();

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CMeshWall::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_info.pVtxBuff != nullptr)
	{
		m_info.pVtxBuff->Release();
		m_info.pVtxBuff = nullptr;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (m_info.pIdxBuff != nullptr)
	{
		m_info.pIdxBuff->Release();
		m_info.pIdxBuff = nullptr;
	}

	Release();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CMeshWall::Update(void)
{
	// �ϐ��錾
	float vec;						// �x�N�g��
	bool bHit = false;				// �q�b�g����
	float fRate;					// �x�N�g���̊���
	float fMaxArea, fNowArea;		// ���̖ʐρ^�ő�ʐ�
	D3DXVECTOR3 pos0 = {}, pos1 = {};	// �ꏊ
	D3DXVECTOR3 vecLine = {}, vecToPos = {};
	D3DXVECTOR3 Cross = {};	// ��_�̏ꏊ

	D3DXVECTOR3 PlayerPos = CPlayer::GetInstance()->GetPosition();
	D3DXVECTOR3 playerMove = CPlayer::GetInstance()->GetMovement();

	// �ꏊ�̌v�Z
	pos0 = (m_info.pos + D3DXVECTOR3(cosf(-m_info.rot.y) * -1.0f, 0.0f, sinf(-m_info.rot.y) * -1.0f));
	pos1 = (m_info.pos + D3DXVECTOR3(cosf(-m_info.rot.y) * 1.0f, 0.0f, sinf(-m_info.rot.y) * 1.0f));

	// pos0��pos1�Ƃ̋�����
	vecLine = pos1 - pos0;

	// �ǂ���Ώۂ܂ł̈ʒu
	vecToPos = PlayerPos - m_info.pos;

	// �ő�ʐ�
	fMaxArea = (vecLine.z * playerMove.x) - (vecLine.x * playerMove.z);

	// ���̖ʐ�
	fNowArea = (vecToPos.z * playerMove.x) - (vecLine.x * playerMove.z);

	// �x�N�g���̊���
	fRate = fNowArea / fMaxArea;

	// ��_
	Cross = D3DXVECTOR3(pos0.x + vecLine.x * fRate, PlayerPos.y, pos0.z + vecLine.z * fRate);

	vec = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);

	/*if (PlayerPos.x >= pos0.x && PlayerPos.x >= pos0.x,
		PlayerPos.x <= pos1.x && PlayerPos.x <= pos1.x)*/
	{
		if (vec < 0)
		{
			CManager::GetInstance()->GetDebugProc()->Print("\n\n����Ȃ�\n\n");
			bHit = true;
		}
	}
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CMeshWall::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X

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
	pDevice->SetStreamSource(0, m_info.pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_info.pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAddress(m_nTextureIdx));

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,
		9,			//�p�ӂ������_�̐�
		0,
		12);		//�`�悷��v���~�e�B�u�̐�
}