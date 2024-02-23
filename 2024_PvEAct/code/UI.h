//===========================================================================================
// 
// UI����[UI.h]
// Author : Takeru Ogasawara
// 
//===========================================================================================
#ifndef _UI_H_
#define _UI_H_

#include "object2D.h"

// UI�N���X
class CUI : public CObject2D
{
public:
	CUI(int nPriority = 7);	// �R���X�g���N�^
	~CUI();	// �f�X�g���N�^

	static CUI* Create();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
};

#endif // !_LIFE_H_