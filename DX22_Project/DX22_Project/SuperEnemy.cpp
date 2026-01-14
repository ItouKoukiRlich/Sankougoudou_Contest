//==== インクルード部 ====
#include "SuperEnemy.h"

bool	SuperEnemy::m_bMission	= false;
Model*  SuperEnemy::m_pModel	= nullptr;

namespace nameSuperEnemy
{
	constexpr int	cg_MaxHP = 10;
	constexpr float cg_DrawHpPos = 4.0f;	//位置からどのくらい離れているか
	constexpr float cg_DrawIconPos = 6.0f;	//位置からどのくらい離れているか
	constexpr float cg_Size = 3.0f;
}
using namespace nameSuperEnemy;

SuperEnemy::SuperEnemy()
{
}

SuperEnemy::~SuperEnemy()
{
}

void SuperEnemy::Update()
{
}

void SuperEnemy::Draw()
{
}
