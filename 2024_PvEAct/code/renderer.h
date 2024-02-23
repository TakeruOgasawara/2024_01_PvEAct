//===============================================================================
//
// [renderer.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _RENDERER_H_				//このマクロ定義がされていなかったら
#define _RENDERER_H_				//2重インクルード防止のマクロを定義

#include "main.h"
#include "sound.h"

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void);			//Directx3Dへのデバイスへのポインタ

private:
	LPDIRECT3D9 m_pD3D;						//Directx3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//Directx3Dへのデバイスへのポインタ
	bool m_bWireframe;
};

#endif