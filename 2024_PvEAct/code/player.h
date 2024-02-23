//===============================================================================
//
// [player.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _PLAYER_H_				//このマクロ定義がされていなかったら
#define _PLAYER_H_				//2重インクルード防止のマクロを定義

#include "main.h"
#include "characterDiv.h"

//前方宣言
class CCollision;
class CMotionDiv;
class CLockon;
class CJanken;
class CSphereCollider;
class CWepon;
class CSlowManager;

//プレイヤークラス
class CPlayer : public CCharacterDiv
{
public:
	// 下半身モーションの状態
	enum ELowerMotion
	{
		LOWER_MOTION_NORMAL = 0,	// 通常
		LOWER_MOTION_RUN,			// 走る
		LOWER_MOTION_DASH,			// 前ダッシュ
		LOWER_MOTION_ATTACK00,
		LOWER_MOTION_ATTACK01,
		LOWER_MOTION_MAX
	};
	// 上半身モーションの状態
	enum EUpperMotion
	{
		UPPER_MOTION_NORMAL = 0,	// 通常
		UPPER_MOTION_RUN,			// 走る
		UPPER_MOTION_DASH,			// 前ダッシュ
		UPPER_MOTION_ATTACK00,		// 攻撃
		UPPER_MOTION_ATTACK01,		// 攻撃
		UPPER_MOTION_MAX
	};

	// モーション情報の構造体
	struct SMotionInfo
	{
		CMotionDiv* pMotionDiv;	// 分割モーションポインタ
	};

	// モーションの使用状態の構造体
	struct SIsMotion
	{
		bool bAvoidance;	// 回避
		bool bAttack;	// 攻撃1
		bool bAttack1;	// 攻撃2
	}; 

	struct SIsGameTrigger
	{
		bool bJust;
		bool bRush;	// ラッシュするかどうか
	};

	// 基本情報の構造体
	struct SInfo
	{
		D3DXVECTOR3 moveAvo;	// 回避
		SIsMotion isMotionList;	// モーションの使用状態
		float fLife;	// 体力
		float fRotDest;	//目標の向き
		float fRotDiff;	//目標と現在の差分
	};

	struct SAvoid
	{
		float fTime;	// 回避時間
		float fAngleMove;	// 角度の移動量
		float fDiff;	// 
		float fDest;	// 
		float fTempLength;
		D3DXVECTOR3 move;
	};

public:
	CPlayer(int nPriority = 3);	// コンストラクタ
	~CPlayer();	// デストラクタ

	static CPlayer* Create(D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f}, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });
	static CPlayer* GetInstance(void);
	static void Release(void);

	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Hit(float fDamage);

	// 設定用
	void SetIsJust(bool bJust) { m_isTriger.bJust = bJust; }

	// 取得用
	SIsMotion IsMotion(void) { return m_info.isMotionList; }
	float GetAvoidTime(void) { return m_avoid.fTime; }
	bool GetIsJust(void) { return m_isTriger.bJust; }
	float GetLife(void) { return m_info.fLife; }
	SIsGameTrigger GetIsGameTriger(void) { return m_isTriger; }

private:
	// メンバ関数
	void BattleNormal(void);
	void BattleJust(void);
	void BattleRush(void);
	void BattleJanken(void);

	void ManageInput(void);
	void InputMove(void);
	void InputAttack(void);
	void InputJanken(void);
	void InputAvoidance(void);

	void Lockon(void);

	void Attack(void);
	bool IsAttack(bool bAway);

	void Evasion(void);	// 回避
	void AfterImage(void);
	void RotDest(void); // 目的
	// モーション
	void ManageMotion(void);
	void LowerMotion(void);
	void UpperMotion(void);

	void RimitPos(D3DXVECTOR3* pPos);
	void RotAor(void);
	void Debug(void);

	// メンバ変数
	static CPlayer* m_pPlayer;
	SInfo m_info;	// 基本情報
	SMotionInfo m_motionInfo;	// モーション状態
	CSphereCollider* m_pSphereCollider;	//当たり判定
	CLockon* m_pLockon;	// ロックオンポインタ
	CJanken* m_pJanken;	// じゃんけんポインタ
	CSlowManager* m_pSlow;
	CCharacterDiv* m_pAfterImage;	// 残像
	bool m_bLockon;	// ロックオンするかどうか
	float m_fCntDash;
	float m_fTempLength;
	SIsGameTrigger m_isTriger;
	CWepon* m_pWepon;
	SAvoid m_avoid;
};

#endif