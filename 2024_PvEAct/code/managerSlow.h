//===============================================================================
//
// スロー処理[slow.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _SLOW_MANAGER_H_
#define _SLOW_MANAGER_H_

// インクルード宣言
#include "main.h"
#include "object.h"

// スローマネージャクラス
class CSlowManager : public CObject
{
private:
	static const int MAX_OBJECT = 24;	// 最大数

public:
	enum ECamp	// 陣営
	{
		CAMP_NONE,
		CAMP_PAYER,
		CAMP_ENEMY,
		CAMP_MAX
	};

	enum ETag	// タグ
	{
		TAG_NONE = 0,
		TAG_PLAYER,
		TAG_ENEMY,
		TAG_MAX
	};

public:
	CSlowManager(int nCamp = CAMP_NONE);	// コンストラクタ
	~CSlowManager();	// デストラクタ

	// 生成
	static CSlowManager* Create(void);
	static CSlowManager* Create(ECamp camp, ETag tag);

	// プロトタイプ宣言
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	static void ReleaseAll(void);

	void Player(float fValue);
	void Enemy(float fValue);
	void Tag(ECamp camp, ETag tag, float fValue);
	void All(float fValue);	// 全体をスロー
	void Default(void);	// デフォルト

	// 設定
	void SetTag(ETag tag) { m_tag = tag; }	// タグ
	void SetCamp(ECamp camp) { m_camp = camp; }	// 所属陣営
	void SetValue(float fValue) { m_fValue = fValue; }

	// 取得
	ETag GetTag(void) const { return m_tag; }
	ECamp GetCamp(void) const { return m_camp; }
	float GetValue(void) const { return m_fValue; }
	CSlowManager** GetInfo(void) { return &m_apSlow[0][0]; }

private:
	// メンバ変数
	static CSlowManager* m_pManager;
	ETag m_tag;	// タグ
	float m_fValue;	// 重要
	float m_fCount;

	// 情報
	static CSlowManager* m_apSlow[CAMP_MAX][MAX_OBJECT];	// 管理用
	ECamp m_camp;	// 所属
	int m_nID;	// それぞれのID
	float m_fTime;
	static int m_nNumAll;	// 総数
};

#endif