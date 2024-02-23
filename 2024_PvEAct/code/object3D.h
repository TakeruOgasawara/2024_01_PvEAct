//===============================================================================
//
// [object3D.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBJECT3D_H_				//このマクロ定義がされていなかったら
#define _OBJECT3D_H_				//2重インクルード防止のマクロを定義

#include "main.h"
#include "object.h"

class CObject3D : public CObject
{
public:

	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_FIELD,  
		TYPE_WALL,
		TYPE_MAX
	};

public:
	CObject3D();			//コンストラクタ
	~CObject3D();			//デストラクタ

	static CObject3D *Create();
	static CObject3D *Create(D3DXVECTOR3 pos);
	static CObject3D *Create(D3DXVECTOR3 pos, TYPE type);
	static CObject3D *Create(D3DXVECTOR3 pos, const char *cTexName);
	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, const char *cTexName);
	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, const char *cTexName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定用
	void SetVerTex_field(void);
	void SetVerTex_wall(void);
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(float x, float z);
	void SetColor(D3DXCOLOR col);
	void SetType(TYPE type) { m_type = type; }

	//取得用
	D3DXVECTOR3 GetPosition(void) const { return m_pos; };
	D3DXVECTOR3 GetRotation(void) const { return m_rot; };

	//テクスチャ割り当て
	void BindTexture(int nIdx);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//バッファへの頂点情報
	D3DXMATRIX m_mtxWorld;				//ワールドマトリックス
	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_rot;					//向き
	D3DXCOLOR m_col;
	float fSize_x;
	float fSize_z;
	int m_nTextureIdx;
	TYPE m_type;
};

#endif