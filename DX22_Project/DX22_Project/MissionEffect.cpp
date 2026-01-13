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

	//ミッションクリア演出
	constexpr DXf2	cg_InitPosClear = { 0.5f, -0.5f };
	constexpr float cg_ClearInFlame = (0.5f - -0.5f) / 30.0f;
	constexpr DXf2  cg_ChangeNum	= { (0.1f - 0.001f) / 20.0f, (0.25f - 1.0f) / 20.0f };
	constexpr float cg_Alpha		= 1.0f / 60.0f;
}
using namespace nameMissionEffect;

MissionEffect::MissionEffect()
	:m_Anime(MissionEffect::Anime::eAnimeNone)
	,m_AnimeCount(0)
{
	m_ui.SetUI("Assets/Texture/Mission/Back.png", MissionEffect::eTexStartBack, cg_InitPos, 0.2f);
	m_ui.SetUI("Assets/Texture/Mission/Mission.png", MissionEffect::eTexStart, cg_InitPos, 0.2f);
	m_ui.ChangeScale(MissionEffect::eTexStartBack, 1.24f, 1.0f);

	m_ClearUI.SetUI("Assets/Texture/Mission/Clear.png", MissionEffect::eTexClear, cg_InitPosClear, 0.5f);
	m_ClearUI.ChangeScale(eTexClear, 0.001f, 1.0f);
	//m_ClearUI.ChangeScale(eAnimeClear, 0.1f, 0.25f);
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
	if (m_AnimeCount < 30)
	{
		m_ClearUI.AddPos(eTexClear, 0.0f, cg_ClearInFlame);
	}
	else if (m_AnimeCount < 30 + 20)
	{
		m_ClearUI.AddScale(eTexClear, cg_ChangeNum.x, cg_ChangeNum.y);
	}
	else if (m_AnimeCount < 30 + 20 + 60)
	{
		m_ClearUI.ChangeColor(eTexClear, 1.0f, 1.0f, 1.0f, 1.0f - cg_Alpha * (float)(m_AnimeCount - 50));
	}
	else
	{
		m_Anime = MissionEffect::Anime::eAnimeNone;	//アニメーション終了
		m_ClearUI.ChangePos(eTexClear, cg_InitPosClear.x, cg_InitPosClear.y);
		m_ClearUI.ChangeColor(eTexClear, 1.0f, 1.0f, 1.0f, 1.0f );
		m_ClearUI.ChangeScale(eTexClear, 0.001f, 1.0f);
	}
	m_AnimeCount++;
}

void MissionEffect::DrawStart()
{
	m_ui.Draw();
}

void MissionEffect::DrawClear()
{
	m_ClearUI.Draw();
}