//======================================================================================
//
// 外積の当たり判定処理[clossCollider.h]
// Author : Takeru Ogasawara
//
//======================================================================================
#ifndef _CLOSS_COLLIDER_H_
#define _CLOSS_COLLIDER_H_

// インクルード宣言
#include "collision.h"

//=============================================
// 外積の当たり判定クラス
//=============================================
class CClossCollider : public CCollision
{
public:
	CClossCollider();
	~CClossCollider() override;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	bool IsTrigger(ETag tag);

private:
	float m_fWidth;
};

#endif
