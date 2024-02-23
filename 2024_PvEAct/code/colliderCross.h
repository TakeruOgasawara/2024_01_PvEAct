//======================================================================================
//
// �O�ς̓����蔻�菈��[clossCollider.h]
// Author : Takeru Ogasawara
//
//======================================================================================
#ifndef _CLOSS_COLLIDER_H_
#define _CLOSS_COLLIDER_H_

// �C���N���[�h�錾
#include "collision.h"

//=============================================
// �O�ς̓����蔻��N���X
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
