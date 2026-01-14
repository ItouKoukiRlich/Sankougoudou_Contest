#ifndef ___ENEMY_H___
#define ___ENEMY_H___

//==== インクルード部 ====
#include"GameObject.h"
#include"Bullet.h"
#include"Model.h"
#include"Camera.h"
#include"EnemyIcon.h"
#include"EnemyHP.h"
#include"Collision.h"
#include"Geometory.h"
#include"Function.h"
class Player;

namespace nameEnemy
{
	constexpr int HitDamage = 30;	//当たった際のダメージ
}

class Enemy : public GameObject
{
public:
	Enemy();
	virtual~Enemy();
	virtual void Update() = 0;
	virtual void Draw();

	void ModelDraw(Model* pModel);

	/// <summary>
	/// フィールドに存在しているか確認
	/// </summary>
	/// <returns>存在していたらtrue</returns>
	bool CheckActive() const;
	
	/// <summary>
	/// 敵を生成
	/// </summary>
	/// <param name="pos">：生成位置</param>
	virtual void CreateEnemy(DXf3 pos);

	/// <summary>
	/// ノーマルの敵を生成
	/// </summary>
	virtual void CreateEnemyNormal(DXf3 pos, int type, DXf3 move = { 5.0f, 5.0f, 5.0f });

	/// <summary>
	/// 敵を消す
	/// </summary>
	void Delete();

	/// <summary>
	///	弾の数を入手
	/// </summary>
	/// <returns></returns>
	int GetBulletNum() const;

	/// <summary>
	/// 弾のアドレスを入手
	/// </summary>
	/// <returns></returns>
	Bullet* GetBullet() const;

	/// <summary>
	/// HPの減算
	/// </summary>
	/// <param name="damage">：引く値</param>
	/// /// <returns>HPが0になればtrue</returns>
	bool MinusHP(int damage);

	/// <summary>
	/// 当たり判定を取得
	/// </summary>
	/// <returns></returns>
	Collision::Sphere GetCollision() const;

	static void SetCamera(Camera* pCamera);
	static void SetPlayer(Player* pPlayer);

protected:
	static Camera* m_pCamera;
	static Player* m_pPlayer;

protected:
	int			m_nMaxLife;		//最大HP
	int			m_nLife;		//HP
	DXf3		m_Angle;		//回転の角度
	Bullet*		m_pBullet;		//弾
	bool		m_bActive;		//フィールドに生成しているならtrue
	int			m_nBulletNum;	//弾の数
	EnemyIcon	m_Icon;			//アイコン
	EnemyHP		m_HPui;			//HPの表示
	Collision::Sphere m_Collision;
};

#endif