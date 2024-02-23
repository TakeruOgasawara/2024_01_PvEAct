//=============================================================================
// 
// モーション処理[character.cpp]
// Author : Takeru Ogasawara
// 
//=============================================================================

// インクルード宣言
#include "motion.h"
#include "renderer.h"
#include "manager.h"
#include "xfile.h"
#include "texture.h"
#include "modelParts.h"

#include "general.h"
#include "debugproc.h"

#define SPEED	(0.5f);

//=============================================================================
// コンストラクタ
//=============================================================================
CMotion::CMotion()
{
	// 値のクリア
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_apModelPart[0], sizeof(m_apModelPart));
	ZeroMemory(&m_motion, sizeof(m_motion));
	ZeroMemory(&m_aMotionInfo[0], sizeof(m_aMotionInfo));
	m_fCntMmultiply = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
//	生成
//=============================================================================
CMotion* CMotion::Create(const char* pTextPath)
{
	CMotion* pMotion = nullptr;

	if (pMotion == nullptr)
	{
		pMotion = new CMotion;

		if (pMotion != nullptr)
		{
			pMotion->Init(pTextPath);

			return pMotion;
		}
	}

	return pMotion;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMotion::Init()
{
	return S_OK;
}

//=============================================================================
// 初期化処理(オーバーロード)
//=============================================================================
HRESULT CMotion::Init(const char* pTextPath)
{
	m_info.pos = { 0.0f, 0.0f, 0.0f };
	m_info.rot = { 0.0f, 0.0f, 0.0f };

	Load(pTextPath);

	m_motion.nType = 0;
	m_fCntMmultiply = 1.0f;

	InitMotion(0);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMotion::Uninit()
{
	for (int i = 0; i < m_motion.nNumMotion; i++)
	{
		if (m_aMotionInfo[i] != NULL)
		{
			delete m_aMotionInfo[i];
			m_aMotionInfo[i] = nullptr;
		}
	}

	for (int i = 0; i < m_info.nNumModelparts; i++)
	{
		if (m_apModelPart[i]->pParts != nullptr)
		{
			m_apModelPart[i]->pParts->Uninit();
			m_apModelPart[i]->pParts = nullptr;
		}
		if (m_apModelPart[i] != nullptr)
		{
			delete m_apModelPart[i];
			m_apModelPart[i]->pParts = nullptr;
		}
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMotion::Update()
{
	// 情報の取得
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CXfile* pXfile = CXfile::GetInstance();

	if (m_aMotionInfo[m_motion.nType] == nullptr)
	{
		return;
	}

	// 変数宣言
	D3DXVECTOR3 posPart = {}, rotPart = {};	// パーツの位置・向き
	D3DXVECTOR3 nextPos = {};	// 次の位置
	D3DXVECTOR3 nextRot = {};	// 次の向き
	SKey diff = {}, dest = {};	// 差分・目標
	SKey CurKey = {}, nextKey = {};	// 現在・次のキー
	float fFrame = 0.0f;

	int nType = m_motion.nType;

	// 次のキーの設定
	int nNextKey = (m_motion.nKey + 1 + m_aMotionInfo[nType]->nNumKey) % m_aMotionInfo[nType]->nNumKey;

	for (int nCntPart = 0; nCntPart < m_info.nNumModelparts; nCntPart++)
	{
		// パーツ情報の取得
		posPart = m_apModelPart[nCntPart]->pParts->GetPosition();
		rotPart = m_apModelPart[nCntPart]->pParts->GetRotation();

		// 現在のキー情報を代入
		CurKey.fPosX = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fPosX;
		CurKey.fPosY = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fPosY;
		CurKey.fPosZ = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fPosZ;
		CurKey.fRotX = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fRotX;
		CurKey.fRotY = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fRotY;
		CurKey.fRotZ = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].aKey[nCntPart].fRotZ;
		// 次のキー情報を代入
		nextKey.fPosX = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fPosX;
		nextKey.fPosY = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fPosY;
		nextKey.fPosZ = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fPosZ;
		nextKey.fRotX = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fRotX;
		nextKey.fRotY = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fRotY;
		nextKey.fRotZ = m_aMotionInfo[nType]->aKeyInfo[nNextKey].aKey[nCntPart].fRotZ;
		// パーツ情報の取得
		posPart = m_apModelPart[nCntPart]->pParts->GetPosOrg();
		// フレーム情報の代入
		fFrame = m_aMotionInfo[nType]->aKeyInfo[m_motion.nKey].fFrame;

		// キーの情報から位置・向きを算出
		diff.fPosX = nextKey.fPosX - CurKey.fPosX;
		diff.fPosY = nextKey.fPosY - CurKey.fPosY;
		diff.fPosZ = nextKey.fPosZ - CurKey.fPosZ;
		diff.fRotX = nextKey.fRotX - CurKey.fRotX;
		diff.fRotY = nextKey.fRotY - CurKey.fRotY;
		diff.fRotZ = nextKey.fRotZ - CurKey.fRotZ;

		// 向きの修正
		general::RotLimit(D3DXVECTOR3(diff.fRotX, diff.fRotY, diff.fRotZ));

		// パーツの位置・向きを設定
		dest.fPosX = posPart.x + CurKey.fPosX + diff.fPosX * (m_motion.fCounter / fFrame);
		dest.fPosY = posPart.y + CurKey.fPosY + diff.fPosY * (m_motion.fCounter / fFrame);
		dest.fPosZ = posPart.z + CurKey.fPosZ + diff.fPosZ * (m_motion.fCounter / fFrame);

		dest.fRotX = CurKey.fRotX + diff.fRotX * (m_motion.fCounter / fFrame);
		dest.fRotY = CurKey.fRotY + diff.fRotY * (m_motion.fCounter / fFrame);
		dest.fRotZ = CurKey.fRotZ + diff.fRotZ * (m_motion.fCounter / fFrame);

		// 向きの修正
		general::RotLimit(D3DXVECTOR3(dest.fRotX, dest.fRotY, dest.fRotZ));

		// 位置・向きの反映
		if (m_apModelPart[nCntPart] != nullptr)
		{
			m_apModelPart[nCntPart]->pParts->SetPosition(D3DXVECTOR3(dest.fPosX, dest.fPosY, dest.fPosZ));
			m_apModelPart[nCntPart]->pParts->SetRotation(D3DXVECTOR3(dest.fRotX, dest.fRotY, dest.fRotZ));
		}
	}

	if (!m_motion.bFinish)
	{
		// モーションカウンターを進める
		m_motion.fCounter += (1.0f * m_fCntMmultiply);
	}

	if (m_motion.nKey > m_aMotionInfo[nType]->nNumKey - 1)
	{// キー総数を超えたら

		// 現在のモーションタイプの保存
		//m_aMotion[nCntDiv].nTypeOld = m_aMotion[nCntDiv].nType;

		// 終了した状態へ
		m_motion.bFinish = true;

		m_motion.fCounter = 0.0f;

		if (m_aMotionInfo[nType]->bLoop)
		{
			// モーション設定
			SetMotion(nType);
		}
	}

	if (m_motion.fCounter > m_aMotionInfo[m_motion.nType]->aKeyInfo[m_motion.nKey].fFrame)
	{// キーのフレーム数になったら
		
		// 現在のキーを進める
		m_motion.nKey = (m_motion.nKey + 1 + m_aMotionInfo[m_motion.nType]->nNumKey) % m_aMotionInfo[m_motion.nType]->nNumKey;

		m_motion.fCounter = 0;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMotion::Draw()
{
	// マトリックスの設定
	general::SetMatrix(&m_info.mtxWorld, m_info.pos, m_info.rot);

	// マトリックス掛け合わせ
	MultiplyMtx();
}

//=============================================================================
// モーション設定
//=============================================================================
void CMotion::SetMotion(int nType)
{
	m_motion.nType = nType;
	m_motion.bFinish = false;
	m_motion.nKey = 0;
	m_motion.fCounter = 0.0f;
}

//=============================================================================
// マトリックス掛け合わせ
//=============================================================================
void CMotion::MultiplyMtx(void)
{
	// 情報取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CTexture* pTexture = CTexture::GetInstance();

	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX* pMtx, *pMtxParent;	// 親のマトリックス

	for (int nCntParts = 0; nCntParts < m_info.nNumModelparts; nCntParts++)
	{
		// マトリックスの取得
		pMtx = m_apModelPart[nCntParts]->pParts->GetMatrix();

		// ワールドマトリックス初期化
		D3DXMatrixIdentity(pMtx);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,m_apModelPart[nCntParts]->pParts->GetRotation().y, m_apModelPart[nCntParts]->pParts->GetRotation().x, m_apModelPart[nCntParts]->pParts->GetRotation().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			m_apModelPart[nCntParts]->pParts->GetPosition().x, m_apModelPart[nCntParts]->pParts->GetPosition().y, m_apModelPart[nCntParts]->pParts->GetPosition().z);
		D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);

		if (m_apModelPart[nCntParts]->nIdxParent != -1)
		{//親パーツがある場合

			// 親マトリックスの取得
			pMtxParent = m_apModelPart[m_apModelPart[nCntParts]->nIdxParent]->pParts->GetMatrix();
		}
		else
		{
			pMtxParent = &m_info.mtxWorld;
		}

		// 親パーツとパーツのワールドマトリックスをかけ合わせる
		D3DXMatrixMultiply(pMtx, pMtx, pMtxParent);

		// ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, pMtx);

		m_apModelPart[nCntParts]->pParts->Draw();
	}
}

//=============================================================================
// 最初のモーション
//=============================================================================
void CMotion::InitMotion(int nType)
{
	for (int nCntKey = 0; nCntKey < m_aMotionInfo[nType]->nNumKey; nCntKey++)
	{
		for (int nCntPart = 0; nCntPart < m_motion.nNumParts; nCntPart++)
		{
			m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart];
			

			m_apModelPart[nCntPart]->pParts->SetPosition(D3DXVECTOR3
			(
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fPosX,
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fPosY,
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fPosZ)
			);
			m_apModelPart[nCntPart]->pParts->SetRotation(D3DXVECTOR3
			(
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fRotX,
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fRotY,
				m_aMotionInfo[nType]->aKeyInfo[nCntKey].aKey[nCntPart].fRotZ)
			);
		}
	}
}

//=============================================================================
// 読み込み
//=============================================================================
void CMotion::Load(const char* cTextPath)
{
	// 変数宣言
	char cTemp[254] = {};

	D3DXVECTOR3 pos = {};

	// ファイルから読み込む
	FILE* pFile = fopen(cTextPath, "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合

		// キャラクタ情報の読み込み
		LoadCharacter(pFile);

		// モーション設定
		LoadMotion(pFile);

		// ファイルを閉じる
		fclose(pFile);
	}
}

//=============================================================================
// キャラクター情報読み込み
//=============================================================================
void CMotion::LoadCharacter(FILE* pFile)
{
	// 変数宣言
	char cTemp[254] = {};	// 一時保存用
	char aPath[254] = {};	// ファイルパス用
	int nCntModel = 0;
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

			// モデル数読込み
			(void)fscanf(pFile, "%d", &m_info.nNumModelparts);

			break;
		}

		// エラー表示
		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			assert(("キャラクタ情報の読み込みに失敗", false));
		}
	}

	for (int nCntModel = 0; nCntModel < m_info.nNumModelparts;)
	{
		// 文字列読込み
		(void)fscanf(pFile, "%s", &cTemp[0]);

		if (strcmp(cTemp, "MODEL_FILENAME") == 0)
		{// ファイル名読込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			// モデルパス読込み
			(void)fscanf(pFile, "%s", &aPath[0]);

			// インスタンス生成
			m_apModelPart[nCntModel] = new SModelParts;
			m_apModelPart[nCntModel]->pParts = CModelParts::Create();

			// モデル番号取得
			int nIdx = CXfile::GetInstance()->Load(&aPath[0]);

			// モデル読込み
			m_apModelPart[nCntModel]->pParts->SetIdxModel(nIdx);
			m_apModelPart[nCntModel]->pParts->BindModel(m_apModelPart[nCntModel]->pParts->GetIdxModel());

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

		if (strcmp(cTemp, "CHARACTERSET") != 0)
		{// キャラクター組み立ての判断
			continue;
		}

		while (1)
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
					// 文字列読み込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "INDEX") == 0)
					{// 番号読み込み
						// "="読込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apModelPart[nCntModel]->nIdx);
					}
					if (strcmp(cTemp, "PARENT") == 0)
					{// 親番号読み込み
						// "="読込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apModelPart[nCntModel]->nIdxParent);
					}
					if (strcmp(cTemp, "POS") == 0)
					{// 位置読み込み
						// "="読込み
						(void)fscanf(pFile, "%s", &cTemp[0]);
						
						D3DXVECTOR3 pos = {};

						(void)fscanf(pFile, "%f", &pos.x);
						(void)fscanf(pFile, "%f", &pos.y);
						(void)fscanf(pFile, "%f", &pos.z);
							
						m_apModelPart[nCntModel]->pParts->SetPosition(pos);
						m_apModelPart[nCntModel]->pParts->SetPosOrg(pos);
					}
					if (strcmp(cTemp, "ROT") == 0)
					{//向き読み込み
						// "="読込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						D3DXVECTOR3 rot = {};

						(void)fscanf(pFile, "%f", &rot.x);
						(void)fscanf(pFile, "%f", &rot.y);
						(void)fscanf(pFile, "%f", &rot.z);

						m_apModelPart[nCntModel]->pParts->SetRotation(rot);
					}
				}
				nCntModel++;
				m_motion.nNumParts++;
			}
		}
	}
}

//=============================================================================
// モーション情報の読み込み
//=============================================================================
void CMotion::LoadMotion(FILE* pFile)
{			
	// 変数宣言
	char cTemp[254] = {};
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntPart = 0;

	while (1)
	{
		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			return;
		}

		// 文字列読込み
		(void)fscanf(pFile, "%s", &cTemp[0]);

		// モーション設定
		if (strcmp(cTemp, "MOTIONSET") == 0)
		{
			// インスタンス生成
			m_aMotionInfo[nCntMotion] = new SMotionInfo;

			while (strcmp(cTemp, "END_MOTIONSET") != 0)
			{
				// 文字列の読込み
				(void)fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "LOOP") == 0)
				{// ループ判断

					(void)fscanf(pFile, "%s", &cTemp[0]);	// "="読込み
					(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntMotion]->bLoop);
				}
				if (strcmp(cTemp, "NUM_KEY") == 0)
				{// キー数

					(void)fscanf(pFile, "%s", &cTemp[0]);	// "="読込み
					(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion]->nNumKey);
				}

				if (strcmp(cTemp, "KEYSET") == 0)
				{
					// 文字列の読込み
					(void)fscanf(pFile, "%s", &cTemp[0]);

					// フレーム数取得
					while (1)
					{
						// 文字列の読込み
						(void)fscanf(pFile, "%s", &cTemp[0]);

						if (strcmp(cTemp, "FRAME") == 0)
						{
							(void)fscanf(pFile, "%s", &cTemp[0]);	// "="読込み
							(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].fFrame);

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
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
								}
								if (strcmp(cTemp, "ROT") == 0)
								{// 向き取得

									(void)fscanf(pFile, "%s", &cTemp[0]);	// "="読込み
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
									(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion]->aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
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
			m_motion.nNumMotion++;	// モーション数のカウントアップ

			if (m_motion.nNumMotion > motion::MOTION_MAX)
			{
				assert(("モーションの最大数を超えました", false));
			}
		}
	}
}

//=============================================================================
// デバッグ表示
//=============================================================================
void CMotion::Debug(void)
{
#ifndef _DEBUG
	return;
#endif

	CManager::GetInstance()->GetDebugProc()->Print("【モーション情報】\n");
	CManager::GetInstance()->GetDebugProc()->Print("フレーム : %d/%d\n", m_aMotionInfo[m_motion.nType]->aKeyInfo[m_motion.nKey].fFrame);
	CManager::GetInstance()->GetDebugProc()->Print("パーツ数 : %d\n", m_motion.nNumParts);
	CManager::GetInstance()->GetDebugProc()->Print("モーション数 : %d\n", m_motion.nNumMotion);
	CManager::GetInstance()->GetDebugProc()->Print("キー数 : %d\n", m_aMotionInfo[m_motion.nType]->nNumKey);
	CManager::GetInstance()->GetDebugProc()->Print("現在キー : %d\n", m_motion.nKey);
	//CManager::GetInstance()->GetDebugProc()->Print("次のキー : %d\n", nNextKey);
}
