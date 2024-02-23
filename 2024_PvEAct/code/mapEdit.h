//===============================================================================
//
// [mapEdit.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _MAPEDIT_H_	//このマクロ定義がされていなかったら
#define _MAPEDIT_H_	//2重インクルード防止のマクロを定義

#include "main.h"
#include "object.h"

// 前方宣言
class CXfile;

// オブジェクトXクラス
class CMapEdit : public CObject
{
public:
	enum EType
	{
		TYPE_HOUSE = 0,
		TYPE_MAX
	};

public:
	CMapEdit(int nPriority = 0);	//コンストラクタ
	~CMapEdit();			//デストラクタ

	static CMapEdit* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定用
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }

	//取得用
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }

private:
	// メンバ関数
	void Operation(void);	// 操作

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//バッファへの頂点情報
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	D3DXVECTOR3 m_scale;	// 大きさ

	CXfile* m_pXfile;	// xファイル情報
	int m_nRegistModelIdx;	// 登録モデル番号
	int m_nTypeIdx;	// 種類番号
	EType m_type;	// モデル種類
	bool m_bUse;	// 使うかどうか
};

#endif