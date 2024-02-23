//===========================================================================================
//
// ��`�����蔻�菈��[clossCollider.cpp]
// Author : Takeru Ogasawara
//
//===========================================================================================
#include "colliderCross.h"
#include "manager.h"
#include "renderer.h"

//===========================================================================================
// �R���X�g���N�^
//===========================================================================================
CClossCollider::CClossCollider() : m_fWidth(0.0f)
{

}

//===========================================================================================
// �f�X�g���N�^
//===========================================================================================
CClossCollider::~CClossCollider()
{

}

//===========================================================================================
// ����������
//===========================================================================================
HRESULT CClossCollider::Init(void)
{
	return S_OK;
}

//===========================================================================================
// �I������
//===========================================================================================
void CClossCollider::Uninit(void)
{

}

//===========================================================================================
// �X�V����
//===========================================================================================
void CClossCollider::Update(void)
{

}

//===========================================================================================
// �G�ꂽ����
//===========================================================================================
bool CClossCollider::IsTrigger(ETag tag)
{
	for (int nCnt = 0; nCnt < collision::MAX_OBJECT; nCnt++)
	{
		CCollision** ppCollision = GetCollision();

		if (ppCollision[nCnt] == nullptr)
		{
			continue;
		}

		if (ppCollision[nCnt]->GetType() != TYPE_SPHERE)
		{
			continue;
		}

		if (ppCollision[nCnt]->GetTag() == tag || ppCollision[nCnt] == this)
		{
			continue;
		}

		// �ϐ��錾
		//float vec;						// �x�N�g��
		//bool bHit = false;				// �q�b�g����
		//float fRate;					// �x�N�g���̊���
		//float fMaxArea, fNowArea;		// ���̖ʐρ^�ő�ʐ�
		//D3DXVECTOR3 pos0 = {}, pos1 = {};			// �ꏊ
		//D3DXVECTOR3 vecLine = {}, vecToPos = {};
		//D3DXVECTOR3 Cross = {};				// ��_�̏ꏊ

		//D3DXVECTOR3 ClsPos = ppCollision[nCnt]->GetPosition();
		//D3DXVECTOR3 ClsRot = ppCollision[nCnt]->GetRotation();

		// �ꏊ�̌v�Z
		//pos0 = (ClsPos + D3DXVECTOR3(cosf(-ClsRot.y) * -1.0f, 0.0f, sinf(-ClsRot.y) * -1.0f));
		//pos1 = (ClsPos + D3DXVECTOR3(cosf(-ClsRot.y) * 1.0f, 0.0f, sinf(-ClsRot.y) * 1.0f));

		// pos0��pos1�Ƃ̋�����
		//vecLine = pos1 - pos0;

		// �ǂ���e�܂ł̈ʒu
		//vecToPos = pBullet->pos - pMeshWall->pos;

		//�ő�ʐ�
		//fMaxArea = (g_vecLine.z * pBullet->move.x) - (g_vecLine.x * pBullet->move.z);

		//���̖ʐ�
		//fNowArea = (g_vecToPos.z * pBullet->move.x) - (g_vecToPos.x * pBullet->move.z);

		// �x�N�g���̊���
		//fRate = fNowArea / fMaxArea;

		// ��_
		//Cross = D3DXVECTOR3(pos0.x + g_vecLine.x * fRate, pBullet->pos.y, pos0.z + g_vecLine.z * fRate);

		//vec = (g_vecLine.z * g_vecToPos.x) - (g_vecLine.x * g_vecToPos.z);

		//if (pBullet->pos.x >= pos0.x && pBullet->pos.x >= pos0.x,
		//	pBullet->pos.x <= pos1.x && pBullet->pos.x <= pos1.x)
		//{
		//	if (vec < 0)
		//	{
		//		pBullet->pos = Cross;
		//	}
		//}
	}

	return false;
}
