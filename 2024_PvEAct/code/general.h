//===============================================================================
//
// �ėp�֐�[general.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _GENERAL_H_	//���̃}�N����`������Ă��Ȃ�������
#define _GENERAL_H_	//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include <cmath>

//****************************************
// �O���錾
//****************************************
class CObject;

//****************************************
// �֗��֐��錾
//****************************************
namespace general
{
	void RotLimit(D3DXVECTOR3* pRot);	// �����̏C��
	void RotLimit(float* pRot);	// �����̏C��
	void RotRevi(float* pRot);	// 

	// �}�g���b�N�X�ݒ�
	void SetMatrix(D3DXMATRIX* pMtxWorld, D3DXVECTOR3 pos, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });
	void SetMatrix(D3DXMATRIX* pMtxWorld, D3DXMATRIX pMtxWorldOwner, D3DXVECTOR3 pos, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });

	// ����
	void GetTime(void);
};

#endif