//===========================================================================================
//
// ���b�N�I������[lockOn.cpp]
// Author : Takeru Ogasawara
//
//==========================================================================================
#include "gauge.h"

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CGauge::CGauge()
{
	m_fDecrease = 0.0f;
}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CGauge::~CGauge()
{

}

//===========================================================================================
// ����
//===========================================================================================
CGauge* CGauge::Create(D3DXVECTOR3 pos)
{
	/*CGauge* pGauge = nullptr;

	if (pGauge == nullptr)
	{
		pGauge = new CGauge;

		pGauge->Init();
		pGauge->SetPosition(pos);
	}*/

	return nullptr;
}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CGauge::Init(void)
{
	// �e�N���X�̏�����
	CObject2D::Init();

	SetSize(1000.0f, 20.0f);

	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CGauge::Uninit(void)
{
	// �e�N���X�̏I������
	CObject2D::Uninit();
}

//===========================================================================================
// �X�V����
//===========================================================================================
void CGauge::Update(void)
{
	// �e�N���X�̍X�V����
	CObject2D::Update();
}

//===========================================================================================
// �`�揈��
//===========================================================================================
void CGauge::Draw(void)
{
	// �e�N���X�̕`�揈��
	CObject2D::Draw();
}