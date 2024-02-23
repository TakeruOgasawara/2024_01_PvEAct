//===========================================================================================
//
// [player.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputJoypad.h"
#include "inputMouse.h"
#include "debugproc.h"
#include "camera.h"
#include "motionDiv.h"
#include "general.h"

#include "lockOn.h"
#include "enemyBoss.h"
#include "janken.h"
#include "colliderSphere.h"
#include "characterDiv.h"
#include "character.h"
#include "motion.h"

#include "wepon.h"
#include "modelParts.h"

#include "managerSlow.h"

#include "managerEvent.h"
#include "managerInput.h"

// 定数定義
namespace
{
	static const float MOVE_SPEED = 4.5f;	// 移動量
	static const float ATT = 0.6f;	// 移動量の減衰
	static const float ADJUST_ROT = 0.08f;	// 向きの補正
	static const float GRAVITY = 0.0f;	// 重力
	static const float LIFE = 10.0f;
	static const float ATTACK_LENGTH = 200.0f;	// 攻撃判定の距離
	static const float ATTACK_RANGE = 0.6f;	// 攻撃判定の範囲
	static const float BLOWN_AWAY = 10.0f;	// 吹っ飛び率

	static const float AVOIDANCE_TIME = 5.0f;	// 回避時間
	static const float AVOIDANCE_SPEED = 3.0f;	// 回避速度 (1.0f * AVOIDANCE_SPEED)

	static const D3DXVECTOR3 LIMIT_POS = { 1500.0f, 0.0f, 1500.0f };	// 行動範囲(自然数で入力)
}

// 静的メンバ変数宣言
CPlayer* CPlayer::m_pPlayer = nullptr;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CPlayer::CPlayer(int nPriority)
{
	// 値のクリア
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_motionInfo, sizeof(m_motionInfo));
	ZeroMemory(&m_isTriger, sizeof(m_isTriger));
	ZeroMemory(&m_avoid, sizeof(m_avoid));
	m_bLockon = false;
	m_fCntDash = 0.0f;
	m_fTempLength = 0.0f;
	m_pSphereCollider = nullptr;
	m_pLockon = nullptr;
	m_pJanken = nullptr;
	m_pWepon = nullptr;
	m_pAfterImage = nullptr;
	m_pSlow = nullptr;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CPlayer::~CPlayer()
{

}

//===========================================================================================
// 生成
//===========================================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CPlayer;

		if (m_pPlayer != nullptr)
		{
			m_pPlayer->Init(pos, rot);

			return m_pPlayer;
		}
	}

	return m_pPlayer;
}

//===========================================================================================
// 生成
//===========================================================================================
CPlayer* CPlayer::GetInstance(void)
{
	if (m_pPlayer == nullptr)
	{
		return nullptr;
		assert(("プレイヤー情報の取得に失敗", false));
	}

	return m_pPlayer;
}

//===========================================================================================
// 破棄
//===========================================================================================
void CPlayer::Release(void)
{
	if (m_pPlayer == nullptr)
	{
		return;
	}
	else
	{
		m_pPlayer->Uninit();

		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CPlayer::Init(void)
{
	return S_OK;
}

//===========================================================================================
// 初期化処理(オーバーロード)
//===========================================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	SetPosition(pos);
	SetRotation(rot);

	// プレイヤーモデル&モーションの読み込み
	CCharacterDiv::Init("data\\TEXT\\motion\\player\\motion_Upper.txt", "data\\TEXT\\motion\\player\\motion_Lower.txt");

	// 当たり判定を生成
	m_pSphereCollider = CSphereCollider::Create(CCollision::TAG_PLAYER, CCollision::TYPE_SPHERE, this);
	if (m_pSphereCollider != nullptr)
	{
		m_pSphereCollider->SetRadius(80.0f);
	}

	// スロー
	m_pSlow = CSlowManager::Create(CSlowManager::CAMP_PAYER, CSlowManager::TAG_PLAYER);

	// 体力設定
	m_info.fLife = LIFE;

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CPlayer::Uninit(void)
{
	m_pPlayer = nullptr;

	// 親クラスの終了
	CCharacterDiv::Uninit();

	// 自分自身の破棄
	CObject::Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CPlayer::Update(void)
{
	if (GetBody() == nullptr)
	{
		return;
	}

	// 情報の取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();	// キーボード
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();	// ジョイパッド
	CManagerEvent* pEvent = CManagerEvent::GetInstance();	// ゲーム状態
	float fSlow = 1.0f;	// スローの基準値
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 親クラスの更新
	CCharacterDiv::Update();

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
	}

	// トランスフォームの取得
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 move = GetMovement();

	// モーション
	ManageMotion();

	if (m_pSlow != nullptr)
	{// スロー値の取得
		fSlow = m_pSlow->GetValue();
	}

	// 行動制限
	RimitPos(&pos);

	// 移動量を更新(減衰させる)
	move.x += (0.0f - move.x) * ATT;
	move.z += (0.0f - move.z) * ATT;

	// 移動量の代入
	pos += move * fSlow;

	// カメラの追従
	if (pCamera != nullptr)
	{
		pCamera->PlayerFollow();
	}

	if (m_pSphereCollider != nullptr)
	{
		// 位置設定
		m_pSphereCollider->SetPosition(pos);
		// 円形の押し出し処理
		m_pSphereCollider->PushSphere(&pos, CCollision::TAG_OBJECT);
	}

	// トランスフォームの反映
	SetPosition(pos);
	SetMovement(move);
	SetRotation(rot);
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CPlayer::Draw(void)
{
	// 親クラスの描画
	CCharacterDiv::Draw();

	// デバッグ表示
	Debug();
}

//===========================================================================================
// ヒット処理
//===========================================================================================
void CPlayer::Hit(float fDamage)
{
	if (m_info.isMotionList.bAvoidance)
	{
		if (m_avoid.fTime > 1.8f)
		{
			// ゲームモードをジャスト回避
			CManagerEvent::GetInstance()->SetMode(CManagerEvent::MODE_JUST);
		}
	}

	m_info.fLife -= fDamage;

	if (m_info.fLife < 0.0f)
	{
		m_info.fLife = 0.0f;

		Uninit();
	}
}

//===========================================================================================
// 通常戦闘の時
//===========================================================================================
void CPlayer::BattleNormal(void)
{
	// 情報の取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 入力関係
	ManageInput();

	// モーション
	ManageMotion();

	// 攻撃
	Attack();

	// ロックオン
	Lockon();

	// 回避
	//Evasion();

	if (pCamera != nullptr)
	{
		// カメラの追従
		pCamera->PlayerFollow();

		if (!m_bLockon)
		{
			// カメラ
			pCamera->MouseManage();

			// パッドによるカメラ操作
			pCamera->JoypadViewOperation(0.03f, 0.06f);
		}
	}
}

//===========================================================================================
// ジャストの時
//===========================================================================================
void CPlayer::BattleJust(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	InputMove();

	m_info.isMotionList.bAvoidance = false;

	// 背後にカメラを設定
	pCamera->PlayerLockon(0.4f);

	CManagerEvent::GetInstance()->SetMode(CManagerEvent::MODE_RUSH);
}

//===========================================================================================
// ラッシュ戦闘時
//===========================================================================================
void CPlayer::BattleRush(void)
{
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();
	CInputMouse* pInputMouse = CInputMouse::GetInstance();

	if (pInpuJoyPad->GetTrigger(CInputJoypad::BUTTON_RB, 0))
	{
		InputAttack();
	}

	// 攻撃
	Attack();
}

//===========================================================================================
// じゃんけん戦闘の時
//===========================================================================================
void CPlayer::BattleJanken(void)
{
	if (m_pJanken == nullptr)
	{// じゃんけんの生成
		m_pJanken = CJanken::Create();
		m_pJanken->IsJanken(true);
	}

	if (m_pJanken != nullptr)
	{
		// じゃんけん用の固定カメラ
		CManager::GetInstance()->GetCamera()->FixedJanken();

		// カメラの追従
		CManager::GetInstance()->GetCamera()->PlayerFollow();

		// 目的の向きを向く（今回はボス）
		RotDest();

		// じゃんけんの入力
		InputJanken();

		if (m_pJanken->GetResult(m_pJanken->PLAYER_PLAYER) == m_pJanken->RESULT_WIN)
		{// 結果が勝ちだった場合
			Attack();	// 攻撃
		}

		if (!m_pJanken->IsJanken())
		{// 終了していたら

			if (m_pJanken != nullptr)
			{// 破棄
				m_pJanken->Uninit();
				m_pJanken = nullptr;
			}

			// 通常の戦闘状態へ
			//CManagerEvent::GetInstance()->SetMode(CManagerEvent::MODE_NORMAL);
		}
	}
}

//===========================================================================================
// じゃんけん攻撃
//===========================================================================================
void CPlayer::InputJanken(void)
{
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();
	CInputMouse* pInputMouse = CInputMouse::GetInstance();

	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_M) || pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_Y, 0))
	{
		m_pJanken->SetJanken(m_pJanken->JANKEN_GU);
	}
	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_N) || pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_A, 0))
	{
		m_pJanken->SetJanken(m_pJanken->JANKEN_CHOKI);
	}
	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_B) || pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_X, 0))
	{
		m_pJanken->SetJanken(m_pJanken->JANKEN_PA);
	}
}

//===========================================================================================
// 入力マネージャ
//===========================================================================================
void CPlayer::ManageInput(void)
{
	// 攻撃
	InputAttack();

	// 回避
	//InputAvoidance();

	if (!m_info.isMotionList.bAttack && !m_info.isMotionList.bAttack1 && !m_info.isMotionList.bAvoidance)
	{
		// 移動処理
		InputMove();
	}
}

//------------------------------------------------------------------------
// 移動入力
//------------------------------------------------------------------------
void CPlayer::InputMove(void)
{
	D3DXVECTOR3 rotCamera = CManager::GetInstance()->GetCamera()->GetRotation();
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad *pInpuJoyPad = CInputJoypad::GetInstance();
	float fTick = (CManager::GetInstance()->GetTick() * 100.0f);


	D3DXVECTOR3 move = GetMovement();

	// 移動処理
	if (pInputKeyboard->GetPress(DIK_A)|| pInpuJoyPad->GetJoyStickLX(0) < 0)
	{
		if (pInputKeyboard->GetPress(DIK_W) == true || pInpuJoyPad->GetJoyStickLY(0) > 0)
		{// 左上移動
			m_info.fRotDest = rotCamera.y + D3DX_PI * 0.75f;

			move.x += sinf(rotCamera.y + -D3DX_PI * 0.25f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + -D3DX_PI * 0.25f) * fTick * MOVE_SPEED;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInpuJoyPad->GetJoyStickLY(0) < 0)
		{// 左下移動
			m_info.fRotDest = rotCamera.y + D3DX_PI * 0.25f;

			move.x += sinf(rotCamera.y + -D3DX_PI * 0.75f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + -D3DX_PI * 0.75f) * fTick * MOVE_SPEED;
		}
		else
		{// 左移動
			m_info.fRotDest = rotCamera.y + D3DX_PI * 0.5f;

			move.x += sinf(rotCamera.y + -D3DX_PI * 0.5f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + -D3DX_PI * 0.5f) * fTick * MOVE_SPEED;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true || pInpuJoyPad->GetJoyStickLX(0) > 0)
	{
		if (pInputKeyboard->GetPress(DIK_W) == true || pInpuJoyPad->GetJoyStickLY(0) > 0)
		{//　右上移動
			m_info.fRotDest = rotCamera.y + -D3DX_PI * 0.75f;

			move.x += sinf(rotCamera.y + D3DX_PI * 0.25f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + D3DX_PI * 0.25f) * fTick * MOVE_SPEED;

		}
		else if (pInputKeyboard->GetPress(DIK_S) == true || pInpuJoyPad->GetJoyStickLY(0) < 0)
		{//　右下移動
			m_info.fRotDest = rotCamera.y + -D3DX_PI * 0.25f;

			move.x += sinf(rotCamera.y + D3DX_PI * 0.75f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + D3DX_PI * 0.75f) * fTick * MOVE_SPEED;
		}
		else
		{// 右移動
			m_info.fRotDest = rotCamera.y + -D3DX_PI * 0.5f;

			move.x += sinf(rotCamera.y + D3DX_PI * 0.5f) * fTick * MOVE_SPEED;
			move.z += cosf(rotCamera.y + D3DX_PI * 0.5f) * fTick * MOVE_SPEED;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true || pInpuJoyPad->GetJoyStickLY(0) > 0)
	{// 前移動
		m_info.fRotDest = rotCamera.y + -D3DX_PI;

		move.x += sinf(rotCamera.y + 0.0f) * fTick * MOVE_SPEED;
		move.z += cosf(rotCamera.y + 0.0f) * fTick * MOVE_SPEED;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true || pInpuJoyPad->GetJoyStickLY(0) < 0)
	{// 後ろ移動
		m_info.fRotDest = rotCamera.y;

		move.x += sinf(rotCamera.y + D3DX_PI) * fTick * MOVE_SPEED;
		move.z += cosf(rotCamera.y + D3DX_PI) * fTick * MOVE_SPEED;
	}

	SetMovement(move);
	
	// 向きの補正
	RotAor();
}

//------------------------------------------------------------------------
// 攻撃入力
//------------------------------------------------------------------------
void CPlayer::InputAttack(void)
{
	// 情報の取得
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();
	CInputMouse* pInputMouse = CInputMouse::GetInstance();

	int nKey = GetBody()->GetKey(CCharacterDiv::DIV_UPPER);	// 現在のキー
	int nNumKey = GetBody()->GetMotionInfo(CCharacterDiv::DIV_UPPER, GetType(CCharacterDiv::DIV_UPPER)).nNumKey;	// キーの総数
	int nMotionType = GetBody()->GetType(CCharacterDiv::DIV_UPPER);

	// 攻撃状態1へ
	m_info.isMotionList.bAttack = pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_RB, 0);
	
	// 攻撃状態2へ
	/*if (nMotionType == UPPER_MOTION_ATTACK00 && nKey > 3 && nKey <= nNumKey)
	{
		if (pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_RB, 0))
		{
			m_info.isMotionList.bAttack1 = true;
		}
	}*/
}

//------------------------------------------------------------------------
// 回避入力
//------------------------------------------------------------------------
void CPlayer::InputAvoidance(void)
{
	// 情報の取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();

	if (m_info.isMotionList.bAvoidance)
	{
		return;
	}

	if (pInputKeyboard->GetTrigger(DIK_LSHIFT) || pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_LB, 0))
	{
		float stick = sinf(pInpuJoyPad->GetJoyStickLX(0)) * cosf(pInpuJoyPad->GetJoyStickLY(0));

		// 変数宣言
		CEnemyBoss* pBoss = CEnemyBoss::GetInstance();
		D3DXVECTOR3 posBoss = {};
		D3DXVECTOR3 pos = GetPosition();

		if (pBoss != nullptr)
		{
			posBoss = pBoss->GetPosition();
		}

		// 角度を求める
		float fAngle = atan2f((posBoss.x - pos.x), (posBoss.z - pos.z));

		if (stick < 0.0f)
		{
			m_avoid.fAngleMove = 1.0f;
 		}
		else
		{
			m_avoid.fAngleMove = -1.0f;
		}

		m_info.isMotionList.bAvoidance = true;
	}
}

//===========================================================================================
// 攻撃
//===========================================================================================
void CPlayer::Attack(void)
{
	// 情報の取得
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();
	CInputMouse* pInputMouse = CInputMouse::GetInstance();

	D3DXVECTOR3 rot = GetPosition();
	D3DXVECTOR3 move = GetMovement();

	int nKey = GetBody()->GetKey(CCharacterDiv::DIV_UPPER);	// 現在のキー
	int nNumKey = GetBody()->GetMotionInfo(CCharacterDiv::DIV_UPPER, GetType(CCharacterDiv::DIV_UPPER)).nNumKey;	// キーの総数
	int nTypeMotion = GetBody()->GetType(CCharacterDiv::DIV_UPPER);
	float fTick = CManager::GetInstance()->GetTick() * 100.0f;

	if (nTypeMotion == UPPER_MOTION_ATTACK00)
	{
		if (nKey == 3)
		{
			IsAttack(true);
		}
		if (nKey >= 1)
		{
			// 移動
			move.x = sinf(rot.y + D3DX_PI) * fTick * 5.0f;
			move.z = cosf(rot.y + D3DX_PI) * fTick * 5.0f;
		}
	}

	if (nTypeMotion == UPPER_MOTION_ATTACK01)
	{
		if (nKey == 2)
		{
			IsAttack(true);
		}
		if (nKey == 1)
		{
			// 移動
			move.x = sinf(rot.y + D3DX_PI) * fTick * 5.0f;
			move.z = cosf(rot.y + D3DX_PI) * fTick * 5.0f;

			SetMovement(move);
		}
	}
}

//===========================================================================================
// 攻撃判定(扇形)
//===========================================================================================
bool CPlayer::IsAttack(bool bAway)
{
	CEnemyBoss* pBoss = CEnemyBoss::GetInstance();

	if (pBoss == nullptr)
	{
		return false;
	}

	D3DXVECTOR3 posBoss = pBoss->GetPosition();
	D3DXVECTOR3 moveBoss = pBoss->GetMovement();

	float vec1 = 0.0f, vec2 = 0.0f;	// ベクトル
	D3DXVECTOR3 pos0 = {};	// 始点
	D3DXVECTOR3 pos1 = {}, pos2 = {};	// 終点1/終点2
	D3DXVECTOR3 vecLine = {}, vecToPos = {};	// 始点と終点の距離/始点から対象の距離
	D3DXVECTOR3 vecLine1 = {};	// 始点と終点の距離

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 move = GetMovement();

	// 攻撃範囲の設定
	float fLength = sqrtf
	(
		(posBoss.x - pos.x) * (posBoss.x - pos.x) +
		(posBoss.z - pos.z) * (posBoss.z - pos.z)
	);

	// 角度を求める
	//float fAngle = atan2f((pos.x - posBoss.x), (pos.z - posBoss.z));
	float fAngle = atan2f((posBoss.x - pos.x), (posBoss.z - pos.z));

	// 範囲内判定
	if (fLength < ATTACK_LENGTH)
	{
		// 始点の計算
		pos0.x = pos.x;
		pos0.y = rot.y;
		pos0.z = pos.z;
		// 終点1の計算
		pos1.x = pos0.x + (sinf(rot.y + D3DX_PI * ATTACK_RANGE));
		pos1.y = 0.0f;
		pos1.z = pos0.z + (cosf(rot.y + D3DX_PI * ATTACK_RANGE));
		// 終点2の計算
		pos2.x = pos0.x + (sinf(rot.y + -D3DX_PI * ATTACK_RANGE));
		pos2.y = 0.0f;
		pos2.z = pos0.z + (cosf(rot.y + -D3DX_PI * ATTACK_RANGE));

		// 始点と終点との距離間
		vecLine = pos1 - pos0;
		vecLine1 = pos2 - pos0;

		// 始点から対象までの位置
		vecToPos = posBoss - pos0;

		// ベクトルの計算
		vec1 = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
		vec2 = (vecLine1.z * vecToPos.x) - (vecLine1.x * vecToPos.z);

		if (vec1 > 0 && vec2 < 0)
		{
			CManager::GetInstance()->GetDebugProc()->Print("\n【【判定内】】\n");

			if (bAway)
			{
				pBoss->SetMovement(
					{
						sinf(fAngle) * BLOWN_AWAY,
						0.0f,
						cosf(fAngle) * BLOWN_AWAY,
					});
			}

			// ダメージを与える
			pBoss->Hit(0.1f);

			return true;
		}
	}

	/*CManager::GetInstance()->GetDebugProc()->Print("始点:%f %f %f\n", pos0.x, pos0.y, pos0.z);
	CManager::GetInstance()->GetDebugProc()->Print("終点1:%f %f %f\n", pos1.x, pos1.y, pos1.z);
	CManager::GetInstance()->GetDebugProc()->Print("終点2:%f %f %f\n", pos2.x, pos2.y, pos2.z);
	CManager::GetInstance()->GetDebugProc()->Print("始点との距離1:%f %f %f\n", vecLine.x, vecLine.y, vecLine.z);
	CManager::GetInstance()->GetDebugProc()->Print("始点との距離2:%f %f %f\n", vecLine1.x, vecLine1.y, vecLine1.z);
	CManager::GetInstance()->GetDebugProc()->Print("ベクトル1:%f\n", vec1);
	CManager::GetInstance()->GetDebugProc()->Print("ベクトル2:%f\n\n", vec2);
	CManager::GetInstance()->GetDebugProc()->Print("向き:%f\n\n", fAngle);*/

	return false;
}

//===========================================================================================
// ロックオン
//===========================================================================================
void CPlayer::Lockon(void)
{
	CInputMouse *pInputMouse = CInputMouse::GetInstance();
	CInputJoypad *pInpuJoyPad = CInputJoypad::GetInstance();
	CEnemyBoss *pBoss = CEnemyBoss::GetInstance();
	CCamera *pCamera =CManager::GetInstance()->GetCamera();

	if (pBoss != nullptr)
	{
		D3DXVECTOR3 posBoss = pBoss->GetPosition();

		if (pInputMouse->GetTrigger(pInputMouse->MOUSE_RIGHT) || pInpuJoyPad->GetTrigger(pInpuJoyPad->BUTTON_RS, 0))
		{
			m_bLockon = m_bLockon ? false : true;
		}

		if (!m_bLockon)
		{
			if (m_pLockon != nullptr)
			{
				m_pLockon->Uninit();
				m_pLockon = nullptr;
			}

			return;
		}

		if (m_pLockon == nullptr)
		{
			CMotion* pMotion = pBoss->GetBody();

			D3DXVECTOR3 pos = pMotion->GetParts(0)->pParts->GetPosition();

			m_pLockon = CLockon::Create({ posBoss.x, pos.y + 20.0f, posBoss.z });
		}

		if (m_pLockon != nullptr)
		{
			m_pLockon->SetPosition({ posBoss.x, 150.0f, posBoss.z });
			m_pLockon->SetSize({ 10.0f, 10.0f });
		}

		// 目的の方向へ
		RotDest();
	
		if (pCamera != nullptr)
		{
			//CMotion* pMotion = pBoss->GetBody();

			//D3DXVECTOR3 pos = pMotion->GetParts(0)->pParts->GetPosition();

			//// 攻撃範囲の設定
			//float fLength = sqrtf
			//(
			//	(posBoss.x - pos.x) * (posBoss.x - pos.x) +
			//	(posBoss.z - pos.z) * (posBoss.z - pos.z)
			//);

			//// 注視点の設定
			//pCamera->SetPosR({ posBoss.x, pos.y + 50.0f, posBoss.z });
			////pCamera->SetLength(fLength);

			//// 視点の設定
			//pCamera->SetPosV(
			//	{ 
			//		pos.x + sinf(m_info.rot.y) * 200.0f,
			//		200.0f,
			//		pos.z + cosf(m_info.rot.y) * 200.0f 
			//	}
			//);

			// 背後にカメラを設定
			pCamera->PlayerLockon(0.4f);
		}
	}
}

//===========================================================================================
// 回避
//===========================================================================================
void CPlayer::Evasion(void)
{
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputJoypad* pInpuJoyPad = CInputJoypad::GetInstance();
	CInputMouse* pInputMouse = CInputMouse::GetInstance();
	
	// 変数宣言
	bool bJust = false;
	CEnemyBoss* pBoss = CEnemyBoss::GetInstance();
	D3DXVECTOR3 posBoss = pBoss->GetPosition();
	float fTickAvoidance = (CManager::GetInstance()->GetTick() * 10.0f);	// 回避時間用

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetPosition();
	D3DXVECTOR3 move = GetMovement();

	////float fAngle = atan2f((posBoss.x - pos.x), (posBoss.z - pos.z));
	//if (!m_info.isMotionList.bAvoidance)
	//{
	//	// 長さを求める
	//	m_fTempLength = sqrtf((m_info.pos.x - posBoss.x) * (m_info.pos.x - posBoss.x) + (m_info.pos.z - posBoss.z) * (m_info.pos.z - posBoss.z));
	//}

	//if (!m_info.isMotionList.bAvoidance)
	//{
	//	// 長さを求める
	//	m_avoid.fTempLength = sqrtf((pos.x - posBoss.x) * (pos.x - posBoss.x) + (pos.z - posBoss.z) * (pos.z - posBoss.z));

	//	m_avoid.move = move;
	//}

	//if (m_info.isMotionList.bAvoidance)
	//{
	//	float fTickMove = (CManager::GetInstance()->GetTick() * 100.0f);	// 移動用

	//	//SetMovement(m_avoid.move);
	//	fAngle += 0.1f;

	//	m_info.pos.x = posBoss.x + sinf(fAngle) * m_fTempLength;
	//	m_info.pos.z = posBoss.z + cosf(fAngle) * m_fTempLength;

	//	// 回避時間を減らす
	//	m_avoid.fTime -= fTickAvoidance;

	//	if (m_avoid.fTime < 0.0f)
	//	{// 時間が終わったら
	//		m_avoid.fTime = AVOIDANCE_TIME;
	//		m_info.isMotionList.bAvoidance = false;
	//	}
	//}

	CManager::GetInstance()->GetDebugProc()->Print("%f\n", m_avoid.fDiff);
	//CManager::GetInstance()->GetDebugProc()->Print("1:%f 2:%f\n\n", DestPos1, DestPos2);
}

//===========================================================================================
// 残像
//===========================================================================================
void CPlayer::AfterImage(void)
{

}

//===========================================================================================
// 目的の方向へ向く（今回は敵の方向）
//===========================================================================================
void CPlayer::RotDest(void)
{
	// 情報の取得	
	CEnemyBoss* pBoss = CEnemyBoss::GetInstance();

	if (pBoss == nullptr)
	{
		return;
	}

	// 取得
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// 角度を求める
	float fAngle = atan2f((pos.x - pBoss->GetPosition().x), (pos.z - pBoss->GetPosition().z));

	// ボスの方向を向く
	rot.y = fAngle;

	SetRotation(rot);
}

//===========================================================================================
// モーション
//===========================================================================================
void CPlayer::ManageMotion(void)
{
	// 下半身モーション
	LowerMotion();
	// 上半身モーション
	UpperMotion();

	if (m_pSlow != nullptr)
	{
		GetBody()->SetMmultiply(m_pSlow->GetValue());
	}
}

//===========================================================================================
// 下半身モーション
//===========================================================================================
void CPlayer::LowerMotion(void)
{
	// モーションの番号を取得
	int nDiv = CCharacterDiv::DIV_LOWER;
	int nMotionLower = GetType(nDiv);
	int nKey = GetBody()->GetKey(nDiv);
	int nNumKey = GetBody()->GetMotionInfo(nDiv, nMotionLower).nNumKey;
	D3DXVECTOR3 move = GetMovement();

	bool bFinish = GetBody()->IsFinish(DIV_LOWER);	// 終了状態

	float fSpeed = sqrtf(move.x * move.x + move.z * move.z);

	if (m_info.isMotionList.bAvoidance || (nMotionLower == LOWER_MOTION_DASH && !bFinish))
	{// 回避状態
		if (nMotionLower != LOWER_MOTION_DASH)
		{
			//SetMotion(nDiv, LOWER_MOTION_DASH);
		}
	}
	else if (m_info.isMotionList.bAttack1 || (nMotionLower == LOWER_MOTION_ATTACK01 && !bFinish))
	{// ２回目の攻撃状態
		if (nMotionLower != LOWER_MOTION_ATTACK01)
		{// モーションタイプが同じではない場合
			SetMotion(nDiv, LOWER_MOTION_ATTACK01);
		}
		//m_info.isMotionList.bAttack1 = false;
	}
	else if (m_info.isMotionList.bAttack || (nMotionLower == LOWER_MOTION_ATTACK00 && !bFinish))
	{// １回目の攻撃状態
		if (nMotionLower != LOWER_MOTION_ATTACK00)
		{
			SetMotion(nDiv, LOWER_MOTION_ATTACK00);
		}
	}
	else
	{
		if (fSpeed > 0.2f)
		{// 走る
			if (nMotionLower != LOWER_MOTION_RUN)
			{
				SetMotion(nDiv, LOWER_MOTION_RUN);
			}
		}
		else
		{// 
			if (nMotionLower != LOWER_MOTION_NORMAL)
			{
				SetMotion(nDiv, LOWER_MOTION_NORMAL);
			}
		}
	}
}

//===========================================================================================
// 上半身モーション
//===========================================================================================
void CPlayer::UpperMotion(void)
{
	// モーション情報の取得
	int nDiv = CCharacterDiv::DIV_UPPER;
	int nMotionLower = GetType(nDiv);	// モーションの種類
	int nKey = GetBody()->GetKey(nDiv);	// 現在のキー数
	int nNumKey = GetBody()->GetMotionInfo(nDiv, nMotionLower).nNumKey;	// キーの総数
	D3DXVECTOR3 move = GetMovement();

	bool bFinish = GetBody()->IsFinish(DIV_UPPER);	// 終了状態

	float fSpeed = sqrtf(move.x * move.x + move.z * move.z);

	if (m_info.isMotionList.bAvoidance && bFinish)
	{// 回避状態かつ終了してる場合
		if (nMotionLower != UPPER_MOTION_DASH)
		{// モーションタイプが同じではない場合
			//SetMotion(nDiv, UPPER_MOTION_DASH);
		}
		//m_info.isMotionList.bAvoidance = false;
	}
	else if (m_info.isMotionList.bAttack1 || (nMotionLower == UPPER_MOTION_ATTACK01 && !bFinish))
	{// ２回目の攻撃状態
		if (nMotionLower != UPPER_MOTION_ATTACK01)
		{// モーションタイプが同じではない場合
			SetMotion(nDiv, UPPER_MOTION_ATTACK01);
		}
		m_info.isMotionList.bAttack1 = false;
	}
	else if (m_info.isMotionList.bAttack || (nMotionLower == UPPER_MOTION_ATTACK00 && !bFinish))
	{// １回目の攻撃状態またはタイプが一緒で終了してない場合

		if (nMotionLower != UPPER_MOTION_ATTACK00)
		{// モーションタイプが同じではない場合
			SetMotion(nDiv, UPPER_MOTION_ATTACK00);
		}
	}
	else
	{
		if (fSpeed > 0.2f)
		{// 走る
			if (nMotionLower != UPPER_MOTION_RUN)
			{// モーションタイプが同じではない場合
				SetMotion(nDiv, UPPER_MOTION_RUN);
			}
		}
		else
		{// 
			if (nMotionLower != UPPER_MOTION_NORMAL)
			{// モーションタイプが同じではない場合
				SetMotion(nDiv, UPPER_MOTION_NORMAL);
			}
		}
	}
}

//===========================================================================================
// 移動範囲の範囲指定
//===========================================================================================
void CPlayer::RimitPos(D3DXVECTOR3* pPos)
{
	if (pPos->x <= -LIMIT_POS.x)
	{
		pPos->x = -LIMIT_POS.x;
	}
	if (pPos->x >= LIMIT_POS.x)
	{
		pPos->x = LIMIT_POS.x;
	}
	if (pPos->z <= -LIMIT_POS.z)
	{
		pPos->z = -LIMIT_POS.z;
	}
	if (pPos->z >= LIMIT_POS.z)
	{
		pPos->z = LIMIT_POS.z;
	}
}

//===========================================================================================
// 向き補正
//===========================================================================================
void CPlayer::RotAor(void)
{
	D3DXVECTOR3 rot = GetRotation();

	m_info.fRotDiff = m_info.fRotDest - rot.y;	// 目標の角度までの差分を求める

	if (m_info.fRotDiff > D3DX_PI)
	{
		m_info.fRotDiff -= (D3DX_PI * 2);
	}
	if (m_info.fRotDiff < -D3DX_PI)
	{
		m_info.fRotDiff += (D3DX_PI * 2);
	}

	rot.y += m_info.fRotDiff * ADJUST_ROT;	// 補正する

	// 向きの修正
	general::RotLimit(&rot);

	SetRotation(rot);
}

//===========================================================================================
// デバッグ
//===========================================================================================
void CPlayer::Debug(void)
{
#ifndef _DEBUG
	return;
#endif

	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 move = GetMovement();

	int nKey1 = GetBody()->GetKey(0);	// 現在のキー数
	int nKey2 = GetBody()->GetKey(1);	// 現在のキー数
	int nNumKey1 = GetBody()->GetMotionInfo(0, GetType(0)).nNumKey;	// キーの総数
	int nNumKey2 = GetBody()->GetMotionInfo(1, GetType(1)).nNumKey;	// キーの総数

	//デバッグ表示
	pDebug->Print("【プレイヤー情報】\n");
	pDebug->Print("位置：x:%f y:%f z:%f\n", pos.x, pos.y, pos.z);
	pDebug->Print("向き：x:%f y:%f z:%f\n", rot.x, rot.y, rot.z);
	pDebug->Print("移動量： x:%f y:%f z:%f\n", move.x, move.y, move.z);
	pDebug->Print("体力：%f\n", m_info.fLife);
	pDebug->Print("モーション番号：下%d 上%d\n", GetBody()->GetType(0), GetBody()->GetType(1));
	pDebug->Print("キー番号：下%d 上%d\n", nKey1, nKey2);
	pDebug->Print("キー総数：下%d 上%d\n\n", nNumKey1, nNumKey2);
	pDebug->Print("ティック:%f\n", CManager::GetInstance()->GetTick());
	pDebug->Print("LXstick:%f\n", CInputJoypad::GetInstance()->GetJoyStickLX(0));
	pDebug->Print("LYstick:%f\n", CInputJoypad::GetInstance()->GetJoyStickLY(0));
}
