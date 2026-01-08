#ifndef ___NORMALENEMYBULLET_H___

//==== インクルード部 ====
#include"Bullet.h"
#include"Player.h"

//==== 定数・マクロ定義 ====
#define NEB NormalEnemyBullet	//NormalEnemyBullet の省略

class NormalEnemyBullet : public Bullet
{
public:
	NormalEnemyBullet();
	~NormalEnemyBullet() override;
	void Update() override;
	void Draw() override;
	void CreateBullet() override;
	void CreateBullet(DirectX::XMVECTOR vec, DirectX::XMFLOAT3 pos) override;
	static void SetCamera(Player* pPlayer);

private:
	static Player* m_pPlayer;
};

#endif