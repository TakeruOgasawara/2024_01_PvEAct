//===============================================================================
//
// [player.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _PLAYER_H_				//���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_H_				//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include "characterDiv.h"

//�O���錾
class CCollision;
class CMotionDiv;
class CLockon;
class CJanken;
class CSphereCollider;
class CWepon;
class CSlowManager;

//�v���C���[�N���X
class CPlayer : public CCharacterDiv
{
public:
	// �����g���[�V�����̏��
	enum ELowerMotion
	{
		LOWER_MOTION_NORMAL = 0,	// �ʏ�
		LOWER_MOTION_RUN,			// ����
		LOWER_MOTION_DASH,			// �O�_�b�V��
		LOWER_MOTION_ATTACK00,
		LOWER_MOTION_ATTACK01,
		LOWER_MOTION_MAX
	};
	// �㔼�g���[�V�����̏��
	enum EUpperMotion
	{
		UPPER_MOTION_NORMAL = 0,	// �ʏ�
		UPPER_MOTION_RUN,			// ����
		UPPER_MOTION_DASH,			// �O�_�b�V��
		UPPER_MOTION_ATTACK00,		// �U��
		UPPER_MOTION_ATTACK01,		// �U��
		UPPER_MOTION_MAX
	};

	// ���[�V�������̍\����
	struct SMotionInfo
	{
		CMotionDiv* pMotionDiv;	// �������[�V�����|�C���^
	};

	// ���[�V�����̎g�p��Ԃ̍\����
	struct SIsMotion
	{
		bool bAvoidance;	// ���
		bool bAttack;	// �U��1
		bool bAttack1;	// �U��2
	}; 

	struct SIsGameTrigger
	{
		bool bJust;
		bool bRush;	// ���b�V�����邩�ǂ���
	};

	// ��{���̍\����
	struct SInfo
	{
		D3DXVECTOR3 moveAvo;	// ���
		SIsMotion isMotionList;	// ���[�V�����̎g�p���
		float fLife;	// �̗�
		float fRotDest;	//�ڕW�̌���
		float fRotDiff;	//�ڕW�ƌ��݂̍���
	};

	struct SAvoid
	{
		float fTime;	// �������
		float fAngleMove;	// �p�x�̈ړ���
		float fDiff;	// 
		float fDest;	// 
		float fTempLength;
		D3DXVECTOR3 move;
	};

public:
	CPlayer(int nPriority = 3);	// �R���X�g���N�^
	~CPlayer();	// �f�X�g���N�^

	static CPlayer* Create(D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f}, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f });
	static CPlayer* GetInstance(void);
	static void Release(void);

	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Hit(float fDamage);

	// �ݒ�p
	void SetIsJust(bool bJust) { m_isTriger.bJust = bJust; }

	// �擾�p
	SIsMotion IsMotion(void) { return m_info.isMotionList; }
	float GetAvoidTime(void) { return m_avoid.fTime; }
	bool GetIsJust(void) { return m_isTriger.bJust; }
	float GetLife(void) { return m_info.fLife; }
	SIsGameTrigger GetIsGameTriger(void) { return m_isTriger; }

private:
	// �����o�֐�
	void BattleNormal(void);
	void BattleJust(void);
	void BattleRush(void);
	void BattleJanken(void);

	void ManageInput(void);
	void InputMove(void);
	void InputAttack(void);
	void InputJanken(void);
	void InputAvoidance(void);

	void Lockon(void);

	void Attack(void);
	bool IsAttack(bool bAway);

	void Evasion(void);	// ���
	void AfterImage(void);
	void RotDest(void); // �ړI
	// ���[�V����
	void ManageMotion(void);
	void LowerMotion(void);
	void UpperMotion(void);

	void RimitPos(D3DXVECTOR3* pPos);
	void RotAor(void);
	void Debug(void);

	// �����o�ϐ�
	static CPlayer* m_pPlayer;
	SInfo m_info;	// ��{���
	SMotionInfo m_motionInfo;	// ���[�V�������
	CSphereCollider* m_pSphereCollider;	//�����蔻��
	CLockon* m_pLockon;	// ���b�N�I���|�C���^
	CJanken* m_pJanken;	// ����񂯂�|�C���^
	CSlowManager* m_pSlow;
	CCharacterDiv* m_pAfterImage;	// �c��
	bool m_bLockon;	// ���b�N�I�����邩�ǂ���
	float m_fCntDash;
	float m_fTempLength;
	SIsGameTrigger m_isTriger;
	CWepon* m_pWepon;
	SAvoid m_avoid;
};

#endif