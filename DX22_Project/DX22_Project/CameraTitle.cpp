//==== インクルード部 ====
#include"CameraTitle.h"
#include"MathStruct.h"

CameraTitle::CameraTitle()
{
	m_look	= MakeDXf3(0.0f, 4.5f, 0.0f);	//注視点
	m_pos	= MakeDXf3(0.0f, 4.5f, -8.0f);	//位置
}

CameraTitle::~CameraTitle()
{
}

void CameraTitle::Update()
{
}