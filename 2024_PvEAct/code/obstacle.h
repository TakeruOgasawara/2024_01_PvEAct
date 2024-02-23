//===============================================================================
//
// [mapEdit.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBSTACLE_H_	//���̃}�N����`������Ă��Ȃ�������
#define _OBSTACLE_H_	//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include "objectX.h"

// �O���錾
class CXfile;
class CObjectX;

// �I�u�W�F�N�gX�N���X
class CObstacle : public CObjectX
{
public:
	CObstacle(int nPriority = 0);	// �R���X�g���N�^
	~CObstacle();	// �f�X�g���N�^

	static CObstacle* Create(const char* c_pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f }, float fLife = 0.0f , bool bDestoroy = false);

	HRESULT Init(const char* c_pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLife, bool bDestoroy);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Hit(float fDamage);	// �_���[�W����

	// �ݒ�p
	//void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	//void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetDestoroy(bool bDestoroy) { m_bDestoroy = bDestoroy; }
	void SetLife(float fLife) { m_fLife = fLife; }

	// �擾�p
	//D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	//D3DXVECTOR3 GetRotation(void) const { return m_rot; }

private:
	// �����o�֐�

	// �����o�ϐ�
	bool m_bDestoroy;	// �j��ł��邩�ǂ���
	float m_fLife;	// ����
};

#endif