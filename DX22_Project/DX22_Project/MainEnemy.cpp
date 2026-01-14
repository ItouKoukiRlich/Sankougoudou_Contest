//==== インクルード部 ====
#include "MainEnemy.h"

bool	MainEnemy::m_bMission	= false;
Model*  MainEnemy::m_pModel		= nullptr;

namespace nameMainEnemy
{
	constexpr int	cg_MaxHP = 10;
	constexpr float cg_DrawHpPos = 4.0f;	//位置からどのくらい離れているか
	constexpr float cg_DrawIconPos = 6.0f;	//位置からどのくらい離れているか
	constexpr float cg_Size = 3.0f;
}
using namespace nameMainEnemy;

MainEnemy::MainEnemy()
{

}

MainEnemy::~MainEnemy()
{

}

void MainEnemy::Update()
{

}

void MainEnemy::Draw()
{

}