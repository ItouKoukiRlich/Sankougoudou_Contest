#ifndef ___MISSIONTURRET_H___
#define ___MISSIONTURRET_H___

//==== インクルード部 ====
#include"Mission.h"

class MissionTurret : Mission
{
public:
	MissionTurret();
	~MissionTurret() override;
	void Update() override;
	void Draw() override;

private:

};

#endif