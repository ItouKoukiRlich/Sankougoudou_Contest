//==== インクルード部 ====
#include"CameraGame.h"
#include"Input.h"
#include"Player.h"
#include<math.h>

CameraGame::CameraGame()
	:m_radXZ(180.0f)
	,m_radY(30.0f)
	,m_radius(5.0f)
	,m_Degree(-45.0f)
	,m_pPlayer(nullptr)
{
	
}

CameraGame::~CameraGame()
{
	
}

void CameraGame::Update()
{
	if (!m_pPlayer) return;	//プレイヤーのアドレスが設定されていなければ処理しない

	DirectX::XMFLOAT3 PlayerPos = m_pPlayer->GetPos();	//プレイヤー位置を入手

	//---- 注視点の位置 ----
	float cosA = cosf(DirectX::XMConvertToRadians(m_Degree));
	float sinA = sinf(DirectX::XMConvertToRadians(m_Degree));

	m_look.x = PlayerPos.x + cm_PlayerAndCameraRadius * cosA + cm_PlayerAndCameraRadius * sinA;
	m_look.y = PlayerPos.y;
	m_look.z = PlayerPos.z + -cm_PlayerAndCameraRadius * sinA + cm_PlayerAndCameraRadius * cosA;

	//---- カメラの位置 ----
	float cosB = cosf(DirectX::XMConvertToRadians(m_Degree + 180.0f));
	float sinB = sinf(DirectX::XMConvertToRadians(m_Degree + 180.0f));
	m_pos.x = PlayerPos.x +  cm_PlayerAndCameraRadius * cosB + cm_PlayerAndCameraRadius * sinB;
	m_pos.z = PlayerPos.z + -cm_PlayerAndCameraRadius * sinB + cm_PlayerAndCameraRadius * cosB;

	//---- カメラの回り込みの移動 ----
	if (IsKeyPress(VK_LEFT))	{ m_Degree--; };
	if (IsKeyPress(VK_RIGHT))	{ m_Degree++; };
	if (IsKeyPress(VK_UP))		{ m_radY  += 1.0f; };
	if (IsKeyPress(VK_DOWN))	{ m_radY  -= 1.0f; };


	//カメラの位置の計算
	/*m_pos.x =
		cosf(DirectX::XMConvertToRadians(m_radY)) *
		sinf(DirectX::XMConvertToRadians(m_radXZ)) *
		m_radius + m_look.x;*/
	m_pos.y =
		sinf(DirectX::XMConvertToRadians(m_radY)) *
		m_radius + m_look.y;
	/*m_pos.z =
		cosf(DirectX::XMConvertToRadians(m_radY)) *
		cosf(DirectX::XMConvertToRadians(m_radXZ)) *
		m_radius + m_look.z;*/
}

void CameraGame::Draw()
{
	m_DebugBlock.Draw(m_look, this);
}

void CameraGame::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}
