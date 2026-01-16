//==== インクルード部 ====
#include "Gameover.h"
#include"Input.h"
#include"Main.h"
#include"ChangeScene.h"
namespace nameGameOver
{
	constexpr int cg_nFlashFlame = 20;
}
using namespace nameGameOver;

GameOver::GameOver()
	:m_alpha(0.0f)
	,m_nCount(0)
{
	m_ui.SetUI("Assets/Texture/fade.png",				GameOver::eBack,	{ 0.5f, 0.5f });
	m_ui.SetUI("Assets/Texture/GameOver/GameOver.png",	GameOver::eFont,	{ 0.5f, 0.3f }, 0.1);
	m_ui.SetUI("Assets/Texture/GameOver/Message.png",	GameOver::eMessage,	{ 0.5f, 0.5f }, 0.1f);
	m_ui.ChangeUVsize(GameOver::eMessage, 1.0f, 0.5f);
}

GameOver::~GameOver()
{
}

void GameOver::Update()
{
	if (IsKeyTrigger(VK_RETURN) || IsKeyTrigger(VK_SPACE))
	{
		if (!ChangeScene::CheckFade())
		{
			proChangeScene(eTitle);
		}
	}

	m_ui.ChangeColor(eBack, 1.0f, 1.0f, 1.0f, m_alpha);
	m_ui.ChangeColor(eFont, 1.0f, 1.0f, 1.0f, m_alpha);
	m_alpha += 0.01f;

	if (m_nCount < cg_nFlashFlame) m_ui.ChangeColor(eMessage, 1.0f, 1.0f, 1.0f, m_alpha);
	else
	{
		m_ui.ChangeColor(eMessage, 1.0f, 1.0f, 1.0f, 0.0f);
		if (m_nCount < cg_nFlashFlame * 2) m_nCount = 0;
	}
	m_nCount++;
	if (m_alpha > 1.0f) m_alpha = 1.0f;
}

void GameOver::Draw()
{
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);
	m_ui.Draw();
	SetRenderTargets(1, &pRTV, pDSV);
}

void GameOver::Start(GameOver::Type type)
{
	m_alpha = 0.0f;
	m_nCount = 0;

	m_ui.ChangeColor(eBack, 1.0f, 1.0f, 1.0f, 0.0f);
	m_ui.ChangeColor(eFont, 1.0f, 1.0f, 1.0f, 0.0f);
	m_ui.ChangeColor(eMessage, 1.0f, 1.0f, 1.0f, 0.0f);

	switch (type)
	{
	case GameOver::eOutField:	m_ui.ChangeUVpos(GameOver::eMessage, 0.0f, 0.0f); break;
	case GameOver::eDead:		m_ui.ChangeUVpos(GameOver::eMessage, 0.0f, 0.5f); break;
	}
}
