//===============================================================================
//
// �ėp�֐�[general.cpp]
// Author : Takeru Ogasawara
//
//===============================================================================

#include "generic.h"
#include "manager.h"
#include "renderer.h"
#include <time.h>

//===============================================================================
// �����̏C��
//===============================================================================
void generic::RotLimit(D3DXVECTOR3* pRot)
{
	//y
	if (pRot->y > D3DX_PI)
	{
		pRot->y = -D3DX_PI;
	}
	if (pRot->y < -D3DX_PI)
	{
		pRot->y = D3DX_PI;
	}
	//z
	if (pRot->z > D3DX_PI)
	{
		pRot->z = -D3DX_PI;
	}
	if (pRot->z < -D3DX_PI)
	{
		pRot->z = D3DX_PI;
	}
	//x
	if (pRot->x > D3DX_PI)
	{
		pRot->x = -D3DX_PI;
	}
	if (pRot->x < -D3DX_PI)
	{
		pRot->x = D3DX_PI;
	}
}

//===============================================================================
// �����̏C��
//===============================================================================
void generic::RotLimit(float* pRot)
{
	if (*pRot > D3DX_PI)
	{
		*pRot = -D3DX_PI;
	}
	if (*pRot < -D3DX_PI)
	{
		*pRot = D3DX_PI;
	}
}

//===============================================================================
// �}�g���b�N�X�̐ݒ�
//===============================================================================
void generic::SetMatrix(D3DXMATRIX* pMtxWorld, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(pMtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, pMtxWorld);
}

//===============================================================================
// �}�g���b�N�X�̐ݒ�
//===============================================================================
void generic::SetMatrix(D3DXMATRIX* pMtxWorld, D3DXMATRIX pMtxWorldOwner, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(pMtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxTrans);

	// �}�g���b�N�X�̊|�����킹
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &pMtxWorldOwner);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, pMtxWorld);
}

void generic::GetTime(void)
{
	time_t time(time_t * tloc);
}
