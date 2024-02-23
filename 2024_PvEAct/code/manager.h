//===============================================================================
//
// [manager.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _MANAGER_H_				//���̃}�N����`������Ă��Ȃ�������
#define _MANAGER_H_				//2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"
#include "object.h"
#include "scene.h"

//�O���錾
class CRenderer;
class CDebugProc;
class CSound;
class CCamera;
class CLight;
class CObjectX;

class CScene;
class CTitle;
class CGame;
class CResult;
class CFade;

// �}�l�[�W���N���X(�S�̊Ǘ�)
class CManager
{
public:
	CManager();
	~CManager();

	static CManager* GetInstance(void);
	static void Release(void);

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static void SetMode(CScene::EMode mode);
	static CScene::EMode GetMode(void) { return CScene::GetMode(); }
	void SetTick(float fTick) { m_fTick = fTick; }
	
	CRenderer *GetRenderer(void) { return m_info.pRenderer; }
	CDebugProc *GetDebugProc(void) { return m_info.pDebugProc; }
	CSound *GetSound(void) { return m_info.pSound; }
	CCamera *GetCamera(void) { return m_info.pCamera; }
	CLight *GetLight(void) { return m_info.pLight; }
	CScene *GetScene(void) { return m_info.pScene; }
	CFade *GetFade(void) { return m_info.pFade; }
	float GetTick(void) { return m_fTick; }

private:
	static CManager* m_pManager;

	struct SInfo
	{
		CRenderer* pRenderer;
		CDebugProc* pDebugProc;
		CSound* pSound;
		CCamera* pCamera;
		CLight* pLight;

		CScene* pScene;
		CTitle* pTitle;
		CGame* pGame;
		CResult* pResult;
		CFade* pFade;
	};

	SInfo m_info;	// ���
	float m_fTick;
	bool m_bUse;
};

#endif
