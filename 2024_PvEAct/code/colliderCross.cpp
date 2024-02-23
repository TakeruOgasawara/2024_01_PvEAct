//===========================================================================================
//
// 矩形当たり判定処理[clossCollider.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "colliderCross.h"
#include "manager.h"
#include "renderer.h"

//===========================================================================================
// コンストラクタ
//===========================================================================================
CClossCollider::CClossCollider() : m_fWidth(0.0f)
{

}

//===========================================================================================
// デストラクタ
//===========================================================================================
CClossCollider::~CClossCollider()
{

}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CClossCollider::Init(void)
{
	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CClossCollider::Uninit(void)
{

}

//===========================================================================================
// 更新処理
//===========================================================================================
void CClossCollider::Update(void)
{

}

//===========================================================================================
// 触れた判定
//===========================================================================================
bool CClossCollider::IsTrigger(ETag tag)
{
	for (int nCnt = 0; nCnt < collision::MAX_OBJECT; nCnt++)
	{
		CCollision** ppCollision = GetCollision();

		if (ppCollision[nCnt] == nullptr)
		{
			continue;
		}

		if (ppCollision[nCnt]->GetType() != TYPE_SPHERE)
		{
			continue;
		}

		if (ppCollision[nCnt]->GetTag() == tag || ppCollision[nCnt] == this)
		{
			continue;
		}

		// 変数宣言
		//float vec;						// ベクトル
		//bool bHit = false;				// ヒット判定
		//float fRate;					// ベクトルの割合
		//float fMaxArea, fNowArea;		// 今の面積／最大面積
		//D3DXVECTOR3 pos0 = {}, pos1 = {};			// 場所
		//D3DXVECTOR3 vecLine = {}, vecToPos = {};
		//D3DXVECTOR3 Cross = {};				// 交点の場所

		//D3DXVECTOR3 ClsPos = ppCollision[nCnt]->GetPosition();
		//D3DXVECTOR3 ClsRot = ppCollision[nCnt]->GetRotation();

		// 場所の計算
		//pos0 = (ClsPos + D3DXVECTOR3(cosf(-ClsRot.y) * -1.0f, 0.0f, sinf(-ClsRot.y) * -1.0f));
		//pos1 = (ClsPos + D3DXVECTOR3(cosf(-ClsRot.y) * 1.0f, 0.0f, sinf(-ClsRot.y) * 1.0f));

		// pos0とpos1との距離間
		//vecLine = pos1 - pos0;

		// 壁から弾までの位置
		//vecToPos = pBullet->pos - pMeshWall->pos;

		//最大面積
		//fMaxArea = (g_vecLine.z * pBullet->move.x) - (g_vecLine.x * pBullet->move.z);

		//今の面積
		//fNowArea = (g_vecToPos.z * pBullet->move.x) - (g_vecToPos.x * pBullet->move.z);

		// ベクトルの割合
		//fRate = fNowArea / fMaxArea;

		// 交点
		//Cross = D3DXVECTOR3(pos0.x + g_vecLine.x * fRate, pBullet->pos.y, pos0.z + g_vecLine.z * fRate);

		//vec = (g_vecLine.z * g_vecToPos.x) - (g_vecLine.x * g_vecToPos.z);

		//if (pBullet->pos.x >= pos0.x && pBullet->pos.x >= pos0.x,
		//	pBullet->pos.x <= pos1.x && pBullet->pos.x <= pos1.x)
		//{
		//	if (vec < 0)
		//	{
		//		pBullet->pos = Cross;
		//	}
		//}
	}

	return false;
}
