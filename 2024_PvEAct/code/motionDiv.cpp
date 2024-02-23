//=============================================================================
// 
//  キャラクター処理[character.cpp]
// Author : Takeru Ogasawara
// 
//=============================================================================

// インクルード宣言
#include "motionDiv.h"
#include "renderer.h"
#include "manager.h"
#include "xfile.h"
#include "texture.h"
#include "modelParts.h"

#include "input.h"
#include "general.h"
#include "debugproc.h"
#include <stdio.h>

//=============================================================================
// コンストラクタ
//=============================================================================
CMotionDiv::CMotionDiv()
{
	// 値のクリア
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_apModelPart[0], sizeof(m_apModelPart));
	ZeroMemory(&m_aMotion[0], sizeof(m_aMotion));
	ZeroMemory(&m_aMotionInfo[0], sizeof(m_aMotionInfo));
	m_fCntMmultiply = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMotionDiv::~CMotionDiv()
{

}

//=============================================================================
//	生成
//=============================================================================
CMotionDiv* CMotionDiv::Create(const char* pUpperPath, const char* pLowerPath)
{
	CMotionDiv* pMotion = nullptr;

	if (pMotion == nullptr)
	{
		pMotion = new CMotionDiv;

		if (pMotion != nullptr)
		{
			pMotion->Init(pUpperPath, pLowerPath);

			return pMotion;
		}
	}

	return pMotion;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMotionDiv::Init()
{

	return S_OK;
}

//=============================================================================
// 初期化処理(オーバーロード)
//=============================================================================
HRESULT CMotionDiv::Init(const char* pUpperPath, const char* pLowerPath)
{
	m_info.pos = { 0.0f, 0.0f, 0.0f };
	m_info.rot = { 0.0f, 0.0f, 0.0f };

	// 読込み
	Load(pUpperPath, pLowerPath);

	// 最初に行うモーションを設定
	InitMotionDiv(0);
	InitPose(0);

	// カウント
	m_fCntMmultiply = 1.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMotionDiv::Uninit()
{
	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{
		for (int nCntParts = 0; nCntParts < m_info.aNumLoadPartsDiv[nCntDiv]; nCntParts++)
		{
			// モデルパーツの破棄
			if (m_apModelPart[nCntDiv][nCntParts]->pParts != nullptr)
			{
				m_apModelPart[nCntDiv][nCntParts]->pParts->Uninit();
				m_apModelPart[nCntDiv][nCntParts]->pParts = nullptr;
			}
			if (m_apModelPart[nCntDiv][nCntParts] != nullptr)
			{
				delete m_apModelPart[nCntDiv][nCntParts];
				m_apModelPart[nCntDiv][nCntParts] = nullptr;
			}
		}
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMotionDiv::Update()
{
	// モーションの更新
	UpdateMotionDiv();
}

//=============================================================================
// 描画処理
//=============================================================================
void CMotionDiv::Draw()
{
	// マトリックスの設定
	general::SetMatrix(&m_info.mtxWorld, m_info.pos, m_info.rot);

	// マトリックス掛け合わせ
	MultiplyMtx();
}

//=============================================================================
// マトリックス掛け合わせ
//=============================================================================
void CMotionDiv::MultiplyMtx(void)
{
	// 情報取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CTexture* pTexture = CTexture::GetInstance();

	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX* pMtx{}, * pMtxParent{};	// 親のマトリックス

	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{
		if (m_apModelPart[nCntDiv][0] == nullptr)
		{
			continue;
		}

		for (int nCntParts = 0; nCntParts < m_info.aNumLoadPartsDiv[nCntDiv]; nCntParts++)
		{
			// マトリックスの取得
			pMtx = m_apModelPart[nCntDiv][nCntParts]->pParts->GetMatrix();

			// ワールドマトリックス初期化
			D3DXMatrixIdentity(pMtx);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(
				&mtxRot,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetRotation().y,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetRotation().x,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetRotation().z
			);
			D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(
				&mtxTrans,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetPosition().x,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetPosition().y,
				m_apModelPart[nCntDiv][nCntParts]->pParts->GetPosition().z
			);
			D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);

			if (m_apModelPart[nCntDiv][nCntParts]->nIdxParent != -1)
			{// 親パーツがある場合

				// 親マトリックスの取得
				pMtxParent = m_apModelPart[nCntDiv][m_apModelPart[nCntDiv][nCntParts]->nIdxParent]->pParts->GetMatrix();
			}
			else
			{
				if (nCntDiv == DIV_LOWER)
				{
					pMtxParent = &m_info.mtxWorld;
				}
				else if (nCntDiv == DIV_UPPER)
				{// カウントが上半身の場合親を下半身に設定する
					pMtxParent = m_apModelPart[DIV_LOWER][0]->pParts->GetMatrix();
				}
			}

			// 親パーツとパーツのワールドマトリックスをかけ合わせる
			D3DXMatrixMultiply(pMtx, pMtx, pMtxParent);

			// ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, pMtx);

			// パーツの描画
			m_apModelPart[nCntDiv][nCntParts]->pParts->Draw();
		}
	}
}

//=============================================================================
// 最初のモーション
//=============================================================================
void CMotionDiv::InitMotionDiv(int nType)
{
	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{
		for (int nCntKey = 0; nCntKey < m_aMotionInfo[nCntDiv][nType].nNumKey; nCntKey++)
		{
			for (int nCntPart = 0; nCntPart < m_aMotion[nCntDiv].nNumParts; nCntPart++)
			{
				m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart];


				m_apModelPart[nCntDiv][nCntPart]->pParts->SetPosition(D3DXVECTOR3
				(
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fPosX,
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fPosY,
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ)
				);
				m_apModelPart[nCntDiv][nCntPart]->pParts->SetRotation(D3DXVECTOR3
				(
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fRotX,
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fRotY,
					m_aMotionInfo[nCntDiv][nType].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ)
				);
			}
		}
	}
}

//=============================================================================
// モーションの更新
//=============================================================================
void CMotionDiv::UpdateMotionDiv(void)
{
	// 変数宣言
	D3DXVECTOR3 posPart = {}, rotPart = {};	// 
	D3DXVECTOR3 nextPos = {};	// 次の位置
	D3DXVECTOR3 nextRot = {};	// 次の向き
	SKey diff = {}, dest = {};	// 差分・目標
	SKey CurKey = {}, nextKey = {}, oldKey = {};
	int nType = 0;
	int nNextKey = 0;
	float fFrame = 0.0f;

	// 現在の位置を保存
	m_info.posOld = m_info.pos;

	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{

		if (m_apModelPart[nCntDiv][0] == nullptr)
		{
			break;
		}

		nType = m_aMotion[nCntDiv].nType;
		
		if (m_aMotionInfo[nCntDiv][nType].nNumKey <= 0)
		{
			continue;
		}

		// 次のキーの設定
		nNextKey = (m_aMotion[nCntDiv].nKey + 1 + m_aMotionInfo[nCntDiv][nType].nNumKey) % m_aMotionInfo[nCntDiv][nType].nNumKey;

		m_aMotion[nCntDiv].bLoop = m_aMotionInfo[nCntDiv][nType].bLoop;

		for (int nCntPart = 0; nCntPart < m_info.aNumLoadPartsDiv[nCntDiv]; nCntPart++)
		{
			// パーツ情報の取得
			posPart = m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosOrg();
			rotPart = m_apModelPart[nCntDiv][nCntPart]->pParts->GetRotation();

			// フレーム情報の代入
			fFrame = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].fFrame;
			// 現在のキー情報を代入
			CurKey.fPosX = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fPosX;
			CurKey.fPosY = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fPosY;
			CurKey.fPosZ = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fPosZ;
			CurKey.fRotX = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fRotX;
			CurKey.fRotY = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fRotY;
			CurKey.fRotZ = m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].aKey[nCntPart].fRotZ;
			// 次のキー情報を代入
			nextKey.fPosX = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fPosX;
			nextKey.fPosY = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fPosY;
			nextKey.fPosZ = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fPosZ;
			nextKey.fRotX = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fRotX;
			nextKey.fRotY = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fRotY;
			nextKey.fRotZ = m_aMotionInfo[nCntDiv][nType].aKeyInfo[nNextKey].aKey[nCntPart].fRotZ;
			// 過去のキー
			oldKey.fPosX = m_aKeyOld[nCntDiv][nCntPart].fPosX;
			oldKey.fPosY = m_aKeyOld[nCntDiv][nCntPart].fPosY;
			oldKey.fPosZ = m_aKeyOld[nCntDiv][nCntPart].fPosZ;
			oldKey.fRotX = m_aKeyOld[nCntDiv][nCntPart].fRotX;
			oldKey.fRotY = m_aKeyOld[nCntDiv][nCntPart].fRotY;
			oldKey.fRotZ = m_aKeyOld[nCntDiv][nCntPart].fRotZ;


			// 位置・向きの差分を算出 =====================================================================================
			//diff.fPosX = nextKey.fPosX - CurKey.fPosX;
			//diff.fPosY = nextKey.fPosY - CurKey.fPosY;
			//diff.fPosZ = nextKey.fPosZ - CurKey.fPosZ;
			//diff.fRotX = nextKey.fRotX - CurKey.fRotX;
			//diff.fRotY = nextKey.fRotY - CurKey.fRotY;
			//diff.fRotZ = nextKey.fRotZ - CurKey.fRotZ;

			// ブレンド
			diff.fPosX = nextKey.fPosX - oldKey.fPosX;
			diff.fPosY = nextKey.fPosY - oldKey.fPosY;
			diff.fPosZ = nextKey.fPosZ - oldKey.fPosZ;
			diff.fRotX = nextKey.fRotX - oldKey.fRotX;
			diff.fRotY = nextKey.fRotY - oldKey.fRotY;
			diff.fRotZ = nextKey.fRotZ - oldKey.fRotZ;

			// 向きの修正
			general::RotLimit(D3DXVECTOR3(diff.fRotX, diff.fRotY, diff.fRotZ));

			// 位置・向きを設定 =====================================================================================
			// ブレンド
			dest.fPosX = posPart.x + oldKey.fPosX + diff.fPosX * (m_aMotion[nCntDiv].fCounter / fFrame);
			dest.fPosY = posPart.y + oldKey.fPosY + diff.fPosY * (m_aMotion[nCntDiv].fCounter / fFrame);
			dest.fPosZ = posPart.z + oldKey.fPosZ + diff.fPosZ * (m_aMotion[nCntDiv].fCounter / fFrame);
			dest.fRotX = oldKey.fRotX + diff.fRotX * (m_aMotion[nCntDiv].fCounter / fFrame);
			dest.fRotY = oldKey.fRotY + diff.fRotY * (m_aMotion[nCntDiv].fCounter / fFrame);
			dest.fRotZ = oldKey.fRotZ + diff.fRotZ * (m_aMotion[nCntDiv].fCounter / fFrame);

			// 向きの修正
			general::RotLimit(D3DXVECTOR3(dest.fRotX, dest.fRotY, dest.fRotZ));

			// 位置・向きの反映
			if (m_apModelPart[nCntPart] != nullptr)
			{
				m_apModelPart[nCntDiv][nCntPart]->pParts->SetPosition(D3DXVECTOR3(dest.fPosX, dest.fPosY, dest.fPosZ));
				m_apModelPart[nCntDiv][nCntPart]->pParts->SetRotation(D3DXVECTOR3(dest.fRotX, dest.fRotY, dest.fRotZ));
			}
		}
		
		//if (!m_aMotion[nCntDiv].bFinish)
		{
			// モーションカウンターを進める
			m_aMotion[nCntDiv].fCounter += (1.0f * m_fCntMmultiply);
		}

		if (m_aMotion[nCntDiv].nKey >= m_aMotionInfo[nCntDiv][nType].nNumKey - 1)
		{// キー総数を超えたら

			// 現在のモーションタイプの保存
			m_aMotion[nCntDiv].nTypeOld = m_aMotion[nCntDiv].nType;

			// 終了した状態へ
			m_aMotion[nCntDiv].bFinish = true;

			if (m_aMotionInfo[nCntDiv][nType].bLoop)
			{
				// モーション設定
				SetMotion(nCntDiv, nType);
			}
		}

		if (m_aMotion[nCntDiv].fCounter > (float)m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].fFrame)
		{// キーのフレーム数になったら

			if (m_aMotion[nCntDiv].nKey < m_aMotionInfo[nCntDiv][nType].nNumKey)
			{
				// 現在のキーを進める
				m_aMotion[nCntDiv].nKey = (m_aMotion[nCntDiv].nKey + 1 + m_aMotionInfo[nCntDiv][nType].nNumKey) % m_aMotionInfo[nCntDiv][nType].nNumKey;

				// カウンターを戻す
				m_aMotion[nCntDiv].fCounter = 0.0f;

				// 過去キーの設定
				SetKeyOld();
			}
		}

		// デバッグ表示
		//Debug(nCntDiv, nType);
	}
}

//=============================================================================
// モーション設定
//=============================================================================
void CMotionDiv::SetMotion(int nNumDiv, int type)
{
	SetKeyOld();	// 過去キーの設定
	m_aMotion[nNumDiv].bFinish = false;	// 終了してない状態
	m_aMotion[nNumDiv].nType = type;	// モーションタイプの設定
	m_aMotion[nNumDiv].nKey = 0;		// 現在のキーをクリア
	m_aMotion[nNumDiv].fCounter = 0.0f;	// モーションカウンターをクリア
}

//=====================================================
// ポーズの初期設定
//=====================================================
void CMotionDiv::InitPose(int nType)
{
	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{
		for (int i = 0; i < m_aMotionInfo[nCntDiv][nType].nNumKey; i++)
		{
			for (int nCntPart = 0; nCntPart < m_aMotion[nCntDiv].nNumParts; nCntPart++)
			{
				m_aKeyOld[nCntDiv][nCntPart] = m_aMotionInfo[nCntDiv][nType].aKeyInfo[i].aKey[nCntPart];
			}
		}
	}
}

//=====================================================
// 終了するキーの情報保存
//=====================================================
void CMotionDiv::SetKeyOld(void)
{
	for (int nCntDiv = 0; nCntDiv < motionDiv::MODEL_MAX; nCntDiv++)
	{
		for (int nCntPart = 0; nCntPart < m_aMotion[nCntDiv].nNumParts; nCntPart++)
		{
			m_aKeyOld[nCntDiv][nCntPart].fPosX = m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosition().x - m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosOrg().x;
			m_aKeyOld[nCntDiv][nCntPart].fPosY = m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosition().y - m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosOrg().y;
			m_aKeyOld[nCntDiv][nCntPart].fPosZ = m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosition().z - m_apModelPart[nCntDiv][nCntPart]->pParts->GetPosOrg().z;

			m_aKeyOld[nCntDiv][nCntPart].fRotX = m_apModelPart[nCntDiv][nCntPart]->pParts->GetRotation().x;
			m_aKeyOld[nCntDiv][nCntPart].fRotY = m_apModelPart[nCntDiv][nCntPart]->pParts->GetRotation().y;
			m_aKeyOld[nCntDiv][nCntPart].fRotZ = m_apModelPart[nCntDiv][nCntPart]->pParts->GetRotation().z;
		}
	}
}

//=============================================================================
// 読み込み
//=============================================================================
void CMotionDiv::Load(const char* pUpperPath, const char* pLowerPath)
{
	FILE* pFile = nullptr;	// ファイルポインタ宣言

	for (int nCntDiv = 0; nCntDiv < DIV_MAX; nCntDiv++)
	{
		if (nCntDiv == DIV_LOWER)
		{
			// ファイルを開く
			pFile = fopen(pLowerPath, "r");
		}
		else if(nCntDiv == DIV_UPPER)
		{
			// ファイルを開く
			pFile = fopen(pUpperPath, "r");
		}

		if (pFile != nullptr)
		{// ファイルが開けた場合

			// キャラクターの読み込み
			LoadCharacter(pFile, nCntDiv);

			// モーションの読込み
			LoadMotion(pFile, nCntDiv);

			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//	キャラクターの読込み
//=============================================================================
void CMotionDiv::LoadCharacter(FILE* pFile, int nCntDiv)
{
	// 変数宣言
	char cTemp[254] = {};	// 一時保存用
	char aPath[254] = {};	// ファイルパス用
	bool bStart = false;	// キャラクタの組み立てができてるかどうか

	//----------------------------------------------------------------------
	// ファイル名読込み
	//----------------------------------------------------------------------
	while (1)
	{
		// 文字列読込み
		(void)fscanf(pFile, "%s", &cTemp[0]);

		if (strcmp(cTemp, "NUM_MODEL") == 0)
		{
			// "="読込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			// 読み込むモデル(パーツ)数読込み
			(void)fscanf(pFile, "%d", &m_info.aNumLoadPartsDiv[nCntDiv]);

			break;
		}

		// エラー表示
		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			assert(("キャラクタ情報の読み込みに失敗", false));
		}
	}

	for (int nCntModel = 0; nCntModel < m_info.aNumLoadPartsDiv[nCntDiv];)
	{
		// 文字列読込み
		(void)fscanf(pFile, "%s", &cTemp[0]);

		if (strcmp(cTemp, "MODEL_FILENAME") == 0)
		{// ファイル名読込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			// モデルパス読込み
			(void)fscanf(pFile, "%s", &aPath[0]);

			// インスタンス生成
			m_apModelPart[nCntDiv][nCntModel] = new SModelParts;
			m_apModelPart[nCntDiv][nCntModel]->pParts = CModelParts::Create();

			// モデル番号取得
			int nIdx = CXfile::GetInstance()->Load(&aPath[0]);

			// モデル読込み
			m_apModelPart[nCntDiv][nCntModel]->pParts->SetIdxModel(nIdx);
			m_apModelPart[nCntDiv][nCntModel]->pParts->BindModel(m_apModelPart[nCntDiv][nCntModel]->pParts->GetIdxModel());

			nCntModel++;
		}
	}

	//----------------------------------------------------------------------
	// キャラクターの組み立て
	//----------------------------------------------------------------------
	while (1)
	{
		// 文字列読み込み
		(void)fscanf(pFile, "%s", &cTemp[0]);

		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{// 終了判断
			return;
		}

		if (strcmp(cTemp, "END_CHARACTERSET") == 0)
		{// 組み立て終了の判断
			return;
		}

		if (strcmp(cTemp, "CHARACTERSET") != 0)
		{// キャラクター組み立て開始の判断
			continue;
		}

		while (strcmp(cTemp, "END_SCRIPT") != 0)
		{
			if (strcmp(cTemp, "NUM_PARTS") == 0)
			{
				// "="読み込み
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_info.aNumSetPartsDiv[nCntDiv]);

				break;
			}

			// 文字列読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);
		}

		for (int nCntParts = 0; nCntParts < m_info.aNumSetPartsDiv[nCntDiv];)
		{
			// 文字列読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "END_CHARACTERSET") == 0)
			{// 組み立て終了の判断
				return;
			}

			if (strcmp(cTemp, "PARTSSET") == 0)
			{// パーツの設定

				while (strcmp(cTemp, "END_PARTSSET") != 0)
				{
					// 文字列読込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "INDEX") == 0)
					{// 番号読込み
						// "="読込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apModelPart[nCntDiv][nCntParts]->nIdx);
					}
					if (strcmp(cTemp, "PARENT") == 0)
					{// 親番号読込み
						// "="読込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apModelPart[nCntDiv][nCntParts]->nIdxParent);

						if (m_apModelPart[nCntDiv][nCntParts]->nIdxParent <= -1)
						{// 親番号の保存

							m_aMotion[nCntDiv].nIdxParent = nCntParts;
						}
					}
					if (strcmp(cTemp, "POS") == 0)
					{// 位置読込み
						// "="読込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						D3DXVECTOR3 pos = {};

						(void)fscanf(pFile, "%f", &pos.x);
						(void)fscanf(pFile, "%f", &pos.y);
						(void)fscanf(pFile, "%f", &pos.z);

						m_apModelPart[nCntDiv][nCntParts]->pParts->SetPosition(pos);
						m_apModelPart[nCntDiv][nCntParts]->pParts->SetPosOrg(pos);
					}
					if (strcmp(cTemp, "ROT") == 0)
					{//向き読込み
						// "="読込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						D3DXVECTOR3 rot = {};

						(void)fscanf(pFile, "%f", &rot.x);
						(void)fscanf(pFile, "%f", &rot.y);
						(void)fscanf(pFile, "%f", &rot.z);

						m_apModelPart[nCntDiv][nCntParts]->pParts->SetRotation(rot);
					}
				}
				nCntParts++;
				m_aMotion[nCntDiv].nNumParts++;
			}
		}
	}	
}

//=============================================================================
// モーションの読込み
//=============================================================================
void CMotionDiv::LoadMotion(FILE* pFile, int nCntDiv)
{
	// 変数宣言
	char cTemp[254] = {};
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntPart = 0;

	while (1)
	{
		// 文字列読込み
		(void)fscanf(pFile, "%s", &cTemp[0]);

		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			return;
		}

		// モーション設定
		if (strcmp(cTemp, "MOTIONSET") == 0)
		{
			while (strcmp(cTemp, "END_MOTIONSET") != 0)
			{
				// 文字列の読込み
				(void)fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "LOOP") == 0)
				{// ループ判断

					(void)fscanf(pFile, "%s", &cTemp[0]);	// "="読込み
					(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntDiv][nCntMotion].bLoop);
				}
				if (strcmp(cTemp, "NUM_KEY") == 0)
				{// キー数

					(void)fscanf(pFile, "%s", &cTemp[0]);	// "="読込み
					(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntDiv][nCntMotion].nNumKey);
				}

				if (strcmp(cTemp, "KEYSET") == 0)
				{
					// フレーム数取得
					while (1)
					{
						// 文字列の読込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "FRAME") == 0)
						{
							(void)fscanf(pFile, "%s", &cTemp[0]);	// "="読込み

							(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].fFrame);

							break;
						}
					}

					while (strcmp(cTemp, "END_KEYSET") != 0)
					{
						// 文字列の読込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "KEY") == 0)
						{// キー情報の取得

							while (strcmp(cTemp, "END_KEY") != 0)
							{// 終わりまでキー設定

								// 文字列の読込み
								(void)fscanf(pFile, "%s", &cTemp[0]);

								if (strcmp(cTemp, "POS") == 0)
								{// 位置取得

									(void)fscanf(pFile, "%s", &cTemp[0]);	// "="読込み
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
								}
								if (strcmp(cTemp, "ROT") == 0)
								{// 向き取得

									(void)fscanf(pFile, "%s", &cTemp[0]);	// "="読込み
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntDiv][nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
								}
							}
							nCntPart++;
						}
					}
					nCntKey++;
					nCntPart = 0;
				}
			}
			nCntKey = 0;
			nCntMotion++;
			m_aMotion[nCntDiv].nNumMotion++;	// モーション数のカウントアップ
		}
	}
}

//=============================================================================
// パーツ情報の取得
//=============================================================================
CModelParts* CMotionDiv::GetParts(int nNumDiv, int nNumPart)
{
	if (m_apModelPart[nNumDiv][nNumPart]->pParts == nullptr)
	{
		return nullptr;
	}

	return m_apModelPart[nNumDiv][nNumPart]->pParts;
}

//=============================================================================
// デバッグ表示
//=============================================================================
void CMotionDiv::Debug(int nCntDiv, int nType)
{
#ifndef _DEBUG
	return;
#endif

	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	
	pDebug->Print("\n【モーション情報】\n");
	pDebug->Print("フレーム : %f/%f\n", m_aMotion[nCntDiv].fCounter, m_aMotionInfo[nCntDiv][nType].aKeyInfo[m_aMotion[nCntDiv].nKey].fFrame);
	pDebug->Print("パーツ数 : %d\n", m_aMotion[nCntDiv].nNumParts);
	pDebug->Print("キー数 : %d\n", m_aMotionInfo[nCntDiv][nType].nNumKey);
	pDebug->Print("現在キー : %d\n", m_aMotion[nCntDiv].nKey);
	pDebug->Print("モーション数 : %d\n", m_aMotion[nCntDiv].nNumMotion);
}
