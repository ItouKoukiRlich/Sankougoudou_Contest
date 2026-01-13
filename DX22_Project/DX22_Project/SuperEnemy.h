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

private:

};

#endif