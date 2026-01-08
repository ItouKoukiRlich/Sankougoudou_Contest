#ifndef ___ENEMY_H___
#define ___ENEMY_H___

//==== インクルード部 ====
#include"GameObject.h"
#include"Bullet.h"
#include"Model.h"
#include"Camera.h"

class Enemy : public GameObject
{
public:
	Enemy();
	virtual~Enemy();
	virtual void Update() = 0;
	virtual void Draw();

	/// <summary>
	/// フィールドに存在しているか確認
	/// </summary>
	/// <returns>存在していたらtrue</returns>
	bool CheckActive() const;
	
	/// <summary>
	/// 敵を生成
	/// </summary>
	/// <param name="pos">：生成位置</param>
	void CreateEnemy(DXf3 pos);

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

	static void SetCamera(Camera* pCamera);

protected:
	static Camera* m_pCamera;

protected:
	int		m_nLife;		//HP
	DXf3	m_Angle;		//回転の角度
	Bullet* m_pBullet;		//弾
	Model*	m_pModel;		//モデル
	bool	m_bActive;		//フィールドに生成しているならtrue
	int		m_nBulletNum;	//弾の数
};

#endif