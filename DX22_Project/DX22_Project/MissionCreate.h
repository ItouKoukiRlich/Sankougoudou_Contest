#ifndef ___MISSIONCREATE_H___
#define ___MISSIONCREATE_H___

//==== インクルード部 ====
#include"Mission.h"

class MissionCreate : public Mission
{
public:
	MissionCreate();
	~MissionCreate() override;
	void Update() override;
	void Draw() override;
};

#endif