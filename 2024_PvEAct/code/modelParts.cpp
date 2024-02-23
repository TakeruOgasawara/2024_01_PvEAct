//*****************************************************
//
// パーツの処理[parts.cpp]
// Author : Takeru Ogasawara
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "modelParts.h"

#include "debugproc.h"

//====================================================
// コンストラクタ
//====================================================
CModelParts::CModelParts()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_IdxModelParts = -1;
	m_fRadius = 0.0f;
	m_bChangeCol = false;
	m_col = { 0.0f,0.0f,0.0f,0.0f };
	m_pModelParts = nullptr;
	ZeroMemory(&m_mtxWorld, sizeof(m_mtxWorld));
}

//====================================================
// デストラクタ
//====================================================
CModelParts::~CModelParts()
{

}

//=====================================================
// 生成処理
//=====================================================
CModelParts* CModelParts::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModelParts* pParts = nullptr;

	if (pParts == nullptr)
	{
		// インスタンス生成
		pParts = new CModelParts;

		pParts->m_pos = pos;
		pParts->m_rot = rot;

		// 初期化処理
		pParts->Init();
	}

	return pParts;
}

//====================================================
// 初期化処理
//====================================================
HRESULT CModelParts::Init(void)
{
	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CModelParts::Uninit(void)
{
	if (m_pModelParts != nullptr)
	{
		m_pModelParts = nullptr;
	}

	delete this;
}

//====================================================
// 更新処理
//====================================================
void CModelParts::Update(void)
{
	// 移動量を加算
	m_pos += m_move;
}

//====================================================
// 描画処理
//====================================================
void CModelParts::Draw(void)
{
	if (m_pModelParts == nullptr)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATERIAL* pMat;	// マテリアルデータへのポインタ
	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用
	LPDIRECT3DTEXTURE9 pTexture = nullptr;
	int nIdxTex = 0;

	// 現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pModelParts->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelParts->dwNumMat; nCntMat++)
	{
		// マテリアルの保存
		matDef = pMat[nCntMat].MatD3D;

		if (m_bChangeCol)
		{
			// 色の設定
			pMat[nCntMat].MatD3D.Diffuse = m_col;
		}

		// マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		nIdxTex = m_pModelParts->pIdxTexture[nCntMat];

		// テクスチャの取得
		pTexture = CTexture::GetInstance()->GetAddress(nIdxTex);
		
		// テクスチャ設定
		pDevice->SetTexture(0, pTexture);
		
		// モデル（パーツ）描画
		m_pModelParts->pMesh->DrawSubset(nCntMat);

		// 色を戻す
		pMat[nCntMat].MatD3D = matDef;

		// マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//====================================================
// 影用の描画処理
//====================================================
void CModelParts::DrawShadow(void)
{
	if (m_pModelParts == nullptr)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATERIAL* pMat;	//マテリアルデータへのポインタ
	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	LPDIRECT3DTEXTURE9 pTexture = nullptr;	// テクスチャポインタ

	// 現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pModelParts->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelParts->dwNumMat; nCntMat++)
	{
		// マテリアルの保存
		matDef = pMat[nCntMat].MatD3D;

		// 色の設定
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの取得
		CTexture::GetInstance()->GetAddress(m_pModelParts->pIdxTexture[nCntMat]);

		//テクスチャ設定
		pDevice->SetTexture(0, pTexture);

		//モデル（パーツ）描画
		m_pModelParts->pMesh->DrawSubset(nCntMat);

		// 色を戻す
		pMat[nCntMat].MatD3D = matDef;

		// マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//====================================================
// 残像の描画処理
//====================================================
void CModelParts::DrawAfterImage(void)
{
	if (m_pModelParts == nullptr)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATERIAL* pMat;	//マテリアルデータへのポインタ
	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	LPDIRECT3DTEXTURE9 pTexture = nullptr;	// テクスチャポインタ

	// 現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pModelParts->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelParts->dwNumMat; nCntMat++)
	{
		// マテリアルの保存
		matDef = pMat[nCntMat].MatD3D;

		// マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの取得
		pTexture = CTexture::GetInstance()->GetAddress(m_pModelParts->pIdxTexture[nCntMat]);

		//テクスチャ設定
		pDevice->SetTexture(0, pTexture);

		//モデル（パーツ）描画
		m_pModelParts->pMesh->DrawSubset(nCntMat);

		// 色を戻す
		pMat[nCntMat].MatD3D = matDef;

		// マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=====================================================
// マトリックス設定処理
//=====================================================
void CModelParts::SetMatrix(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=====================================================
// モデルの最長頂点取得
//=====================================================
void CModelParts::SetRadius(void)
{
	// 計算用変数
	int nNumVtx;			// 頂点数
	DWORD dwSizeFVF;		// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;			// 頂点バッファへのポインタ
	float fLength = 0.0f;

	// 頂点数の取得
	nNumVtx = m_pModelParts->pMesh->GetNumVertices();

	// フォーマットサイズ入手
	dwSizeFVF = D3DXGetFVFVertexSize(m_pModelParts->pMesh->GetFVF());

	// 頂点バッファのロック
	m_pModelParts->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (D3DXVec3Length(&vtx) > fLength)
		{
			fLength = D3DXVec3Length(&vtx);
		}

		pVtxBuff += dwSizeFVF;
	}

	// 頂点バッファのアンロック
	m_pModelParts->pMesh->UnlockVertexBuffer();

	// 長さを代入
	m_fRadius = fLength;
}

//=====================================================
// モデルの割り当て
//=====================================================
void CModelParts::BindModel(int nIdx)
{
	// モデル割り当て
	m_pModelParts = CXfile::GetInstance()->GetXfile(nIdx);
}