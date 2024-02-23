//===============================================================================
//
// �Q�[�����[game.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _GAME_H_				//���̃}�N����`������Ă��Ȃ�������
#define _GAME_H_				//2�d�C���N���[�h�X�q�̃}�N�����`

#include "main.h"
#include "manager.h"

//�O���錾
class CMap;
class CSlowManager;
class CPause;
class CSound;
class CObject2D;

//�^�C�g���N���X
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CPause *GetPause(void) { return m_pPause; }

private:
	void SlowUpdate(void);
	static CPause *m_pPause;
	CSound *m_pSound;
};

#endif