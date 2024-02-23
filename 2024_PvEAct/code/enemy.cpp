//===========================================================================================
//
// [player3D.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <stdio.h>
#include "enemy.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputJoypad.h"
#include "debugproc.h"
#include "camera.h"
#include "motionDiv.h"
#include "general.h"

// 定数定義
namespace
{
	static const float MOVE_SPEED = 5.5f;	// 移動量
	static const float ATT = 0.5f;	// 移動量の減衰
	static const float ADJUST_ROT = 0.08f;	// 向きの補正
	static const float GRAVITY = 1.0f;	// 重力
}

// 静的メンバ変数宣言
CEnemy* CEnemy::m_pEnemy = nullptr;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CEnemy::CEnemy(int nPriority)
{
	// 値のクリア
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_motionInfo, sizeof(m_motionInfo));
	
	m_pCollision = nullptr;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CEnemy::~CEnemy()
{

}

//===========================================================================================
// 生成
//===========================================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pEnemy == nullptr)
	{
		m_pEnemy = new CEnemy;

		if (m_pEnemy != nullptr)
		{
			m_pEnemy->Init(pos, rot);

			return m_pEnemy;
		}
	}

	return m_pEnemy;
}

//===========================================================================================
// 生成
//===========================================================================================
CEnemy* CEnemy::GetInstance(void)
{
	if (m_pEnemy == nullptr)
	{
		assert(("エネミー情報の取得に失敗", false));
	}

	return m_pEnemy;
}

//===========================================================================================
// 破棄
//===========================================================================================
void CEnemy::Release(void)
{
	if (m_pEnemy == nullptr)
	{
		return;
	}
	else
	{
		m_pEnemy->Uninit();

		delete m_pEnemy;
		m_pEnemy = nullptr;
	}
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CEnemy::Init(void)
{
	return S_OK;
}

//===========================================================================================
// 初期化処理(オーバーロード)
//===========================================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//m_motionInfo.pMotionDiv = CMotionDiv::Create("data\\TEXT\\motion\\motion_playerUpper.txt", "data\\TEXT\\motion\\motion_playerLower.txt");

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CEnemy::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CEnemy::Update(void)
{	
	// 移動量を更新(減衰させる)
	m_info.move.x += (0.0f - m_info.move.x) * ATT;
	m_info.move.z += (0.0f - m_info.move.z) * ATT;

	// 移動量の代入
	m_info.pos += m_info.move;

	// 反映
	if (m_motionInfo.pMotionDiv != nullptr)
	{
		m_motionInfo.pMotionDiv->SetPosition(m_info.pos);
		m_motionInfo.pMotionDiv->SetRotation(m_info.rot);
	}

	//デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print("【プレイヤー情報】\n");
	CManager::GetInstance()->GetDebugProc()->Print("位置： x:%f y:%f z:%f\n", m_info.pos.x, m_info.pos.y, m_info.pos.z);
	CManager::GetInstance()->GetDebugProc()->Print("向き： x:%f y:%f z:%f\n", m_info.rot.x, m_info.rot.y, m_info.rot.z);
	CManager::GetInstance()->GetDebugProc()->Print("移動量： x:%f y:%f z:%f\n\n", m_info.move.x, m_info.move.y, m_info.move.z);
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CEnemy::Draw(void)
{

}

//===========================================================================================
// 向き補正
//===========================================================================================
void CEnemy::RotAor(void)
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