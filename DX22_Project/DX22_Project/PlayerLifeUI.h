#ifndef ___PLAYERLIFEUI_H___
#define ___PLAYERLIFEUI_H___

//==== インクルード部 ====
#include"Player.h"
#include"UI.h"

class PlayerLifeUI
{
public:
	PlayerLifeUI(Player& player);
	~PlayerLifeUI();
	void Update();
	void Draw();

private:
	enum TexTag
	{
		eBase,
		eHP,
		eHPprev,
		eHyaku,
		eZyuu,
		eIti,
		eMaxTag
	};

private:
	Player& m_Player;
	UI		m_ui;
	int		m_nLifePrev;	//1フレーム前のHP
	int		m_nOldLife;		//過去のHP(HP変動前)
	unsigned int		m_nFlameCount;	//フレームのカウント
};

#endif