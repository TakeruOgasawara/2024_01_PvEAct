//===========================================================================================
//
// 円形当たり判定処理[.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================

// インクルード宣言
#include "colliderSphere.h"
#include "collision.h"
#include "xfile.h"
#include "object3D.h"

//===========================================================================================
// コンストラクタ
//===========================================================================================
CSphereCollider::CSphereCollider() : m_fRadius(0.0f), m_pObject3D(nullptr)
{

}

//===========================================================================================
// デストラクタ
//===========================================================================================
CSphereCollider::~CSphereCollider()
{

}

//===========================================================================================
// 生成
//===========================================================================================
CSphereCollider* CSphereCollider::Create(ETag tag, EType type, CObject* pObject)
{
	CSphereCollider* pSpere = nullptr;

	if (pSpere == nullptr)
	{
		pSpere = new CSphereCollider;

		pSpere->Init();

		pSpere->SetTag(tag);
		pSpere->SetType(type);
		pSpere->SetOwner(pObject);

		return pSpere;
	}

	return nullptr;
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CSphereCollider::Init(void)
{
#ifdef _DEBUG
	if (m_pObject3D == nullptr)
	{// ビルボードの生成
		m_pObject3D = CObject3D::Create(GetPosition(), "data\\TEXTURE\\col.png");

		if (m_pObject3D != nullptr)
		{
			m_pObject3D->SetSize(m_fRadius, m_fRadius);
		}
	}
#endif


	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CSphereCollider::Uninit(void)
{
	if (m_pObject3D != nullptr)
	{// ビルボードの削除
		m_pObject3D->Uninit();
		m_pObject3D = nullptr;
	}

	// 親クラスの終了処理
	CCollision::Uninit();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CSphereCollider::Update(void)
{
	if (m_pObject3D != nullptr)
	{// 
		D3DXVECTOR3 pos = GetPosition();
		m_pObject3D->SetPosition(pos);
		m_pObject3D->SetSize(m_fRadius, m_fRadius);
	}
}

//===========================================================================================
// 円の押し出し処理
//===========================================================================================
void CSphereCollider::PushSphere(D3DXVECTOR3* pPos, ETag tag)
{
	if (tag <= TAG_NONE && tag >= TAG_MAX)
	{
		return;
	}

	CCollision** ppCollision = GetCollision();

	for (int nCnt = 0; nCnt < collision::MAX_OBJECT; nCnt++)
	{
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

		// 同類との対角線の長さを算出する
		float fLengthDiff = sqrtf
		(
			(ppCollision[nCnt]->GetPosition().x - pPos->x) * (ppCollision[nCnt]->GetPosition().x - pPos->x) +
			(ppCollision[nCnt]->GetPosition().z - pPos->z) * (ppCollision[nCnt]->GetPosition().z - pPos->z)
		);

		// 位置の差分
		D3DXVECTOR3 posDiff =
		{
			ppCollision[nCnt]->GetPosition().x - pPos->x,
			0.0f,
			ppCollision[nCnt]->GetPosition().z - pPos->z,
		};
		
		// ぶつかる時の距離
		float fLength = ppCollision[nCnt]->GetRadius() + GetRadius();

		if (fLengthDiff < fLength)
		{
			D3DXVECTOR3 posAfter = {0.0f, 0.0f, 0.0f};

			// 差分ベクトルから押し出し後の位置を計算
			D3DXVec3Normalize(&posAfter, &posDiff);

			posAfter *= -fLength;

			posAfter += ppCollision[nCnt]->GetPosition();

			*pPos = posAfter;
		}
	}
}

//===========================================================================================
// 円のトリガー判定
//===========================================================================================
bool CSphereCollider::OnEnter(D3DXVECTOR3* pPos, ETag tag)
{
	if (tag <= TAG_NONE && tag >= TAG_MAX)
	{
		return false;
	}

	CCollision** ppCollision = GetCollision();

	for (int nCnt = 0; nCnt < collision::MAX_OBJECT; nCnt++)
	{
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

		// 対角線の長さを算出する
		float fLengthDiff = sqrtf
		(
			(ppCollision[nCnt]->GetPosition().x - pPos->x) * (ppCollision[nCnt]->GetPosition().x - pPos->x) +
			(ppCollision[nCnt]->GetPosition().z - pPos->z) * (ppCollision[nCnt]->GetPosition().z - pPos->z)
		);

		// 位置の差分
		D3DXVECTOR3 posDiff = ppCollision[nCnt]->GetPosition() - *pPos;

		// ぶつかる時の距離
		float fLength = ppCollision[nCnt]->GetRadius() + GetRadius();

		if (fLengthDiff < fLength)
		{
			return true;
		}
	}

	return false;
}
