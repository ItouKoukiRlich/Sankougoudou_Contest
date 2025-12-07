#ifndef ___CAMERADEBUG_H___
#define ___CAMERADEBUG_H___

//==== インクルード部 ====
#include"Camera.h"
#include"Input.h"

//==== クラス ====
class CameraDebug : public Camera
{
public:
	CameraDebug();
	~CameraDebug();
	void Update() final;

private:
	const float cm_CameraMove = 0.1f;		//カメラの移動量

private:
	float m_radXZ;		//カメラの横回転
	float m_radY;		//カメラ縦回転
	float m_radius;		//カメラと注意点の距離1
};


#endif