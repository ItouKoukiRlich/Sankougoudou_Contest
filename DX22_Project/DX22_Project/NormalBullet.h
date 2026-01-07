#ifndef ___NORMALBULLET_H___
#define ___NORMALBULLET_H___

//==== インクルード部 ====
#include"Bullet.h"
#include"DebugBlock.h"

class Player;

class NormalBullet : public Bullet
{
public:
	NormalBullet(Player* pPlayer);
	~NormalBullet();
	void Update() override;
	void Draw() override;
	void CreateBullet() override;

private:
	static Player* m_pPlayer;	//プレイヤー
	

#ifdef _DEBUG
	DebugBlock m_block;

#endif
};

#endif