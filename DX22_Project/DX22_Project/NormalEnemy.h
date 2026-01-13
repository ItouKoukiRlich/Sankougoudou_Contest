#ifndef ___NORMALENEMY_H___
#define ___NORMALENEMY_H___

//==== インクルード部 ====
#include"Enemy.h"

class NormalEnemy : public Enemy
{
public:
	enum NormalEnemyType
	{
		e21,
		e12,
		e21tate,
		e12tate,
	};

public:
	NormalEnemy();
	~NormalEnemy() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// ミッション対象のフラグを設定
	/// </summary>
	/// <param name="flag">：ミッション対象ならtrue</param>
	static void SetMissionFlag(bool flag);

	void CreateEnemyNormal(DXf3 pos, int type, DXf3 move = { 5.0f, 5.0f, 5.0f }) override;

private:
	static bool m_bMission;
	DXf3		m_CenterPos;
	DXf3		m_Move;
	float		m_Degree;
	float		m_Degree2;
	NormalEnemyType m_neType;
};

#endif