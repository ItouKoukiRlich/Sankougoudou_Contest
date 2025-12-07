//==== インクルード部 ====
#include "Camera.h"
#include "Defines.h"

Camera::Camera()
	:m_pos(2.0f, 2.0f, -2.0f)
	,m_look(0.0f, 0.0f, 0.0f)
	,m_up(0.0f, 1.0f, 0.0f)
	,m_fovy(60.0f)
	,m_aspect(16.0f / 9.0f)
	,m_near(CMETER(30.0f))
	,m_far(METER(1000.0f))
{

}

Camera::~Camera()
{

}

void Camera::Update()
{

}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix(bool transpose)
{
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMMATRIX view;

	DirectX::XMVECTOR pos	= DirectX::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0.0f);
	DirectX::XMVECTOR look	= DirectX::XMVectorSet(m_look.x, m_look.y, m_look.z, 0.0f);
	DirectX::XMVECTOR up	= DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 0.0f);

	view = DirectX::XMMatrixLookAtLH(pos, look, up);
	if (transpose)
	{
		view = DirectX::XMMatrixTranspose(view);	//転置
	}

	DirectX::XMStoreFloat4x4(&mat, view);

	return mat;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix(bool transpose)
{
	DirectX::XMFLOAT4X4	mat;
	DirectX::XMMATRIX	proj;

	proj = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(m_fovy), m_aspect, m_near, m_far);

	if (transpose)
	{
		proj = DirectX::XMMatrixTranspose(proj);	//転置
	}

	DirectX::XMStoreFloat4x4(&mat, proj);

	return mat;
}

DirectX::XMFLOAT3 Camera::GetPos()
{
	return m_pos;
}

DirectX::XMFLOAT3 Camera::GetLook()
{
	return m_look;
}

void Camera::SetLook(DirectX::XMFLOAT3 look)
{
	m_look = look;
}
