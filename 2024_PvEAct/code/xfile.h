//===============================================================================
//
// [Xfile.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _XFILE_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _XFILE_H_	// 2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"

// �萔��`
namespace xfile
{
	static const int MAX_FILE = 64;
	static const int MAX_NAME = 254;
};

// �e�N�X�`���N���X
class CXfile
{
public:
	// X�t�@�C�����̍\����
	struct SXFile
	{
		LPD3DXMESH pMesh;	// �g�p����Ă��郂�f����
		LPD3DXBUFFER pBuffMat;	// ���_�J���[���
		DWORD dwNumMat;	// �J���[�̐�
		D3DXVECTOR3 vtxMin, vtxMax;	// �I�u�W�F�N�g�̍ő�l�E�ŏ��l
		char aXfileName[xfile::MAX_NAME];	// �t�@�C����
		int* pIdxTexture;	// �e�N�X�`���ԍ�
	};

public:
	CXfile();	// �R���X�g���N�^
	~CXfile();	// �f�X�g���N�^

	void Unload(void);	// �j��

	static CXfile* Create(void);
	static CXfile* GetInstance(void);
	static void Release(void);

	void InitLoad(void);
	int Load(const char *pXfileName);	// �o�^

	SXFile* GetXfile(int nIdx);
	LPD3DXMESH GetMesh(int nIdx);	// ���b�V�����̎擾
	LPD3DXBUFFER GetBuffMat(int nIdx);	// �o�b�t�@���̎擾
	DWORD GetDwNumMat(int nIdx);	// �}�e���A�����̎擾
	D3DXVECTOR3 GetMax(int nIdx) { return m_apXFile[nIdx]->vtxMax; }	// �ő�l�̎擾
	D3DXVECTOR3 GetMin(int nIdx) { return m_apXFile[nIdx]->vtxMin; }	// �ŏ��l�̎擾
	char* GetFileName(int nIdx) { return &m_apXFile[nIdx]->aXfileName[0]; }	// �t�@�C�����̎擾
	int* GetTextureIdx(int nIdx) { return m_apXFile[nIdx]->pIdxTexture; }	// �e�N�X�`���ԍ��̎擾
	int GetNumAll(void) { return m_nNumAll; }	// �����̎擾

private:
	// �����o�֐�
	void VtxMaxMin(int nNumCnt);	// �ő�l�E�ŏ��l

	// �����o�ϐ�
	static CXfile* m_pXfile;	// ���g�̃|�C���^
	SXFile* m_apXFile[xfile::MAX_FILE];
	int m_nNumAll;
};

#endif