//====================================================================
//
// メッシュウォールの処理[field.h]
// Author: 小笠原彪
//
//====================================================================
#ifndef _MESH_WALL_H_
#define _MESH_WALL_H_

#include "main.h"
#include "object.h"

// メッシュウォールクラス
class CMeshWall : public CObject
{
private:
	// 情報の構造体
	typedef struct
	{
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//頂点バッファへのポインタ
		LPDIRECT3DINDEXBUFFER9 pIdxBuff;	//インデックスバッファへのポインタ
		D3DXVECTOR3 pos;	//位置
		D3DXVECTOR3 rot;	//向き
		D3DXMATRIX mtxWorld;	//ワールドマトリックス
		float fWidth;
		float fHeight;
	} MeshWall;

public:
	CMeshWall();
	~CMeshWall();

	static CMeshWall* Create(D3DXVECTOR3 pos);

	// プロトタイプ宣言
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }

	// 取得
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }

private:
	MeshWall m_info;
	int m_nTextureIdx;

};

#endif