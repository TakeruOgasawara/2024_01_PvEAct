//===============================================================================
//
// 汎用関数[general.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _GENERAL_H_	//このマクロ定義がされていなかったら
#define _GENERAL_H_	//2重インクルード防止のマクロを定義

#include "main.h"
#include <cmath>

//****************************************
// 前方宣言
//****************************************
class CObject;

//****************************************
// 便利関数宣言
//****************************************
namespace general
{
	void RotLimit(D3DXVECTOR3* pRot);	// 向きの修正
	void RotLimit(float* pRot);	// 向きの修正
	void RotRevi(float* pRot);	// 

	// マトリックス設定
	void SetMatrix(D3DXMATRIX* pMtxWorld, D3DXVECTOR3 pos, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });
	void SetMatrix(D3DXMATRIX* pMtxWorld, D3DXMATRIX pMtxWorldOwner, D3DXVECTOR3 pos, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });

	// 時間
	void GetTime(void);
};

#endif