//==== インクルード部 ====
#include "CreateEnemy.h"

bool	CreateEnemy::m_bMission = false;
Model*  CreateEnemy::m_pModel	= nullptr;

namespace nameCreateEnemy
{
	constexpr int	cg_MaxHP		= 10;
	constexpr float cg_DrawHpPos	= 4.0f;	//位置からどのくらい離れているか
	constexpr float cg_DrawIconPos	= 6.0f;	//位置からどのくらい離れているか
	constexpr float cg_Size			= 3.0f;
}
using namespace nameCreateEnemy;

CreateEnemy::CreateEnemy()
{
	//---- ライフ ----
	m_nLife = cg_MaxHP;

	//---- 当たり判定 ----
	m_Collision.radius = cg_Size * 0.5f;
}

CreateEnemy::~CreateEnemy()
{
}

void CreateEnemy::Update()
{
}

void CreateEnemy::Draw()
{
}

void CreateEnemy::SetMissionFlag(bool flag)
{
	m_bMission = flag;
}