#ifndef ___MISSION_H___
#define ___MISSION_H___

//==== インクルード部 ====
#include"UI.h"

class Mission
{
public:
	Mission();
	virtual ~Mission();
	virtual void Update();
	virtual void Draw();

	/// <summary>
	/// ミッション開始
	/// </summary>
	void MissionStart();

private:
	UI	m_ui;

protected:
	bool m_bActive;		//ミッションがアクティブになっているか(アクティブならtrue)
	//目標討伐数
	//今の討伐数
};

#endif