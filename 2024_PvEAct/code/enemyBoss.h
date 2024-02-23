//===============================================================================
//
// [enemy.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _ENEMYBOSS_H_	//���̃}�N����`������Ă��Ȃ�������
#define _ENEMYBOSS_H_	//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include "character.h"

//�O���錾
class CMotion;
class CObjectX;
class CObject3D;
class CSphereCollider;
class CSlowManager;

//�v���C���[�N���X
class CEnemyBoss : public CCharacter
{
public:
	// �㔼�g���[�V�����̏��
	enum EMotion
	{
		MOTION_NORMAL = 0,	// �ʏ�
		MOTION_ATTACK00,		// �U��
		MOTION_MAX
	};

	enum EState
	{
		STATE_SURVIVAL = 0,
		STATE_DEATH,
		STATE_MAX
	};

	// ���[�V�������̍\����
	struct SMotionInfo
	{
		CMotion* pMotion;	// �������[�V�����|�C���^
	};

	// ��{���̍\����
	struct SInfo
	{
		D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 posOld;	//�ߋ��ʒu
		D3DXVECTOR3 rot;	// ����
		D3DXVECTOR3 move;	//�ړ���
		EState state;
		float fRotDest;	//�ڕW�̊p�x
		float fRotDiff;	//�ڕW�ƌ��݂̍���
		float fLife;	// �̗�
	};

public:
	CEnemyBoss(int nPriority = 3);	// �R���X�g���N�^
	~CEnemyBoss();	// �f�X�g���N�^

	static CEnemyBoss* Create(D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f}, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });
	static CEnemyBoss* GetInstance(void);
	static void Release(void);

	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Hit(float fDamage);

	//�ݒ�p
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }
	void SetMovement(D3DXVECTOR3 move) { m_info.move = move; }
	void SetDamage(float fDamage) { m_DiffLife = fDamage; }

	//�擾�p
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	D3DXVECTOR3 GetMovement(void) const { return m_info.move; }
	D3DXMATRIX* GetMatrix(void) { return &m_info.mtxWorld; }
	float GetLife(void) { return m_info.fLife; }
	float GetDamage(void) { return m_DiffLife; }
	EState GetState(void) { return m_info.state; }

private:
	// �����o�֐�
	void BattleNormal(void);
	void BattleJust(void);
	void BattleRush(void);
	void BattleJanken(void);

	void RotAor(void);
	void Attack(void);
	bool IsAttack(void);
	void Rot(void);		// ����
	void Move(void);	// �ړ�
	void Rush(void);	// �ːi
	void Stop(void);	// ��~

	void Motion(void);

	void Debug(void);
	
	// �����o�ϐ�
	SInfo m_info;	// ��{���
	SMotionInfo m_motionInfo;	// ���[�V�������
	CSphereCollider* m_pSphereCollider;	//�����蔻��
	int m_fAttacCnt;
	CObjectX* m_pObjectX;
	CObject3D* m_pAttackRange;
	CSlowManager* m_pSlow;

	bool m_bAttack;
	float m_DiffLife;

	static CEnemyBoss* m_pEnemy;
};

#endif