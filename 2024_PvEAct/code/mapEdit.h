//===============================================================================
//
// [mapEdit.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _MAPEDIT_H_	//���̃}�N����`������Ă��Ȃ�������
#define _MAPEDIT_H_	//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include "object.h"

// �O���錾
class CXfile;

// �I�u�W�F�N�gX�N���X
class CMapEdit : public CObject
{
public:
	enum EType
	{
		TYPE_HOUSE = 0,
		TYPE_MAX
	};

public:
	CMapEdit(int nPriority = 0);	//�R���X�g���N�^
	~CMapEdit();			//�f�X�g���N�^

	static CMapEdit* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ݒ�p
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }

	//�擾�p
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }

private:
	// �����o�֐�
	void Operation(void);	// ����

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//�o�b�t�@�ւ̒��_���
	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	D3DXVECTOR3 m_scale;	// �傫��

	CXfile* m_pXfile;	// x�t�@�C�����
	int m_nRegistModelIdx;	// �o�^���f���ԍ�
	int m_nTypeIdx;	// ��ޔԍ�
	EType m_type;	// ���f�����
	bool m_bUse;	// �g�����ǂ���
};

#endif