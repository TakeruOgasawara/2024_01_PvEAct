//===============================================================================
//
// [billboard.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _BILLBOARD_H_				//このマクロ定義がされていなかったら
#define _BILLBOARD_H_				//2重インクルード防止のマクロを定義

#include "main.h"
#include "object.h"

//ビルボードクラス
class CBillboard : public CObject
{
public:
	CBillboard(int nPriority = 3);			//コンストラクタ
	~CBillboard();			//デストラクタ

	static CBillboard *Create(void);
	static CBillboard *Create(D3DXVECTOR3 pos);
	static CBillboard* Create(const char* pTexPath, D3DXVECTOR3 pos = { 0.0, 0.0f, 0.0f });

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定用
	void SetverTex(void);			//頂点設定
	virtual void SetPosition(D3DXVECTOR3 pos);			//位置設定
	virtual void SetRotation(D3DXVECTOR3 rot);			//向き設定
	void SetSize(D3DXVECTOR2 size);
	void SetRadius(D3DXVECTOR2 radius);
	void SetColor(D3DXCOLOR col);

	//取得用
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }			//位置取得
	D3DXVECTOR3 GetRotation(void) const  { return m_rot; }			//向き取得
	D3DXVECTOR2 GetSize(void) { return m_size; }			//大きさ取得
	D3DXCOLOR GetColor(void) { return m_col; }			//色取得
	float GetLength(void) { return m_fLength; }			//長さ取得
	float GetAngle(void) { return m_fAngle; }			//角度取得

	//テクスチャ割り当て
	void BindTexture(int nIdx);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへの頂点情報
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//バッファへの頂点情報
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_move;			//移動
	D3DXVECTOR2 m_size;			//大きさ
	D3DXCOLOR m_col;			//色
	float m_fLength;			//長さ
	float m_fAngle;				//角度
	bool m_bUse;				//使っているか
	int m_nTextureIdx;			//テクスチャ番号

};
#endif