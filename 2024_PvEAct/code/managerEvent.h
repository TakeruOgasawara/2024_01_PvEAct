#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "main.h"
#include "object.h"

// 前方宣言
class CSlowManager;
class CManagerUI;
class CObject2D;

// ゲームマネージャクラス
class CManagerEvent : public CObject
{
public:
	enum EGameMode
	{
		MODE_NORMAL = 0,
		MODE_JUST,
		MODE_RUSH,
		MODE_JANKEN,
		MODE_MAX
	};

public:
	CManagerEvent();
	~CManagerEvent();

	static CManagerEvent* Create(void);
	static CManagerEvent* GetInstance(void);
	static void Release(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMode(EGameMode mode) { m_mode = mode; }
	EGameMode GetMode(void) { return m_mode; }

private:
	// メンバ関数
	void SlowUpdate(void);
	void Normal(void);
	void Rush(void);
	void Janken(void);


	// メンバ変数
	static CManagerEvent* m_pManager;
	CSlowManager* m_pSlowManager;
	EGameMode m_mode;
	float m_fSlowTime;
	int m_nRushTime;

	struct SUi
	{
		CObject2D* pRushUI;
		CObject2D* pJankenUI;
	};
	SUi m_ui;
};

#endif
