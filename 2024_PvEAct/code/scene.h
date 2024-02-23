//===============================================================================
//
// �V�[������[scene.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _SCENE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _SCENE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"

//***********************************
// �O���錾
//***********************************
class CTitle;
class CGame;
class CResult;
class CFade;

//***********************************
// �V�[���N���X
//***********************************
class CScene
{
public:
	// �V�[�����
	enum EMode
	{
		MODE_TITLE = 0,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	} ;

	CScene();	// �R���X�g���N�^
	~CScene();	// �f�X�g���N�^

	static CScene* Create(EMode mode);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetMode(EMode mode) { m_mode = mode; }
	static EMode GetMode(void) { return m_mode; }

private:
	static EMode m_mode;
};

#endif
