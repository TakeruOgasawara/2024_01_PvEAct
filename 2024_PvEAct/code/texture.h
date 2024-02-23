//===============================================================================
//
// [texture.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _TEXTURE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _TEXTURE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"

//�}�N����`
#define MAX_TEXTURE			(256)

//�e�N�X�`���N���X
class CTexture
{
private:
	struct Data
	{
		LPDIRECT3DTEXTURE9 pTexture;	// �e�N�X�`���|�C���^
		char aName[MAX_TEXTURE];	// �e�N�X�`����
	};

public:
	CTexture();	// �R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

	static CTexture* Create(void);
	static CTexture* GetInstance(void);
	static void Release(void);

	HRESULT Load(void);
	void Unload(void);

	int Regist(const char *pTexPath);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:
	// �����o�ϐ�
	static CTexture* m_pTexture;

	Data m_aData[MAX_TEXTURE];
	static int m_nNumAll;	// ����
};

#endif