//====================================================================
//
// ���b�V���t�B�[���h�̏���[meahfield.h]
// Author: ���}���V
//
//====================================================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

#include "main.h"
#include "object.h"

// ���b�V���E�H�[���N���X
class CMeshField : public CObject
{
private:
	// ���̍\����
	typedef struct
	{
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
		LPDIRECT3DINDEXBUFFER9 pIdxBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		D3DXVECTOR3 pos;	//�ʒu
		D3DXVECTOR3 rot;	//����
		D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
		float fWidth;	// ����
		float fdepth;	// ���s��
	} MeshWall;

public:
	CMeshField();
	~CMeshField();

	static CMeshField* Create(D3DXVECTOR3 pos);

	// �v���g�^�C�v�錾
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ݒ�
	void SetPosition(D3DXVECTOR3 pos) { }
	void SetRotation(D3DXVECTOR3 rot) { }

	// �擾
	D3DXVECTOR3 GetPosition(void) const { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_info.rot; }

private:
	MeshWall m_info;
	int m_nTextureIdx;

};

#endif