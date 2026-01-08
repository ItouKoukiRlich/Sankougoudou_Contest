#ifndef ___WARNING_H___
#define ___WARNING_H___

//==== インクルード部 ====
#include"UI.h"
#include"Player.h"

class WarningUI
{
public:
	WarningUI(Player& player);
	~WarningUI();
	void Update();
	void Draw();

private:
	enum UI_No
	{
		eBack,
		eFont,
		Max
	};

	/// <summary>
	/// 警告を出すか確認
	/// </summary>
	/// <returns>出すならtrue</returns>
	bool CheckWarning();

private:
	UI m_ui;
	Player& m_Player;
	int m_Angle;		//sin波で透明度を変更
};

#endif