//======================================================================================
//
// メッシュシリンダーの処理[meshcylindwe.h]
// Author;小笠原　彪
//
//======================================================================================
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//マクロ定義
#define BG_TEX_FULE		"data\\TEXTURE\\bg00.png"			//奥背景の画像を読み込む
#define MESH_LENGTH		(1700.0f)			//シリンダーの半径の長さ
#define MESH_HEIGHT		(1500.0f)			//シリンダーの高さ

//====================================================================
// コンストラクタ
//====================================================================
CMeshCylinder::CMeshCylinder()
{
	// 値のクリア
	ZeroMemory(&m_info, sizeof(m_info));
}

//====================================================================
// デストラクタ
//====================================================================
CMeshCylinder::~CMeshCylinder()
{

}

//====================================================================
// 初期化処理
//====================================================================
HRESULT CMeshCylinder::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//テクスチャの読み込み
	m_info.nIdxTexture = CTexture::GetInstance()->Regist(BG_TEX_FULE);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&m_info.pVtxBuff,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.0f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 0.0f) * MESH_LENGTH);
	pVtx[1].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.25f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 0.25f) * MESH_LENGTH);
	pVtx[2].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.5f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 0.5f) * MESH_LENGTH);
	pVtx[3].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.75f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 0.75f) * MESH_LENGTH);
	pVtx[4].pos = D3DXVECTOR3(cosf(D3DX_PI * 1.0f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 1.0f) * MESH_LENGTH);
	pVtx[5].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.75f) * MESH_LENGTH, MESH_HEIGHT, sinf(-D3DX_PI * 0.75f) * MESH_LENGTH);
	pVtx[6].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.5f) * MESH_LENGTH, MESH_HEIGHT, sinf(-D3DX_PI * 0.5f) * MESH_LENGTH);
	pVtx[7].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.25f) * MESH_LENGTH, MESH_HEIGHT, sinf(-D3DX_PI * 0.25f) * MESH_LENGTH);
	////重なっている所(上側)
	pVtx[8].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.0f) * MESH_LENGTH, MESH_HEIGHT, sinf(D3DX_PI * 0.0f) * MESH_LENGTH);
	//下側
	pVtx[9].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.0f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 0.0f) * MESH_LENGTH);
	pVtx[10].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.25f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 0.25f) * MESH_LENGTH);
	pVtx[11].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.5f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 0.5f) * MESH_LENGTH);
	pVtx[12].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.75f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 0.75f) * MESH_LENGTH);
	pVtx[13].pos = D3DXVECTOR3(cosf(D3DX_PI * 1.0f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 1.0f) * MESH_LENGTH);
	pVtx[14].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.75f) * MESH_LENGTH, 0.0f, sinf(-D3DX_PI * 0.75f) * MESH_LENGTH);
	pVtx[15].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.5f) * MESH_LENGTH, 0.0f, sinf(-D3DX_PI * 0.5f) * MESH_LENGTH);
	pVtx[16].pos = D3DXVECTOR3(cosf(-D3DX_PI * 0.25f) * MESH_LENGTH, 0.0f, sinf(-D3DX_PI * 0.25f) * MESH_LENGTH);
	//重なっている所(下側)
	pVtx[17].pos = D3DXVECTOR3(cosf(D3DX_PI * 0.0f) * MESH_LENGTH, 0.0f, sinf(D3DX_PI * 0.0f) * MESH_LENGTH);

	for (int nCnt = 0; nCnt < 18; nCnt++)
	{
		//法線ベクトルの設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラー(0.0f～1.0f内で設定)
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.375f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.625f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.75f, 0.0f);
	pVtx[7].tex = D3DXVECTOR2(0.875f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 0.0f);
	//下側
	pVtx[9].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[10].tex = D3DXVECTOR2(0.125f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(0.25f, 1.0f);
	pVtx[12].tex = D3DXVECTOR2(0.375f, 1.0f);
	pVtx[13].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[14].tex = D3DXVECTOR2(0.625f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(0.75f, 1.0f);
	pVtx[16].tex = D3DXVECTOR2(0.875f, 1.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_info.pVtxBuff->Unlock();

	//インデックスバッファの生成(番号を覚えるためのメモリ)
	pDevice->CreateIndexBuffer(sizeof(WORD) * 20,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_info.pIdxBuff, NULL);

	WORD* pIdx;			//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	m_info.pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//頂点番号データの設定
	pIdx[0] = 0;
	pIdx[1] = 9;
	pIdx[2] = 1;
	pIdx[3] = 10;

	pIdx[4] = 2;
	pIdx[5] = 11;
	pIdx[6] = 3;
	pIdx[7] = 12;

	pIdx[8] = 4;
	pIdx[9] = 13;
	pIdx[10] = 5;
	pIdx[11] = 14;

	pIdx[12] = 6;
	pIdx[13] = 15;
	pIdx[14] = 7;
	pIdx[15] = 16;

	pIdx[16] = 8;
	pIdx[17] = 17;

	//インデックスバッファをアンロックする
	m_info.pIdxBuff->Unlock();

	return E_NOTIMPL;
}

//====================================================================
// 終了処理
//====================================================================
void CMeshCylinder::Uninit(void)
{
	//頂点バッファの破棄
	if (m_info.pVtxBuff != NULL)
	{
		m_info.pVtxBuff->Release();
		m_info.pVtxBuff = NULL;
	}
	//インデックスバッファの破棄
	if (m_info.pIdxBuff != NULL)
	{
		m_info.pIdxBuff->Release();
		m_info.pIdxBuff = NULL;
	}
}

//====================================================================
// 更新処理
//====================================================================
void CMeshCylinder::Update(void)
{

}

//====================================================================
// 描画処理
//====================================================================
void CMeshCylinder::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_info.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_info.rot.y, m_info.rot.x, m_info.rot.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_info.pos.x, m_info.pos.y, m_info.pos.z);
	D3DXMatrixMultiply(&m_info.mtxWorld, &m_info.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_info.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_info.pVtxBuff, 0,
		sizeof(VERTEX_3D));			//頂点情報構造体のサイズ

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_info.pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAddress(m_info.nIdxTexture));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,
		9,			//用意した頂点の数
		0,
		20);		//描画するプリミティブの数
}
