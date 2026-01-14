//==== インクルード部 ====
#include "MissionCreate.h"

MissionCreate::MissionCreate()
{
	m_uvPos = { 0.0f, 0.2f };
	m_nCurrent = 0;
	m_nMax = 3;
}

MissionCreate::~MissionCreate()
{
}

void MissionCreate::Update()
{
}

void MissionCreate::Draw()
{
	Mission::Draw();
}
