#ifndef ___SPEEDBULLET_H___
#define ___SPEEDBULLET_H___

//==== インクルード部 ====
#include"Bullet.h"

class SpeedBullet : public Bullet
{
public:
	SpeedBullet();
	~SpeedBullet() override;
	void Update() override;
	void Draw() override;
	void CreateBullet(DirectX::XMVECTOR vec, DirectX::XMFLOAT3 pos) override;

private:

};

#endif