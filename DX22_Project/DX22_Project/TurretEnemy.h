#ifndef ___TURRETENEMY_H___
#define ___TURRETENEMY_H___

//==== インクルード部 ====
#include"Enemy.h"
#include"Player.h"

//==== 定数・マクロ定義 ====
namespace nameTarretEnemy
{
	constexpr int	cg_nShotCountMax = 180;		//何フレーム置きに弾を発射するか
	constexpr float cg_ShotLength = 60.0f;	//射程距離
}

class TurretEnemy : public Enemy
{
public:
	TurretEnemy();
	~TurretEnemy() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// ミッション対象のフラグを設定
	/// </summary>
	/// <param name="flag">：ミッション対象ならtrue</param>
	static void SetMissionFlag(bool flag);

	static void staticDraw(Camera* pCamera, DXf3 pos);

private:
	/// <summary>
	/// 発射条件をみたしているか確認
	/// </summary>
	/// <returns>：発射条件を満たすならならtrue</returns>
	bool CheckShot();

private:
	static bool		m_bMission;
	static Model*	m_pModel;

private:
	unsigned int m_nCount;	//カウント
};

#endif