#ifndef ___MAINENEMY_H___
#define ___MAINENEMY_H___

//==== インクルード部 ====
#include"Enemy.h"

class MainEnemy : public Enemy
{
public:
	MainEnemy();
	~MainEnemy() override;
	void Update() override;
	void Draw() override;

private:

};

#endif