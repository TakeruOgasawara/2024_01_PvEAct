//===============================================================================
//
// 汎用関数[general.cpp]
// Author : Takeru Ogasawara
//
//===============================================================================

#include "generic.h"
#include "manager.h"
#include "renderer.h"
#include <time.h>

//===============================================================================
// 向きの修正
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
// 向きの修正
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
// マトリックスの設定
//===============================================================================
void generic::SetMatrix(D3DXMATRIX* pMtxWorld, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(pMtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, pMtxWorld);
}

//===============================================================================
// マトリックスの設定
//===============================================================================
void generic::SetMatrix(D3DXMATRIX* pMtxWorld, D3DXMATRIX pMtxWorldOwner, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(pMtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &mtxTrans);

	// マトリックスの掛け合わせ
	D3DXMatrixMultiply(pMtxWorld, pMtxWorld, &pMtxWorldOwner);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, pMtxWorld);
}

void generic::GetTime(void)
{
	time_t time(time_t * tloc);
}
