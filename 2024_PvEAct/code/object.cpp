//===========================================================================================
//
// [object.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"

//***************************************************
// 静的メンバ変数の宣言
//***************************************************
CObject* CObject::m_apTop[object::NUM_PRIORITY] = {};	// 先頭のオブジェクトへのポインタ
CObject* CObject::m_apEnd[object::NUM_PRIORITY] = {};	// 最後尾のオブジェクトへのポインタ
int CObject::m_nNumAll = 0;
int CObject::m_nNumEffectAll = 0;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CObject::CObject(int nPriority)
{
	//値の初期化
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_bDeath = false;
	m_type = TYPE_NONE;

	if (nPriority < 0)
	{
		nPriority = 0;
	}
	else if (nPriority > object::NUM_PRIORITY)
	{
		nPriority = object::NUM_PRIORITY - 1;
	}

	m_nPriority = nPriority;

	if (m_apTop[m_nPriority] == nullptr && m_apEnd[m_nPriority] == nullptr)
	{//先頭、最後尾に情報が入っていなかった場合
		m_apTop[m_nPriority] = this;	//先頭アドレスに自分自身を代入
		m_apEnd[m_nPriority] = this;	//最後尾アドレスに自分自身を代入
	}
	else
	{
		m_pPrev = m_apEnd[m_nPriority];	//前のオブジェクトを設定
		m_apEnd[m_nPriority] = this;	//最後尾アドレスに自分自身を代入
		m_pPrev->m_pNext = this;	//次のオブジェクトを設定
	}

	m_nNumAll++;	//オブジェクト総数のカウントアップ
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CObject::~CObject()
{
	m_nNumAll--;	//オブジェクト総数のカウントダウン
}

//===========================================================================================
// 全てのオブジェクトの破棄
//===========================================================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < object::NUM_PRIORITY; nCntPriority++)
	{
		CObject* pObject = m_apTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			pObject->Uninit();	//終了

			pObject = pObjectNext;	//次のオブジェクトを代入
		}
	}

	for (int nCntPri = 0; nCntPri < object::NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject* pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject* pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath == true)
			{
				// 削除
				pObject->Death();
			}

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}
}

//===========================================================================================
// 全てのオブジェクトの更新
//===========================================================================================
void CObject::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < object::NUM_PRIORITY; nCntPriority++)
	{
		CObject* pObject = m_apTop[nCntPriority];	// 先頭オブジェクトを代入

		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext;	// 次のオブジェクトを保存

			pObject->Update();	// 更新

			pObject = pObjectNext;	// 次のオブジェクトを代入
		}
	}

	for (int nCntPri = 0; nCntPri < object::NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject* pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject* pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath == true)
			{
				// 削除
				pObject->Death();
			}

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}
}

//===========================================================================================
// 全てのオブジェクトの描画
//===========================================================================================
void CObject::DrawAll(void)
{
	//カメラ情報の取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	//カメラのセット
	if (pCamera != nullptr)
	{
		pCamera->SetCamera();
	}

	for (int nCntPriority = 0; nCntPriority < object::NUM_PRIORITY; nCntPriority++)
	{
		CObject* pObject = m_apTop[nCntPriority];	//先頭オブジェクトを代入

		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext;	//次のオブジェクトを保存

			pObject->Draw();	//描画

			pObject = pObjectNext;	//次のオブジェクトを代入
		}
	}

	for (int nCntPri = 0; nCntPri < object::NUM_PRIORITY; nCntPri++)
	{
		// 先頭オブジェクトを代入
		CObject* pObject = m_apTop[nCntPri];

		while (pObject != nullptr)
		{
			// 次のアドレスを保存
			CObject* pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath == true)
			{
				// 削除
				pObject->Death();
			}

			// 次のアドレスを代入
			pObject = pObjectNext;
		}
	}
}

//===================================================
// オブジェクトの破棄
//===================================================
void CObject::Release(void)
{
	m_bDeath = true;	//死亡状態へ
}

//===================================================
// 死亡フラグが立っているオブジェクトを破棄する
//===================================================
void CObject::Death(void)
{
	if (m_apEnd[m_nPriority] != this && m_apTop[m_nPriority] != this)
	{// 真ん中のアドレスの破棄
		if (m_pPrev != nullptr)
		{
			// 前のアドレスから次のアドレスをつなぐ
			m_pPrev->m_pNext = m_pNext;
		}

		if (m_pNext != nullptr)
		{
			// 次のアドレスから前のアドレスをつなぐ
			m_pNext->m_pPrev = m_pPrev;
		}
	}

	if (m_apTop[m_nPriority] == this)
	{// 先頭アドレスの破棄
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = nullptr;
		}

		// 先頭アドレスを次のアドレスに引き継ぐ
		m_apTop[m_nPriority] = m_pNext;
	}

	if (m_apEnd[m_nPriority] == this)
	{// 最後尾アドレスの破棄
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = nullptr;
		}

		// 最後尾アドレスを前のアドレスに引き継ぐ
		m_apEnd[m_nPriority] = m_pPrev;
	}

	delete this;
}

