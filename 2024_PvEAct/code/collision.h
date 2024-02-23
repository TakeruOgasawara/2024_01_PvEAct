//======================================================================================
//
// �����蔻�菈��[collision.h]
// Author : Takeru Ogasawara
//
//======================================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

// �C���N���[�h�錾
#include "main.h"
#include "object.h"

// �O���錾
class CObject;

// �萔��`
namespace collision
{
	static const int MAX_OBJECT = 254;
}

//=============================================
// �����蔻��N���X
//=============================================
class CCollision : public CObject
{
public:
	enum ETag
	{
		TAG_NONE = 0,
		TAG_PLAYER,
		TAG_OBJECT,
		TAG_ENEMY_BOSS,
		TAG_MAX
	};

	enum EType
	{
		TYPE_NONE = 0, // ���ł��Ȃ�
		TYPE_BOX,	// ��`�̓����蔻��
		TYPE_SPHERE,	// ���̓����蔻��
		TYPE_MAX
	};

public:
	CCollision();	// �R���X�g���N�^
	virtual ~CCollision();	// �f�X�g���N�^

	static CCollision* Create(void);
	static CCollision* Create(ETag tag, EType type, CObject* pObject);

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	void ReleaseAll(void);

	// �ݒ�
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetTag(ETag tag) { m_tag = tag; }
	void SetType(EType type) { m_type = type; }
	void SetOwner(CObject* pObject) { m_pObject = pObject; }
	void SetIsTrigger(bool bTrigger) { m_bTrigger = bTrigger; }
	virtual void SetRadius(float fRadius) {};

	// �擾
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	ETag GetTag(void) const { return m_tag; }
	EType GetType(void) const { return m_type; }
	CObject* GetOwner(void) const;
	bool GetIsTrigger(void) const { return m_bTrigger; }
	virtual float GetRadius(void) const{ return 0.0f; }
	CCollision** GetCollision(void) { return &m_apCollision[0]; }
	
private:
	// �����o�ϐ�
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	ETag m_tag;	// �^�O
	EType m_type;	// ������
	CObject* m_pObject;
	bool m_bTrigger;

	// ���
	static CCollision* m_apCollision[collision::MAX_OBJECT];
	int m_nID;
	static int m_nNumAll;
};

#endif
