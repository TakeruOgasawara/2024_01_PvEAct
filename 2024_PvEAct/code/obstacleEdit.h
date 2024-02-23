//===============================================================================
//
// [mapEdit.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBSTACLEEDIT_H_	//���̃}�N����`������Ă��Ȃ�������
#define _OBSTACLEEDIT_H_	//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include "object.h"
#include "obstacle.h"

// �O���錾
class CXfile;
class CObjectX;

// �I�u�W�F�N�gX�N���X
class CObstacleEdit : public CObstacle
{
public:
	// ��Q���̎��	�����₷���Ƃɒǉ�
	enum EType
	{
		TYPE_HOUSE = 0,
		TYPE_I,
		TYPE_MAX
	};

public:
	CObstacleEdit(int nPriority = 0);	// �R���X�g���N�^
	~CObstacleEdit();	// �f�X�g���N�^

	static CObstacleEdit* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// �����o�֐�
	void Operation(D3DXVECTOR3* pos, D3DXVECTOR3* rot);	// ����
	void Save(void);

	// �����o�ϐ�
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	EType m_type;	// ���f�����

	CXfile* m_pXfile;	// x�t�@�C�����
	int m_nRegistModelIdx;	// �o�^���f���ԍ�
	int m_nTypeIdx;	// ��ޔԍ�
	bool m_bUse;	// �g�����ǂ���

	static int m_nNumAll;	// ����
};

#endif