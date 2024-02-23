//===============================================================================
//
// �I�u�W�F�N�g2D[object2D.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBJECT2D_H_				//���̃}�N����`������Ă��Ȃ�������
#define _OBJECT2D_H_				//2�d�C���N���[�h�X�q�̃}�N�����`

// �C���N���[�h�錾
#include "main.h"
#include "object.h"

// �I�u�W�F�N�g2D�N���X
class CObject2D : public CObject
{
private:
	struct SInfo
	{
		LPDIRECT3DTEXTURE9 pTexture;	// �e�N�X�`���ւ̒��_���
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// �o�b�t�@�ւ̒��_���
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
		D3DXCOLOR col;	// �F
		float fWidth;	// ��
		float fHeight;	// �c
		float fLength;	// ����
		float fAngle;	// �p�x
		int nIdxTexture;	// �e�N�X�`���ԍ�
	};

public:
	CObject2D(int nPriority = 3);		//�R���X�g���N�^
	~CObject2D();	//�f�X�g���N�^

	static CObject2D *Create(void);
	static CObject2D *Create(D3DXVECTOR3 pos);
	static CObject2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static CObject2D *Create(D3DXVECTOR3 pos, const char *FileName);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//�ݒ�p
	void SetVertex_center(void);
	void SetVertex_left(void);
	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }
	void SetColor(D3DXCOLOR col) { m_info.col = col; }
	void SetSize(float fWidth, float fHeight);
	void SetTex(D3DXVECTOR2 left, D3DXVECTOR2 right);
	void BindTexture(int nIdx) { m_info.nIdxTexture = nIdx; }

	//�擾�p
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }
	D3DXCOLOR GetColor(void) { return m_info.col; }
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_info.fWidth, m_info.fHeight); }
	float GetAngle(void) { return m_info.fAngle; }
	float GetLength(void) { return m_info.fLength; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	int GetNumObj3D(void) { return m_nNumObj2D; };

private:
	SInfo m_info;
	static int m_nNumObj2D;
};

#endif