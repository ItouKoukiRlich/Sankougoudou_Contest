//==== インクルード部 ====
#include "Clear.h"
#include"Effect.h"
#include"Main.h"
#include"Input.h"
#include"ChangeScene.h"

namespace nameClear
{
	constexpr float InSpeed		= (0.8f - 1.5f) / 10.0f;
	constexpr float SlowSpeed	= 0.1f / 40.0f;
	constexpr float LastSpeed	= 0.2f / 10.0f;
}
using namespace nameClear;

Clear::Clear()
	:m_phase(Phase::FadeIn)
	,m_nAnimeCount(0)
	,m_pPlayer(nullptr)
	,m_alpha(0.0f)
	,m_Angle(0.0f)
{
	m_ui.SetUI("Assets/Texture/Clear/Clear_up.png",		Tex::ClearUp, { 1.5f, 0.5f }, 0.05f);
	m_ui.SetUI("Assets/Texture/Clear/Clear_down.png",	Tex::ClearDown, { -0.5f, 0.55f }, 0.05f);
	m_ui.ChangeScale(Tex::ClearUp, 1.27f, 1.0f);
	m_ui.ChangeScale(Tex::ClearDown, 1.27f, 1.0f);
	EFK_INS->Load(u"Assets/Effect/Clear.efkefc");

	m_ui.SetUI("Assets/Texture/Clear/Back2.png", Tex::eBack2,{ 0.5f, 0.5f },  0.5f);
	m_ui.SetUI("Assets/Texture/Clear/Back.png",  Tex::eBack, { 0.5f, 0.5f },  0.5f);
	m_ui.SetUI("Assets/Texture/Clear/Font.png",  Tex::eFont, { 0.5f, 0.75f }, 0.05f);
	m_ui.ChangeColor(eBack, 1.0f, 1.0f, 1.0f, 0.0f);
	m_ui.ChangeColor(eBack2, 1.0f, 1.0f, 1.0f, 0.0f);
	m_ui.ChangeColor(eFont, 1.0f, 1.0f, 1.0f, 0.0f);
	m_ui.ChangeScale(eBack, 1.3f, 2.0f);
	m_ui.ChangeScale(eBack2, 1.3f, 2.0f);
}

Clear::~Clear()
{
}

void Clear::Update()
{
	switch (m_phase)
	{
	case Phase::FadeIn:
		if (m_nAnimeCount < 10)
		{
			m_ui.AddPos(Tex::ClearUp,	 InSpeed, 0.0f);
			m_ui.AddPos(Tex::ClearDown, -InSpeed, 0.0f);
		}
		else if (m_nAnimeCount < 40)
		{
			m_ui.AddPos(Tex::ClearUp,  -SlowSpeed, 0.0f);
			m_ui.AddPos(Tex::ClearDown, SlowSpeed, 0.0f);
		}
		else if (m_nAnimeCount < 50)
		{
			m_ui.AddPos(Tex::ClearUp,  -LastSpeed, 0.0f);
			m_ui.AddPos(Tex::ClearDown, LastSpeed, 0.0f);
		}
		else
		{
			//エフェクト再生
			int num = 0;
			DXf3 pos = m_pPlayer->GetPos();
			EFK_INS->Play(u"Assets/Effect/Clear.efkefc", pos, &num);
			m_phase = Phase::Always;
			m_ui.ChangePos(Tex::ClearUp,   0.5f, 0.5f);
			m_ui.ChangePos(Tex::ClearDown, 0.5f, 0.55f);
		}
		break;

	case Phase::Always:
		m_ui.ChangeColor(eBack, 1.0f, 1.0f, 1.0f, m_alpha);
		m_ui.ChangeColor(eBack2, 1.0f, 1.0f, 1.0f, m_alpha);
		m_ui.AddUVpos(eBack2, 0.0f, 0.001f);
		m_alpha += 0.01f;
		if (m_alpha > 1.0f) m_alpha = 1.0f;
		m_Angle += 0.05f;
		if (m_Angle > 180.0f) m_Angle = 0.0f;
		m_ui.ChangeColor(eFont, 1.0f, 0.0f, 0.0f, sinf(m_Angle));
		if (IsKeyTrigger(VK_RETURN))
		{
			if (!ChangeScene::CheckFade())
				proChangeScene(eTitle);
		}
		break;
	}
	m_nAnimeCount++;
}

void Clear::Draw()
{
	switch (m_phase)
	{
	case Phase::FadeIn:
		m_ui.Draw();
		break;

	case Phase::Always:
		m_ui.Draw();
		break;
	}
}

void Clear::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}