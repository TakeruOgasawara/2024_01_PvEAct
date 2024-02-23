//======================================================================================
//
// フェード処理[fade.cpp]
// Author : Takeru Ogasawara
//
//======================================================================================
#include "fade.h"

//マクロ定義
#define FADE_TIME		(0.04f)			//フェードの間隔

//========================================================================
// コンストラクタ
//========================================================================
CFade::CFade(int nPriority) : CObject2D(nPriority)
{
	m_state = STATE_NONE;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//========================================================================
// デストラクタ
//========================================================================
CFade::~CFade()
{

}

//========================================================================
// 生成
//========================================================================
CFade *CFade::Create(CScene::EMode mode)
{
	CFade *pFade = nullptr;

	if (pFade == nullptr)
	{
		pFade = new CFade;

		if (pFade != nullptr)
		{
			pFade->Init(mode);
			pFade->SetState(STATE_IN);
			return pFade;
		}
	}

	return pFade;
}

//========================================================================
// 初期化処理(オーバーロード)
//========================================================================
HRESULT CFade::Init(CScene::EMode modeNext)
{
	//生成
	CObject2D::Init();

	//位置・サイズ・色の設定
	SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//モードの設定
	Set(modeNext);

	return S_OK;
}

//========================================================================
// 終了処理
//========================================================================
void CFade::Uninit(void)
{
	// 親クラスの破棄
	CObject2D::Uninit();
}

//========================================================================
// 更新処理
//========================================================================
void CFade::Update(void)
{
	if (m_state != STATE_NONE)
	{
		if (m_state == STATE_IN)
		{//フェードイン状態

			m_col.a -= FADE_TIME;			//ポリゴンを透明にしていく

			if (m_col.a <= 0.0f)
			{
				m_col.a = 0.0f;
				m_state = STATE_NONE;				//何もしていない状態
			}
		}
		else if (m_state == STATE_OUT)
		{//フェードアウト状態
			m_col.a += FADE_TIME;			//ポリゴンを不透明にしていく

			if (m_col.a >= 1.0f)
			{
				m_col.a = 1.0f;

				//モードの設定(次の画面に移動)
				CManager::SetMode(m_modeNext);

				return;
			}
		}
		
		//色の反映
		SetColor(m_col);
	}
}

//========================================================================
// 描画処理
//========================================================================
void CFade::Draw(void)
{
	if (m_state != STATE_NONE)
	{
		CObject2D::Draw();
	}
}

//========================================================================
// 設定処理
//========================================================================
void CFade::Set(CScene::EMode modeNext)
{
	if (m_state != STATE_OUT)
	{
		m_state = STATE_OUT;			//フェードアウト状態
		m_modeNext = modeNext;			//次の画面(モード)を設定
		m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

//========================================================================
//フェード状態の設定
//========================================================================
void CFade::SetState(STATE state)
{
	m_state = state;

	switch (m_state)
	{
	case STATE_NONE:
		m_col.a = 0.0f;
		break;

	case STATE_IN:
		m_col.a = 1.0f;
		break;

	case STATE_OUT:
		m_col.a = 0.0f;
		break;

	default:

		break;
	}

	SetColor(m_col);
}