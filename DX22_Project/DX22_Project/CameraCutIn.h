#ifndef ___CUTINCAMERA_H___
#define ___CUTINCAMERA_H___

//==== インクルード部 ====
#include"Camera.h"

class CameraCutIn : public Camera
{
public:
	CameraCutIn();
	~CameraCutIn() override;
	void Update() override;

private:

};

#endif