//===============================================================================
//
// �L�����N�^�[[character.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _CHARACTER_DIV_H_
#define _CHARACTER_DIV_H_

// �C���N���[�h�錾
#include "main.h"
#include "object.h"

// �萔��`
namespace character
{
	const int MODEL_MAX = 20;
}

// �O���錾
class CObjectX;
class CXfile;
class CMotionDiv;

// �L�����N�^�[�N���X
class CCharacterDiv : public CObject
{
public:
	// �����̎��
	enum EDivType
	{
		DIV_LOWER = 0,
		DIV_UPPER,
		DIV_MAX
	};

private:
	// �L�����N�^���\����
	struct SCharacter
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 posOld;	// �ߋ��ʒu
		D3DXVECTOR3 rot;	// ����
		D3DXVECTOR3 move;
		D3DXMATRIX mtxWorld;	// �}�g���b�N�X
		CMotionDiv* pBody;	// 
	};

public:
	CCharacterDiv();	// �R���X�g���N�^
	~CCharacterDiv();	// �f�X�g���N�^

	static CCharacterDiv* Create(const char* pUpperPath, const char* pLowerPath);

	// �v���g�^�C�v�錾
	HRESULT Init();
	HRESULT Init(const char* pUpperPath, const char* pLowerPath);
	void Uninit();
	void Update();
	void Draw();

	// �ݒ�
	void SetPosition(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 rot);
	void SetMovement(D3DXVECTOR3 move) { m_info.move = move; }
	void SetMotion(int nNumDiv, int nMotion);
	void SetBody(CMotionDiv* pBody);

	// �擾
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	D3DXVECTOR3 GetMovement(void) const { return m_info.move; }
	int GetType(int nNumDiv);
	CMotionDiv* GetBody(void);

private:
	// �����o�ϐ�
	SCharacter m_info;
};

#endif