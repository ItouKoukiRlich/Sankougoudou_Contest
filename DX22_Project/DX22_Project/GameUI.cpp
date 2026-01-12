//==== インクルード部 ====
#include "GameUI.h"

GameUI::GameUI()
	:m_pPlayerLife(nullptr)
	,m_pWarningUI(nullptr)
{

}

GameUI::~GameUI()
{
	SAFE_DELETE(m_pWarningUI);
	SAFE_DELETE(m_pPlayerLife);
}

void GameUI::Update()
{
	if (m_pPlayerLife) m_pPlayerLife->Update();
	if (m_pWarningUI) m_pWarningUI->Update();
	m_MissionEffect.Update();
}

void GameUI::Draw()
{
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);

	if (m_pPlayerLife) m_pPlayerLife->Draw();
	if (m_pWarningUI) m_pWarningUI->Draw();
	m_MissionEffect.Draw();

	SetRenderTargets(1, &pRTV, pDSV);
}

void GameUI::CreatePlayerLife(Player& player)
{
	m_pPlayerLife = new PlayerLifeUI(player);
}

void GameUI::CreateWarningUI(Player& player)
{
	m_pWarningUI = new WarningUI(player);
}

void GameUI::PlayMissionEffect(MissionEffect::Anime anime)
{
	m_MissionEffect.Start(anime);
}
