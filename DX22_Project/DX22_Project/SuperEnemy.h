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

	/// <summary>
	/// ミッション対象のフラグを設定
	/// </summary>
	/// <param name="flag">：ミッション対象ならtrue</param>
	static void SetMissionFlag(bool flag);

private:
	static Model* m_pModel;
	static bool		m_bMission;

private:

};

#endif