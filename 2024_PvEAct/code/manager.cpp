//===========================================================================================
//
// [manager.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include"renderer.h"
#include "object.h"
#include "manager.h"
#include "managerInput.h"
#include "debugproc.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "xfile.h"

#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"

#include "object3D.h"

//静的メンバ変数宣言
CManager* CManager::m_pManager = nullptr;
CScene::EMode CScene::m_mode = CScene::MODE_TITLE;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CManager::CManager()
{
	// 値のクリア
	ZeroMemory(&m_info, sizeof(m_info));
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CManager::~CManager()
{

}

//===========================================================================================
// インスタンスの取得
//===========================================================================================
CManager* CManager::GetInstance(void)
{
	if (m_pManager == nullptr)
	{
		m_pManager = new CManager;

		return m_pManager;
	}

	return m_pManager;
}

//===========================================================================================
// 破棄
//===========================================================================================
void CManager::Release(void)
{
	if (m_pManager == nullptr)
	{
		return;
	}
	else
	{
		m_pManager->Uninit();

		delete m_pManager;
		m_pManager = nullptr;
	}
}


//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//---------------------------
	// インスタンス生成
	//---------------------------
	// レンダラーの生成
	if (m_info.pRenderer == nullptr)
	{
		m_info.pRenderer = new CRenderer;

		if (m_info.pRenderer != nullptr)
		{
			//レンダラーの初期化処理
			if (FAILED(m_info.pRenderer->Init(hWnd, TRUE)))
			{
				return E_FAIL;
			}
		}
	}

	// インプットマネージャーの生成
	CManagerInput::Create(hInstance, hWnd);

	// サウンド
	//if (m_info.m_pSound == nullptr)
	//{
	//	m_info.m_pSound = new CSound;
	//	if (m_info.m_pSound != nullptr)
	//	{
	//		// サウンドの初期化処理
	//		m_info.m_pSound->Init(hWnd);
	//	}
	//}
	// デバッグプロックの生成
	if (m_info.pDebugProc == nullptr)
	{
		m_info.pDebugProc = new CDebugProc;

		// 初期化処理
		if (m_info.pDebugProc != nullptr)
		{
			m_info.pDebugProc->Init();
		}
	}
	// カメラの生成
	if (m_info.pCamera == nullptr)
	{
		m_info.pCamera = new CCamera;

		// 初期化処理
		if (m_info.pCamera != nullptr)
		{
			m_info.pCamera->Init();
		}
	}
	// ライトの生成
	if (m_info.pLight == nullptr)
	{
		m_info.pLight = new CLight;

		// 初期化処理
		if (m_info.pLight != nullptr)
		{
			m_info.pLight->Init();
		}
	}

	// テクスチャの生成
	CTexture::Create();

	// Xファイルの生成
	CXfile::Create();

	// モード設定
	SetMode(GetMode());
	
	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CManager::Uninit(void)
{
	// 全オブジェクトの破棄(+終了処理)
	CObject::ReleaseAll();

	// インプットマネージャーの破棄
	CManagerInput::Release();

	// デバッグプロック破棄
	if (m_info.pDebugProc != nullptr)
	{
		// デバッグプロックの終了処理
		m_info.pDebugProc->Uninit();

		//メモリの開放
		delete m_info.pDebugProc;
		m_info.pDebugProc = nullptr;
	}
	// レンダラーの破棄
	if (m_info.pRenderer != nullptr)
	{
		// レンダラーの終了処理
		m_info.pRenderer->Uninit();

		// メモリの開放
		delete m_info.pRenderer;
		m_info.pRenderer = nullptr;
	}
	// サウンドの破棄
	//if (m_info.pSound != nullptr)
	//{
	//	//サウンドの終了処理
	//	m_info.pSound->Stop();
	//	m_info.pSound->Uninit();

	//	delete m_info.pSound;
	//	m_info.pSound = nullptr;
	//}
	// カメラの破棄
	if (m_info.pCamera != nullptr)
	{
		//カメラの終了処理
		m_info.pCamera->Uninit();
		m_info.pCamera->Uninit();

		delete m_info.pCamera;
		m_info.pCamera = nullptr;
	}
	// ライトの破棄
	if (m_info.pLight != nullptr)
	{
		//ライトの終了処理
		m_info.pLight->Uninit();

		delete m_info.pLight;
		m_info.pLight = nullptr;
	}

	// テクスチャの破棄
	CTexture::Release();

	// Xファイルの破棄
	CXfile::Release();

	// シーンの終了、破棄
	if (m_info.pScene != nullptr)
	{
		// シーンの終了処理
		m_info.pScene->Uninit();
		delete m_info.pScene;
		m_info.pScene = nullptr;
	}
	
	if (m_info.pFade != nullptr)
	{
		m_info.pFade = nullptr;
	}
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CManager::Update(void)
{
	CManager::GetDebugProc()->Print("FPS[%d]\n", GetFps());
	CManager::GetDebugProc()->Print("オブジェクト総数[%d]\n\n", CObject::GetNumAll());

	m_info.pDebugProc->Print("【 操作方法 】\n");
	m_info.pDebugProc->Print("F1 : デバッグ表示[ON/OFF]\nF2 : 法線表示[ON/OFF]\n");

	// デバッグプロックの更新処理
	if (CManager::GetDebugProc() != nullptr)
	{
		CManager::GetDebugProc()->Update();
	}

	// インプットマネージャーの更新処理
	CManagerInput::GetInstance()->Update();

	// シーンの更新
	if (m_info.pScene != nullptr)
	{
		m_info.pScene->Update();
	} 
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CManager::Draw(void)
{
	//レンダラーの描画処理
	m_info.pScene->Draw();
}

//===========================================================================================
// モードの設定
//===========================================================================================
void CManager::SetMode(CScene::EMode mode)
{
	//// サウンドの停止
	//if (m_pManager->m_info.pSound != nullptr)
	//{
	//	m_pManager->m_info.pSound->Stop();
	//}

	// 現在のモードを破棄
	if (m_pManager->m_info.pScene != nullptr)
	{
		m_pManager->m_info.pScene->Uninit();
		delete m_pManager->m_info.pScene;
		m_pManager->m_info.pScene = nullptr;
	}

	// 全オブジェクトの削除
	CObject::ReleaseAll();

	// テクスチャの終了、破棄
	if (CTexture::GetInstance() != nullptr)
	{
		// テクスチャの破棄
		CTexture::Release();

		// テクスチャの生成
		CTexture::Create();
	}

	// Xファイルの破棄
	if (CXfile::GetInstance() != nullptr)
	{
		// Xファイルの破棄
		CXfile::Release();

		// Xファイルの生成
		CXfile::Create();
	}
	
	// フェードの削除
	m_pManager->m_info.pFade = nullptr;

	// 新しいモードの生成
	m_pManager->m_info.pScene = CScene::Create(mode);
	m_pManager->m_info.pScene->SetMode(mode);

	if (m_pManager->m_info.pScene != nullptr)
	{
		m_pManager->m_info.pScene->Init();
	}

	// フェードの生成
	if (m_pManager->m_info.pFade == nullptr)
	{
		m_pManager->m_info.pFade = CFade::Create(mode);
	}
}