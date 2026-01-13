#ifndef ___MISSIONMAIN_H___
#define ___MISSIONMAIN_H___

//==== インクルード部 ====
#include"Mission.h"

class MissionMain : public Mission
{
public:
	MissionMain();
	~MissionMain() override;
	void Update() override;
	void Draw() override;
};

#endif