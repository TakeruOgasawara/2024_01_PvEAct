#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "main.h"
#include "object.h"

// �O���錾
class CSlowManager;
class CManagerUI;
class CObject2D;

// �Q�[���}�l�[�W���N���X
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
	// �����o�֐�
	void SlowUpdate(void);
	void Normal(void);
	void Rush(void);
	void Janken(void);


	// �����o�ϐ�
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
