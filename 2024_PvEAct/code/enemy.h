//===============================================================================
//
// [enemy.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _ENEMY_H_	//このマクロ定義がされていなかったら
#define _ENEMY_H_	//2重インクルード防止のマクロを定義

#include "main.h"
#include "object.h"

//前方宣言
class CCollision;
class CMotionDiv;

//プレイヤークラス
class CEnemy : public CObject
{
public:
	// 上半身モーションの状態
	enum EUpperMotion
	{
		UPPER_MOTION_NORMAL = 0,	// 通常
		UPPER_MOTION_RUN,			// 走る
		UPPER_MOTION_MAX
	};
	// 下半身モーションの状態
	enum ELowerMotion
	{
		LOWER_MOTION_NORMAL = 0,	// 通常
		LOWER_MOTION_RUN,			// 走る
		LOWER_MOTION_MAX
	};

	// モーション情報の構造体
	struct SMotionInfo
	{
		EUpperMotion upperState;	// 上半身
		ELowerMotion lowerState;	// 下半身
		CMotionDiv* pMotionDiv;	// 分割モーションポインタ
	};

	// 基本情報の構造体
	struct SInfo
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	//過去位置
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 move;	//移動量
		float fRotDest;	//目標の角度
		float fRotDiff;	//目標と現在の差分
	};

public:
	CEnemy(int nPriority = 1);	// コンストラクタ
	~CEnemy();	// デストラクタ

	static CEnemy* Create(D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f}, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });
	static CEnemy* GetInstance(void);
	static void Release(void);

	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定用
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }

	//取得用
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	D3DXVECTOR3 GetMove(void) { return m_info.move; }

private:
	// メンバ関数
	void RotAor(void);

	// メンバ変数
	SInfo m_info;	// 基本情報
	SMotionInfo m_motionInfo;	// モーション状態
	CCollision* m_pCollision;	//当たり判定

	static CEnemy* m_pEnemy;
};

#endif