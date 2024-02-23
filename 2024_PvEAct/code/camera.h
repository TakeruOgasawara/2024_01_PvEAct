//===============================================================================
//
// �J�����̏���[camera.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _CAMERA_H_	//���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_	//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"

//***************************************************
// �J�����N���X
//***************************************************
class CCamera
{
public:
	CCamera();	// �R���X�g���N�^
	~CCamera();	// �f�X�g���N�^

	// �v���g�^�C�v�錾
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void MouseManage(void);
	void JoypadViewOperation(float fVertSpeed, float fHorizontalSpeed);
	void PlayerFollow(void);
	void PlayerLockon(float fOffset);
	void FixedJanken(void);	// �Œ�J����

	// �ݒ�
	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; }
	void SetPosR(D3DXVECTOR3 posR) { m_posR = posR; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetLength(float fLength) { m_fLength = fLength; }

	// �擾
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	float GetLength(void) { return m_fLength; }

private:
	// �����o�֐�
	void RegardpointOffset(void);
	void ViewpointOffset(void);
	void RotLimit(void);
	void SetShake(void);

	void SetShake(float intensity, float time);
	void ShakeUpdate(void);

	struct SShaker
	{
		float fIntensity;
		float fTime;
		float fStartTime;
	};

	// �����o�ϐ�
	D3DXMATRIX m_mtxView;	// �r���[�}�g���b�N�X
	D3DXMATRIX m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXVECTOR3 m_posV;	// ���_
	D3DXVECTOR3 m_posR;	// �����_
	D3DXVECTOR3 m_vecU;	// ������x�N�g��
	D3DXVECTOR3 m_rot;	// ����
	D3DXVECTOR3 m_rotOld;	// �ߋ�����
	D3DXVECTOR3 m_move;	// �ړ���
	D3DXVECTOR3 m_posVDest;	// ���_�̍���
	D3DXVECTOR3 m_posRDest;	// �����_�̍���
	float m_fLength;	// ���_�ƒ����_�̋���
	float m_fShakeTime;

	SShaker m_shakeInfo;
};

#endif