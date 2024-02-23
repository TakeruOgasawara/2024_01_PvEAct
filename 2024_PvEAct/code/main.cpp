//===========================================================================================
//
// [main.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include"renderer.h"
#include "object.h"
#include "object2D.h"
#include "manager.h"
#include "debugproc.h"

//マクロ定義
#define CLASS_NAME		"Windowclass"			//ウィンドウクラスの名前
#define WINDOW_NAME		"Directxの初期設定"		//ウィンドウの名前
#define MAXIMIZE_WINDOW	(TRUE)					//ウィンドウ最大化のON/OFF

#define ID_BUTTON_FINISH	(101)				//終了ボタンのID

//プロタイプ宣言
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPalam);

//グローバル宣言
int g_nCountFPS = 0;							//FPSカウンター

//===========================================================================================
// メイン関数
//===========================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR LpCmadLine, int nCmdShow)
{
	//終了時にメモリリークを出力
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WNDCLASSEX wcex
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							//ウィンドウのスタイル
		WindowProc,							//ウィンドウプロシージャ
		0,									//0にする(通常は使用しない)
		0,									//0にする(通常は使用しない)
		hInstance,							//インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),	//タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),		//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		NULL,								//メニューバー
		CLASS_NAME,							//ウィンドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)		//ファイルのアイコン
	};

	HWND hWnd;				//ウィンドウハンドル(識別子)
	MSG msg;				//メッセージを格納する変数

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//ウィンドウを生成
	hWnd = CreateWindowEx(0,	//拡張ウィンドウスタイル
		CLASS_NAME,				//ウィンドウクラス名
		WINDOW_NAME,			//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
		CW_USEDEFAULT,			//ウィンドウの左上X座標
		CW_USEDEFAULT,			//ウィンドウの左上Y座標
		SCREEN_WIDTH,			//ウィンドウの幅
		SCREEN_HEIGHT,			//ウィンドウの高さ
		NULL,					//親ウィンドウのハンドル
		NULL,					//メニューハンドルまたは子ウィンドウID
		hInstance,				//インスタンスハンドル
		NULL);					//ウィンドウ制作データ

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };				//画面サイズの構造体

	//ウインドクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, MAXIMIZE_WINDOW);

	//ウィンドウを生成
	hWnd = CreateWindowEx(0,	// 拡張ウィンドウスタイル
		CLASS_NAME,				// ウィンドウクラス名
		WINDOW_NAME,			// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル
		CW_USEDEFAULT,			// ウィンドウの左上X座標
		CW_USEDEFAULT,			// ウィンドウの左上Y座標
		(rect.right - rect.left),	// ウィンドウの幅
		(rect.bottom - rect.top),	// ウィンドウの高さ
		NULL,					// 親ウィンドウのハンドル
		NULL,					// メニューハンドルまたは子ウィンドウID
		hInstance,				// インスタンスハンドル
		NULL);					// ウィンドウ制作データ

	DWORD dwCurrentTime;		// 現在時刻
	DWORD dwExecLastTime;		// 最後に処理した時刻
	DWORD dwFrameCount;			// フレームカウント
	DWORD dwFPSLastTime;		// 最後にFPSを

	// 分解能を設定
	timeBeginPeriod(1);

	// 初期化処理
	dwCurrentTime = 0;					// 初期化する
	dwExecLastTime = timeGetTime();		// 現在時刻を取得(保存)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);			// ウィンドウの表示状態を設定
	UpdateWindow(hWnd);					// クライアント領域を更新

	// マネージャの初期化
	CManager::GetInstance()->Init(hInstance, hWnd, TRUE);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else 
			{
				// メッセージを設定
				TranslateMessage(&msg);			// 仮想キーメッセージを文字へ変換
				DispatchMessage(&msg);			// ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{// Directxの処理
			dwCurrentTime = timeGetTime();		// 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
				// FPSを経過
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;					// FPSを測定した時刻を保存
				dwFrameCount = 0;								// フレームカウントをクリア

			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60分の1秒経過
				float fTick = (float)(dwCurrentTime - dwExecLastTime) * 0.001f;

				dwExecLastTime = dwCurrentTime;	// 処理開始の時刻(現在時刻)を保存

				// ティックの設定
				CManager::GetInstance()->SetTick(fTick);

				// マネージャの更新処理
				CManager::GetInstance()->Update();

				// マネージャの描画処理
				CManager::GetInstance()->Draw();

				dwFrameCount++;	// フレームカウントを加算
			}
		}
	}

	// マネージャの削除
	CManager::GetInstance()->Release();
	
	//分解能を戻す
	timeEndPeriod(1);

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//===============================
// ウィンドウプロシージャ
//===============================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;							//返り値を格納

	switch (uMsg)
	{
	case WM_DESTROY:			//ウィンドウ破棄のメッセージ

								//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:			//キー押下のメッセージ

		switch (wParam)
		{
		case VK_ESCAPE:			//[ESC]キーが押された

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
				break;
			}
		}
		break;

	case WM_COMMAND:	//コマンド発行のメッセージ
		if (LOWORD(wParam) == ID_BUTTON_FINISH)
		{//終了ボタンが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
		}

		break;

	case WM_CLOSE:		//コマンド発行のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		//0を返さないと終了してしまう
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int GetFps(void)
{
	return g_nCountFPS;
}
