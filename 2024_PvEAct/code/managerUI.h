#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "main.h"
#include "object.h"

// �O���錾
class CObject2D;
class CLifeGauge;
class CTime;

// �Q�[���}�l�[�W���N���X
class CManagerUI : public CObject
{
public:
	CManagerUI();
	~CManagerUI();

	static CManagerUI* Create(void);
	static CManagerUI* GetInstance(void);
	static void Release(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// �����o�֐�
	void CreateLife(void);
	void SetLifePlayer(void);
	void SetLifeEnemy(void);
	void UpdateLife(void);
	void CreateTime(void);
	void SlowUpdate(void);

	// �����o�ϐ�
	static CManagerUI* m_pUiManager;

	struct SUi
	{
		CObject2D* pRushUi;
		CLifeGauge* pGaugePlayer;
		CLifeGauge* pGaugeEnemy;
		CTime* pTime;
	};

	SUi m_ui;
};

#endif
