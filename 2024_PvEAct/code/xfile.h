//===============================================================================
//
// [Xfile.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _XFILE_H_	// このマクロ定義がされていなかったら
#define _XFILE_H_	// 2重インクルード防止のマクロを定義

#include "main.h"

// 定数定義
namespace xfile
{
	static const int MAX_FILE = 64;
	static const int MAX_NAME = 254;
};

// テクスチャクラス
class CXfile
{
public:
	// Xファイル情報の構造体
	struct SXFile
	{
		LPD3DXMESH pMesh;	// 使用されているモデル数
		LPD3DXBUFFER pBuffMat;	// 頂点カラー情報
		DWORD dwNumMat;	// カラーの数
		D3DXVECTOR3 vtxMin, vtxMax;	// オブジェクトの最大値・最小値
		char aXfileName[xfile::MAX_NAME];	// ファイル名
		int* pIdxTexture;	// テクスチャ番号
	};

public:
	CXfile();	// コンストラクタ
	~CXfile();	// デストラクタ

	void Unload(void);	// 破棄

	static CXfile* Create(void);
	static CXfile* GetInstance(void);
	static void Release(void);

	void InitLoad(void);
	int Load(const char *pXfileName);	// 登録

	SXFile* GetXfile(int nIdx);
	LPD3DXMESH GetMesh(int nIdx);	// メッシュ情報の取得
	LPD3DXBUFFER GetBuffMat(int nIdx);	// バッファ情報の取得
	DWORD GetDwNumMat(int nIdx);	// マテリアル数の取得
	D3DXVECTOR3 GetMax(int nIdx) { return m_apXFile[nIdx]->vtxMax; }	// 最大値の取得
	D3DXVECTOR3 GetMin(int nIdx) { return m_apXFile[nIdx]->vtxMin; }	// 最小値の取得
	char* GetFileName(int nIdx) { return &m_apXFile[nIdx]->aXfileName[0]; }	// ファイル名の取得
	int* GetTextureIdx(int nIdx) { return m_apXFile[nIdx]->pIdxTexture; }	// テクスチャ番号の取得
	int GetNumAll(void) { return m_nNumAll; }	// 総数の取得

private:
	// メンバ関数
	void VtxMaxMin(int nNumCnt);	// 最大値・最小値

	// メンバ変数
	static CXfile* m_pXfile;	// 自身のポインタ
	SXFile* m_apXFile[xfile::MAX_FILE];
	int m_nNumAll;
};

#endif