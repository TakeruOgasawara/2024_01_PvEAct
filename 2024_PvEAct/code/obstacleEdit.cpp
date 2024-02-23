//===========================================================================================
//
// 障害物エディット[ObstacleEdit.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "obstacleEdit.h"
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "object.h"
#include "xfile.h"
#include "inputKeyboard.h"
#include "debugproc.h"

#include <stdio.h>

// 定数定義
namespace
{
	// xファイルパス
	const char* apPathList[CObstacleEdit::TYPE_MAX] =
	{
		"data\\MODEL\\object\\TanukiShop.x",
		"data\\MODEL\\object\\mitumisi.x",
	};

	// 速さ
	const float SPEED = 20.0f;
}

// 静的メンバ変数宣言
int CObstacleEdit::m_nNumAll = 0;

//===========================================================================================
// コンストラクタ
//===========================================================================================
CObstacleEdit::CObstacleEdit(int nPriority) /*: CObject(nPriority)*/
{
	m_bUse = false;
	m_type = TYPE_HOUSE;

	m_nNumAll++;	// 総数カウントアップ
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CObstacleEdit::~CObstacleEdit()
{
	m_nNumAll--;	// 総数のカウントダウン
}

//===========================================================================================
// 生成
//===========================================================================================
CObstacleEdit* CObstacleEdit::Create(void)
{
	CObstacleEdit* pObjX = nullptr;

#ifdef _DEBUG	// デバック時実行

	if (pObjX == nullptr)
	{
		pObjX = new CObstacleEdit;

		if (pObjX != NULL)
		{
			pObjX->Init();

			return pObjX;
		}
	}

#endif

	return pObjX;
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CObstacleEdit::Init(void)
{
	// 選択モデル番号の設定
	m_nTypeIdx = TYPE_HOUSE;

	// 
	m_type = (EType)m_nTypeIdx;

	// 初期化処理
	CObstacle::Init(apPathList[0], { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 0.0f, false);

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CObstacleEdit::Uninit(void)
{
	Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CObstacleEdit::Update(void)
{
	// 情報の取得
	CInputKeyboard* pInputKey = CInputKeyboard::GetInstance();
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CXfile* pXfile = CXfile::GetInstance();

	D3DXVECTOR3 scale = GetScale();
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// 操作
	Operation(&pos, &rot);

	// 大きさの反映
	SetScale(scale);

	// 位置の反映
	SetPosition(pos);

	// 向きの反映
	SetRotation(rot);

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print("\n\n【エディットモード中】\n\n");
	CManager::GetInstance()->GetDebugProc()->Print("位置： x:%f y:%f z:%f\n", pos.x, pos.y, pos.z);
	CManager::GetInstance()->GetDebugProc()->Print("向き： x:%f y:%f z:%f\n", rot.x, rot.y, rot.z);
	CManager::GetInstance()->GetDebugProc()->Print("種類： %d\n\n", m_type);

	CManager::GetInstance()->GetDebugProc()->Print("設置： ENTER\n");
	CManager::GetInstance()->GetDebugProc()->Print("種類変更： 1 or 2\n");
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CObstacleEdit::Draw(void)
{
	CObstacle::Draw();
}

//===========================================================================================
// 操作
//===========================================================================================
void CObstacleEdit::Operation(D3DXVECTOR3* pos, D3DXVECTOR3* rot)
{
	// 情報の取得
	CInputKeyboard* pInputKey = CInputKeyboard::GetInstance();
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CXfile* pXfile = CXfile::GetInstance();

	// 移動
	if (pInputKey->GetTrigger(DIK_Q))
	{
		pos->y += SPEED;
	}
	if (pInputKey->GetTrigger(DIK_E))
	{
		pos->y -= SPEED;
	}
	if (pInputKey->GetTrigger(DIK_A))
	{
		pos->x -= SPEED;
	}
	if (pInputKey->GetTrigger(DIK_D))
	{
		pos->x += SPEED;
	}
	if (pInputKey->GetTrigger(DIK_W))
	{
		pos->z += SPEED;
	}
	if (pInputKey->GetTrigger(DIK_S))
	{
		pos->z -= SPEED;
	}

	// オブジェクト種類の変更
	if (pInputKey->GetTrigger(DIK_1))
	{
		// カウントアップ
		m_nTypeIdx = (m_nTypeIdx - 1 + TYPE_MAX) % TYPE_MAX;

		m_type = (EType)m_nTypeIdx;

		SetModel(apPathList[m_nTypeIdx]);
	}
	else if (pInputKey->GetTrigger(DIK_2))
	{
		// カウントダウン
		m_nTypeIdx = (m_nTypeIdx + 1 + TYPE_MAX) % TYPE_MAX;

		m_type = (EType)m_nTypeIdx;

		SetModel(apPathList[m_nTypeIdx]);
	}

	// オブジェクトの設置
	if (pInputKey->GetTrigger(DIK_RETURN))
	{
		CObstacle::Create(apPathList[m_nTypeIdx], *pos);
	}
	
	// ファイル書き込み(セーブ)
	if (pInputKey->GetTrigger(DIK_F9))
	{
		//Save();
	}
}

//===========================================================================================
// 書き込み
//===========================================================================================
void CObstacleEdit::Save(void)
{
	////ポインタ宣言
	//FILE* pFile;

	////ファイルを開く
	//pFile = fopen("", "wb");

	//if (pFile != nullptr)
	//{//ファイルが開けた場合
	//	fwrite(&m_nNumAll, sizeof(int), 1, pFile);

	//	for (int nCntBlock = 0; nCntBlock < m_nNumAll; nCntBlock++)
	//	{
	//		if (m_apBlock[nCntBlock] != nullptr)
	//		{
	//			memBlock.pos = m_apBlock[nCntBlock]->GetPosition();
	//			memBlock.rot = m_apBlock[nCntBlock]->GetRot();
	//			memBlock.type = m_apBlock[nCntBlock]->m_type;

	//			//バイナリファイルに書き込む
	//			fwrite(&memBlock, sizeof(MemBlock), 1, pFile);
	//		}
	//	}

	//	//ファイルを閉じる
	//	fclose(pFile);
	//}
	//else
	//{//ファイルが開けなかった場合
	//	assert(("ブロックデータの保存失敗", false));
	//}
}
