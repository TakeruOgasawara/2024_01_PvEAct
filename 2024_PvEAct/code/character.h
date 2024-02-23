//===============================================================================
//
// �L�����N�^�[[character.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

// �C���N���[�h�錾
#include "main.h"
#include "object.h"

// �O���錾
class CObjectX;
class CXfile;
class CMotion;

// �L�����N�^�[�N���X
class CCharacter : public CObject
{
private:
	// �萔
	const int MODEL_MAX = 20;

private:
	// �L�����N�^���\����
	struct SCharacter
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 posOld;	// �ߋ��ʒu
		D3DXVECTOR3 rot;	// ����
		D3DXVECTOR3 move;
		D3DXMATRIX mtxWorld;	// �}�g���b�N�X
		CMotion* pBody;	// 
	};

public:
	CCharacter();	// �R���X�g���N�^
	~CCharacter();	// �f�X�g���N�^

	static CCharacter* Create(const char* pPath);

	// �v���g�^�C�v�錾
	HRESULT Init();
	HRESULT Init(const char* pPath);
	void Uninit();
	void Update();
	void Draw();

	// �ݒ�
	void SetPosition(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 rot);
	void SetMotion(int nMotion);
	void SetBody(CMotion* pBody);

	// �擾
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	D3DXVECTOR3 GetMovement(void) const { return m_info.move; }
	int GetType(void);
	CMotion* GetBody(void);

private:
	// �����o�ϐ�
	SCharacter m_info;
};

#endif