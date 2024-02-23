//===============================================================================
//
// [janken.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _JANKEN_H_				//このマクロ定義がされていなかったら
#define _JANKEN_H_				//2重インクルード防止のマクロを定義

#include "main.h"
#include "object.h"

// 前方宣言
class CObject2D;

// じゃんけんクラス
class CJanken : public CObject
{
public:
	// プレイヤー種類
	enum EPlayer
	{
		PLAYER_PLAYER,		// 一人
		PLAYER_COM,		// コンピュータ
		PLAYER_MAX
	};

	// じゃんけんの種類
	enum EJanken
	{
		JANKEN_NONE = 0,
		JANKEN_GU,
		JANKEN_CHOKI,
		JANKEN_PA,
		JANKE_MAX
	};

	// じゃんけん結果
	enum EResult
	{
		RESULT_NONE = 0,
		RESULT_WIN,
		RESULT_LOSE,
		RESULT_DRAW,
		RESULT_MAX
	};

	// じゃんけんの流れ
	enum EFlow
	{
		FLOW_NONE = 0,
		FLOW_INIT,
		FLOW_CHECK_ANSWER,
		FLOW_END,
		FLOW_MAX
	};

	struct SPlayerInfo
	{
		EJanken janken;	
		EResult result;
		bool bAnswer;	// 答えたかどうか
	};

public:
	CJanken(int nPriority = 3);	// コンストラクタ
	~CJanken();	// デストラクタ

	static CJanken* Create();
	static CJanken* GetInstace(void);
	static void Release(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetJanken(EJanken janken);
	void SetFlow(EFlow flow);
	void IsJanken(bool bJanken);
	bool IsJanken(void) { return m_bJanken; }
	EResult GetResult(int nNumPlayer) { return m_aPlayerInfo[nNumPlayer].result; }

	void SetPosition(D3DXVECTOR3 pos) {}
	void SetRotation(D3DXVECTOR3 rot) {}
	D3DXVECTOR3 GetPosition(void) const {return {};}
	D3DXVECTOR3 GetRotation(void) const {return {};}

private:

	void Flow(void);	// 更新のマネージャ
	void ComAnswer(void);	// コンピュータの回答
	void CheckAnswer(void);	// 答え合わせ
	void continuity(void);	// 連続

	CObject2D* m_apChoices2dUI[3];	// 選択UI
	SPlayerInfo m_aPlayerInfo[PLAYER_MAX];	// プレイヤー情報
	EFlow m_flow;	// 流れ
	int m_nNumAllWin;	// 勝利数
	int m_nNum;	// じゃんけん数
	bool m_bJanken;	// じゃんけんをするかどうか

	static CJanken* m_pJanken;	// 自分自身のポインタ
};

#endif