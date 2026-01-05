#ifndef ___GAMEUI_H___
#define ___GAMEUI_H___

//==== インクルード部 ====
#include"PlayerLifeUI.h"
#include"Player.h"

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

private:
	PlayerLifeUI* m_pPlayerLife;	//プレイヤーのライフUI
	//スキルのUI
};

#endif