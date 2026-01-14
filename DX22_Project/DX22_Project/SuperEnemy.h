#ifndef ___SUPERENEMY_H___
#define ___SUPERENEMY_H___

//==== インクルード部 ====
#include"Enemy.h"

class SuperEnemy : public Enemy
{
public:
	SuperEnemy();
	~SuperEnemy() override;
	void Update() override;
	void Draw() override;
	void CreateEnemy(DXf3 pos) override;

	/// <summary>
	/// ミッション対象のフラグを設定
	/// </summary>
	/// <param name="flag">：ミッション対象ならtrue</param>
	static void SetMissionFlag(bool flag);

private:
	enum Step
	{
		e1,	//浮遊
		e2,	//プレイヤーに近づく
		e3	//プレイヤーへ自爆攻撃
	};

private:
	static Model* m_pModel;
	static bool		m_bMission;

private:

	/// <summary>
	/// 描画処理
	/// </summary>
	void DrawSuper();

private:
	Step	m_Step;
	float	m_fDistance;	//プレイヤーとの距離
	float	m_fAngle;
};

#endif