//======================================================================================
//
// �~�`�����蔻�菈��[circleCollider.h]
// Author : Takeru Ogasawara
//
//======================================================================================
#ifndef _SPHERE_COLLIDER_H_
#define _SPHERE_COLLIDER_H_

// �C���N���[�h�錾
#include "collision.h"

// �O���錾
class CObject3D;

//=============================================
// �~�`�̓����蔻��N���X
//=============================================
class CSphereCollider : public CCollision
{
public:
	// �R���X�g���N�^�A�f�X�g���N�^�Ȃ�
	CSphereCollider();
	~CSphereCollider() override;

	static CSphereCollider* Create(ETag tag, EType type, CObject* pObject);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void PushSphere(D3DXVECTOR3* pPos, ETag tag);	// �����o��
	bool OnEnter(D3DXVECTOR3* pPos, ETag tag);

	// �ݒ�
	void SetRadius(float fRadius) override { m_fRadius = fRadius; }

	// �擾
	float GetRadius(void) const override { return m_fRadius; }

private:
	float m_fRadius;	// ���a
	CObject3D* m_pObject3D;
};

#endif
