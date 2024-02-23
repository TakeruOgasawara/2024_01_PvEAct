//===============================================================================
//
// キャラクター[character.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

// インクルード宣言
#include "main.h"
#include "object.h"
#include <stdio.h>

// 定数定義
namespace motion
{
	static const int MODEL_MAX = 20;
	static const int MOTION_MAX = 40;
}

// 前方宣言
class CObjectX;
class CXfile;
class CModelParts;

// キャラクタークラス
class CMotion : public CObject
{
private:
	// キャラクタ情報の構造体
	struct SInfo
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		D3DXMATRIX mtxWorld;	// マトリックス
		int nNumModelparts;	// モデルパーツ総数
	};

	// モデルパーツ情報の構造体
	struct SModelParts
	{
		int nIdx;
		int nIdxParent;	// 親番号
		CModelParts* pParts;	// パーツ情報
	};

	//--------------------------
	// モーション用の構造体
	//--------------------------
	// キーの構造体
	struct SKey
	{
		float fPosX;
		float fPosY;
		float fPosZ;
		float fRotX;
		float fRotY;
		float fRotZ;
	};
	
	// キー情報の構造体
	struct SKeyInfo
	{
		float fFrame;	// 再生フレーム
		SKey aKey[motion::MODEL_MAX];	// 各モデルのキー要素
	};

	// モーション情報の構造体
	struct SMotionInfo
	{
		bool bLoop;	// ループするかどうか
		int nNumKey;	// キーの総数
		SKeyInfo aKeyInfo[motion::MODEL_MAX];	// キー情報
	};

	// モーション情報の構造体
	struct SMotion
	{
		int nNumMotion;	//モーションの総数
		int nType;	//モーションの種類
		int nTypeOld;	//前回のモーションの種類
		int nKey;	//現在のキー
		float fCounter;	// カウンター
		int nNumParts;	// パーツの数

		bool bFinish;	// 終了したかどうか
	};

public:
	CMotion();	// コンストラクタ
	~CMotion();	// デストラクタ

	static CMotion* Create(const char* pTextPath);

	// プロトタイプ宣言
	HRESULT Init();
	HRESULT Init(const char* pTextPath);
	void Uninit();
	void Update();
	void Draw();

	// 設定
	void SetMotion(int nType);
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }
	void SetMmultiply(float fMmultiply) { m_fCntMmultiply = fMmultiply; }	// モーションカウンターの乗算


	// 取得
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	int GetType(void) { return m_motion.nType; }
	int GetOldType(void) { return m_motion.nTypeOld; }
	int GetIdxParent(int nNum) { return m_apModelPart[nNum]->nIdxParent; }
	int GetKey(void) { return m_motion.nKey; }
	float GetCounter(int nNumDiv) { return m_motion.fCounter; }
	float GetCntMmultiply(void) { return m_fCntMmultiply; }
	bool IsFinish(void) { return m_motion.bFinish; }
	bool IsLoop(int nNumType) { return m_aMotionInfo[nNumType]->bLoop; }
	SModelParts* GetParts(int nNumPart) { return m_apModelPart[nNumPart]; }
	SMotionInfo* GetMotionInfo(int nNumMotion) { return m_aMotionInfo[nNumMotion]; }
	SMotion GetMotion(void) { return m_motion; }

private:
	// メンバ関数
	void InitMotion(int nType);
	void Load(const char* cTextPath);
	void LoadCharacter(FILE* pFile);
	void LoadMotion(FILE* pFile);
	void MultiplyMtx(void);
	void Debug(void);

	// メンバ変数
	SInfo m_info;	// メイン情報
	SModelParts* m_apModelPart[motion::MODEL_MAX];	// パーツ情報
	SMotion m_motion;
	SMotionInfo* m_aMotionInfo[motion::MOTION_MAX];	// モーション情報
	float m_fCntMmultiply;
};

#endif