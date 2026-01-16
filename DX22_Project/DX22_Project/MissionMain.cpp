#include "MissionMain.h"

MissionMain::MissionMain()
{
	m_uvPos = { 0.0f, 0.8f };
	m_nCurrent = 0;
	m_nMax = 3;
}

MissionMain::~MissionMain()
{
}

void MissionMain::Update()
{
}

void MissionMain::Draw()
{
	Mission::Draw();
}
