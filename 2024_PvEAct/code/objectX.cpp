//===========================================================================================
//
// [objectX.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "object.h"
#include "general.h"

#define SIZE (20.0f)

//===========================================================================================
// コンストラクタ
//===========================================================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pXfile = nullptr;
	m_nModelIdx = 0;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CObjectX::~CObjectX()
{

}

//===========================================================================================
// 生成
//===========================================================================================
CObjectX * CObjectX::Create(void)
{
	CObjectX *pObjX = nullptr;

	if (pObjX == nullptr)
	{
		pObjX = new CObjectX;

		if (pObjX != nullptr)
		{
			pObjX->Init();

			return pObjX;
		}
	}

	return pObjX;
}

//===========================================================================================
// 生成
//===========================================================================================
CObjectX *CObjectX::Create(const char *c_pFileName, D3DXVECTOR3 pos)
{
	CObjectX *pObjX = nullptr;

	if (pObjX == nullptr)
	{
		pObjX = new CObjectX;

		if (pObjX != nullptr)
		{
			// 初期化処理
			pObjX->Init(c_pFileName, pos);

			// 位置設定
			pObjX->SetPosition(pos);

			// 種類設定
			pObjX->SetType(TYPE_OBJECTX);

			return pObjX;
		}
		else
		{
			return NULL;
		}
	}

	return pObjX;
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CObjectX::Init(void)
{
	// 割り当て
	m_nModelIdx = CXfile::GetInstance()->Load("data\\MODEL\\object\\block00.x");
	
	// サイズ
	m_scale = { 1.0f, 1.0f, 1.0f };

	return S_OK;
}

//===========================================================================================
// 初期化処理(オーバーロード)
//===========================================================================================
HRESULT CObjectX::Init(const char *c_pFileName, D3DXVECTOR3 pos)
{
	// テクスチャの割り当て
	int nIdx = CXfile::GetInstance()->Load(c_pFileName);

	// 情報を割り当てる
	BindXfile(nIdx);
	
	// サイズ
	m_scale = { 1.0f, 1.0f, 1.0f };

	// 位置
	m_pos = pos;

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CObjectX::Uninit(void)
{
	// 自身の破棄
	Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CObjectX::Update(void)
{
	m_pos = GetPosition();
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CObjectX::Draw(void)
{
	if (m_pXfile == nullptr)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;	// マテリアルデータへのポインタ
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	//// ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&m_mtxWorld);

	//// スケールの反映
	//D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//// 向きを反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//// 位置を反映
	//D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//// ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// マトリックスの設定
	general::SetMatrix(&m_mtxWorld, m_pos, m_rot);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pXfile->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pXfile->dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの取得
		pTexture = CTexture::GetInstance()->GetAddress(m_pXfile->pIdxTexture[nCntMat]);
		
		// テクスチャの設定
		pDevice->SetTexture(0, pTexture);

		// オブジェクト(パーツ)の描画
		m_pXfile->pMesh->DrawSubset(nCntMat);
	}

	// 保存されていたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===========================================================================================
// 描画専用
//===========================================================================================
void CObjectX::DrawOnly(void)
{
	if (m_pXfile == nullptr)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;	// マテリアルデータへのポインタ
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pXfile->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pXfile->dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの取得
		pTexture = CTexture::GetInstance()->GetAddress(m_pXfile->pIdxTexture[nCntMat]);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture);

		// オブジェクト(パーツ)の描画
		m_pXfile->pMesh->DrawSubset(nCntMat);
	}
}

//===========================================================================================
// モデルの設定
//===========================================================================================
void CObjectX::SetModel(const char* cPath)
{
	if (m_pXfile != nullptr)
	{
		m_nModelIdx = CTexture::GetInstance()->Regist(cPath);
	}
}

//===========================================================================================
// 最大値
//===========================================================================================
D3DXVECTOR3 CObjectX::GetVtxMax(void) const
{
	if (m_pXfile != nullptr)
	{
		return m_pXfile->vtxMax;
	}

	return D3DXVECTOR3();
}

//===========================================================================================
// 最小値
//===========================================================================================
D3DXVECTOR3 CObjectX::GetVtxMin(void) const
{
	if (m_pXfile != nullptr)
	{
		return m_pXfile->vtxMin;
	}

	return D3DXVECTOR3();
}

//===========================================================================================
// Xファイル情報の取得
//===========================================================================================
//CXfile* CObjectX::GetXfile(void)
//{
//	if (m_pXfile == nullptr)
//	{
//		return nullptr;
//	}
//
//	return m_pXfile;
//}
