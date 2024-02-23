//===============================================================================
//
// �L�����N�^�[[character.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _MOTIONDIFF_H_
#define _MOTIONDIFF_H_

// �C���N���[�h�錾
#include "main.h"
#include "object.h"
#include <stdio.h>

// �萔��`
namespace motionDiv
{
	static const int MODEL_MAX = 20;
	static const int MOTION_MAX = 10;
}

// �O���錾
class CObjectX;
class CXfile;
class CModelParts;

// �����L�����N�^�[�N���X
class CMotionDiv : public CObject
{
private:
	// �����̎��
	enum EDivType
	{
		DIV_LOWER = 0,
		DIV_UPPER,
		DIV_MAX
	};

	//-----------------------------
	// �L�����g�ݗ��ėp�̍\����
	//-----------------------------
	// �L�����N�^�[���̍\����
	struct SInfo
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 posOld;	// �ߋ��ʒu
		D3DXVECTOR3 rot;	// ����
		D3DXMATRIX mtxWorld;	// �}�g���b�N�X
		int aNumLoadPartsDiv[DIV_MAX];	// �ǂݍ��ނ̃p�[�c��
		int aNumSetPartsDiv[DIV_MAX];	// �ݒu����̃p�[�c��
		int nAllParts;	// ���f���p�[�c����
	};
	// ���f���p�[�c���̍\����
	struct SModelParts
	{
		int nIdx;	// �ԍ�
		int nIdxParent;	// �e�ԍ�
		CModelParts* pParts;	// �p�[�c���
	};

	//-----------------------------
	// ���[�V�����p�̍\����
	//-----------------------------
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
		SKey aKey[motionDiv::MODEL_MAX];	// �e���f���̃L�[�v�f
	};
	// ���[�V�������̍\����
	struct SMotionInfo
	{
		bool bLoop;	// ���[�v���邩�ǂ���
		int nNumKey;	// �L�[�̑���
		SKeyInfo aKeyInfo[motionDiv::MODEL_MAX];	// �L�[���
	};
	// ���[�V�������̍\����
	struct SMotion
	{
		int nNumMotion;	// ���[�V�����̑���
		int nType;		// ���[�V�����̎��
		int nTypeOld;	// �O��̃��[�V�����̎��
		int nKey;		// ���݂̃L�[
		int nIdxParent;	// �e�ԍ�
		int nNumParts;	// �p�[�c�̐�
		float fCounter;	// �J�E���^�[
		bool bFinish;	// �I���������ǂ���
		bool bLoop;		// ���[�v���邩�ǂ���
	};

public:
	CMotionDiv();	// �R���X�g���N�^
	~CMotionDiv();	// �f�X�g���N�^

	static CMotionDiv* Create(const char* pUpperPath, const char* pLowerPath);

	// �v���g�^�C�v�錾
	HRESULT Init();
	HRESULT Init(const char* pUpperPath, const char* pLowerPath);
	void Uninit();
	void Update();
	void Draw();
	
	// �ݒ�
	void SetMotion(int nNumDiv, int type);	// ���[�V����
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }	// �ʒu
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }	// ����
	void SetCounter(int nNumDiv, float fCounter) { m_aMotion[nNumDiv].fCounter = fCounter; }	// ���[�V�����J�E���^�[
	void SetMmultiply(float fMmultiply) { m_fCntMmultiply = fMmultiply; }	// ���[�V�����J�E���^�[�̏�Z

	// �擾
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	int GetType(int nNumDiv) { return m_aMotion[nNumDiv].nType; }
	int GetOldType(int nNumDiv) { return m_aMotion[nNumDiv].nTypeOld; }
	int GetIdxParent(int nNum) { return m_aMotion[nNum].nIdxParent; }
	int GetKey(int nNumDiv) { return m_aMotion[nNumDiv].nKey; }
	float GetCounter(int nNumDiv) { return m_aMotion[nNumDiv].fCounter; }
	float GetCntMmultiply(void) { return m_fCntMmultiply; }
	bool IsFinish(int nNumDiv) { return m_aMotion[nNumDiv].bFinish; }
	bool IsLoop(int nNumDiv) { return m_aMotion[nNumDiv].bLoop; }
	CModelParts* GetParts(int nNumDiv, int nNumPart);
	SMotionInfo GetMotionInfo(int nNumDiv, int nNumMotion) { return m_aMotionInfo[nNumDiv][nNumMotion]; }

private:
	// �����o�֐�
	void InitMotionDiv(int nType);
	void InitPose(int nType);
	void SetKeyOld(void);
	void UpdateMotionDiv(void);
	void Load(const char* pUpperPath, const char* pLowerPath);
	void LoadCharacter(FILE* pFile, int nCntDiv);
	void LoadMotion(FILE* pFile, int nCntDiv);
	void MultiplyMtx(void);
	void Debug(int nCntDiv, int nType);

	// �����o�ϐ�
	SInfo m_info;	// ���C�����
	SModelParts* m_apModelPart[EDivType::DIV_MAX][motionDiv::MODEL_MAX];	// �p�[�c���
	SMotion m_aMotion[EDivType::DIV_MAX];	// ���[�V�������
	SMotionInfo m_aMotionInfo[EDivType::DIV_MAX][motionDiv::MOTION_MAX];	// ���[�V�������
	SKey m_aKeyOld[DIV_MAX][motionDiv::MODEL_MAX];	// �O��̃L�[���̍\����
	float m_fCntMmultiply;	// ���[�V�����J�E���g�̔{��
};

#endif