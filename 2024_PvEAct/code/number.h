//===============================================================================
//
// [number.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _NUMBER_H_				//���̃}�N����`������Ă��Ȃ�������
#define _NUMBER_H_				//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include "object.h"

//�O���錾
class CUI;

//�i���o�[�N���X
class CNumber
{
private:
	struct SInfo
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		//int nIdxTexture;	// �e�N�X�`���ԍ�
	};

public:
	CNumber();
	~CNumber();

	static CNumber *Create(D3DXVECTOR3 pos);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPosition(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 rot);
	void SetColor(D3DXCOLOR col);
	void SetSize(float fWidth, float fHeight);
	void SetTex(D3DXVECTOR2 left, D3DXVECTOR2 right);
	void BindTexture(const char* pTexPath);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);

private:
	SInfo m_info;
	CUI *m_pNumberUI;
};

#endif