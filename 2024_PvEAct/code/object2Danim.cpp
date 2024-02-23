//===========================================================================================
//
// [object2Danim.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "renderer.h"
#include "manager.h"
#include "object2Danim.h"
#include "input.h"

//マクロ定義
#define SIZE_X		(100.0f)	//
#define SIZE_Y		(100.0f)	//
#define ANGLE_X		(10.0f)		//
#define ANGLE_Y		(10.0f)		//
//アニメーションの処理
#define ANIM_FIRST			(5)					//↓の数がANIM_FIRSTになった時最初から繰り返す
#define ANIM_PTTERN			(8)					//読み込むアニメのパターン数
#define TEX_ANIM0			(4)						//動かすアニメのパターン数の半分
#define TEX_ANIM1			(0.125f)					//動かすアニメ1コマの横幅(0.0～1.0)
#define TEX_ANIM2			(0.5f)					//動かすアニメ1コマの縦幅(0.0～1.0)

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CObject2Danim::m_pTexture = NULL;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CObject2Danim::CObject2Danim()
{
	 m_pTexture = NULL;			//テクスチャ情報
	m_nCntPttern = 0;
	m_nPttern = 0;
	m_fTexSizeX = 0.0f;
	m_fTexSizeY = 0.0f;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CObject2Danim::~CObject2Danim()
{

}

//===========================================================================================
// テクスチャの読み込み
//===========================================================================================
HRESULT CObject2Danim::Load(void)
{
	return E_NOTIMPL;
}

//===========================================================================================
// 生成処理
//===========================================================================================
CObject2Danim * CObject2Danim::Create(void)
{
	CObject2Danim *pObject2Danim = NULL;

	if (pObject2Danim == NULL)
	{
		//オブジェクト2Dの生成
		pObject2Danim = new CObject2Danim;

		if (pObject2Danim != NULL)
		{
			//初期化処理
			pObject2Danim->Init();

			return pObject2Danim;
		}
		else
		{
			return NULL;
		}
	}

	return NULL;
}

//===========================================================================================
// 生成処理(位置)
//===========================================================================================
CObject2Danim *CObject2Danim::Create(D3DXVECTOR3 pos)
{
	CObject2Danim *pObject2Danim = NULL;

	if (pObject2Danim == NULL)
	{
		//オブジェクト2Dの生成
		pObject2Danim = new CObject2Danim;

		if (pObject2Danim != NULL)
		{
			//初期化処理
			pObject2Danim->Init();

			pObject2Danim->SetPosition(pos);

			return pObject2Danim;
		}
		else
		{
			return NULL;
		}
	}

	return NULL;
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CObject2Danim::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bomb0.png", &m_pTexture)))
	{
		return E_FAIL;
	}
	
	CObject2D::Init();

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CObject2Danim::Uninit(void)
{
	CObject2D::Uninit();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CObject2Danim::Update(void)
{
	
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CObject2Danim::Draw(void)
{
	CObject2D::Draw();
}

//===========================================================================================
// アニメーション処理
//===========================================================================================
void CObject2Danim::SetAnimation(int nPttern)
{
	//　頂点情報へのポインタ
	VERTEX_2D *pVtx;

	m_fTexSizeX = 1.0f / nPttern;
	
	if ((m_nCntPttern % ANIM_FIRST) == 0)
	{
		//　パターンNo.を更新する
		m_nPttern = (m_nPttern + 1) % nPttern;

		//　頂点バッファをロックし、頂点情報へのポインタを取得
		CObject2D::GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

		//　頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2((m_nPttern % nPttern / 2) * m_fTexSizeX, (m_nPttern / (nPttern / 2)) * TEX_ANIM2);
		pVtx[1].tex = D3DXVECTOR2((m_nPttern % nPttern / 2) * m_fTexSizeX + m_fTexSizeX, (m_nPttern / (nPttern / 2)) * TEX_ANIM2);
		pVtx[2].tex = D3DXVECTOR2((m_nPttern % nPttern / 2) * m_fTexSizeX, (m_nPttern / (nPttern / 2)) * TEX_ANIM2 + TEX_ANIM2);
		pVtx[3].tex = D3DXVECTOR2((m_nPttern % nPttern / 2) * m_fTexSizeX + m_fTexSizeX, (m_nPttern / (nPttern / 2)) * TEX_ANIM2 + TEX_ANIM2);

		//　頂点バッファをアンロックする
		CObject2D::GetVtxBuff()->Unlock();

		if (m_nPttern == nPttern - 1)
		{
			Uninit();
		}
	}

	//　アニメーションカウント
	m_nCntPttern++;
}

//===========================================================================================
// アニメーション処理
//===========================================================================================
void CObject2Danim::SetTexture(float fTexU, float fTexV)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	CObject2D::GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTexV);
	pVtx[3].tex = D3DXVECTOR2(fTexU, fTexV);

	//頂点バッファをアンロックする
	CObject2D::GetVtxBuff()->Unlock();
}
