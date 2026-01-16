#ifndef ___GAMEMENU_H___
#define ___GAMEMENU_H___

//==== インクルード部 ====
#include"UI.h"

class GameMenu
{
public:
	GameMenu();
	~GameMenu();
	void Update();
	void Draw();

private:
	UI m_ui;
};

#endif