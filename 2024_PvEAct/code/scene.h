//===============================================================================
//
// シーン処理[scene.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _SCENE_H_				//このマクロ定義がされていなかったら
#define _SCENE_H_				//2重インクルード防止のマクロを定義

#include "main.h"

//***********************************
// 前方宣言
//***********************************
class CTitle;
class CGame;
class CResult;
class CFade;

//***********************************
// シーンクラス
//***********************************
class CScene
{
public:
	// シーン種類
	enum EMode
	{
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	} ;

	CScene();	// コンストラクタ
	~CScene();	// デストラクタ

	static CScene* Create(EMode mode);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetMode(EMode mode) { m_mode = mode; }
	static EMode GetMode(void) { return m_mode; }

private:
	static EMode m_mode;
};

#endif
