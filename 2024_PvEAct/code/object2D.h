//===============================================================================
//
// オブジェクト2D[object2D.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBJECT2D_H_				//このマクロ定義がされていなかったら
#define _OBJECT2D_H_				//2重インクルード帽子のマクロを定義

// インクルード宣言
#include "main.h"
#include "object.h"

// オブジェクト2Dクラス
class CObject2D : public CObject
{
private:
	struct SInfo
	{
		LPDIRECT3DTEXTURE9 pTexture;	// テクスチャへの頂点情報
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// バッファへの頂点情報
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		D3DXCOLOR col;	// 色
		float fWidth;	// 横
		float fHeight;	// 縦
		float fLength;	// 長さ
		float fAngle;	// 角度
		int nIdxTexture;	// テクスチャ番号
	};

public:
	CObject2D(int nPriority = 3);		//コンストラクタ
	~CObject2D();	//デストラクタ

	static CObject2D *Create(void);
	static CObject2D *Create(D3DXVECTOR3 pos);
	static CObject2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static CObject2D *Create(D3DXVECTOR3 pos, const char *FileName);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//設定用
	void SetVertex_center(void);
	void SetVertex_left(void);
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }
	void SetColor(D3DXCOLOR col) { m_info.col = col; }
	void SetSize(float fWidth, float fHeight);
	void SetTex(D3DXVECTOR2 left, D3DXVECTOR2 right);
	void BindTexture(int nIdx) { m_info.nIdxTexture = nIdx; }

	//取得用
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	D3DXCOLOR GetColor(void) { return m_info.col; }
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_info.fWidth, m_info.fHeight); }
	float GetAngle(void) { return m_info.fAngle; }
	float GetLength(void) { return m_info.fLength; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	int GetNumObj3D(void) { return m_nNumObj2D; };

private:
	SInfo m_info;
	static int m_nNumObj2D;
};

#endif