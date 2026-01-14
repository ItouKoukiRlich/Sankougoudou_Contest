//==== インクルード部 ====
#include "SuperEnemy.h"
#include"ShaderList.h"
#include"Player.h"

bool	SuperEnemy::m_bMission	= false;
Model*  SuperEnemy::m_pModel	= nullptr;

namespace nameSuperEnemy
{
	constexpr int	cg_MaxHP = 6;
	constexpr float cg_DrawHpPos = 4.0f;	//位置からどのくらい離れているか
	constexpr float cg_DrawIconPos = 6.0f;	//位置からどのくらい離れているか
	constexpr float cg_Size = 3.0f;
}
using namespace nameSuperEnemy;

SuperEnemy::SuperEnemy()
	:m_Step(e1)
	,m_fDistance(0.0f)
	,m_fAngle(0.0f)
{
	//---- モデル確保 ----
	if (!m_pModel)
	{
		m_pModel = new Model;
		if (!m_pModel->Load("Assets/Model/Enemy/Super.fbx", cg_Size, Model::Flip::XFlip))
		{
			MessageBox(NULL, "TurretEnemy_Model_error", "error", MB_OK);
		}
	}

	//---- ライフ ----
	m_nLife = m_nMaxLife = cg_MaxHP;

	//---- 当たり判定 ----
	m_Collision.radius = (cg_Size + 2.0f) * 0.5f;
}

SuperEnemy::~SuperEnemy()
{
	SAFE_DELETE(m_pBullet);
	SAFE_DELETE(m_pModel);
}

void SuperEnemy::Update()
{
	//==== 変数宣言 ====
	DirectX::XMVECTOR vecPos;
	DirectX::XMVECTOR vecPlayerPos;
	DirectX::XMVECTOR Length;
	float Distance = 0.0f;

	//==== プレイヤー位置を入手 ====
	DXf3 PlayerPos = m_pPlayer->GetPos();

	if (!m_bActive) return;
	switch (m_Step)
	{
	case e1:

		//---- 調整後に距離を計算する ----
		vecPos			= DirectX::XMLoadFloat3(&m_Pos);
		vecPlayerPos	= DirectX::XMLoadFloat3(&PlayerPos);
		Length			= DirectX::XMVectorSubtract(vecPlayerPos, vecPos);
		DirectX::XMStoreFloat(&Distance, DirectX::XMVector3Length(Length));
		if (Distance <= 30.0f)
			m_Step = e2;

		break;

	case e2:

		//---- 位置を調整する ----
		if (m_Pos.y < PlayerPos.y) m_Pos.y += 0.15f;
		else m_Pos.y -= 0.08f;
		if (m_Pos.x < PlayerPos.x) m_Pos.x += 0.1f;
		else m_Pos.x -= 0.06f;
		if (m_Pos.z < PlayerPos.z) m_Pos.z += 0.1f;
		else m_Pos.z -= 0.06f;

		//---- 調整後に距離を計算する ----
		vecPos		= DirectX::XMLoadFloat3(&m_Pos);
		vecPlayerPos	= DirectX::XMLoadFloat3(&PlayerPos);
		Length		= DirectX::XMVectorSubtract(vecPlayerPos, vecPos);
		DirectX::XMStoreFloat(&Distance, DirectX::XMVector3Length(Length));
		if (Distance <= 10.0f)
		{
			m_Step = e3;
			m_fDistance = Distance;	//距離を保存
			float x = m_Pos.x - PlayerPos.x;
			float z = m_Pos.z - PlayerPos.z;
			
			m_fAngle = atan2f(z, x);;//角度を保存
		}
		break;

	case e3:
		m_fAngle += 0.1f;
		m_Pos.x = PlayerPos.x + m_fDistance * cosf(m_fAngle);
		m_Pos.z = PlayerPos.z + m_fDistance * sinf(m_fAngle);
		m_fDistance -= 0.01f;
		break;
	}

	m_Collision.center = m_Pos;
}

void SuperEnemy::Draw()
{
	if (!m_bActive) return;
	if (m_bMission)
	{
		m_Icon.SetPos({ m_Pos.x, m_Pos.y + cg_DrawIconPos, m_Pos.z });
		m_Icon.Draw();
	}
	DrawSuper();
	m_HPui.Draw(cg_MaxHP, m_nLife, { m_Pos.x, m_Pos.y + cg_DrawHpPos, m_Pos.z });
}

void SuperEnemy::SetMissionFlag(bool flag)
{
	m_bMission = flag;
}

void SuperEnemy::DrawSuper()
{
	//==== 頂点シェーダーに渡す変換行列の変数を宣言 ====
	DirectX::XMFLOAT4X4 fWVP[3] = {};
	fWVP[1] = m_pCamera->GetViewMatrix();
	fWVP[2] = m_pCamera->GetProjectionMatrix();

	//==== ワールド行列の作成 ====
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX Translation = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(m_Angle.x);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_Angle.y);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_Angle.z);
	world = Scale * Rx * Ry * Rz * Translation;
	DirectX::XMStoreFloat4x4(&fWVP[0], DirectX::XMMatrixTranspose(world));

	//シェーダーへの変換行列を設定
	ShaderList::SetWVP(fWVP);

	//モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	if (m_Step == e1) m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_UNLIT));
	else
	{
		DXf3 player = m_pPlayer->GetPos();
		m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));
		ShaderList::SetLight({ 1.0f, 1.0f, 1.0f, 1.0f }, 
			//{player.x - m_Pos.x, player.y - m_Pos.y, player.z - m_Pos.z });
			{ m_Pos.x + player.x, m_Pos.y - player.y, m_Pos.z - player.z});
	}
	// マテリアル別にメッシュを表示 
	for (unsigned int i = 0; i < m_pModel->GetMeshNum(); ++i)
	{
		// モデルのメッシュを取得 
		const Model::Mesh mesh = *m_pModel->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得 
		Model::Material material = *m_pModel->GetMaterial(mesh.materialID);

		switch (m_Step)
		{
		case e2:
			material.ambient.x = 0.3f;
			material.ambient.y = 0.0f;
			material.ambient.z = 0.0f;
			break;

		case e3:
			material.ambient.x = 1.0f;
			material.ambient.y = 0.0f;
			material.ambient.z = 0.0f;
			break;
		}

		// シェーダーへマテリアルを設定 
		ShaderList::SetMaterial(material);

		// モデルの描画 
		m_pModel->Draw(i);
	}
}

void SuperEnemy::CreateEnemy(DXf3 pos)
{
	m_bActive = true;
	m_Pos = pos;
	m_Collision.center = pos;
	m_nLife = m_nMaxLife;
	m_Step = e1;
}