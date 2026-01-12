#ifndef ___CLEAR_H___
#define ___CLEAR_H___

//==== インクルード部 ====
#include"UI.h"
#include"Player.h"

class Clear
{
public:
	Clear();
	~Clear();
	void Update();
	void Draw();

	void SetPlayer(Player* pPlayer);

private:
	enum Phase
	{
		FadeIn,
		Always,
	};

	enum Tex
	{
		ClearUp,
		ClearDown,
		eBack,
		eBack2,
		eFont,
		eFontBack
	};

private:
	Player*			m_pPlayer;
	int				m_nAnimeCount;
	Clear::Phase	m_phase;
	UI				m_ui;
	float m_alpha;
	float m_Angle;
};

#endif