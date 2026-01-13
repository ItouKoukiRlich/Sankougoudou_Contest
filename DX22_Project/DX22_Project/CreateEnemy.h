#ifndef ___CREATEENEMY_H___
#define ___CREATEENEMY_H___

//==== インクルード部 ====
#include"Enemy.h"

class CreateEnemy : public Enemy
{
public:
	CreateEnemy();
	~CreateEnemy() override;
	void Update() override;
	void Draw() override;

private:

};

#endif