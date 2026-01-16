//==== インクルード部 ====
#include "GameMenu.h"

GameMenu::GameMenu()
{
	m_ui.SetUI("Assets/Texture/sousaback.png", 1, { 0.5f, 0.5f }, 0.5f);
	m_ui.SetUI("Assets/Texture/sousa.png", -1, { 0.5f, 0.35f }, 0.6f);
	
	m_ui.ChangeScale(-1, 1.0f, 1.1f);
	m_ui.ChangeScale(1, 1.15f, 2.0f);
}

GameMenu::~GameMenu()
{

}

void GameMenu::Update()
{
	
}

void GameMenu::Draw()
{
	m_ui.Draw();
}