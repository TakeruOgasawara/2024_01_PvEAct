//===============================================================================
//
// [enemy.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _ENEMY_H_	//���̃}�N����`������Ă��Ȃ�������
#define _ENEMY_H_	//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include "object.h"

//�O���錾
class CCollision;
class CMotionDiv;

//�v���C���[�N���X
class CEnemy : public CObject
{
public:
	// �㔼�g���[�V�����̏��
	enum EUpperMotion
	{
		UPPER_MOTION_NORMAL = 0,	// �ʏ�
		UPPER_MOTION_RUN,			// ����
		UPPER_MOTION_MAX
	};
	// �����g���[�V�����̏��
	enum ELowerMotion
	{
		LOWER_MOTION_NORMAL = 0,	// �ʏ�
		LOWER_MOTION_RUN,			// ����
		LOWER_MOTION_MAX
	};

	// ���[�V�������̍\����
	struct SMotionInfo
	{
		EUpperMotion upperState;	// �㔼�g
		ELowerMotion lowerState;	// �����g
		CMotionDiv* pMotionDiv;	// �������[�V�����|�C���^
	};

	// ��{���̍\����
	struct SInfo
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 posOld;	//�ߋ��ʒu
		D3DXVECTOR3 rot;	// ����
		D3DXVECTOR3 move;	//�ړ���
		float fRotDest;	//�ڕW�̊p�x
		float fRotDiff;	//�ڕW�ƌ��݂̍���
	};

public:
	CEnemy(int nPriority = 1);	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	static CEnemy* Create(D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f}, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });
	static CEnemy* GetInstance(void);
	static void Release(void);

	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ݒ�p
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }

	//�擾�p
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	D3DXVECTOR3 GetMove(void) { return m_info.move; }

private:
	// �����o�֐�
	void RotAor(void);

	// �����o�ϐ�
	SInfo m_info;	// ��{���
	SMotionInfo m_motionInfo;	// ���[�V�������
	CCollision* m_pCollision;	//�����蔻��

	static CEnemy* m_pEnemy;
};

#endif