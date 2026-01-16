//==== インクルード部 ====
#include "ChangeScene.h"

bool ChangeScene::m_bFade = false;
ChangeScene::phase ChangeScene::m_phase = ChangeScene::phase::None;

ChangeScene::ChangeScene()
	:m_alpha(0.0f)
{
	
}

ChangeScene::~ChangeScene()
{

}

void ChangeScene::Update()
{
	if (!m_bFade) return;

	switch (m_phase)
	{
	case FadeIn:
		//フェードイン
		m_alpha += 0.01f;
		if (m_alpha >= 1.0f)
		{
			m_alpha = 1.0f;
			m_phase = AllBlack;
		}
		m_ui.ChangeColor(1, 1.0f, 1.0f, 1.0f, m_alpha);
		break;

	case FadeOut:
		//フェードアウト
		m_alpha -= 0.01f;
		if (m_alpha <= 0.0f)
		{
			m_alpha = 0.0f;
			m_phase = None;
			m_bFade = false;
		}
		m_ui.ChangeColor(1, 1.0f, 1.0f, 1.0f, m_alpha);
		break;
	}
	
}

void ChangeScene::Draw()
{
	if (!m_bFade) return;
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);
	m_ui.Draw();
	SetRenderTargets(1, &pRTV, pDSV);
}

void ChangeScene::SetingUI()
{
	m_ui.SetUI("Assets/Texture/Fade.png", 1, { 0.5f, 0.5f }, 2.5f);
	m_ui.ChangeColor(1, 1.0f, 1.0f, 1.0f, 0.0f);
}

bool ChangeScene::CheckFade()
{
	return m_bFade;
}

void ChangeScene::StartFade()
{
	m_bFade = true;
	m_phase = FadeIn;
}

ChangeScene::phase ChangeScene::GetPhase()
{
	return m_phase;
}

void ChangeScene::SetPhase(phase pha)
{
	m_phase = pha;
}