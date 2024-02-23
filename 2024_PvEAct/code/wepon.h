//===============================================================================
//
// ����̏���[wepon.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _WEPON_H_				//���̃}�N����`������Ă��Ȃ�������
#define _WEPON_H_				//2�d�C���N���[�h�X�q�̃}�N�����`

#include "main.h"
#include "objectX.h"

// ����H�炤
class CWepon : public CObjectX
{
public:
	enum EType	// ������
	{
		TYPE_NONE = 0,
		TYPE_SWORD,
		TYPE_MAX
	};

public:
	CWepon();
	virtual ~CWepon();

	static CWepon* Create(const char* pPath, D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f});

	virtual HRESULT Init(const char* pPath, D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void Follow(void);

	void SetMtxOwner(D3DXMATRIX pMtx) { m_mtxOwner = pMtx; }

private:

	D3DXMATRIX m_mtxOwner;	// ����̎�����
};


#endif
