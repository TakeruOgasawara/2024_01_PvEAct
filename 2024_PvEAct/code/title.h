//===============================================================================
//
// タイトル画面[title.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _TITLE_H_				//このマクロ定義がされていなかったら
#define _TITLE_H_				//2重インクルード帽子のマクロを定義

#include "main.h"
#include "manager.h"

//前方宣言
class CObject2D;
class CMap;

//タイトルクラス
class CTitle : public CScene
{
private:

	enum STATE
	{
		STATE_NONE = 0,
		STATE_IN,
		STATE_OUT,
		STATE_MAX
	};

	struct SUI
	{
		CObject2D* pBg;
		CObject2D* pPress;
	};

public:
	CTitle();
	~CTitle();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw(); 

private:
	// メンバ関数
	void CreateBg(void);
	void CreatePress(void);
	void UpdatePress(void);

	// メンバ変数
	int m_nTextureIdx;	//テクスチャへの頂点情報
	SUI m_ui;
	STATE m_state;
};

#endif