

#ifndef _LIFE_H_
#define _LIFE_H_

#include "object.h"

// �O���錾
class CUI;

// �̗̓N���X
class CLifeGauge : public CObject
{
private:
	enum EState
	{
		STATE_LIVE = 0,	// ����
		STATE_DEATH,	// ���S
		STATE_MAX
	};

	struct SInfo
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXCOLOR colLife;	// �F
		D3DXCOLOR colFrame;
		float fLife;	// �̗�
		int nIdxTexture;	// �e�N�X�`���ԍ�
	};

	struct SUI
	{
		CUI* pGauge;
		CUI* pAfterImage;
		CUI* pFrame;
	};

public:
	CLifeGauge();
	~CLifeGauge();

	static CLifeGauge* Create();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPosition(D3DXVECTOR3 posLife, D3DXVECTOR3 posFrame);
	void SetLife(float fMaxLife) { m_info.fLife = fMaxLife; }
	void SetSizeGauge(float width, float height);
	void SetSizeFrame(float width, float height);
	void SetColor(D3DXCOLOR colLife, D3DXCOLOR colFrame);
	void BindTexture(const char* pTexPath_gauge, const char* pTexPath_frame);

	void Damage(float fDamage);

	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }

private:
	EState m_state;
	SInfo m_info;
	SUI m_ui;
	bool m_bDamage;
};

#endif

