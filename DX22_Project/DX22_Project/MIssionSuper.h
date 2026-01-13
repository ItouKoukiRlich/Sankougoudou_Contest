#ifndef ___MISSIONSUPER_H___
#define ___MISSIONSUPER_H___

//==== インクルード部 ====
#include"Mission.h"

class MissionSuper : public Mission
{
public:
	MissionSuper();
	~MissionSuper() override;
	void Update() override;
	void Draw() override;
};

#endif