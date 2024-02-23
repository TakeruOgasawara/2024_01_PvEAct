//====================================================================
//
// メッシュシリンダーの処理[meshcylindwe.h]
// Author: 小笠原彪
//
//====================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"
#include "object.h"

class CMeshCylinder : public CObject
{
private:
	struct Sinfo
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//頂点バッファへのポインタ
		LPDIRECT3DINDEXBUFFER9 pIdxBuff;	//インデックスバッファへのポインタ
		D3DXMATRIX mtxWorld;	// マトリックスワールド
		int nIdxTexture;	// テクスチャ番号
	};

public:
	CMeshCylinder();
	~CMeshCylinder();

	//プロトタイプ宣言
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }

	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }

private:
	Sinfo m_info;

};

#endif