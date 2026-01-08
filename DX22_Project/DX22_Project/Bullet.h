#ifndef ___BULLET_H___
#define ___BULLET_H___

//==== インクルード部 ====
#include"GameObject.h"
#include"DirectXMath.h"
#include"Effect.h"
#include"Collision.h"
#include"DebugBlock.h"

class Bullet : public GameObject
{
public:
	Bullet();
	virtual ~Bullet();
	virtual void Update() = 0;
	virtual void Draw();
	virtual void CreateBullet();
	/// <summary>
	/// 弾をベクトルと発射地点をしていして作成する場合
	/// </summary>
	/// <param name="vec">：作成する弾の進むベクトル</param>
	/// <param name="pos">：弾の発射位置</param>
	virtual void CreateBullet(DirectX::XMVECTOR vec, DirectX::XMFLOAT3 pos);

	/// <summary>
	/// 発射しているか
	/// </summary>
	/// <returns>発射中ならtrue</returns>
	bool GetActive() const;

	/// <summary>
	/// 当たり判定を取得
	/// </summary>
	/// <returns>：当たり判定</returns>
	Collision::Sphere GetCollision() const;

	/// <summary>
	/// 威力を入手
	/// </summary>
	/// <returns>：威力</returns>
	int GetDamage() const;

	/// <summary>
	/// 弾を消す
	/// </summary>
	void Stop();

	/// <summary>
	/// 当たり判定を簡易表示
	/// </summary>
	void DrawCollision() const;

protected:
	int					m_nDamage;		//威力
	DirectX::XMVECTOR	m_Vec;			//進むベクトル
	bool				m_bActive;		//発射していたらtrue
	int					m_nActiveCount;	//発射してから何フレーム経つかのカウント
	Effekseer::Handle	m_handle;		//エフェクトのハンドル
	Collision::Sphere	m_Collision;	//当たり判定
};

#endif