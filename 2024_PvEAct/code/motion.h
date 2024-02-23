//===============================================================================
//
// �L�����N�^�[[character.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

// �C���N���[�h�錾
#include "main.h"
#include "object.h"
#include <stdio.h>

// �萔��`
namespace motion
{
	static const int MODEL_MAX = 20;
	static const int MOTION_MAX = 40;
}

// �O���錾
class CObjectX;
class CXfile;
class CModelParts;

// �L�����N�^�[�N���X
class CMotion : public CObject
{
private:
	// �L�����N�^���̍\����
	struct SInfo
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
		D3DXMATRIX mtxWorld;	// �}�g���b�N�X
		int nNumModelparts;	// ���f���p�[�c����
	};

	// ���f���p�[�c���̍\����
	struct SModelParts
	{
		int nIdx;
		int nIdxParent;	// �e�ԍ�
		CModelParts* pParts;	// �p�[�c���
	};

	//--------------------------
	// ���[�V�����p�̍\����
	//--------------------------
	// �L�[�̍\����
	struct SKey
	{
		float fPosX;
		float fPosY;
		float fPosZ;
		float fRotX;
		float fRotY;
		float fRotZ;
	};
	
	// �L�[���̍\����
	struct SKeyInfo
	{
		float fFrame;	// �Đ��t���[��
		SKey aKey[motion::MODEL_MAX];	// �e���f���̃L�[�v�f
	};

	// ���[�V�������̍\����
	struct SMotionInfo
	{
		bool bLoop;	// ���[�v���邩�ǂ���
		int nNumKey;	// �L�[�̑���
		SKeyInfo aKeyInfo[motion::MODEL_MAX];	// �L�[���
	};

	// ���[�V�������̍\����
	struct SMotion
	{
		int nNumMotion;	//���[�V�����̑���
		int nType;	//���[�V�����̎��
		int nTypeOld;	//�O��̃��[�V�����̎��
		int nKey;	//���݂̃L�[
		float fCounter;	// �J�E���^�[
		int nNumParts;	// �p�[�c�̐�

		bool bFinish;	// �I���������ǂ���
	};

public:
	CMotion();	// �R���X�g���N�^
	~CMotion();	// �f�X�g���N�^

	static CMotion* Create(const char* pTextPath);

	// �v���g�^�C�v�錾
	HRESULT Init();
	HRESULT Init(const char* pTextPath);
	void Uninit();
	void Update();
	void Draw();

	// �ݒ�
	void SetMotion(int nType);
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }
	void SetMmultiply(float fMmultiply) { m_fCntMmultiply = fMmultiply; }	// ���[�V�����J�E���^�[�̏�Z


	// �擾
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	int GetType(void) { return m_motion.nType; }
	int GetOldType(void) { return m_motion.nTypeOld; }
	int GetIdxParent(int nNum) { return m_apModelPart[nNum]->nIdxParent; }
	int GetKey(void) { return m_motion.nKey; }
	float GetCounter(int nNumDiv) { return m_motion.fCounter; }
	float GetCntMmultiply(void) { return m_fCntMmultiply; }
	bool IsFinish(void) { return m_motion.bFinish; }
	bool IsLoop(int nNumType) { return m_aMotionInfo[nNumType]->bLoop; }
	SModelParts* GetParts(int nNumPart) { return m_apModelPart[nNumPart]; }
	SMotionInfo* GetMotionInfo(int nNumMotion) { return m_aMotionInfo[nNumMotion]; }
	SMotion GetMotion(void) { return m_motion; }

private:
	// �����o�֐�
	void InitMotion(int nType);
	void Load(const char* cTextPath);
	void LoadCharacter(FILE* pFile);
	void LoadMotion(FILE* pFile);
	void MultiplyMtx(void);
	void Debug(void);

	// �����o�ϐ�
	SInfo m_info;	// ���C�����
	SModelParts* m_apModelPart[motion::MODEL_MAX];	// �p�[�c���
	SMotion m_motion;
	SMotionInfo* m_aMotionInfo[motion::MOTION_MAX];	// ���[�V�������
	float m_fCntMmultiply;
};

#endif