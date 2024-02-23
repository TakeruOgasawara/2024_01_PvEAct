//===========================================================================================
//
// 障害物エディット[ObstacleEdit.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "obstacle.h"
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "object.h"
#include "xfile.h"
#include "input.h"
#include "debugproc.h"

#include <stdio.h>

//===========================================================================================
// コンストラクタ
//===========================================================================================
CObstacle::CObstacle(int nPriority) : CObjectX(nPriority)
{
	m_bDestoroy = false;
	m_fLife = 0;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CObstacle::~CObstacle()
{
	
}

//===========================================================================================
// 生成
//===========================================================================================
CObstacle* CObstacle::Create(const char* c_pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLife, bool bDestoroy)
{
	CObstacle* pObjX = nullptr;

	if (pObjX == nullptr)
	{
		pObjX = new CObstacle;

		if (pObjX != nullptr)
		{
			pObjX->Init(c_pFileName, pos, rot, fLife, bDestoroy);

			return pObjX;
		}
	}

	return pObjX;
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CObstacle::Init(const char* c_pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLife, bool bDestoroy)
{
	// 親クラスの初期化
	CObjectX::Init(c_pFileName, pos);

	// 向き設定
	SetRotation(rot);

	// 破壊状態の設定
	m_bDestoroy = bDestoroy;

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CObstacle::Uninit(void)
{
	// 親クラスの終了処理
	CObjectX::Uninit();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CObstacle::Update(void)
{
	// 親クラスの更新処理
	CObjectX::Update();
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CObstacle::Draw(void)
{
	// 親クラスの描画処理
	CObjectX::Draw();
}

//===========================================================================================
// ダメージ処理
//===========================================================================================
void CObstacle::Hit(float fDamage)
{
	if (m_bDestoroy)
	{
		m_fLife -= fDamage;

		if (m_fLife < 0)
		{
			m_fLife = 0.0f;

			// 破棄する
			Uninit();
		}
	}
}
