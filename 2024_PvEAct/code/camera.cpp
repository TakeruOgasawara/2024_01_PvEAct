//===========================================================================================
//
// [camera.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
#include "inputJoypad.h"
#include "debugproc.h"
#include "general.h"
#include "player.h"
#include "enemyBoss.h"

//********************************************************
// 定数宣言
//********************************************************
namespace
{
	D3DXVECTOR3 POS_V = { 0.0f, 200.0f, -400.0f };	// 視点位置
	D3DXVECTOR3 POS_R = { 0.0f, 100.0f, 0.0f };	// 注視点位置
	D3DXVECTOR3 ROT = { 0.0f, 0.0f, 0.0f };	// 向き
	float LENGTH = 300.0f;	// 視点と注視点の距離
	float MAE = 0.0f;	// 対象から注視点との距離
}

//===========================================================================================
// コンストラクタ
//===========================================================================================
CCamera::CCamera()
{
	m_posV = { 0.0f, 0.0f, 0.0f };
	m_posR = { 0.0f, 0.0f, 0.0f };
	m_vecU = { 0.0f, 0.0f, 0.0f };
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_rotOld = { 0.0f, 0.0f, 0.0f };
	m_move = { 0.0f, 0.0f, 0.0f };
	m_posVDest = { 0.0f, 0.0f, 0.0f };
	m_posRDest = { 0.0f, 0.0f, 0.0f };
	m_fLength = 0.0f;
	m_fShakeTime = 0.0f;

	ZeroMemory(&m_shakeInfo, sizeof(m_shakeInfo));
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CCamera::~CCamera()
{

}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CCamera::Init(void)
{
	m_posV = POS_V;
	m_posR = POS_R;
	m_vecU = { 0.0f, 1.0f, 0.0f };
	m_rot.x = -0.6f;
	m_rot.z = -0.6f;
	m_fLength = LENGTH;

	//SetShake(10.0f, 2.0f);

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CCamera::Uninit(void)
{

}

//===========================================================================================
// 更新処理
//===========================================================================================
void CCamera::Update(void)
{
	// 向きの制限
	RotLimit();
	//MouseManage();

	//ShakeUpdate();

	CManager::GetInstance()->GetDebugProc()->Print("\n【カメラ情報】\n");
	CManager::GetInstance()->GetDebugProc()->Print("視点： x:%f y:%f z:%f\n", m_posV.x, m_posV.y, m_posV.z);
	CManager::GetInstance()->GetDebugProc()->Print("注視点： x:%f y:%f z:%f\n", m_posR.x, m_posR.y, m_posR.z);
	CManager::GetInstance()->GetDebugProc()->Print("向き： x:%f, y:%f z:%f\n", m_rot.z, m_rot.y, m_rot.z);
	CManager::GetInstance()->GetDebugProc()->Print("距離間： %f\n\n", m_fLength);
}

//===========================================================================================
// カメラ設定
//===========================================================================================
void CCamera::SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),	// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// 画面のアスペクト比
		10.0f,				// z値の最小値
		10000.0f);			// z値の最大値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,	// 視点
		&m_posR,	// 注視点
		&m_vecU);	// 情報ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//===========================================================================================
// マウス操作
//===========================================================================================
void CCamera::MouseManage(void)
{
	// 情報の取得
	CInputMouse* pInputMouse = CInputMouse::GetInstance();

	D3DXVECTOR3 move = pInputMouse->GetMove();

	if (pInputMouse->GetPress(pInputMouse->MOUSE_LEFT))
	{
		// 視点の移動
		m_rot.z += -move.y * 0.004f;
		m_rot.y += move.x * 0.004f;

		// 視点の更新処理
		ViewpointOffset();
	}
	if (pInputMouse->GetPress(pInputMouse->MOUSE_RIGHT))
	{
		// 視点の移動
		m_rot.z += -move.y * 0.004f;
		m_rot.y += move.x * 0.004f;

		// 視点の更新処理
		RegardpointOffset();
	}

	// 視点・注視点との距離変更
	if (move.z >= 0.001f || move.z <= -0.001f)
	{
		if (m_fLength >= 100.0f)
		{
			// 視点の移動
			m_fLength += -move.z * 0.2f;

			// 視点の更新処理
			ViewpointOffset();
		}
		else
		{
			m_fLength = 100.0000000001f;
		}
	}

	// 向きの修正
	general::RotLimit(&m_rot);
}

//===========================================================================================
// ジョイパッド操作
//===========================================================================================
void CCamera::JoypadViewOperation(float fVertSpeed, float fHorizontalSpeed)
{
	// 情報の取得
	CInputJoypad* pInputJoypad = CInputJoypad::GetInstance();

	// カメラ向きの変更
	if (pInputJoypad->GetJoyStickRX(0) > 0)
	{// スティック右入力
		m_rot.y += fHorizontalSpeed;
	}
	if (pInputJoypad->GetJoyStickRX(0) < 0)
	{// スティック左入力
		m_rot.y -= fHorizontalSpeed;
	}
	if (pInputJoypad->GetJoyStickRY(0) > 0)
	{// スティック上入力
		m_rot.z += fVertSpeed;
	}
	if (pInputJoypad->GetJoyStickRY(0) < 0)
	{// スティック下入力
		m_rot.z -= fVertSpeed;
	}

	// 視点の補正
	ViewpointOffset();
}

//===========================================================================================
// プレイヤーの追従処理
//===========================================================================================
void CCamera::PlayerFollow(void)
{
	// ポインタ情報の取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	// 変数宣言
	D3DXVECTOR3 posRDiff = {}, posVDiff = {};

	// 目的の視点 / 注視点を設定
	m_posVDest =
	{
		m_posRDest.x + (cosf(m_rot.z) * sinf(m_rot.y)) * -m_fLength,
		pPlayer->GetPosition().y + POS_V.y,
		m_posRDest.z + (cosf(m_rot.z) * cosf(m_rot.y)) * -m_fLength
	};
	m_posRDest = 
	{
		pPlayer->GetPosition().x + sinf(pPlayer->GetRotation().y + D3DX_PI) * MAE,
		pPlayer->GetPosition().y + POS_R.y,
		pPlayer->GetPosition().z + cosf(pPlayer->GetRotation().y + D3DX_PI) * MAE
	};

	posRDiff = m_posRDest - m_posR;	// 注視点の差分
	posVDiff = m_posVDest - m_posV;	// 視点の差分

	m_posR += posRDiff * 0.07f;	// 注視点を補正する
	m_posV += posVDiff * 0.08f;	// 視点を補正する
}

//===========================================================================================
// プレイヤーの背面を自動的に向く処理
//===========================================================================================
void CCamera::PlayerLockon(float fOffset)
{
	// 変数宣言
	float fRotDiff = 0.0f;	// 差分保存用

	// 情報の取得
	CPlayer* pPlayer = CPlayer::GetInstance();
	
	// 差分を計算
	fRotDiff = (pPlayer->GetRotation().y + D3DX_PI) - m_rot.y;

	// 目標の角度の補正
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2;
	}
	if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}

	m_rot.y += fRotDiff * fOffset;	//補正する

	// 角度の補正
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	// カメラを更新する
	ViewpointOffset();
}

//===========================================================================================
// 注視点補正
//===========================================================================================
void CCamera::RegardpointOffset(void)
{
	m_posR.x = m_posV.x + (cosf(m_rot.z) * sinf(m_rot.y)) * m_fLength;
	m_posR.y = m_posV.y + sinf(m_rot.z) * m_fLength;
	m_posR.z = m_posV.z + (cosf(m_rot.z) * cosf(m_rot.y)) * m_fLength;
}

//===========================================================================================
// 視点補正
//===========================================================================================
void CCamera::ViewpointOffset(void)
{
	m_posV.x = m_posR.x + (cosf(m_rot.z) * sinf(m_rot.y)) * -m_fLength;
	m_posV.y = m_posR.y + sinf(m_rot.z) * -m_fLength;
	m_posV.z = m_posR.z + (cosf(m_rot.z) * cosf(m_rot.y)) * -m_fLength;
}

//===========================================================================================
// 固定カメラ
//===========================================================================================
void CCamera::FixedJanken(void)
{
	// 変数宣言
	float fRotDest = 0.0f;	// 差分保存用
	float fRotDiff = 0.0f;	// 差分保存用

	// 情報の取得
	CPlayer* pPlayer = CPlayer::GetInstance();
	CEnemyBoss* pBoss = CEnemyBoss::GetInstance();
	D3DXVECTOR3 posPlayer = {}, posBoss = {};
	D3DXVECTOR3 rotPlayer = {}, rotBoss = {};

	if (pPlayer == nullptr || pBoss == nullptr)
	{
		return;
	}
	
	// 位置情報の取得
	posPlayer = pPlayer->GetPosition();
	rotPlayer = pPlayer->GetRotation();
	posBoss = pBoss->GetPosition();
	rotBoss = pBoss->GetRotation();

	// 差分を計算
	fRotDest = (rotPlayer.y + D3DX_PI * 0.85f);

	fRotDiff = fRotDest - m_rot.y;	// 目標の角度までの差分を求める

	general::RotRevi(&fRotDiff);

	// 視点の設定
	//m_posV.x = m_posV.x + sinf(pPlayer->GetRotation().y + D3DX_PI/* * 2.5f*/) * 50.0f;
	//m_posV.y = 500.0f;
	//m_posV.z = m_posV.z + cosf(pPlayer->GetRotation().y + D3DX_PI/* * 2.5f*/) * 50.0f;

	// 注視点の設定
	//SetPosR(posBoss);

	// 視点と注視点の距離
	m_fLength = 300.0f;

	m_rot.y += fRotDiff * 0.2f;	//補正する

	// カメラを更新する
	ViewpointOffset();
	RegardpointOffset();
}

//===========================================================================================
// 視点向きの制限
//===========================================================================================
void CCamera::RotLimit(void)
{
	// 現在の向きを代入
	m_rotOld = m_rot;

	if (m_rot.x < -(D3DX_PI * 0.45f) || m_rot.z < -(D3DX_PI * 0.45f))
	{
		m_rot.z = -(D3DX_PI * 0.45f);
		m_rot.x = -(D3DX_PI * 0.45f);
	}
	else if (m_rot.x > (D3DX_PI * 0.15f) || m_rot.z > (D3DX_PI * 0.15f))
	{
		m_rot.x = (D3DX_PI * 0.15f);
		m_rot.z = (D3DX_PI * 0.15f);
	}
}

//===========================================================================================
// シェイク設定
//===========================================================================================
void CCamera::SetShake(float intensity, float time)
{
	m_shakeInfo.fIntensity = intensity;
	m_shakeInfo.fTime = time;
	m_shakeInfo.fStartTime = (float)clock();
}

//===========================================================================================
// シェイクの更新
//===========================================================================================
void CCamera::ShakeUpdate(void)
{
	if (m_shakeInfo.fTime > 0.0f)
	{
		// シェイク中の処理
		float fElapsed = ((float)clock() - m_shakeInfo.fStartTime) / (float)CLOCKS_PER_SEC;

		if (fElapsed < m_shakeInfo.fTime)
		{
			// ランダムな値でカメラをシフトする
			m_posV += (D3DXVECTOR3(
				(rand() / (float)RAND_MAX - 0.5f) * m_shakeInfo.fIntensity,
				(rand() / (float)RAND_MAX - 0.5f) * m_shakeInfo.fIntensity,
				(rand() / (float)RAND_MAX - 0.5f) * m_shakeInfo.fIntensity
			));
		}
		else
		{
			// シェイクが終了したらリセット
			m_shakeInfo.fTime = 0.0f;
			m_shakeInfo.fIntensity = 0.0f;
		}
	}
}
