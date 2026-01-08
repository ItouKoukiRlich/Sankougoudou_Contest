#ifndef ___TURRETENEMY_H___
#define ___TURRETENEMY_H___

//==== インクルード部 ====
#include"Enemy.h"
#include"Player.h"

//==== 定数・マクロ定義 ====
namespace nameTarretEnemy
{
	constexpr int	cg_nShotCountMax = 600;		//何フレーム置きに弾を発射するか
	constexpr float cg_ShotLength = 30.0f;	//射程距離
}

class TurretEnemy : public Enemy
{
public:
	TurretEnemy();
	~TurretEnemy() override;
	void Update() override;
	void Draw() override;

	static void SetPlayer(Player* player);

private:
	/// <summary>
	/// 発射条件をみたしているか確認
	/// </summary>
	/// <returns>：発射条件を満たすならならtrue</returns>
	bool CheckShot();

private:
	static Player* m_pPlayer;

private:
	unsigned int m_nCount;	//カウント
};

#endif