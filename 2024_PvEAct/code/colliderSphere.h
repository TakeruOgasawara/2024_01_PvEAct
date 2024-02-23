//======================================================================================
//
// 円形当たり判定処理[circleCollider.h]
// Author : Takeru Ogasawara
//
//======================================================================================
#ifndef _SPHERE_COLLIDER_H_
#define _SPHERE_COLLIDER_H_

// インクルード宣言
#include "collision.h"

// 前方宣言
class CObject3D;

//=============================================
// 円形の当たり判定クラス
//=============================================
class CSphereCollider : public CCollision
{
public:
	// コンストラクタ、デストラクタなど
	CSphereCollider();
	~CSphereCollider() override;

	static CSphereCollider* Create(ETag tag, EType type, CObject* pObject);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void PushSphere(D3DXVECTOR3* pPos, ETag tag);	// 押し出し
	bool OnEnter(D3DXVECTOR3* pPos, ETag tag);

	// 設定
	void SetRadius(float fRadius) override { m_fRadius = fRadius; }

	// 取得
	float GetRadius(void) const override { return m_fRadius; }

private:
	float m_fRadius;	// 半径
	CObject3D* m_pObject3D;
};

#endif
