#ifndef ___GAMEUI_H___
#define ___GAMEUI_H___

//==== インクルード部 ====
#include"PlayerLifeUI.h"
#include"Player.h"
#include"WarningUI.h"
#include"MissionEffect.h"

class GameUI
{
public:
	GameUI();
	~GameUI();
	void Update();
	void Draw();

	/// <summary>
	/// プレイヤーのライフのUIを作成
	/// </summary>
	void CreatePlayerLife(Player& player);

	/// <summary>
	/// 警告のUIを作成
	/// </summary>
	/// <param name="player"></param>
	void CreateWarningUI(Player& player);


	/// <summary>
	/// ミッションエフェクトの再生開始
	/// </summary>
	/// <param name="anime"></param>
	void PlayMissionEffect(MissionEffect::Anime anime);

private:
	PlayerLifeUI*	m_pPlayerLife;		//プレイヤーのライフUI
	//スキルのUI
	WarningUI*		m_pWarningUI;		//警告のUI
	MissionEffect	m_MissionEffect;	//ミッションのエフェクト
};

#endif