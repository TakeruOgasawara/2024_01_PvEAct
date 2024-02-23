//===============================================================================
//
// [object.h]
// Author : Takeru Ogasawara
//
//===============================================================================
#ifndef _OBJECT_H_				//このマクロ定義がされていなかったら
#define _OBJECT_H_				//2重インクルード防止のマクロを定義

#include "main.h"

//***************************************************
//前方宣言
//***************************************************
class CObject2D;
class CEnemy3D;
class CPlayer3D;
class CChain;
class CMeshWall;
class CObjectX;

//***************************************************
// 定数定義
//***************************************************
namespace object
{
	static const int MAX_OBJECT = 5000;
	static const int NUM_PRIORITY = 8;
}

//***************************************************
//オブジェクトクラス
//***************************************************
class CObject
{
public:	// 構造体・列挙型用のpublic
	// オブジェクトの種類
	typedef enum
	{
		TYPE_NONE,			// 無し
		TYPE_EFFECT,		// エフェクト
		TYPE_PARTICLE,		// パーティクル
		TYPE_OBJECT3D,		// オブジェクト3D
		TYPE_BILLBOARD,		// ビルボード
		TYPE_OBJECTX,		// オブジェクトX(Xファイルから読み込んだモデル)
		TYPE_MAX
	} TYPE;

public:	//メンバ関数用のpublic

	CObject(int nPriority = 3);	// コンストラクタ デフォルト引数
	virtual ~CObject();	// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) = 0;	// 初期化
	virtual void Uninit(void) = 0;	// 終了
	virtual void Update(void) = 0;	// 更新
	virtual void Draw(void) {};	// 描画

	static void ReleaseAll(void);	// 全オブジェクトの破棄
	static void UpdateAll(void);	// 全オブジェクトの更新
	static void DrawAll(void);		// 全オブジェクトの描画

	// 設定用メンバ関数
	virtual void SetPosition(D3DXVECTOR3 pos) {};	// 位置の設定
	virtual void SetRotation(D3DXVECTOR3 rot) {};
	void SetType(TYPE type) { m_type = type; }		// 種類の設定

	// 取得用メンバ関数
	static CObject* GetTop(const int nPriority) { return m_apTop[nPriority]; }
	static CObject* GetEnd(const int nPriority) { return m_apEnd[nPriority]; }
	CObject* GetNext(void) { return m_pNext; }

	virtual D3DXVECTOR3 GetPosition(void) const { return {}; }	// 位置の取得
	virtual D3DXVECTOR3 GetRotation(void) const { return {}; }	// 向きの取得
	virtual D3DXVECTOR3 GetMovement(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	// 移動量の取得
	virtual float GetRadius(void) { return 0.0f; }

	static int GetNumAll(void) { return m_nNumAll; }	// オブジェクト総数の取得
	static int GetNumEffectAll(void) { return m_nNumEffectAll; }	// エフェクト総数

	TYPE GetType(void) { return m_type; }	// 種類の取得

protected:
	void Release(void);	// オブジェクト自身の破棄

private:
	void Death(void);

	static CObject* m_apTop[object::NUM_PRIORITY];	// 先頭のオブジェクトへのポインタ
	static CObject* m_apEnd[object::NUM_PRIORITY];	// 最後尾のオブジェクトへのポインタ
	CObject* m_pPrev;	// 前のオブジェクトへのポインタ
	CObject* m_pNext;	// 次のオブジェクトへのポインタ
	int m_nPriority;	// 優先順位の位置
	bool m_bDeath;	// オブジェクトの死亡フラグ
	TYPE m_type;	// オブジェクトの種類

	static int m_nNumAll;	// 総数
	static int m_nNumEffectAll;	// エフェクト総数
};

#endif