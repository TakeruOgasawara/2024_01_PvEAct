//===============================================================================
//
// [main.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef MAIN_H_	// このマクロ定義がされていなかったら
#define MAIN_H_	//2 重インクルード防止のマクロを定義
#define DIRECTINPUT_VERSION (0x0800)	// ビルド時の警告対処用マクロ
#include <windows.h>
#include <time.h>	// randの値を毎回変えるために必要
#include <assert.h>	// 警告を使うために必要
#include "d3dx9.h"	// 描画処理に必要
#include "dinput.h"	// 入力処理に必要
#include "xaudio2.h"	// サウンド処理に必要

//***************************************************
//ライブラリのリンク
//***************************************************
#pragma comment(lib, "d3d9.lib")			//描画処理に必要
#pragma comment(lib, "d3dx9.lib")			//[d3d9]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")			//Directxコンポーネント使用に必要
#pragma comment(lib, "winmm.lib")			//システム時刻の取得に必要
#pragma comment(lib, "dinput8.lib")			//入力処理に必要

//***************************************************
// マクロ定義
//***************************************************
#define CLASS_NAME		"Windowclass"	// ウィンドウクラスの名前
#define SCREEN_WIDTH	(1280)	// ウィンドウの幅
#define SCREEN_HEIGHT	(720)	// ウィンドウの高さ

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット[2D]
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット[3D]

//***************************************************
// 頂点情報[2D]の構造体を定義
//***************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標

}VERTEX_2D;

//***************************************************
// 頂点情報[3D]の構造体を定義
//***************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_3D;

int GetFps(void);

//***************************************************
//メモリリーク出力用マクロ
//***************************************************
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>			// メモリリーク検出に必要

//#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

//#else
////#define DBG_NEW new
//#endif

#endif

