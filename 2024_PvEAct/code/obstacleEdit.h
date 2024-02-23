//===============================================================================
//
// [mapEdit.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBSTACLEEDIT_H_	//このマクロ定義がされていなかったら
#define _OBSTACLEEDIT_H_	//2重インクルード防止のマクロを定義

#include "main.h"
#include "object.h"
#include "obstacle.h"

// 前方宣言
class CXfile;
class CObjectX;

// オブジェクトXクラス
class CObstacleEdit : public CObstacle
{
public:
	// 障害物の種類	※増やすごとに追加
	enum EType
	{
		TYPE_HOUSE = 0,
		TYPE_I,
		TYPE_MAX
	};

public:
	CObstacleEdit(int nPriority = 0);	// コンストラクタ
	~CObstacleEdit();	// デストラクタ

	static CObstacleEdit* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// メンバ関数
	void Operation(D3DXVECTOR3* pos, D3DXVECTOR3* rot);	// 操作
	void Save(void);

	// メンバ変数
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	EType m_type;	// モデル種類

	CXfile* m_pXfile;	// xファイル情報
	int m_nRegistModelIdx;	// 登録モデル番号
	int m_nTypeIdx;	// 種類番号
	bool m_bUse;	// 使うかどうか

	static int m_nNumAll;	// 総数
};

#endif