#ifndef ___MAINENEMYBULLET_H___
#define ___MAINENEMYBULLET_H___

//==== インクルード部 ====
#include"Bullet.h"

class Player;

class MainEnemyBullet : public Bullet
{
public:
	MainEnemyBullet();
	~MainEnemyBullet();
	void Update();
	void Draw();

	void CreateBullet(DXf3 pos) override;

	static void SetPlayer(Player* pPlayer);

private:
	enum phase
	{
		phase1,
		phase2
	};

private:
	static Player* m_pPlayer;

	phase m_phase;
	int m_nCount;
};

#endif