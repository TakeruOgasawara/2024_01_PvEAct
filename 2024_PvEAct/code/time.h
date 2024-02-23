//===============================================================================
//
// [time.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _TIME_H_				//このマクロ定義がされていなかったら
#define _TIME_H_				//2重インクルード帽子のマクロを定義

// インクルード宣言
#include "main.h"
#include "object.h"

// 前方宣言
class CNumber;
class CObject2D;

// タイムークラス
class CTime : public CObject
{
private:	//定数(マクロ的な役割)

	static const int NUM_TIME = 5;

public:
	CTime();	//コンストラクタ
	~CTime();	//デストラクタ

	static CTime* Create();
	static CTime* Create(D3DXVECTOR3 pos);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetTime(void);
	void AddTime(int nMinute, int nSecond);

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetColor(D3DXCOLOR col);
	void SetSize(float fWidth, float fHeight);

	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	int GetTime(void);

private:
	CNumber* m_apNumber[NUM_TIME];
	CObject2D* m_pPeriod;
	CObject2D* m_pColon;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXCOLOR m_col;
	int m_nSecondCount;
	int m_nMinuteCount;
	DWORD m_dwGameStartTime;	//ゲーム開始時間
	DWORD m_dwGameTime;	//ゲーム経過時間
	bool m_bStop;
};

#endif