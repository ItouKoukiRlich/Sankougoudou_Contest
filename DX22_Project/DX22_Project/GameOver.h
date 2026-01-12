#ifndef ___GAMEOVER_H___
#define ___GAMEOVER_H___

//==== インクルード部 ====
#include"UI.h"

class GameOver
{
public:
	enum Type
	{
		eOutField,
		eDead,
	};

public:
	GameOver();
	~GameOver();
	void Update();
	void Draw();

	void Start(GameOver::Type type);

private:
	enum TexTag
	{
		eBack,
		eFont,
		eMessage,
	};

private:
	UI		m_ui;
	float	m_alpha;	//透明度
	int		m_nCount;	//点滅用のカウント
};

#endif