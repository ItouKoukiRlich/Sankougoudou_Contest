#ifndef ___TITLEUI_H___
#define ___TITLEUI_H___

//==== インクルード部 ====
#include"UI.h"

class TitleUI
{
public:
	TitleUI();
	~TitleUI();
	void Update();
	void Draw();

private:
	enum TextureNumber
	{
		eBack,
		eTitle,
		eGear1,
		eGear2,
		eStart,
		eStartUderLine,
		TexMax
	};

private:
	void AnimeProcess();

private:
	UI		m_ui;
	float	m_Degree;				//UIの点滅時の透明度指定に使用
	float	m_GearDegree;			//歯車の回転に使用
	float	m_BackU;				//背景テクスチャのu値
	int		m_nAnimeFlameCount;		//アニメーションのフレームカウント
};

#endif