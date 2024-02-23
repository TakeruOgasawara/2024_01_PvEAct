//===============================================================================
//
// キャラクター[character.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _MOTIONDIFF_H_
#define _MOTIONDIFF_H_

// インクルード宣言
#include "main.h"
#include "object.h"
#include <stdio.h>

// 定数定義
namespace motionDiv
{
	static const int MODEL_MAX = 20;
	static const int MOTION_MAX = 10;
}

// 前方宣言
class CObjectX;
class CXfile;
class CModelParts;

// 分割キャラクタークラス
class CMotionDiv : public CObject
{
private:
	// 分割の種類
	enum EDivType
	{
		DIV_LOWER = 0,
		DIV_UPPER,
		DIV_MAX
	};

	//-----------------------------
	// キャラ組み立て用の構造体
	//-----------------------------
	// キャラクター情報の構造体
	struct SInfo
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	// 過去位置
		D3DXVECTOR3 rot;	// 向き
		D3DXMATRIX mtxWorld;	// マトリックス
		int aNumLoadPartsDiv[DIV_MAX];	// 読み込むのパーツ数
		int aNumSetPartsDiv[DIV_MAX];	// 設置するのパーツ数
		int nAllParts;	// モデルパーツ総数
	};
	// モデルパーツ情報の構造体
	struct SModelParts
	{
		int nIdx;	// 番号
		int nIdxParent;	// 親番号
		CModelParts* pParts;	// パーツ情報
	};

	//-----------------------------
	// モーション用の構造体
	//-----------------------------
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
		SKey aKey[motionDiv::MODEL_MAX];	// 各モデルのキー要素
	};
	// モーション情報の構造体
	struct SMotionInfo
	{
		bool bLoop;	// ループするかどうか
		int nNumKey;	// キーの総数
		SKeyInfo aKeyInfo[motionDiv::MODEL_MAX];	// キー情報
	};
	// モーション情報の構造体
	struct SMotion
	{
		int nNumMotion;	// モーションの総数
		int nType;		// モーションの種類
		int nTypeOld;	// 前回のモーションの種類
		int nKey;		// 現在のキー
		int nIdxParent;	// 親番号
		int nNumParts;	// パーツの数
		float fCounter;	// カウンター
		bool bFinish;	// 終了したかどうか
		bool bLoop;		// ループするかどうか
	};

public:
	CMotionDiv();	// コンストラクタ
	~CMotionDiv();	// デストラクタ

	static CMotionDiv* Create(const char* pUpperPath, const char* pLowerPath);

	// プロトタイプ宣言
	HRESULT Init();
	HRESULT Init(const char* pUpperPath, const char* pLowerPath);
	void Uninit();
	void Update();
	void Draw();
	
	// 設定
	void SetMotion(int nNumDiv, int type);	// モーション
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }	// 位置
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }	// 向き
	void SetCounter(int nNumDiv, float fCounter) { m_aMotion[nNumDiv].fCounter = fCounter; }	// モーションカウンター
	void SetMmultiply(float fMmultiply) { m_fCntMmultiply = fMmultiply; }	// モーションカウンターの乗算

	// 取得
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	int GetType(int nNumDiv) { return m_aMotion[nNumDiv].nType; }
	int GetOldType(int nNumDiv) { return m_aMotion[nNumDiv].nTypeOld; }
	int GetIdxParent(int nNum) { return m_aMotion[nNum].nIdxParent; }
	int GetKey(int nNumDiv) { return m_aMotion[nNumDiv].nKey; }
	float GetCounter(int nNumDiv) { return m_aMotion[nNumDiv].fCounter; }
	float GetCntMmultiply(void) { return m_fCntMmultiply; }
	bool IsFinish(int nNumDiv) { return m_aMotion[nNumDiv].bFinish; }
	bool IsLoop(int nNumDiv) { return m_aMotion[nNumDiv].bLoop; }
	CModelParts* GetParts(int nNumDiv, int nNumPart);
	SMotionInfo GetMotionInfo(int nNumDiv, int nNumMotion) { return m_aMotionInfo[nNumDiv][nNumMotion]; }

private:
	// メンバ関数
	void InitMotionDiv(int nType);
	void InitPose(int nType);
	void SetKeyOld(void);
	void UpdateMotionDiv(void);
	void Load(const char* pUpperPath, const char* pLowerPath);
	void LoadCharacter(FILE* pFile, int nCntDiv);
	void LoadMotion(FILE* pFile, int nCntDiv);
	void MultiplyMtx(void);
	void Debug(int nCntDiv, int nType);

	// メンバ変数
	SInfo m_info;	// メイン情報
	SModelParts* m_apModelPart[EDivType::DIV_MAX][motionDiv::MODEL_MAX];	// パーツ情報
	SMotion m_aMotion[EDivType::DIV_MAX];	// モーション情報
	SMotionInfo m_aMotionInfo[EDivType::DIV_MAX][motionDiv::MOTION_MAX];	// モーション情報
	SKey m_aKeyOld[DIV_MAX][motionDiv::MODEL_MAX];	// 前回のキー情報の構造体
	float m_fCntMmultiply;	// モーションカウントの倍率
};

#endif