//==== インクルード部 ====
#include"CameraDebug.h"
#include"Function.h"
#include<math.h>

#define PI (3.141592653589)

CameraDebug::CameraDebug()
	:m_radXZ(180.0f)
	,m_radY(30.0f)
	,m_radius(5.0f)
{
	//初期位置
	m_pos = MakeXMFloat3(0.0f, 2.0f, -2.0f);

	//注視点
	m_look = MakeXMFloat3(0.0f, 0.0f, 0.0f);
}

CameraDebug::~CameraDebug()
{
}

void CameraDebug::Update()
{
	//---- 変数宣言 ----
	DirectX::XMFLOAT3 moveForward, moveRight, moveLeft, moveBackward, moveUp, moveDown;

	//---- 上ベクトルを計算 ----
	DirectX::XMVECTOR vecUp = DirectX::XMLoadFloat3(&m_up);
	vecUp					= DirectX::XMVector3Normalize(vecUp);

	//---- 直進後退ベクトルを計算 ----
	DirectX::XMVECTOR vCameraPos	= DirectX::XMLoadFloat3(&m_pos);	//カメラの位置を計算用に変換
	DirectX::XMVECTOR vLookPos		= DirectX::XMLoadFloat3(&m_look);	//注視点を計算用のデータに変換
	DirectX::XMVECTOR vecForward	= DirectX::XMVectorSubtract(vLookPos, vCameraPos);
	vecForward = DirectX::XMVector3Normalize(vecForward);
	DirectX::XMVECTOR vecBackward = DirectX::XMVectorScale(vecForward, -1.0f);

	//---- 左右ベクトルを計算 ----
	DirectX::XMVECTOR vecRight = {};
	DirectX::XMVECTOR vecLeft	= DirectX::XMVector3Cross(vecForward, vecUp);
	vecLeft						= DirectX::XMVector3Normalize(vecLeft);
	vecRight					= DirectX::XMVectorScale(vecLeft, -1.0f);

	//---- 本当の上下ベクトルを計算 ----
	vecUp = DirectX::XMVector3Cross(vecForward, vecRight);
	DirectX::XMVECTOR vecDown = DirectX::XMVectorScale(vecUp, -1.0f);

	//---- 直進移動 ----
	float foward = 0.0f;
	if (IsKeyPress('W'))
		foward = cm_CameraMove;
	vecForward = DirectX::XMVectorScale(vecForward, foward);
	DirectX::XMStoreFloat3(&moveForward, vecForward);

	//---- 後退移動 ----
	float backward = 0.0f;
	if (IsKeyPress('S'))
		backward = cm_CameraMove;
	vecBackward = DirectX::XMVectorScale(vecBackward, backward);
	DirectX::XMStoreFloat3(&moveBackward, vecBackward);

	//---- 右移動 ----
	float right = 0.0f;
	if (IsKeyPress('D'))
		right = cm_CameraMove;
	vecRight = DirectX::XMVectorScale(vecRight, right);
	DirectX::XMStoreFloat3(&moveRight, vecRight);

	//---- 左移動 ----
	float left = 0.0f;
	if (IsKeyPress('A'))
		left = cm_CameraMove;
	vecLeft = DirectX::XMVectorScale(vecLeft, left);
	DirectX::XMStoreFloat3(&moveLeft, vecLeft);

	//---- 上移動 ----
	float up = 0.0f;
	if (IsKeyPress('E'))
		up = cm_CameraMove;
	vecUp = DirectX::XMVectorScale(vecUp, up);
	DirectX::XMStoreFloat3(&moveUp, vecUp);

	//---- 下移動 ----
	float down = 0.0f;
	if (IsKeyPress('Q'))
		down = cm_CameraMove;
	vecDown = DirectX::XMVectorScale(vecDown, down);
	DirectX::XMStoreFloat3(&moveDown, vecDown);

	m_look.x = m_look.x + moveForward.x + moveRight.x + moveLeft.x + moveBackward.x + moveUp.x + moveDown.x;
	m_look.y = m_look.y + moveForward.y + moveRight.y + moveLeft.y + moveBackward.y + moveUp.y + moveDown.y;
	m_look.z = m_look.z + moveForward.z + moveRight.z + moveLeft.z + moveBackward.z + moveUp.z + moveDown.z;

	//---- カメラの回り込みの移動 ----
	if (IsKeyPress(VK_LEFT)) { m_radXZ += 1.0f;  };
	if (IsKeyPress(VK_RIGHT)) { m_radXZ -= 1.0f;  };
	if (IsKeyPress(VK_UP)) { m_radY += 1.0f; };
	if (IsKeyPress(VK_DOWN)) { m_radY -= 1.0f; };

	//---- カメラと注視点の距離変換 ----
	if (IsKeyPress('R'))
	{
		m_radius -= cm_CameraMove;
		if (m_radius < 0.1f) m_radius = 0.1f;
	}
	if (IsKeyPress('F'))
	{
		m_radius += cm_CameraMove;
	}


	//カメラの位置の計算
	m_pos.x = 
		cosf(DirectX::XMConvertToRadians(m_radY)) * 
		sinf(DirectX::XMConvertToRadians(m_radXZ)) * 
		m_radius + m_look.x;
	m_pos.y = 
		sinf(DirectX::XMConvertToRadians(m_radY)) * 
		m_radius + m_look.y;
	m_pos.z = 
		cosf(DirectX::XMConvertToRadians(m_radY)) * 
		cosf(DirectX::XMConvertToRadians(m_radXZ)) * 
		m_radius + m_look.z;
}