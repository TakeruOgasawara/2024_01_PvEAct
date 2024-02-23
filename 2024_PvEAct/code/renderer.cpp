//===========================================================================================
//
// [renderer.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "object2D.h"
#include "debugproc.h"
#include "sound.h"
#include "inputKeyboard.h"

//===========================================================================================
// コンストラクタ
//===========================================================================================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;	// Directx3Dオブジェクトへのポインタ
	m_pD3DDevice = nullptr;	// Directx3Dへのデバイスへのポインタ

	m_bWireframe = false;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CRenderer::~CRenderer()
{

}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;				//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;		//プレゼンテーションパラメータ

	//DIrect3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のデバイスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));				//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;			//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			//ゲームの画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;			//バックバッファ形式
	d3dpp.BackBufferCount = 1;						//バックバッファの数

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;			//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;		//デプスバッファとして16bit扱う
	d3dpp.Windowed = bWindow;						//ウインドモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;				//インターバル

	//Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//背景
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャーステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//乱数の種を指定(rand使用時、毎回数値を変える)
	srand((unsigned int)time(0));

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CRenderer::Uninit(void)
{
	//Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	//Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CRenderer::Update(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CInputKeyboard *pInputKey = CInputKeyboard::GetInstance();

#ifdef _DEBUG	// デバック時のみ実行
	if (pInputKey->GetTrigger(DIK_F2) == true)
	{//ワイヤーフレーム表示処理
		if (m_bWireframe == true)
		{
			m_bWireframe = false;
		}
		else
		{
			m_bWireframe = true;
		}
	}
	if (m_bWireframe == true)
	{//ワイヤーフレームを表示する
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else if (m_bWireframe == false)
	{//ワイヤーフレームの表示を戻す
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DRS_ZENABLE);
	}
#endif

	//全更新処理
	CObject::UpdateAll();
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CRenderer::Draw(void)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	//画面をクリア(バックバッファとZバッファのクリア)
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		//全更新処理
		CObject::DrawAll();

		// デバッグ表示
		if (pDebugProc != NULL)
		{
			//デバッグプロックの描画処理
			pDebugProc->Draw();
		}

		//描画終了
		m_pD3DDevice->EndScene();
	}

	//バクバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//===================================================
// デバイスの取得
//===================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}