//===============================================================================
//
// [janken.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _JANKEN_H_				//���̃}�N����`������Ă��Ȃ�������
#define _JANKEN_H_				//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include "object.h"

// �O���錾
class CObject2D;

// ����񂯂�N���X
class CJanken : public CObject
{
public:
	// �v���C���[���
	enum EPlayer
	{
		PLAYER_PLAYER,		// ��l
		PLAYER_COM,		// �R���s���[�^
		PLAYER_MAX
	};

	// ����񂯂�̎��
	enum EJanken
	{
		JANKEN_NONE = 0,
		JANKEN_GU,
		JANKEN_CHOKI,
		JANKEN_PA,
		JANKE_MAX
	};

	// ����񂯂񌋉�
	enum EResult
	{
		RESULT_NONE = 0,
		RESULT_WIN,
		RESULT_LOSE,
		RESULT_DRAW,
		RESULT_MAX
	};

	// ����񂯂�̗���
	enum EFlow
	{
		FLOW_NONE = 0,
		FLOW_INIT,
		FLOW_CHECK_ANSWER,
		FLOW_END,
		FLOW_MAX
	};

	struct SPlayerInfo
	{
		EJanken janken;	
		EResult result;
		bool bAnswer;	// ���������ǂ���
	};

public:
	CJanken(int nPriority = 3);	// �R���X�g���N�^
	~CJanken();	// �f�X�g���N�^

	static CJanken* Create();
	static CJanken* GetInstace(void);
	static void Release(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetJanken(EJanken janken);
	void SetFlow(EFlow flow);
	void IsJanken(bool bJanken);
	bool IsJanken(void) { return m_bJanken; }
	EResult GetResult(int nNumPlayer) { return m_aPlayerInfo[nNumPlayer].result; }

	void SetPosition(D3DXVECTOR3 pos) {}
	void SetRotation(D3DXVECTOR3 rot) {}
	D3DXVECTOR3 GetPosition(void) const {return {};}
	D3DXVECTOR3 GetRotation(void) const {return {};}

private:

	void Flow(void);	// �X�V�̃}�l�[�W��
	void ComAnswer(void);	// �R���s���[�^�̉�
	void CheckAnswer(void);	// �������킹
	void continuity(void);	// �A��

	CObject2D* m_apChoices2dUI[3];	// �I��UI
	SPlayerInfo m_aPlayerInfo[PLAYER_MAX];	// �v���C���[���
	EFlow m_flow;	// ����
	int m_nNumAllWin;	// ������
	int m_nNum;	// ����񂯂�
	bool m_bJanken;	// ����񂯂�����邩�ǂ���

	static CJanken* m_pJanken;	// �������g�̃|�C���^
};

#endif