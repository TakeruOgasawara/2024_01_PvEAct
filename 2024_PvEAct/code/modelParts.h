//*****************************************************
//
// パーツの処理[parts.cpp]
//  Author : Takeru Ogasawara
//
//*****************************************************

#ifndef _PARTS_H_
#define _PARTS_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "xfile.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_TEX	(5)	// テクスチャの数

//*****************************************************
// クラス定義
//*****************************************************
class CModelParts
{
public:
	CModelParts();	// コンストラクタ
	~CModelParts();	// デストラクタ

	static CModelParts* Create(D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f }, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void DrawShadow(void);
	void DrawAfterImage(void);

	// 設定
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPosOrg(D3DXVECTOR3 pos) { m_posOrg = pos; }
	void SetIdxModel(int nIdx) { m_IdxModelParts = nIdx; }
	void SetRadius(void);
	void SetMatrix(void);
	void SetModel(CXfile::SXFile* pModel) { m_pModelParts = pModel; }
	void SetEmissiveCol(D3DXCOLOR col) { m_col = col; m_bChangeCol = true; }
	void BindModel(int nIdx);
	void ResetColor(void) { m_bChangeCol = false; }

	// 取得
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	D3DXVECTOR3 GetPosOrg(void) { return m_posOrg; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	D3DXMATRIX* GetMatrix(void) { return &m_mtxWorld; }
	int GetIdxModel(void) { return m_IdxModelParts; }
	float GetRadius(void) { return m_fRadius; }
	CXfile::SXFile* GetModel(void) { return m_pModelParts; }
	D3DXCOLOR GetEmissiveCol(void) { return m_col; }

private:
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_posOrg;	// 最初の位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_rot;	//向き
	D3DXCOLOR m_col;	// 色
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	CXfile::SXFile* m_pModelParts;	// モデルパーツ情報

	int m_IdxModelParts;	// モデルパーツの番号
	float m_fRadius;	// モデルの半径
	bool m_bChangeCol;	// 色を変えるかどうか
};

#endif