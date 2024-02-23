//===============================================================================
//
// キャラクター[character.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _CHARACTER_DIV_H_
#define _CHARACTER_DIV_H_

// インクルード宣言
#include "main.h"
#include "object.h"

// 定数定義
namespace character
{
	const int MODEL_MAX = 20;
}

// 前方宣言
class CObjectX;
class CXfile;
class CMotionDiv;

// キャラクタークラス
class CCharacterDiv : public CObject
{
public:
	// 分割の種類
	enum EDivType
	{
		DIV_LOWER = 0,
		DIV_UPPER,
		DIV_MAX
	};

private:
	// キャラクタ情報構造体
	struct SCharacter
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 posOld;	// 過去位置
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 move;
		D3DXMATRIX mtxWorld;	// マトリックス
		CMotionDiv* pBody;	// 
	};

public:
	CCharacterDiv();	// コンストラクタ
	~CCharacterDiv();	// デストラクタ

	static CCharacterDiv* Create(const char* pUpperPath, const char* pLowerPath);

	// プロトタイプ宣言
	HRESULT Init();
	HRESULT Init(const char* pUpperPath, const char* pLowerPath);
	void Uninit();
	void Update();
	void Draw();

	// 設定
	void SetPosition(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 rot);
	void SetMovement(D3DXVECTOR3 move) { m_info.move = move; }
	void SetMotion(int nNumDiv, int nMotion);
	void SetBody(CMotionDiv* pBody);

	// 取得
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	D3DXVECTOR3 GetMovement(void) const { return m_info.move; }
	int GetType(int nNumDiv);
	CMotionDiv* GetBody(void);

private:
	// メンバ変数
	SCharacter m_info;
};

#endif