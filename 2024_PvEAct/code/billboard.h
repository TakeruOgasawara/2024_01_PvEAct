//===============================================================================
//
// [billboard.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _BILLBOARD_H_				//���̃}�N����`������Ă��Ȃ�������
#define _BILLBOARD_H_				//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include "object.h"

//�r���{�[�h�N���X
class CBillboard : public CObject
{
public:
	CBillboard(int nPriority = 3);			//�R���X�g���N�^
	~CBillboard();			//�f�X�g���N�^

	static CBillboard *Create(void);
	static CBillboard *Create(D3DXVECTOR3 pos);
	static CBillboard* Create(const char* pTexPath, D3DXVECTOR3 pos = { 0.0, 0.0f, 0.0f });

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ݒ�p
	void SetverTex(void);			//���_�ݒ�
	virtual void SetPosition(D3DXVECTOR3 pos);			//�ʒu�ݒ�
	virtual void SetRotation(D3DXVECTOR3 rot);			//�����ݒ�
	void SetSize(D3DXVECTOR2 size);
	void SetRadius(D3DXVECTOR2 radius);
	void SetColor(D3DXCOLOR col);

	//�擾�p
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }			//�ʒu�擾
	D3DXVECTOR3 GetRotation(void) const  { return m_rot; }			//�����擾
	D3DXVECTOR2 GetSize(void) { return m_size; }			//�傫���擾
	D3DXCOLOR GetColor(void) { return m_col; }			//�F�擾
	float GetLength(void) { return m_fLength; }			//�����擾
	float GetAngle(void) { return m_fAngle; }			//�p�x�擾

	//�e�N�X�`�����蓖��
	void BindTexture(int nIdx);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̒��_���
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//�o�b�t�@�ւ̒��_���
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_move;			//�ړ�
	D3DXVECTOR2 m_size;			//�傫��
	D3DXCOLOR m_col;			//�F
	float m_fLength;			//����
	float m_fAngle;				//�p�x
	bool m_bUse;				//�g���Ă��邩
	int m_nTextureIdx;			//�e�N�X�`���ԍ�

};
#endif