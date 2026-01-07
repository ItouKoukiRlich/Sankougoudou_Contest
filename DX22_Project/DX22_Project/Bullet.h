#ifndef ___BULLET_H___
#define ___BULLET_H___

//==== インクルード部 ====
#include"GameObject.h"
#include"DirectXMath.h"
#include"Effect.h"

class Bullet : public GameObject
{
public:
	Bullet();
	virtual ~Bullet();
	virtual void Update() = 0;
	virtual void Draw();
	virtual void CreateBullet();
	
	/// <summary>
	/// 発射しているか
	/// </summary>
	/// <returns>発射中ならtrue</returns>
	bool GetActive() const;

protected:
	DirectX::XMVECTOR	m_Vec;			//進むベクトル
	bool				m_bActive;		//発射していたらtrue
	int					m_nActiveCount;	//発射してから何フレーム経つかのカウント
	Effekseer::Handle	m_handle;		//エフェクトのハンドル
};

#endif