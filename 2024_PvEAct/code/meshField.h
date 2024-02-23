//====================================================================
//
// メッシュフィールドの処理[meahfield.h]
// Author: 小笠原彪
//
//====================================================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

#include "main.h"
#include "object.h"

// メッシュウォールクラス
class CMeshField : public CObject
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
		float fWidth;	// 横幅
		float fdepth;	// 奥行き
	} MeshWall;

public:
	CMeshField();
	~CMeshField();

	static CMeshField* Create(D3DXVECTOR3 pos);

	// プロトタイプ宣言
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetPosition(D3DXVECTOR3 pos) { }
	void SetRotation(D3DXVECTOR3 rot) { }

	// 取得
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }

private:
	MeshWall m_info;
	int m_nTextureIdx;

};

#endif