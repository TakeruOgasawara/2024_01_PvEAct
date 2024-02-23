//===========================================================================================
//
// 武器[wepon.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "wepon.h"
#include "manager.h"
#include "renderer.h"
#include "general.h"
#include "xfile.h"
#include "texture.h"

#include "player.h"
#include "characterDiv.h"
#include "motionDiv.h"
#include "modelParts.h"
#include "debugproc.h"

//===========================================================================================
//  コンストラクタ
//===========================================================================================
CWepon::CWepon()
{
	// マトリックスん初期化
	ZeroMemory(m_mtxOwner, sizeof(m_mtxOwner));
}

//===========================================================================================
// デストラクタ
//===========================================================================================
CWepon::~CWepon()
{
}

//===========================================================================================
// 生成
//===========================================================================================
CWepon* CWepon::Create(const char* pPath, D3DXVECTOR3 pos)
{
	CWepon* pWepon = nullptr;

	if (pWepon == nullptr)
	{
		pWepon = new CWepon;

		pWepon->Init(pPath, pos);

		return pWepon;
	}

	return nullptr;
}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CWepon::Init(const char* pPath, D3DXVECTOR3 pos)
{
	// 親クラスの初期化
	CObjectX::Init(pPath, pos);

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CWepon::Uninit(void)
{
	// 親クラスの終了
	CObjectX::Uninit();
}

//===========================================================================================
// 更新処理
//===========================================================================================
void CWepon::Update(void)
{
	// 親クラスの更新
	CObjectX::Update();

	Follow();

	SetPosition(GetPosition());
	SetRotation(GetRotation());
}

//===========================================================================================
// 描画処理
//===========================================================================================
void CWepon::Draw(void)
{
	// 親クラスの描画
	CObjectX::DrawOnly();
}

//===========================================================================================
// 追従処理
//===========================================================================================
void CWepon::Follow(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	CMotionDiv* pBody = pPlayer->GetBody();

	if (pBody == nullptr)
	{
		return;
	}

	D3DXMATRIX* pMtx = GetMatrix();
	D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f };

	CModelParts* pParts = pBody->GetParts(0, 0);

	if (pParts != nullptr)
	{
		D3DXMATRIX* pMtx = GetMatrix();
		D3DXMATRIX* pMtxPart = pParts->GetMatrix();
		D3DXVECTOR3 offset = { -10.0f,0.0f,0.0f };

		//universal::SetOffSet(pMtx, *pMtxPart, offset);
		general::SetMatrix(pMtx, *pMtxPart, pos);
	}
}
