#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "main.h"
#include "object.h"

// 前方宣言
class CObject2D;
class CLifeGauge;
class CTime;

// ゲームマネージャクラス
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
	// メンバ関数
	void CreateLife(void);
	void SetLifePlayer(void);
	void SetLifeEnemy(void);
	void UpdateLife(void);
	void CreateTime(void);
	void SlowUpdate(void);

	// メンバ変数
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
