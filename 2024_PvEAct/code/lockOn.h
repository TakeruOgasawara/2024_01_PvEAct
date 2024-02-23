//===============================================================================
//
// ���b�N�I������[lockOn.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _LOCKON_H_
#define _LOCKON_H_

#include "main.h"
#include "billboard.h"

// ���b�N�I��
class CLockon : public CBillboard
{
public:
	CLockon();	// �R���X�g���N�^
	~CLockon();	// �f�X�g���N�^

	static CLockon* Create(D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f });
	static CLockon* Create(const char* cTexPath, D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f});

	// �v���g�^�C�v�錾
	HRESULT Init();
	HRESULT Init(D3DXVECTOR3 pos);
	HRESULT Init(const char* cTexPath, D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

private:
};

#endif