//===============================================================================
//
// カメラの処理[camera.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _CAMERA_H_	//このマクロ定義がされていなかったら
#define _CAMERA_H_	//2重インクルード防止のマクロを定義

#include "main.h"

//***************************************************
// カメラクラス
//***************************************************
class CCamera
{
public:
	CCamera();	// コンストラクタ
	~CCamera();	// デストラクタ

	// プロトタイプ宣言
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void MouseManage(void);
	void JoypadViewOperation(float fVertSpeed, float fHorizontalSpeed);
	void PlayerFollow(void);
	void PlayerLockon(float fOffset);
	void FixedJanken(void);	// 固定カメラ

	// 設定
	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; }
	void SetPosR(D3DXVECTOR3 posR) { m_posR = posR; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetLength(float fLength) { m_fLength = fLength; }

	// 取得
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	float GetLength(void) { return m_fLength; }

private:
	// メンバ関数
	void RegardpointOffset(void);
	void ViewpointOffset(void);
	void RotLimit(void);
	void SetShake(void);

	void SetShake(float intensity, float time);
	void ShakeUpdate(void);

	struct SShaker
	{
		float fIntensity;
		float fTime;
		float fStartTime;
	};

	// メンバ変数
	D3DXMATRIX m_mtxView;	// ビューマトリックス
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXVECTOR3 m_posV;	// 視点
	D3DXVECTOR3 m_posR;	// 注視点
	D3DXVECTOR3 m_vecU;	// 上方向ベクトル
	D3DXVECTOR3 m_rot;	// 向き
	D3DXVECTOR3 m_rotOld;	// 過去向き
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_posVDest;	// 視点の差分
	D3DXVECTOR3 m_posRDest;	// 注視点の差分
	float m_fLength;	// 視点と注視点の距離
	float m_fShakeTime;

	SShaker m_shakeInfo;
};

#endif