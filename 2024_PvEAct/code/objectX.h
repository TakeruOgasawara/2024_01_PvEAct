//===============================================================================
//
// [objectX.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBJECTX_H_				//���̃}�N����`������Ă��Ȃ�������
#define _OBJECTX_H_				//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include "object.h"
#include "xfile.h"

// �I�u�W�F�N�gX�N���X
class CObjectX : public CObject
{
public:
	CObjectX(int nPriority = 0);			//�R���X�g���N�^
	~CObjectX();			//�f�X�g���N�^

	static CObjectX *Create(void);
	static CObjectX* Create(const char* c_pFileName, D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f});

	HRESULT Init(void);
	HRESULT Init(const char *c_pFileName, D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f });
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawOnly(void);

	//�ݒ�p
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
	void SetModel(const char* cPath);
	void SetMatrix(D3DXMATRIX pMtx) { m_mtxWorld = pMtx; }
	void BindXfile(int nIdxTexture) { m_pXfile = CXfile::GetInstance()->GetXfile(nIdxTexture); }

	//�擾�p
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	D3DXVECTOR3 GetScale(void) const { return m_scale; }
	D3DXVECTOR3 GetVtxMax(void) const;
	D3DXVECTOR3 GetVtxMin(void) const;
	CObjectX *GetObjectX(void) { return this; }
	D3DXMATRIX* GetMatrix(void) { return &m_mtxWorld; }
	CXfile* GetXfile(void);
	int GetModelIdx(void) { return m_nModelIdx; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// �o�b�t�@�ւ̒��_���
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_rot;	// ����
	D3DXVECTOR3 m_scale;	// �傫��

	CXfile::SXFile* m_pXfile;	// X�t�@�C�����
	int m_nModelIdx;	// ���f���ԍ�
};

#endif