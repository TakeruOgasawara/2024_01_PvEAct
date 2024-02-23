//===========================================================================================
//
// [mapEdit.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "mapEdit.h"
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "object.h"
#include "xfile.h"
#include "inputKeyboard.h"
#include "debugproc.h"

// 定数定義
namespace
{
	// xファイルパス
	const char* apPathList[CMapEdit::TYPE_MAX] =
	{
		"data\\MODEL\\object\\TanukiShop.x"
	};

	const float SPEED = 20.0f;
}


//===========================================================================================
// コンストラクタ
//===========================================================================================
CMapEdit::CMapEdit(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pXfile = nullptr;
	m_nRegistModelIdx = 0;
	m_nTypeIdx = 0;
	m_bUse = false;
	m_type = TYPE_HOUSE;
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CMapEdit::~CMapEdit()
{

}

//===========================================================================================
// 生成
//===========================================================================================
CMapEdit* CMapEdit::Create(void)
{
	CMapEdit* pObjX = nullptr;

	if (pObjX == nullptr)
	{
		pObjX = new CMapEdit;

		if (pObjX != NULL)
		{
			pObjX->Init();

			return pObjX;
		}
	}

	return pObjX;
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CMapEdit::Init(void)
{
	// 選択モデル番号の設定
	m_nTypeIdx = TYPE_HOUSE;

	// 
	m_type = (EType)m_nTypeIdx;

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CMapEdit::Uninit(void)
{
	Release();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CMapEdit::Update(void)
{
#ifdef _DEBUG	//デバック時実行

	//キーボード情報の取得
	CInputKeyboard* pInputKey = CInputKeyboard::GetInstance();
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CXfile* pXfile = CXfile::GetInstance();

	// エディットを使うかどうか
	if (pInputKey->GetTrigger(DIK_F3))
	{
		m_bUse = m_bUse ? false : true;
	}

	if (m_bUse == false )
	{
		return;
	}

	// デバッグ表示
	pDebug->Print("\n\n【エディットモード中】\n");
	pDebug->Print("位置： x:%f y:%f z:%f\n", m_pos.x, m_pos.y, m_pos.z);
	pDebug->Print("向き： x:%f y:%f z:%f\n", m_rot.x, m_rot.y, m_rot.z);
	pDebug->Print("種類： %d\n", m_type);

	// 操作
	Operation();

	// 位置の反映
	SetPosition(m_pos);

#endif
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CMapEdit::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//LPDIRECT3DTEXTURE9 pTexture;
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	//計算用マトリックス
	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL* pMat;	//マテリアルデータへのポインタ

	if (m_pXfile->GetMesh(m_nRegistModelIdx) == nullptr)
	{
		return;
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールの反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pXfile->GetBuffMat(m_nRegistModelIdx)->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)m_pXfile->GetDwNumMat(m_nRegistModelIdx); nCntMat++)
	//{
	//	// マテリアルの設定
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	// テクスチャ番号の取得
	//	pTexture = CTexture::GetInstance()->GetAddress(m_pXfile->p[nCntMat]);

	//	// テクスチャの設定
	//	pDevice->SetTexture(0, CTexture::GetInstance()->GetAddress(pTexture));

	//	// オブジェクト(パーツ)の描画
	//	m_pXfile->GetMesh(m_nRegistModelIdx)->DrawSubset(nCntMat);
	//}

	// 保存されていたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===========================================================================================
// 操作
//===========================================================================================
void CMapEdit::Operation(void)
{
	//キーボード情報の取得
	CInputKeyboard* pInputKey = CInputKeyboard::GetInstance();
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CXfile* pXfile = CXfile::GetInstance();

	//移動
	if (pInputKey->GetTrigger(DIK_UP))
	{
		m_pos.y += SPEED;
	}
	if (pInputKey->GetTrigger(DIK_DOWN))
	{
		m_pos.y -= SPEED;
	}
	if (pInputKey->GetTrigger(DIK_LEFT))
	{
		m_pos.x -= SPEED;
	}
	if (pInputKey->GetTrigger(DIK_RIGHT))
	{
		m_pos.x += SPEED;
	}

	if (pInputKey->GetTrigger(DIK_LSHIFT))
	{
		m_pos.z += SPEED;
	}

	if (pInputKey->GetTrigger(DIK_LCONTROL))
	{
		m_pos.z -= SPEED;
	}

	// オブジェクト種類の変更
	if (pInputKey->GetTrigger(DIK_1))
	{
		m_nTypeIdx++;

		m_nTypeIdx %= TYPE_MAX;			//繰り返し
	}

	//オブジェクトの設置
	if (pInputKey->GetTrigger(DIK_RETURN))
	{

	}

	//ファイル書き込み(セーブ)
	if (pInputKey->GetTrigger(DIK_F9))
	{
		//Save();
	}
}
