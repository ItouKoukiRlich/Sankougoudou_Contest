#ifndef ___CHANGESENE_H___
#define ___CHANGESENE_H___

#include"UI.h"

class ChangeScene
{
public:
	ChangeScene();
	~ChangeScene();
	void Update();
	void Draw();
	void SetingUI();

public:
	enum phase
	{
		FadeIn,
		FadeOut,
		AllBlack,
		None,
	};
	static bool CheckFade();//フェイド中ならtrue
	static void StartFade();
	static phase GetPhase();
	static void SetPhase(phase pha);

private:
	static bool m_bFade;
	UI m_ui;
	float m_alpha;
	static phase m_phase;
};

#endif