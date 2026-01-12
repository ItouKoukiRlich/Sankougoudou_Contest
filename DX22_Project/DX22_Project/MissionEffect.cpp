//==== インクルード部 ====
#include "MissionEffect.h"

namespace nameMissionEffect
{
	//ミッション開始演出関連
	constexpr DXf2	cg_InitPos		= { 1.5f, 0.5f };							//UIの初期位置
	constexpr float cg_InOutFlame	= 10.0f;									//挿入・退場にかかるフレーム
	constexpr float cg_DeltaMoveX	= (0.5f - cg_InitPos.x) / cg_InOutFlame;	//１フレームのX座標の変化量
	constexpr float cg_SLowFlame	= 60.0f;									//何秒間ゆっくりになるか
	constexpr float cg_SlowMove		= -0.003f;									//ゆっくり時の１フレーム変化量
}
using namespace nameMissionEffect;

MissionEffect::MissionEffect()
	:m_Anime(MissionEffect::Anime::eAnimeNone)
	,m_AnimeCount(0)
{
	m_ui.SetUI("Assets/Texture/Mission/Back.png", MissionEffect::eTexStartBack, cg_InitPos, 0.2f);
	m_ui.SetUI("Assets/Texture/Mission/Mission.png", MissionEffect::eTexStart, cg_InitPos, 0.2f);
	m_ui.ChangeScale(MissionEffect::eTexStartBack, 1.24f, 1.0f);
}

MissionEffect::~MissionEffect()
{
}

void MissionEffect::Update()
{
	switch (m_Anime)
	{
	case MissionEffect::Anime::eAnimeStart: UpdateStart(); break;
	case MissionEffect::Anime::eAnimeClear: UpdateClear(); break;
	}
}

void MissionEffect::Draw()
{
	switch (m_Anime)
	{
	case MissionEffect::Anime::eAnimeStart: DrawStart(); break;
	case MissionEffect::Anime::eAnimeClear: DrawClear(); break;
	}
}

void MissionEffect::Start(MissionEffect::Anime anime)
{
	m_Anime			= anime;
	m_AnimeCount	= 0;
}

void MissionEffect::UpdateStart()
{
	if (m_AnimeCount < cg_InOutFlame)
	{
		m_ui.AddPos(MissionEffect::eTexStart, -0.09f, 0.0f);
		m_ui.AddPos(MissionEffect::eTexStartBack, cg_DeltaMoveX, 0.0f);
	}
	else if (m_AnimeCount < cg_InOutFlame + cg_SLowFlame)
	{
		m_ui.AddPos(MissionEffect::eTexStart, cg_SlowMove, 0.0f);
	}
	else if (m_AnimeCount < cg_InOutFlame + cg_InOutFlame + cg_SLowFlame)
	{
		m_ui.AddPos(MissionEffect::eTexStart, -0.09f, 0.0f);
		m_ui.AddPos(MissionEffect::eTexStartBack, cg_DeltaMoveX, 0.0f);
	}
	else
	{
		m_Anime = MissionEffect::Anime::eAnimeNone;	//アニメーション終了
		m_ui.ChangePos(MissionEffect::TexTag::eTexStart,	 cg_InitPos.x, cg_InitPos.y);	//初期化
		m_ui.ChangePos(MissionEffect::TexTag::eTexStartBack, cg_InitPos.x, cg_InitPos.y);	//初期化
	}

	m_AnimeCount++;
}

void MissionEffect::UpdateClear()
{
}

void MissionEffect::DrawStart()
{
	m_ui.Draw();
}

void MissionEffect::DrawClear()
{
}