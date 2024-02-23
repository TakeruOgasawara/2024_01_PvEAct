//===========================================================================================
//
// [object3D.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "object3D.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "texture.h"

//マクロ定義
#define SIZE			(D3DXVECTOR3(1500.0f, 0.0f, 1500.0f))
#define SIZE_WALL		(150.0f)

//===========================================================================================
// コンストラクタ
//===========================================================================================
CObject3D::CObject3D()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pVtxBuff = nullptr;
	fSize_x = 0.0f;
	fSize_z = 0.0f;
	m_nTextureIdx = 0;
	m_type = TYPE_NONE;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CObject3D::~CObject3D()
{

}

//===========================================================================================
// オブジェクト3Dの生成
//===========================================================================================
CObject3D *CObject3D::Create(void)
{
	CObject3D *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		pObject3D = new CObject3D;

		if (pObject3D != nullptr)
		{
			pObject3D->Init();

			return pObject3D;
		}
		else
		{
			return nullptr;
		}
	}

	return pObject3D;
}

//===========================================================================================
// オブジェクト3Dの生成[オバーロード] : Create(位置)
//===========================================================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos)
{
	CObject3D *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		pObject3D = new CObject3D;

		if (pObject3D != nullptr)
		{
			pObject3D->Init();

			pObject3D->SetPosition(pos);

			//テクスチャの割り当て
			//pObject3D->BindTexture(m_nTextureIdx);

			return pObject3D;
		}
		else
		{
			return nullptr;
		}
	}

	return pObject3D;
}

//===========================================================================================
// オブジェクト3Dの生成[オバーロード] : Create(位置、タイプ) 
//===========================================================================================
CObject3D * CObject3D::Create(D3DXVECTOR3 pos, TYPE type)
{
	CObject3D *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		pObject3D = new CObject3D;

		if (pObject3D != nullptr)
		{
			pObject3D->SetType(type);		//種類設定

			pObject3D->Init();	//初期化

			pObject3D->SetPosition(pos);	//位置設定

			return pObject3D;
		}
		else
		{
			return nullptr;
		}
	}

	return pObject3D;
}

//===========================================================================================
// オブジェクト3Dの生成[オバーロード] : Create(位置、テクスチャ名) 
//===========================================================================================
CObject3D * CObject3D::Create(D3DXVECTOR3 pos, const char *cTexName)
{
	CTexture *pTexture = CTexture::GetInstance();
	CObject3D *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		pObject3D = new CObject3D;

		if (pObject3D != nullptr)
		{
			pObject3D->Init();	//初期化

			pObject3D->SetPosition(pos);	//位置設定

			//テクスチャの割り当て
			pObject3D->BindTexture(pTexture->Regist(cTexName));

			return pObject3D;
		}
		else
		{
			return nullptr;
		}
	}

	return pObject3D;
}

//===========================================================================================
// オブジェクト3Dの生成[オバーロード] : Create(位置、大きさ、テクスチャ名) 
//===========================================================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, const char * cTexName)
{
	CTexture *pTexture = CTexture::GetInstance();
	CObject3D *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		pObject3D = new CObject3D;

		if (pObject3D != nullptr)
		{
			pObject3D->Init();	//初期化

			pObject3D->SetPosition(pos);	//位置設定

			pObject3D->SetSize(size.x, size.y);

			//テクスチャの割り当て
			pObject3D->BindTexture(pTexture->Regist(cTexName));

			return pObject3D;
		}
		else
		{
			return nullptr;
		}
	}

	return pObject3D;
}

//===========================================================================================
// オブジェクト3Dの生成[オバーロード] : Create(位置、大きさ、色、テクスチャ名) 
//===========================================================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, const char *cTexName)
{
	CTexture *pTexture = CTexture::GetInstance();
	CObject3D *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		pObject3D = new CObject3D;

		if (pObject3D != nullptr)
		{
			pObject3D->Init();	//初期化

			pObject3D->SetPosition(pos);	//位置設定

			pObject3D->SetSize(size.x, size.y);	//大きさ設定

			pObject3D->SetColor(col);

			//テクスチャの割り当て
			pObject3D->BindTexture(pTexture->Regist(cTexName));

			return pObject3D;
		}
		else
		{
			return nullptr;
		}
	}

	return pObject3D;
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CObject3D::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//種類の設定
	CObject::SetType(CObject::TYPE_OBJECT3D);

	fSize_x = SIZE.x;
	fSize_z = SIZE.z;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	//頂点設定
	switch (m_type)
	{
	case TYPE_FIELD:
		SetVerTex_field();
		break;

	case TYPE_WALL:
		SetVerTex_wall();
		break;

	default:
		SetVerTex_field();
		break;
	}
	
	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CObject3D::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//自分自身の破棄
	Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CObject3D::Update(void)
{
	D3DXVECTOR3 pos = m_pos;

	//頂点設定
	switch (m_type)
	{
	case TYPE_FIELD:
		SetVerTex_field();
		break;

	case TYPE_WALL:
		SetVerTex_wall();
		break;

	default:
		SetVerTex_field();
		break;
	}
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CObject3D::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	CTexture *pTexture = CTexture::GetInstance();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0,
		sizeof(VERTEX_3D));			//頂点情報構造体のサイズ

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nTextureIdx));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
		0,						//描画する最初のインデックス
		2);						//ポリゴン数
}

//===========================================================================================
// 頂点情報の設定(フィールド型)
//===========================================================================================
void CObject3D::SetVerTex_field(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fSize_x, 0.0f, +fSize_z);
	pVtx[1].pos = D3DXVECTOR3(+fSize_x, 0.0f, +fSize_z);
	pVtx[2].pos = D3DXVECTOR3(-fSize_x, 0.0f, -fSize_z);
	pVtx[3].pos = D3DXVECTOR3(+fSize_x, 0.0f, -fSize_z);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラー(0.0f～1.0f内で設定)
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================================================================
// 頂点情報の設定(ウォール型)
//===========================================================================================
void CObject3D::SetVerTex_wall(void)
{
	D3DXVECTOR3 pos = m_pos;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-SIZE_WALL, +SIZE_WALL, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+SIZE_WALL, +SIZE_WALL, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-SIZE_WALL, -SIZE_WALL, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+SIZE_WALL, -SIZE_WALL, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラー(0.0f～1.0f内で設定)
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	pos = m_pos;
}

//===========================================================================================
// 大きさ設定
//===========================================================================================
void CObject3D::SetSize(float x, float z)
{
	fSize_x = x;
	fSize_z = z;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-x, 0.0f, z);
	pVtx[1].pos = D3DXVECTOR3(x, 0.0f, z);
	pVtx[2].pos = D3DXVECTOR3(-x, 0.0f, -z);
	pVtx[3].pos = D3DXVECTOR3(x, 0.0f, -z);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================================================================
// 色の設定
//===========================================================================================
void CObject3D::SetColor(D3DXCOLOR col)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラー(0.0f～1.0f内で設定)
	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================================================================
// テクスチャ番号の割り当て
//===========================================================================================
void CObject3D::BindTexture(int nIdx)
{
	m_nTextureIdx = nIdx;
}
