//===============================================================================
//
// [mapEdit.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBSTACLE_H_	//このマクロ定義がされていなかったら
#define _OBSTACLE_H_	//2重インクルード防止のマクロを定義

#include "main.h"
#include "objectX.h"

// 前方宣言
class CXfile;
class CObjectX;

// オブジェクトXクラス
class CObstacle : public CObjectX
{
public:
	CObstacle(int nPriority = 0);	// コンストラクタ
	~CObstacle();	// デストラクタ

	static CObstacle* Create(const char* c_pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f }, float fLife = 0.0f , bool bDestoroy = false);

	HRESULT Init(const char* c_pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLife, bool bDestoroy);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Hit(float fDamage);	// ダメージ判定

	// 設定用
	//void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	//void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetDestoroy(bool bDestoroy) { m_bDestoroy = bDestoroy; }
	void SetLife(float fLife) { m_fLife = fLife; }

	// 取得用
	//D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	//D3DXVECTOR3 GetRotation(void) const { return m_rot; }

private:
	// メンバ関数

	// メンバ変数
	bool m_bDestoroy;	// 破壊できるかどうか
	float m_fLife;	// 寿命
};

#endif