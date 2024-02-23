#ifndef _FRAME_H_
#define _FRAME_H_

#include "main.h"
#include "object2D.h"

// フレームクラス
class CFrame : public CObject2D
{
public:
	CFrame();
	~CFrame();

	static CFrame* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	float m_fFecrease;
};

#endif // !_FRAME_H_
