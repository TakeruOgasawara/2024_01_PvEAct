//======================================================================================
//
// ���b�N�I������[lockOn.cpp]
// Author : Takeru Ogasawara
//
//======================================================================================
#include "lockOn.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "billboard.h"

//�}�N����`
#define SIZE_X			(20.0f)
#define SIZE_Y			(20.0f)

//======================================================================================
// �R���X�g���N�^
//======================================================================================
CLockon::CLockon()
{

}

//======================================================================================
// �f�X�g���N�^
//======================================================================================
CLockon::~CLockon()
{

}

//======================================================================================
// ����
//======================================================================================
CLockon* CLockon::Create(D3DXVECTOR3 pos)
{
	CLockon* pPause = nullptr;

	pPause = new CLockon;

	if (pPause != nullptr)
	{
		pPause->Init(pos);

		return pPause;
	}

	return pPause;
}

//======================================================================================
// ����
//======================================================================================
CLockon* CLockon::Create(const char* cTexPath, D3DXVECTOR3 pos)
{
	CLockon* pPause = nullptr;

	pPause = new CLockon;

	if (pPause != nullptr)
	{
		pPause->Init(cTexPath, pos);

		return pPause;
	}

	return pPause;
}

//======================================================================================
// ����������
//======================================================================================
HRESULT CLockon::Init()
{
	// �e�N���X�̏�����
	CBillboard::Init();

	return S_OK;
}

//======================================================================================
// ����������
//======================================================================================
HRESULT CLockon::Init(D3DXVECTOR3 pos)
{
	// ����
	CBillboard::Init();

	// �ʒu�̐ݒ�
	SetPosition(pos);

	// �e�N�X�`���̊��蓖��
	BindTexture(CTexture::GetInstance()->Regist("data\\TEXTURE\\point.png"));

	return S_OK;
}

//======================================================================================
// ����������(�I�[�o�[���[�h)
//======================================================================================
HRESULT CLockon::Init(const char* cTexPath, D3DXVECTOR3 pos)
{
	// ����
	CBillboard::Init();

	// �ʒu�̐ݒ�
	SetPosition(pos);

	// �e�N�X�`���̊��蓖��
	BindTexture(CTexture::GetInstance()->Regist(cTexPath));

	return S_OK;
}

//======================================================================================
// �I������
//======================================================================================
void CLockon::Uninit()
{
	CBillboard::Uninit();
}

//======================================================================================
// �X�V����
//======================================================================================
void CLockon::Update()
{
	// �e�N���X�̍X�V
	CBillboard::Update();
}

//======================================================================================
// �`�揈��
//======================================================================================
void CLockon::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�̗L��/�����̐ݒ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �e�N���X�̕`��
	CBillboard::Draw();

	// ���C�e�B���O�̗L��/�����̐ݒ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}