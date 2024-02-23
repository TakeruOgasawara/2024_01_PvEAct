//======================================================================================
//
// 矩形当たり判定処理[boxCollider.h]
// Author : Takeru Ogasawara
//
//======================================================================================
#ifndef _BOX_COLLIDER_H_
#define _BOX_COLLIDER_H_

// インクルード宣言
#include "collision.h"

//=============================================
// 矩形の当たり判定クラス
//=============================================
class CBoxCollider : public CCollision
{
public:
	CBoxCollider();
	~CBoxCollider() override;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DXVECTOR3 m_Size;
};

#endif
