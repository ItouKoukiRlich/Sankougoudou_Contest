#include "MIssionSuper.h"

MissionSuper::MissionSuper()
{
	m_uvPos = { 0.0f, 0.6f };
	m_nCurrent = 0;
	m_nMax = 4;
}

MissionSuper::~MissionSuper()
{
}

void MissionSuper::Update()
{
}

void MissionSuper::Draw()
{
	Mission::Draw();
}
