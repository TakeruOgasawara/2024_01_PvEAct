//====================================================================
//
// ���b�V���V�����_�[�̏���[meshcylindwe.h]
// Author: ���}���V
//
//====================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"
#include "object.h"

class CMeshCylinder : public CObject
{
private:
	struct Sinfo
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
		LPDIRECT3DINDEXBUFFER9 pIdxBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		D3DXMATRIX mtxWorld;	// �}�g���b�N�X���[���h
		int nIdxTexture;	// �e�N�X�`���ԍ�
	};

public:
	CMeshCylinder();
	~CMeshCylinder();

	//�v���g�^�C�v�錾
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_info.rot = rot; }

	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }

private:
	Sinfo m_info;

};

#endif