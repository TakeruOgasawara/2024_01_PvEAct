//===============================================================================
//
// [enemy.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _ENEMYBOSS_H_	//このマクロ定義がされていなかったら
#define _ENEMYBOSS_H_	//2重インクルード防止のマクロを定義

#include "main.h"
#include "character.h"

//前方宣言
class CMotion;
class CObjectX;
class CObject3D;
class CSphereCollider;
class CSlowManager;

//プレイヤークラス
class CEnemyBoss : public CCharacter
{
public:
	// 上半身モーションの状態
	enum EMotion
	{
		MOTION_NORMAL = 0,	// 通常
		MOTION_ATTACK00,		// 攻撃
		MOTION_MAX
	};

	enum EState
	{
		STATE_SURVIVAL = 0,
		STATE_DEATH,
		STATE_MAX
	};

	// モーション情報の構造体
	struct SMotionInfo
	{
		CMotion* pMotion;	// 分割モーションポインタ
	};

	// 基本情報の構造体
	struct SInfo
	{
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	//過去位置
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 move;	//移動量
		EState state;
		float fRotDest;	//目標の角度
		float fRotDiff;	//目標と現在の差分
		float fLife;	// 体力
	};

public:
	CEnemyBoss(int nPriority = 3);	// コンストラクタ
	~CEnemyBoss();	// デストラクタ

	static CEnemyBoss* Create(D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f}, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });
	static CEnemyBoss* GetInstance(void);
	static void Release(void);

	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Hit(float fDamage);

	//設定用
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }
	void SetMovement(D3DXVECTOR3 move) { m_info.move = move; }
	void SetDamage(float fDamage) { m_DiffLife = fDamage; }

	//取得用
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	D3DXVECTOR3 GetMovement(void) const { return m_info.move; }
	D3DXMATRIX* GetMatrix(void) { return &m_info.mtxWorld; }
	float GetLife(void) { return m_info.fLife; }
	float GetDamage(void) { return m_DiffLife; }
	EState GetState(void) { return m_info.state; }

private:
	// メンバ関数
	void BattleNormal(void);
	void BattleJust(void);
	void BattleRush(void);
	void BattleJanken(void);

	void RotAor(void);
	void Attack(void);
	bool IsAttack(void);
	void Rot(void);		// 向き
	void Move(void);	// 移動
	void Rush(void);	// 突進
	void Stop(void);	// 停止

	void Motion(void);

	void Debug(void);
	
	// メンバ変数
	SInfo m_info;	// 基本情報
	SMotionInfo m_motionInfo;	// モーション状態
	CSphereCollider* m_pSphereCollider;	//当たり判定
	int m_fAttacCnt;
	CObjectX* m_pObjectX;
	CObject3D* m_pAttackRange;
	CSlowManager* m_pSlow;

	bool m_bAttack;
	float m_DiffLife;

	static CEnemyBoss* m_pEnemy;
};

#endif