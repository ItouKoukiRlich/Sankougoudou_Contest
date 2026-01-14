//==== インクルード部 ====
#include "MIssionNormal.h"

MIssionNormal::MIssionNormal()
{
	m_uvPos = { 0.0f, 0.4f };
	m_nCurrent = 0;
	m_nMax = 8;
}

MIssionNormal::~MIssionNormal()
{
}

void MIssionNormal::Update()
{
}

void MIssionNormal::Draw()
{
	Mission::Draw();
}
