//==== インクルード部 ====
#include "GameUI.h"

GameUI::GameUI()
	:m_pPlayerLife(nullptr)
{

}

GameUI::~GameUI()
{
	SAFE_DELETE(m_pPlayerLife);
}

void GameUI::Update()
{
	if (m_pPlayerLife) m_pPlayerLife->Update();
}

void GameUI::Draw()
{
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);

	if (m_pPlayerLife) m_pPlayerLife->Draw();

	SetRenderTargets(1, &pRTV, pDSV);
}

void GameUI::CreatePlayerLife(Player& player)
{
	m_pPlayerLife = new PlayerLifeUI(player);
}
