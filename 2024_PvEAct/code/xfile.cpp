//===========================================================================================
// 
// [texture.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include <stdlib.h>
#include "xfile.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//********************************************************************
// 静的メンバ変数宣言
//********************************************************************
CXfile* CXfile::m_pXfile = nullptr;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CXfile::CXfile()
{
	// 値のクリア
	ZeroMemory(&m_apXFile[0], sizeof(m_apXFile));
	m_nNumAll = 0;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CXfile::~CXfile()
{

}

//===========================================================================================
// 生成
//===========================================================================================
CXfile* CXfile::Create(void)
{
	if (m_pXfile == nullptr)
	{
		m_pXfile = new CXfile;

		m_pXfile->InitLoad();

		return m_pXfile;
	}

	return m_pXfile;
}

//===========================================================================================
//	インスタンスの取得
//===========================================================================================
CXfile* CXfile::GetInstance(void)
{
	if (m_pXfile == nullptr)
	{
		assert(("xファイル情報の取得に失敗", false));
	}

	return m_pXfile;
}

//===========================================================================================
// インスタンスの破棄
//===========================================================================================
void CXfile::Release(void)
{
	if (m_pXfile == nullptr)
	{
		return;
	}
	else
	{
		m_pXfile->Unload();

		delete m_pXfile;
		m_pXfile = nullptr;
	}
}


//===========================================================================================
// Xファイルを読み込む(初期化処理)
//===========================================================================================
void CXfile::InitLoad(void)
{
	// 最初に読み込むxファイル
	const char *aFileList[xfile::MAX_FILE] =
	{
		
	};

	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (aFileList[i] != nullptr)
		{
			// モデル情報の登録
			Load(aFileList[i]);
		}
		else
		{
			break;
		}
	}
}

//===========================================================================================
// 全て破棄する(終了処理)
//===========================================================================================
void CXfile::Unload(void)
{
	for (int i = 0; i < xfile::MAX_FILE; i++)
	{
		if (m_apXFile[i] != nullptr)
		{
			//ファイル名をクリア
			ZeroMemory(&m_apXFile[i]->aXfileName, sizeof(m_apXFile[i]->aXfileName));
			
			if (m_apXFile[i]->pBuffMat != nullptr)
			{
				m_apXFile[i]->pBuffMat->Release();
				m_apXFile[i]->pBuffMat = nullptr;
			}

			if (m_apXFile[i]->pMesh != nullptr)
			{
				m_apXFile[i]->pMesh->Release();
				m_apXFile[i]->pMesh = nullptr;
			}

			if (m_apXFile[i]->pIdxTexture != nullptr)
			{
				delete m_apXFile[i]->pIdxTexture;
				m_apXFile[i]->pIdxTexture = nullptr;
			}

			// メモリの解放
			delete m_apXFile[i];
			m_apXFile[i] = nullptr;

			m_nNumAll--;
		}
	}
}

//===========================================================================================
// Xファイルに割り当てられた番号を返す
//===========================================================================================
int CXfile::Load(const char *pXfilePath)
{
	for (int i = 0; i < xfile::MAX_FILE; i++)
	{
		if (m_apXFile[i] != nullptr)
		{
			if (strcmp(&m_apXFile[i]->aXfileName[0], pXfilePath) == 0)
			{// 登録されたファイル名と引数のファイル名が同じだった場合

				return i;	// 番号を返す
			}

			continue;
		}

		// 情報の取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスの取得
		D3DXMATERIAL* pMat;	// マテリアルポインタ

		// 生成
		m_apXFile[i] = new SXFile;

		// Xファイル情報の初期化
		ZeroMemory(m_apXFile[i], sizeof(SXFile));

		//  Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX
		(
			pXfilePath,	// ファイル名
			D3DXMESH_SYSTEMMEM, pDevice,
			NULL,
			&m_apXFile[i]->pBuffMat,	// マテリアルデータ
			NULL,
			&m_apXFile[i]->dwNumMat,	// マテリアル数
			&m_apXFile[i]->pMesh)))	// メッシュデータ
		{
			return E_FAIL;	// 読み込み失敗
		}

		// 必要数分テクスチャのポインタを確保する
		m_apXFile[i]->pIdxTexture = new int[m_apXFile[i]->dwNumMat];

		// マテリアルへのポインタを取得
		pMat = (D3DXMATERIAL*)m_apXFile[i]->pBuffMat->GetBufferPointer();

		for (int nCntIdxMat = 0; nCntIdxMat < (int)m_apXFile[i]->dwNumMat; nCntIdxMat++)
		{
			if (pMat[nCntIdxMat].pTextureFilename != nullptr)
			{
				// ファイルからテクスチャを読み込む
				m_apXFile[i]->pIdxTexture[nCntIdxMat] = CTexture::GetInstance()->Regist(pMat[nCntIdxMat].pTextureFilename);
			}
			else
			{
  				m_apXFile[i]->pIdxTexture[nCntIdxMat] = -1;
			}
		}

		// テクスチャ名を書き込む
		strcpy(&m_apXFile[i]->aXfileName[0], pXfilePath);

		// オブジェクトサイズを算出する関数
		VtxMaxMin(i);

		m_nNumAll++;	// 総数カウントアップ

		return i;
	}

	return -1;
}

//===========================================================================================
// オブジェクトのサイズを算出
//===========================================================================================
void CXfile::VtxMaxMin(int nNumCnt)
{
	DWORD dwSizeFVF;	// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;	// 頂点バッファへのポインタ
	int nNumVtx;	// 頂点数

	// 頂点数を取得
	nNumVtx = m_apXFile[nNumCnt]->pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(m_apXFile[nNumCnt]->pMesh->GetFVF());

	// 頂点バッファをロック
	m_apXFile[nNumCnt]->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		if (m_apXFile[nNumCnt]->vtxMax.x < vtx.x)
		{// xが大きかったら
			m_apXFile[nNumCnt]->vtxMax.x = vtx.x;
		}
		if (m_apXFile[nNumCnt]->vtxMax.z < vtx.z)
		{// zの値が大きかったら
			m_apXFile[nNumCnt]->vtxMax.z = vtx.z;
		}

		if (m_apXFile[nNumCnt]->vtxMin.x > vtx.x)
		{// xが小さかったら
			m_apXFile[nNumCnt]->vtxMin.x = vtx.x;
		}
		if (m_apXFile[nNumCnt]->vtxMin.z > vtx.z)
		{// zの値が大きかったら
			m_apXFile[nNumCnt]->vtxMin.z = vtx.z;
		}

		if (m_apXFile[nNumCnt]->vtxMax.y < vtx.y)
		{// xが大きかったら
			m_apXFile[nNumCnt]->vtxMax.y = vtx.y;
		}
		if (m_apXFile[nNumCnt]->vtxMin.y > vtx.y)
		{// xが小さかったら
			m_apXFile[nNumCnt]->vtxMin.y = vtx.y;
		}

		pVtxBuff += dwSizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
	}
	
	// 頂点バッファをアンロック
	m_apXFile[nNumCnt]->pMesh->UnlockVertexBuffer();
}

//===========================================================================================
// アドレスの取得
//===========================================================================================
CXfile::SXFile* CXfile::GetXfile(int nIdx)
{
	if (nIdx < 0 && nIdx >= m_nNumAll)
	{
		return nullptr;
	}

	return m_apXFile[nIdx];
}

//===========================================================================================
// メッシュの取得
//===========================================================================================
LPD3DXMESH CXfile::GetMesh(int nIdx)
{
	if (nIdx > m_nNumAll || nIdx < 0)
	{
		assert(("モデルのメッシュがありません", false));
	}
	return m_apXFile[nIdx]->pMesh;
}

//===========================================================================================
// バッファーの取得
//===========================================================================================
LPD3DXBUFFER CXfile::GetBuffMat(int nIdx)
{
	if (nIdx > m_nNumAll || nIdx < 0)
	{
		assert(("モデルのバッファがありません", false));
	}

	return m_apXFile[nIdx]->pBuffMat;
}

//===========================================================================================
// マテリアルの数取得
//===========================================================================================
DWORD CXfile::GetDwNumMat(int nIdx)
{
	if (nIdx > m_nNumAll || nIdx < 0)
	{
		assert(("モデルのメッシュがありません", false));
	}
	return m_apXFile[nIdx]->dwNumMat;
}

