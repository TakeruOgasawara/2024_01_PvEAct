//===============================================================================
//
// [objectX.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBJECTX_H_				//このマクロ定義がされていなかったら
#define _OBJECTX_H_				//2重インクルード防止のマクロを定義

#include "main.h"
#include "object.h"
#include "xfile.h"

// オブジェクトXクラス
class CObjectX : public CObject
{
public:
	CObjectX(int nPriority = 0);			//コンストラクタ
	~CObjectX();			//デストラクタ

	static CObjectX *Create(void);
	static CObjectX* Create(const char* c_pFileName, D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f});

	HRESULT Init(void);
	HRESULT Init(const char *c_pFileName, D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f });
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawOnly(void);

	//設定用
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
	void SetModel(const char* cPath);
	void SetMatrix(D3DXMATRIX pMtx) { m_mtxWorld = pMtx; }
	void BindXfile(int nIdxTexture) { m_pXfile = CXfile::GetInstance()->GetXfile(nIdxTexture); }

	//取得用
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	D3DXVECTOR3 GetScale(void) const { return m_scale; }
	D3DXVECTOR3 GetVtxMax(void) const;
	D3DXVECTOR3 GetVtxMin(void) const;
	CObjectX *GetObjectX(void) { return this; }
	D3DXMATRIX* GetMatrix(void) { return &m_mtxWorld; }
	CXfile* GetXfile(void);
	int GetModelIdx(void) { return m_nModelIdx; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バッファへの頂点情報
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
	D3DXVECTOR3 m_scale;	// 大きさ

	CXfile::SXFile* m_pXfile;	// Xファイル情報
	int m_nModelIdx;	// モデル番号
};

#endif