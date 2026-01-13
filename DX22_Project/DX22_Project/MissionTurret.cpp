//==== インクルード部 ====
#include "MissionTurret.h"

MissionTurret::MissionTurret()
{
	m_uvPos = { 0.0f, 0.0f };
	m_nMax = 3;
}

MissionTurret::~MissionTurret()
{
}

void MissionTurret::Update()
{
}

void MissionTurret::Draw()
{
	Mission::Draw();
}