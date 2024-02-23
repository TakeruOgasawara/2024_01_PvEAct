//===============================================================================
//
// キャラクター[character.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

// インクルード宣言
#include "main.h"
#include "object.h"

// 前方宣言
class CObjectX;
class CXfile;
class CMotion;

// キャラクタークラス
class CCharacter : public CObject
{
private:
	// 定数
	const int MODEL_MAX = 20;

private:
	// キャラクタ情報構造体
	struct SCharacter
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	// 過去位置
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 move;
		D3DXMATRIX mtxWorld;	// マトリックス
		CMotion* pBody;	// 
	};

public:
	CCharacter();	// コンストラクタ
	~CCharacter();	// デストラクタ

	static CCharacter* Create(const char* pPath);

	// プロトタイプ宣言
	HRESULT Init();
	HRESULT Init(const char* pPath);
	void Uninit();
	void Update();
	void Draw();

	// 設定
	void SetPosition(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 rot);
	void SetMotion(int nMotion);
	void SetBody(CMotion* pBody);

	// 取得
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	D3DXVECTOR3 GetMovement(void) const { return m_info.move; }
	int GetType(void);
	CMotion* GetBody(void);

private:
	// メンバ変数
	SCharacter m_info;
};

#endif