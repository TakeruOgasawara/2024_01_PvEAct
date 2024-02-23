//===========================================================================================
//
// [light.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "light.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

//===========================================================================================
// コンストラク
//===========================================================================================
CLight::CLight()
{

}

//===========================================================================================
// デストラクタ
//===========================================================================================
CLight::~CLight()
{

}

//===========================================================================================
// 初期化処理
//===========================================================================================
HRESULT CLight::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//設定用方向ベクトル
	D3DXVECTOR3 vecDir;	

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		switch (nCntLight)
		{
		case 0:
			//ライト1の種類を設定
			m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライト1の拡散光を設定
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//ライト1の方向を設定
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);			//ベクトルを正規化する
			m_aLight[nCntLight].Direction = vecDir;
			break;

		case 1:
			//ライト2の種類を設定
			m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライト2の拡散光を設定
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			//ライト2の方向を設定
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);			//ベクトルを正規化する
			m_aLight[nCntLight].Direction = vecDir;
			break;

		case 2:
			//ライト3の種類を設定
			m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//ライト3の拡散光を設定
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			//ライト3の方向を設定
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);			//ベクトルを正規化する
			m_aLight[nCntLight].Direction = vecDir;
			break;
		}

		//ライトを設定
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		//ライトを有効化する
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//===========================================================================================
// 終了処理
//===========================================================================================
void CLight::Uninit(void)
{

}

//===========================================================================================
// 更新処理
//===========================================================================================
void CLight::Update(void)
{
	//CManager::GetInstance()->GetDebugProc()->Print("\n[ライト情報]\n");

}
