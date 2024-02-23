//===============================================================================
//
// �^�C�g�����[title.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _TITLE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _TITLE_H_				//2�d�C���N���[�h�X�q�̃}�N�����`

#include "main.h"
#include "manager.h"

//�O���錾
class CObject2D;
class CMap;

//�^�C�g���N���X
class CTitle : public CScene
{
private:

	enum STATE
	{
		STATE_NONE = 0,
		STATE_IN,
		STATE_OUT,
		STATE_MAX
	};

	struct SUI
	{
		CObject2D* pBg;
		CObject2D* pPress;
	};

public:
	CTitle();
	~CTitle();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw(); 

private:
	// �����o�֐�
	void CreateBg(void);
	void CreatePress(void);
	void UpdatePress(void);

	// �����o�ϐ�
	int m_nTextureIdx;	//�e�N�X�`���ւ̒��_���
	SUI m_ui;
	STATE m_state;
};

#endif