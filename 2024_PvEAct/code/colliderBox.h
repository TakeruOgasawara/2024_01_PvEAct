//======================================================================================
//
// ��`�����蔻�菈��[boxCollider.h]
// Author : Takeru Ogasawara
//
//======================================================================================
#ifndef _BOX_COLLIDER_H_
#define _BOX_COLLIDER_H_

// �C���N���[�h�錾
#include "collision.h"

//=============================================
// ��`�̓����蔻��N���X
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
