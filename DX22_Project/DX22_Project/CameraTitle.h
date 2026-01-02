#ifndef ___CAMERATITLE_H___
#define ___CAMERATITLE_H___

//==== インクルード部 ====
#include"Camera.h"

class CameraTitle : public Camera
{
public:
	CameraTitle();
	~CameraTitle();
	void Update() override;

private:

};

#endif