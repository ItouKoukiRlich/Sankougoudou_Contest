#ifndef ___MISSIONNORMAL_H___
#define ___MISSIONNORMAL_H___

//==== インクルード部 ====
#include"Mission.h"

class MIssionNormal : public Mission
{
public:
	MIssionNormal();
	~MIssionNormal() override;
	void Update() override;
	void Draw() override;
};

#endif