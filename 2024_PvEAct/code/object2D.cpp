//===========================================================================================
//
// [object2D.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "texture.h"

//マクロ定義
#define SIZE_X		(100.0f)	//横の長さ
#define SIZE_Y		(100.0f)	//縦の長さ

//静的メンバ変数宣言
int CObject2D::m_nNumObj2D = 0;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(m_info));

	m_nNumObj2D++;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CObject2D::~CObject2D()
{
	m_nNumObj2D--;
}

//===========================================================================================
// 生成処理(初期値)
//===========================================================================================
CObject2D *CObject2D::Create(void)
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//オブジェクト2Dの生成
		pObject2D = new CObject2D;

		if (pObject2D != nullptr)
		{
			//初期化処理
			pObject2D->Init();

			return pObject2D;
		}
		else
		{
			return nullptr;
		}
	}

	return nullptr;
}

//===========================================================================================
// 生成処理(位置)
//===========================================================================================
CObject2D *CObject2D::Create(D3DXVECTOR3 pos)
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//オブジェクト2Dの生成
		pObject2D = new CObject2D;

		if (pObject2D != nullptr)
		{
			//初期化処理
			pObject2D->Init();

			//位置の設定処理
			pObject2D->SetPosition(pos);

			return pObject2D;
		}
		else
		{
			return nullptr;
		}
	}

	return nullptr;
}

//===========================================================================================
// 生成処理(位置、回転)
//===========================================================================================
CObject2D *CObject2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//オブジェクト2Dの生成
		pObject2D = new CObject2D;

		if (pObject2D != nullptr)
		{
			//初期化処理
			pObject2D->Init();

			pObject2D->SetPosition(pos);
			pObject2D->SetRotation(rot);

			return pObject2D;
		}
		else
		{
			return nullptr;
		}
	}

	return pObject2D;
}

//===========================================================================================
// 生成処理(位置、テクスチャ名)
//===========================================================================================
CObject2D * CObject2D::Create(D3DXVECTOR3 pos, const char *name)
{
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		//オブジェクト2Dの生成
		pObject2D = new CObject2D;

		if (pObject2D != nullptr)
		{
			//初期化処理
			pObject2D->Init();

			pObject2D->SetPosition(pos);

			pObject2D->BindTexture(CTexture::GetInstance()->Regist(name));

			return pObject2D;
		}
		else
		{
			return nullptr;
		}
	}

	return pObject2D;
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CObject2D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 位置(デフォルト)
	m_info.pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5, 0.0f);

	// 色(デフォルト)
	m_info.col = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	// サイズ設定(デフォルト)
	m_info.fWidth = SIZE_X;
	m_info.fHeight = SIZE_Y;

	// 対角線の長さを算出(デフォルト)
	m_info.fLength = sqrtf(SIZE_X * SIZE_X + SIZE_Y * SIZE_Y) * 0.5f;

	// 対角線の角度を算出する(デフォルト)
	m_info.fAngle = atan2f(SIZE_X, SIZE_Y);

	// テクスチャ番号
	m_info.nIdxTexture = -1;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED,
		&m_info.pVtxBuff,
		NULL);

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CObject2D::Uninit(void)
{
	// バッファの破棄
	if (m_info.pVtxBuff != nullptr)
	{
		m_info.pVtxBuff->Release();
		m_info.pVtxBuff = nullptr;
	}

	// 破棄
	Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CObject2D::Update(void)
{
	CManager::GetInstance()->GetDebugProc()->Print("a\n");
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CObject2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// テクスチャ情報の取得
	CTexture *pTexture = CTexture::GetInstance();
	
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_info.pVtxBuff, 0, sizeof(VERTEX_2D));

	// テクスチャ設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_info.nIdxTexture));
	
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,			// 描画する最初のインデックス(大事)
		2);			// プリミティブ(ポリゴン)数
}

//=======================================================
// 頂点情報の設定(中心から)
//=======================================================
void CObject2D::SetVertex_center(void)
{
	if (m_info.pVtxBuff == nullptr)
	{
		return;
	}

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_info.pos.x + sinf(m_info.rot.z + -D3DX_PI + m_info.fAngle) * m_info.fLength;
	pVtx[0].pos.y = m_info.pos.y + cosf(m_info.rot.z + -D3DX_PI + m_info.fAngle) * m_info.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_info.pos.x + sinf(m_info.rot.z + D3DX_PI - m_info.fAngle) * m_info.fLength;
	pVtx[1].pos.y = m_info.pos.y + cosf(m_info.rot.z + D3DX_PI - m_info.fAngle) * m_info.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_info.pos.x + sinf(m_info.rot.z - m_info.fAngle) * m_info.fLength;
	pVtx[2].pos.y = m_info.pos.y + cosf(m_info.rot.z - m_info.fAngle) * m_info.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_info.pos.x + sinf(m_info.rot.z + m_info.fAngle) * m_info.fLength;
	pVtx[3].pos.y = m_info.pos.y + cosf(m_info.rot.z + m_info.fAngle) * m_info.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = m_info.col;
	pVtx[1].col = m_info.col;
	pVtx[2].col = m_info.col;
	pVtx[3].col = m_info.col;

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		
	//頂点バッファをアンロックする
	m_info.pVtxBuff->Unlock();
}

//=======================================================
// 頂点情報の設定(左端から)
//=======================================================
void CObject2D::SetVertex_left(void)
{
	if (m_info.pVtxBuff == nullptr)
	{
		return;
	}

	//頂点情報へのポインタ
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	// 左上
	pVtx[0].pos.x = m_info.pos.x + sinf(m_info.rot.z + -D3DX_PI + m_info.fAngle);
	pVtx[0].pos.y = m_info.pos.y + cosf(m_info.rot.z + -D3DX_PI + m_info.fAngle) * m_info.fLength;
	pVtx[0].pos.z = 0.0f;
	// 右上
	pVtx[1].pos.x = m_info.pos.x + sinf(m_info.rot.z + D3DX_PI - m_info.fAngle) * m_info.fLength;
	pVtx[1].pos.y = m_info.pos.y + cosf(m_info.rot.z + D3DX_PI - m_info.fAngle) * m_info.fLength;
	pVtx[1].pos.z = 0.0f;
	// 左下
	pVtx[2].pos.x = m_info.pos.x + sinf(m_info.rot.z - m_info.fAngle);
	pVtx[2].pos.y = m_info.pos.y + cosf(m_info.rot.z - m_info.fAngle) * m_info.fLength;
	pVtx[2].pos.z = 0.0f;
	// 右下
	pVtx[3].pos.x = m_info.pos.x + sinf(m_info.rot.z + m_info.fAngle) * m_info.fLength;
	pVtx[3].pos.y = m_info.pos.y + cosf(m_info.rot.z + m_info.fAngle) * m_info.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = m_info.col;
	pVtx[1].col = m_info.col;
	pVtx[2].col = m_info.col;
	pVtx[3].col = m_info.col;

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_info.pVtxBuff->Unlock();
}

//=======================================================
// サイズの設定
//=======================================================
void CObject2D::SetSize(float fWidth, float fHeight)
{
	m_info.fWidth = fWidth;
	m_info.fHeight = fHeight;

	// 対角線の長さを算出する(向きを使用する場合)
	m_info.fLength = sqrtf(m_info.fWidth * m_info.fWidth + m_info.fHeight * m_info.fHeight) * 0.5f;

	// 対角線の角度を算出する(向きを使用する場合)
	m_info.fAngle = atan2f(m_info.fWidth, m_info.fHeight);
}

//=======================================================
// テクスチャの設定
//=======================================================
void CObject2D::SetTex(D3DXVECTOR2 left, D3DXVECTOR2 right)
{
	if (m_info.pVtxBuff == nullptr)
	{
		return;
	}

	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_info.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標設定
	pVtx[0].tex = left;
	pVtx[1].tex = D3DXVECTOR2(right.x, left.y);
	pVtx[2].tex = D3DXVECTOR2(left.x, right.y);
	pVtx[3].tex = right;

	// 頂点バッファをアンロックする
	m_info.pVtxBuff->Unlock();
}

//=======================================================
// 頂点情報の取得
//=======================================================
LPDIRECT3DVERTEXBUFFER9 CObject2D::GetVtxBuff(void)
{
	return m_info.pVtxBuff;
}
