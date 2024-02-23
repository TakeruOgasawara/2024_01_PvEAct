#ifndef _GAUGE_H_
#define _GAUGE_H_

// �C���N���[�h�錾
#include "main.h"
#include "object2D.h"

// �Q�[�W�N���X
class CGauge : public CObject2D
{
public:
	CGauge();	// �R���X�g���N�^
	~CGauge();	// �f�X�g���N�^

	static CGauge* Create(D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f});

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	float m_fDecrease;	// ������
};

#endif
