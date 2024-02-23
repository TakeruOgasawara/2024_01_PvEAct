//===========================================================================================
//
// [player3D.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <stdio.h>
#include "enemyBoss.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputJoypad.h"
#include "debugproc.h"
#include "camera.h"
#include "motion.h"
#include "general.h"
#include "colliderSphere.h"
#include "managerSlow.h"

#include "objectX.h"
#include "modelParts.h"
#include "player.h"
#include "fade.h"
#include "scene.h"
#include "managerEvent.h"
#include "object3D.h"

// 定数定義
namespace
{
	static const float MOVE_SPEED = 5.5f;	// 移動量
	static const float ATT = 0.5f;	// 移動量の減衰
	static const float ADJUST_ROT = 0.08f;	// 向きの補正
	static const float GRAVITY = 1.0f;	// 重力
	static const float LIFE = 0.3f;

	static const float ATTACK_LENGTH = 300.0f;	// 攻撃判定の距離
	static const float ATTACK_RANGE = 0.6f;	// 攻撃判定の範囲
	static const float BLOWN_AWAY = 10.0f;	// 吹っ飛び率

}

// 静的メンバ変数宣言
CEnemyBoss* CEnemyBoss::m_pEnemy = nullptr;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CEnemyBoss::CEnemyBoss(int nPriority)
{
	// 値のクリア
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_motionInfo, sizeof(m_motionInfo));
	m_bAttack = false;
	m_pSphereCollider = nullptr;
	m_pObjectX = nullptr;
	m_pAttackRange = nullptr;
	m_pSlow = nullptr;
	m_DiffLife = 0.0f;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CEnemyBoss::~CEnemyBoss()
{

}

//===========================================================================================
// 生成
//===========================================================================================
CEnemyBoss *CEnemyBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pEnemy == nullptr)
	{
		m_pEnemy = new CEnemyBoss;

		if (m_pEnemy != nullptr)
		{
			m_pEnemy->Init(pos, rot);

			return m_pEnemy;
		}
	}

	return m_pEnemy;
}

//===========================================================================================
// インスタンスの取得
//===========================================================================================
CEnemyBoss* CEnemyBoss::GetInstance(void)
{
	if (m_pEnemy == nullptr)
	{
		return nullptr;
		//assert(("エネミー情報の取得に失敗", false));
	}

	return m_pEnemy;
}

//===========================================================================================
// 破棄
//===========================================================================================
void CEnemyBoss::Release(void)
{
	if (m_pEnemy == nullptr)
	{
		return;
	}
	else
	{
		//m_pEnemy->Uninit();

		delete m_pEnemy;
		m_pEnemy = nullptr;
	}
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CEnemyBoss::Init(void)
{
	return S_OK;
}

//===========================================================================================
// 初期化処理(オーバーロード)
//===========================================================================================
HRESULT CEnemyBoss::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_info.fLife = LIFE;

	CCharacter::Init("data\\TEXT\\motion\\enemyBoss\\motion.txt");

	SetMotion(MOTION_NORMAL);

	// スロー生成
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_ENEMY, CSlowManager::TAG_ENEMY);

	// 当たり判定の生成
	m_pSphereCollider = CSphereCollider::Create(CSphereCollider::TAG_ENEMY_BOSS, CSphereCollider::TYPE_SPHERE, this);

	if (m_pSphereCollider != nullptr)
	{
		m_pSphereCollider->SetRadius(40.0f);
	}

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CEnemyBoss::Uninit(void)
{
	m_pEnemy = nullptr;

	CCharacter::Uninit();

	// 自分自身の破棄
	CObject::Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CEnemyBoss::Update(void)
{
	if (GetBody() == nullptr)
	{
		return;
	}

	// 変数宣言
	float fSlow = 1.0f;

	CCharacter::Update();

	// ゲームモードの取得
	CManagerEvent* pEvent = CManagerEvent::GetInstance();

	if (pEvent != nullptr)
	{
		// 戦闘の状態
		switch (pEvent->GetMode())
		{
		case pEvent->MODE_NORMAL:
			BattleNormal();
			break;
		case pEvent->MODE_JUST:
			BattleJust();
			break;
		case pEvent->MODE_RUSH:
			BattleRush();
			break;
		case pEvent->MODE_JANKEN:
			BattleJanken();
			break;
		default:
			break;
		}

		// モーション設定
		//Motion();

		// 移動量を更新(減衰させる)
		m_info.move.x += (0.0f - m_info.move.x) * ATT;
		m_info.move.z += (0.0f - m_info.move.z) * ATT;

		if (m_pSlow != nullptr)
		{
			fSlow = m_pSlow->GetValue();
		}

		// モーションのスロー
		GetBody()->SetMmultiply(fSlow);

		// 移動量の代入
		m_info.pos += m_info.move * fSlow;

		if (m_pSphereCollider != nullptr)
		{
			m_pSphereCollider->SetPosition(m_info.pos);
		}

		// 反映
		if (m_motionInfo.pMotion != nullptr)
		{
			m_motionInfo.pMotion->SetPosition(m_info.pos);
			m_motionInfo.pMotion->SetRotation(m_info.rot);
		}

		if (m_info.fLife == 0.0f)
		{
			CManager::GetInstance()->GetFade()->Set(CScene::MODE_RESULT);
		}
	}
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CEnemyBoss::Draw(void)
{
	CCharacter::Draw();

	//if (m_motionInfo.pMotion != nullptr)
	//{// マトリックスの掛け合わせ
	//	general::SetMatrix(&m_info.mtxWorld, m_motionInfo.pMotion->GetPosition());
	//}

	// デバッグ表示
	Debug();
}

//===========================================================================================
// ヒット処理
//===========================================================================================
void CEnemyBoss::Hit(float fDamage)
{
	m_DiffLife = fDamage;
	m_info.fLife -= fDamage;

	if (m_info.fLife <= 0.0f)
	{
		m_info.fLife = 0.0f;

		m_info.state = STATE_DEATH;
	}
}

//===========================================================================================
// 通常
//===========================================================================================
void CEnemyBoss::BattleNormal(void)
{
	//Move();
	Rot();
	//Attack();
}

//===========================================================================================
// ジャスト回避
//===========================================================================================
void CEnemyBoss::BattleJust(void)
{

}

//===========================================================================================
// ラッシュ
//===========================================================================================
void CEnemyBoss::BattleRush(void)
{

}

//===========================================================================================
// じゃんけん
//===========================================================================================
void CEnemyBoss::BattleJanken(void)
{

}

//===========================================================================================
// 向き補正
//===========================================================================================
void CEnemyBoss::RotAor(void)
{
	m_info.fRotDiff = m_info.fRotDest - m_info.rot.y;	//目標の角度までの差分を求める

	if (m_info.fRotDiff > D3DX_PI)
	{
		m_info.fRotDiff -= (D3DX_PI * 2);
	}
	if (m_info.fRotDiff < -D3DX_PI)
	{
		m_info.fRotDiff += (D3DX_PI * 2);
	}

	m_info.rot.y += m_info.fRotDiff * ADJUST_ROT;	//補正する

	// 向きの修正
	general::RotLimit(&m_info.rot);
}

//===========================================================================================
// 攻撃
//===========================================================================================
void CEnemyBoss::Attack(void)
{
	int nKey = GetBody()->GetKey();	// 現在のキー
	int nNumKey = GetBody()->GetMotionInfo(GetType())->nNumKey;	// キーの総数
	int nTypeMotion = GetBody()->GetType();
	float fTick = CManager::GetInstance()->GetTick() * 10.0f;
	bool bFinish = GetBody()->GetMotion().bFinish;

	if (nTypeMotion == MOTION_ATTACK00)
	{
		if (bFinish)
		{
			m_bAttack = true;
		}

		if (nKey == 2)
		{
			IsAttack();
		}
		if (nKey >= 1)
		{
			// 移動
			m_info.move.x = sinf(m_info.rot.y + D3DX_PI) * fTick * 10.0f;
			m_info.move.z = cosf(m_info.rot.y + D3DX_PI) * fTick * 10.0f;
		}
	}
}

//===========================================================================================
// 攻撃判定
//===========================================================================================
bool CEnemyBoss::IsAttack(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return false;
	}

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 movePlayer = pPlayer->GetMovement();

	float vec1 = 0.0f, vec2 = 0.0f;	// ベクトル
	D3DXVECTOR3 pos0 = {};	// 始点
	D3DXVECTOR3 pos1 = {}, pos2 = {};	// 終点1/終点2
	D3DXVECTOR3 vecLine = {}, vecToPos = {};	// 始点と終点の距離/始点から対象の距離
	D3DXVECTOR3 vecLine1 = {};	// 始点と終点の距離

	// 攻撃範囲の設定
	float fLength = sqrtf
	(
		(posPlayer.x - m_info.pos.x) * (posPlayer.x - m_info.pos.x) +
		(posPlayer.z - m_info.pos.z) * (posPlayer.z - m_info.pos.z)
	);

	// 角度を求める
	float fAngle = atan2f((posPlayer.x - m_info.pos.x), (posPlayer.z - m_info.pos.z));

	// 範囲内判定
	if (fLength < ATTACK_LENGTH)
	{
		// 始点の計算
		pos0.x = m_info.pos.x;
		pos0.y = m_info.rot.y;
		pos0.z = m_info.pos.z;
		// 終点の計算
		pos1.x = pos0.x + (sinf(m_info.rot.y + D3DX_PI * ATTACK_RANGE));
		pos1.y = 0.0f;
		pos1.z = pos0.z + (cosf(m_info.rot.y + D3DX_PI * ATTACK_RANGE));
		// 終点1の計算
		pos2.x = pos0.x + (sinf(m_info.rot.y + -D3DX_PI * ATTACK_RANGE));
		pos2.y = 0.0f;
		pos2.z = pos0.z + (cosf(m_info.rot.y + -D3DX_PI * ATTACK_RANGE));
		
		// 始点と終点との距離間
		vecLine = pos1 - pos0;
		vecLine1 = pos2 - pos0;

		// 始点から対象までの位置
		vecToPos = posPlayer - pos0;

		// ベクトル方向
		vec1 = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
		vec2 = (vecLine1.z * vecToPos.x) - (vecLine1.x * vecToPos.z);

		if (vec1 > 0 && vec2 < 0)
		{
			CManager::GetInstance()->GetDebugProc()->Print("\n【【判定内】】\n");

			//if (m_bAttack)
			// ジャスト回避判定
			if (pPlayer->IsMotion().bAvoidance)
			{
				//float time = pPlayer->GetAvoidanceTime();

				//if (time > 1.8f)
				//{
				//	//pPlayer->SetIsJust(true);
				//	//CManagerGameState::GetInstance()->SetMode(CManagerGameState::MODE_JUST);
				//}

				m_bAttack = false;
				return false;
			}
			else if (m_bAttack)
			{
				// 吹き飛ばし
				pPlayer->SetMovement(
					{
						sinf(fAngle) * BLOWN_AWAY,
						0.0f,
						cosf(fAngle) * BLOWN_AWAY,
					});

				pPlayer->Hit(0.1f);

				return true;
			}
		}
	}
	return false;
}

//===========================================================================================
// 向き
//===========================================================================================
void CEnemyBoss::Rot(void)
{
	// 情報の取得
	CPlayer* pPlayer = CPlayer::GetInstance();	// ボス
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// ボス位置

	// 角度を求める
	float fAngle = atan2f((m_info.pos.x - posPlayer.x), (m_info.pos.z - posPlayer.z));

	// ボスの方向を向く
	m_info.rot.y = fAngle;
}

//===========================================================================================
// 行動
//===========================================================================================
void CEnemyBoss::Move(void)
{
	// 情報の取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();	// カメラ
	CPlayer* pPlayer = CPlayer::GetInstance();	// ボス
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// ボス位置

	// 角度を求める
	float fAngle = atan2f((m_info.pos.x - posPlayer.x), (m_info.pos.z - posPlayer.z));

	// 長さを求める
	float fLength = sqrtf((m_info.pos.x - posPlayer.x) * (m_info.pos.x - posPlayer.x) + (m_info.pos.z - posPlayer.z) * (m_info.pos.z - posPlayer.z));

	//敵とプレイヤーの距離を求める
	D3DXVECTOR3 e = 
	{
		(posPlayer.x - m_info.pos.x),
		0.0f,
		(posPlayer.z - m_info.pos.z)
	};

	//移動量に代入
	m_info.move.x += e.x;
	m_info.move.z += e.z;

	//移動量を正規化する
	D3DXVec3Normalize(&m_info.move, &m_info.move);

	// 速度
	m_info.move.x *= MOVE_SPEED;
	m_info.move.z *= MOVE_SPEED;
}

//===========================================================================================
// 突進
//===========================================================================================
void CEnemyBoss::Rush(void)
{

}

//===========================================================================================
// モーション
//===========================================================================================
void CEnemyBoss::Motion(void)
{
	// 情報の取得
	CPlayer* pPlayer = CPlayer::GetInstance();	// ボス
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();	// ボス位置
	int nTypeMotion = GetBody()->GetType();


	// 角度を求める
	//float fAngle = atan2f((m_info.pos.x - posPlayer.x), (m_info.pos.z - posPlayer.z));

	// 長さを求める
	float fLength = sqrtf((m_info.pos.x - posPlayer.x) * (m_info.pos.x - posPlayer.x) + (m_info.pos.z - posPlayer.z) * (m_info.pos.z - posPlayer.z));

	if (fLength < ATTACK_LENGTH)
	{// 範囲内にきたら攻撃モーション
		if (nTypeMotion != MOTION_ATTACK00)
		{
			SetMotion(MOTION_ATTACK00);
		}
	}

	if (m_bAttack && GetBody()->GetMotion().bFinish)
	{
		if (nTypeMotion != MOTION_NORMAL)
		{
			SetMotion(MOTION_NORMAL);
		}
	}
}

//===========================================================================================
// デバッグ表示
//===========================================================================================
void CEnemyBoss::Debug(void)
{
#ifndef _DEBUG
	return;
#endif

	//デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print("【敵情報】\n");
	CManager::GetInstance()->GetDebugProc()->Print("位置： x:%f y:%f z:%f\n", m_info.pos.x, m_info.pos.y, m_info.pos.z);
	CManager::GetInstance()->GetDebugProc()->Print("向き： x:%f y:%f z:%f\n", m_info.rot.x, m_info.rot.y, m_info.rot.z);
	CManager::GetInstance()->GetDebugProc()->Print("移動量： x:%f y:%f z:%f\n", m_info.move.x, m_info.move.y, m_info.move.z);
	CManager::GetInstance()->GetDebugProc()->Print("体力： %f\n\n", m_info.fLife);
}
