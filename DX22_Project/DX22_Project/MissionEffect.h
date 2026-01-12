#ifndef ___MISSIONEFFECT_H___
#define ___MISSIONEFFECT_H___

//==== インクルード部 ====
#include"UI.h"

class MissionEffect
{
public:
	enum Anime
	{
		eAnimeStart,
		eAnimeClear,
		eAnimeNone,
	};

public:
	MissionEffect();
	~MissionEffect();
	void Update();
	void Draw();

	/// <summary>
	/// エフェクトを再生する
	/// </summary>
	/// <param name="anime"></param>
	void Start(MissionEffect::Anime anime);

private:
	enum TexTag
	{
		eTexStart,
		eTexStartBack,
	};

private:
	void UpdateStart();
	void UpdateClear();
	void DrawStart();
	void DrawClear();

private:
	MissionEffect::Anime	m_Anime;
	UI						m_ui;
	int						m_AnimeCount;	//テクスチャアニメーションのカウント
};

#endif