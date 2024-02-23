////*****************************************************
////
//// ��юU��j�Џ���[debrisSpawner.h]
//// Author:���}���V
////
////*****************************************************
//
//#ifndef _DERISSPAWNER_H_
//#define _DERISSPAWNER_H_
//
////*****************************************************
//// �C���N���[�h
////*****************************************************
//#include "object.h"
//
////*****************************************************
//// �N���X��`
////*****************************************************
//class CDebrisSpawner : public CObject
//{
//public:
//	// �j�Ђ̎��
//	enum TYPE
//	{
//		TYPE_NONE = 0,	// ���ł��Ȃ�
//		TYPE_WALL,	// ��
//		TYPE_SOIL,	// �y
//		TYPE_MAX
//	};
//
//	CDebrisSpawner(int nPriority = 3);	// �R���X�g���N�^
//	~CDebrisSpawner();	// �f�X�g���N�^	
//
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void) {};
//	static CDebrisSpawner* Create(D3DXVECTOR3 pos, TYPE type, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f }, int nPriorityEffect = 5);
//	void SetPosition(D3DXVECTOR3 pos);
//	static void Load(void);	// �Ǎ�����
//	static void Unload(void);	// �j������
//
//private:
//	typedef struct
//	{// �p�[�e�B�N�����
//		int nModelIdx;	// ���f���̎��
//		int nLife;	// ����
//		int nLifeDebris;	// �j�Ђ̎���
//		int nNumDebris;	// �j�Ђ̐�
//		float fSpeed;	// �j�Ђ̊�X�s�[�h
//		D3DXCOLOR col;	// �F
//		float fGravity;	// �d��
//		float fRangeRot;	// �����̃����_���͈�
//		int nRot;	// �����𔽉f���邩�ǂ���
//		bool bTurn;	// ���]���邩�ǂ���
//		bool bBounce;	// �o�E���h���邩�ǂ���
//	}PARTICLE_INFO;
//
//	// �����o�֐�
//	void DebrisWall(void);
//
//	// �����o�ϐ�
//	D3DXVECTOR3 m_pos;	// �ʒu
//	D3DXVECTOR3* m_pPosOwner;	// ������̈ʒu
//	D3DXVECTOR3 m_rot;	// ����
//	int m_nLife;	// ����
//	int m_ModelIdx;
//	static PARTICLE_INFO* m_apDebris[TYPE_MAX + 1];
//	TYPE m_type; // ���
//	int m_nPriorityDebris;
//};
//
//#endif