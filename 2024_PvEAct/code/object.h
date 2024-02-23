//===============================================================================
//
// [object.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBJECT_H_				//���̃}�N����`������Ă��Ȃ�������
#define _OBJECT_H_				//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"

//***************************************************
//�O���錾
//***************************************************
class CObject2D;
class CEnemy3D;
class CPlayer3D;
class CChain;
class CMeshWall;
class CObjectX;

//***************************************************
// �萔��`
//***************************************************
namespace object
{
	static const int MAX_OBJECT = 5000;
	static const int NUM_PRIORITY = 8;
}

//***************************************************
//�I�u�W�F�N�g�N���X
//***************************************************
class CObject
{
public:	// �\���́E�񋓌^�p��public
	// �I�u�W�F�N�g�̎��
	typedef enum
	{
		TYPE_NONE,			// ����
		TYPE_EFFECT,		// �G�t�F�N�g
		TYPE_PARTICLE,		// �p�[�e�B�N��
		TYPE_OBJECT3D,		// �I�u�W�F�N�g3D
		TYPE_BILLBOARD,		// �r���{�[�h
		TYPE_OBJECTX,		// �I�u�W�F�N�gX(X�t�@�C������ǂݍ��񂾃��f��)
		TYPE_MAX
	} TYPE;

public:	//�����o�֐��p��public

	CObject(int nPriority = 3);	// �R���X�g���N�^ �f�t�H���g����
	virtual ~CObject();	// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) = 0;	// ������
	virtual void Uninit(void) = 0;	// �I��
	virtual void Update(void) = 0;	// �X�V
	virtual void Draw(void) {};	// �`��

	static void ReleaseAll(void);	// �S�I�u�W�F�N�g�̔j��
	static void UpdateAll(void);	// �S�I�u�W�F�N�g�̍X�V
	static void DrawAll(void);		// �S�I�u�W�F�N�g�̕`��

	// �ݒ�p�����o�֐�
	virtual void SetPosition(D3DXVECTOR3 pos) {};	// �ʒu�̐ݒ�
	virtual void SetRotation(D3DXVECTOR3 rot) {};
	void SetType(TYPE type) { m_type = type; }		// ��ނ̐ݒ�

	// �擾�p�����o�֐�
	static CObject* GetTop(const int nPriority) { return m_apTop[nPriority]; }
	static CObject* GetEnd(const int nPriority) { return m_apEnd[nPriority]; }
	CObject* GetNext(void) { return m_pNext; }

	virtual D3DXVECTOR3 GetPosition(void) const { return {}; }	// �ʒu�̎擾
	virtual D3DXVECTOR3 GetRotation(void) const { return {}; }	// �����̎擾
	virtual D3DXVECTOR3 GetMovement(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// �ړ��ʂ̎擾
	virtual float GetRadius(void) { return 0.0f; }

	static int GetNumAll(void) { return m_nNumAll; }	// �I�u�W�F�N�g�����̎擾
	static int GetNumEffectAll(void) { return m_nNumEffectAll; }	// �G�t�F�N�g����

	TYPE GetType(void) { return m_type; }	// ��ނ̎擾

protected:
	void Release(void);	// �I�u�W�F�N�g���g�̔j��

private:
	void Death(void);

	static CObject* m_apTop[object::NUM_PRIORITY];	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject* m_apEnd[object::NUM_PRIORITY];	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pPrev;	// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^
	int m_nPriority;	// �D�揇�ʂ̈ʒu
	bool m_bDeath;	// �I�u�W�F�N�g�̎��S�t���O
	TYPE m_type;	// �I�u�W�F�N�g�̎��

	static int m_nNumAll;	// ����
	static int m_nNumEffectAll;	// �G�t�F�N�g����
};

#endif