//======================================================================================
//
// メッシュフィールドの処理[polygon.cpp]
// Author;小笠原　彪
//
//======================================================================================
#include "meshwall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

#include "player.h"
#include "debugproc.h"

//マクロ定義
#define BG_TEX_FULE		""			//奥背景の画像を読み込む
#define MESHWALL_WIDTH		(70.0f)		//幅
#define MESHWALL_HEIGHT		(40.0f)			//高さ

//===========================================================================================
// コンストラクタ
//===========================================================================================
CMeshWall::CMeshWall()
{
	ZeroMemory(&m_info, sizeof(m_info));
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CMeshWall::~CMeshWall()
{

}

//===========================================================================================
// 生成
//===========================================================================================
CMeshWall* CMeshWall::Create(D3DXVECTOR3 pos)
{
	CMeshWall* pMeshWall = nullptr;

	if (pMeshWall == nullptr)
	{
		pMeshWall = new CMeshWall;

		pMeshWall->Init();
		pMeshWall->m_info.pos = pos;

		return pMeshWall;
	}

	return nullptr;
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CMeshWall::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// テクスチャの読み込み
	m_nTextureIdx = CTexture::GetInstance()->Regist(BG_TEX_FULE);

	m_info.fWidth = MESHWALL_WIDTH;
	m_info.fHeight = MESHWALL_HEIGHT;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&m_info.pVtxBuff,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_info.fWidth, m_info.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, m_info.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_info.fWidth, m_info.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(-m_info.fWidth, m_info.fHeight * 0.5f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, m_info.fHeight * 0.5f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(m_info.fWidth, m_info.fHeight * 0.5f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(-m_info.fWidth, 0.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[8].pos = D3DXVECTOR3(m_info.fWidth, 0.0f, 0.0f);
	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//頂点カラー(0.0f～1.0f内で設定)
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	//頂点バッファをアンロックする
	m_info.pVtxBuff->Unlock();

	//インデックスバッファの生成(番号を覚えるためのメモリ)
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_info.pIdxBuff, NULL);

	WORD* pIdx;			//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	m_info.pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//頂点番号データの設定
	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;	

	//インデックスバッファをアンロックする
	m_info.pIdxBuff->Unlock();

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CMeshWall::Uninit(void)
{
	//頂点バッファの破棄
	if (m_info.pVtxBuff != nullptr)
	{
		m_info.pVtxBuff->Release();
		m_info.pVtxBuff = nullptr;
	}
	//インデックスバッファの破棄
	if (m_info.pIdxBuff != nullptr)
	{
		m_info.pIdxBuff->Release();
		m_info.pIdxBuff = nullptr;
	}

	Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CMeshWall::Update(void)
{
	// 変数宣言
	float vec;						// ベクトル
	bool bHit = false;				// ヒット判定
	float fRate;					// ベクトルの割合
	float fMaxArea, fNowArea;		// 今の面積／最大面積
	D3DXVECTOR3 pos0 = {}, pos1 = {};	// 場所
	D3DXVECTOR3 vecLine = {}, vecToPos = {};
	D3DXVECTOR3 Cross = {};	// 交点の場所

	D3DXVECTOR3 PlayerPos = CPlayer::GetInstance()->GetPosition();
	D3DXVECTOR3 playerMove = CPlayer::GetInstance()->GetMovement();

	// 場所の計算
	pos0 = (m_info.pos + D3DXVECTOR3(cosf(-m_info.rot.y) * -1.0f, 0.0f, sinf(-m_info.rot.y) * -1.0f));
	pos1 = (m_info.pos + D3DXVECTOR3(cosf(-m_info.rot.y) * 1.0f, 0.0f, sinf(-m_info.rot.y) * 1.0f));

	// pos0とpos1との距離間
	vecLine = pos1 - pos0;

	// 壁から対象までの位置
	vecToPos = PlayerPos - m_info.pos;

	// 最大面積
	fMaxArea = (vecLine.z * playerMove.x) - (vecLine.x * playerMove.z);

	// 今の面積
	fNowArea = (vecToPos.z * playerMove.x) - (vecLine.x * playerMove.z);

	// ベクトルの割合
	fRate = fNowArea / fMaxArea;

	// 交点
	Cross = D3DXVECTOR3(pos0.x + vecLine.x * fRate, PlayerPos.y, pos0.z + vecLine.z * fRate);

	vec = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);

	/*if (PlayerPos.x >= pos0.x && PlayerPos.x >= pos0.x,
		PlayerPos.x <= pos1.x && PlayerPos.x <= pos1.x)*/
	{
		if (vec < 0)
		{
			CManager::GetInstance()->GetDebugProc()->Print("\n\n判定ない\n\n");
			bHit = true;
		}
	}
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CMeshWall::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

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
	pDevice->SetStreamSource(0, m_info.pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_info.pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, CTexture::GetInstance()->GetAddress(m_nTextureIdx));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,
		9,			//用意した頂点の数
		0,
		12);		//描画するプリミティブの数
}