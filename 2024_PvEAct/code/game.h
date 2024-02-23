//===============================================================================
//
// ゲーム画面[game.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _GAME_H_				//このマクロ定義がされていなかったら
#define _GAME_H_				//2重インクルード帽子のマクロを定義

#include "main.h"
#include "manager.h"

//前方宣言
class CMap;
class CSlowManager;
class CPause;
class CSound;
class CObject2D;

//タイトルクラス
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CPause *GetPause(void) { return m_pPause; }

private:
	void SlowUpdate(void);
	static CPause *m_pPause;
	CSound *m_pSound;
};

#endif