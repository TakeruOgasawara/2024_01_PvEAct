#ifndef _GAUGE_H_
#define _GAUGE_H_

// インクルード宣言
#include "main.h"
#include "object2D.h"

// ゲージクラス
class CGauge : public CObject2D
{
public:
	CGauge();	// コンストラクタ
	~CGauge();	// デストラクタ

	static CGauge* Create(D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f});

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	float m_fDecrease;	// 減少量
};

#endif
