//===============================================================================
//
// �X���[����[slow.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _SLOW_MANAGER_H_
#define _SLOW_MANAGER_H_

// �C���N���[�h�錾
#include "main.h"
#include "object.h"

// �X���[�}�l�[�W���N���X
class CSlowManager : public CObject
{
private:
	static const int MAX_OBJECT = 24;	// �ő吔

public:
	enum ECamp	// �w�c
	{
		CAMP_NONE,
		CAMP_PAYER,
		CAMP_ENEMY,
		CAMP_MAX
	};

	enum ETag	// �^�O
	{
		TAG_NONE = 0,
		TAG_PLAYER,
		TAG_ENEMY,
		TAG_MAX
	};

public:
	CSlowManager(int nCamp = CAMP_NONE);	// �R���X�g���N�^
	~CSlowManager();	// �f�X�g���N�^

	// ����
	static CSlowManager* Create(void);
	static CSlowManager* Create(ECamp camp, ETag tag);

	// �v���g�^�C�v�錾
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	static void ReleaseAll(void);

	void Player(float fValue);
	void Enemy(float fValue);
	void Tag(ECamp camp, ETag tag, float fValue);
	void All(float fValue);	// �S�̂��X���[
	void Default(void);	// �f�t�H���g

	// �ݒ�
	void SetTag(ETag tag) { m_tag = tag; }	// �^�O
	void SetCamp(ECamp camp) { m_camp = camp; }	// �����w�c
	void SetValue(float fValue) { m_fValue = fValue; }

	// �擾
	ETag GetTag(void) const { return m_tag; }
	ECamp GetCamp(void) const { return m_camp; }
	float GetValue(void) const { return m_fValue; }
	CSlowManager** GetInfo(void) { return &m_apSlow[0][0]; }

private:
	// �����o�ϐ�
	static CSlowManager* m_pManager;
	ETag m_tag;	// �^�O
	float m_fValue;	// �d�v
	float m_fCount;

	// ���
	static CSlowManager* m_apSlow[CAMP_MAX][MAX_OBJECT];	// �Ǘ��p
	ECamp m_camp;	// ����
	int m_nID;	// ���ꂼ���ID
	float m_fTime;
	static int m_nNumAll;	// ����
};

#endif