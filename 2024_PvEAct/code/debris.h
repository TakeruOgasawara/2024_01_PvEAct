////*****************************************************
////
//// �j�Џ���[debris.h]
//// Author:���}���V
////
////*****************************************************
//
//#ifndef _DERIS_H_
//#define _DERIS_H_
//
////*****************************************************
//// �C���N���[�h
////*****************************************************
//#include "objectX.h"
//
////*****************************************************
//// �N���X��`
////*****************************************************
//class CDebris : public CObjectX
//{
//public:
//	CDebris(int nPriority = 3);
//	~CDebris();
//
//	static CDebris* Create(D3DXVECTOR3 pos, int nLife, int nModelType, D3DXVECTOR3 move = { 0.0f,0.0f,0.0f }, float fGravity = 0.0f, bool bBounce = false, int nPriority = 3);
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//
//private:
//	enum STATE
//	{
//		STATE_NONE = 0,	// ���ł��Ȃ����
//		STATE_NORMAL,	// �ʏ���
//		STATE_SHRINK,	// �k��ł�����
//		STATE_MAX
//	};
//
//	enum TYPE
//	{
//		TYPE_NONE = 0,
//		TYPE_WALL,	// ��
//		TYPE_SOIL,	// �y
//		TYPE_MAX
//	};
//
//	void Shrink(void);
//
//	D3DXVECTOR3 m_rotVelocity;	// ��]���x
//	D3DXVECTOR3 m_move;	// �ړ���
//	int m_nLife;	// ����
//	int m_nModelType;	// ���f�����
//	float m_fGravity;	// �d��
//	bool m_bBounce;
//	STATE m_state;	// ���
//	TYPE m_type;	// 
//};
//
//#endif