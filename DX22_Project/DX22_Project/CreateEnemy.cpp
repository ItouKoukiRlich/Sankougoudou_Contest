//==== インクルード部 ====
#include "CreateEnemy.h"

bool	CreateEnemy::m_bMission = false;
Model*  CreateEnemy::m_pModel	= nullptr;

namespace nameCreateEnemy
{
	constexpr int	cg_MaxHP		= 10;
	constexpr float cg_DrawHpPos	= 4.0f;	//位置からどのくらい離れているか
	constexpr float cg_DrawIconPos	= 6.0f;	//位置からどのくらい離れているか
	constexpr float cg_Size			= 5.0f;
}
using namespace nameCreateEnemy;

CreateEnemy::CreateEnemy()
{
	//---- モデル確保 ----
	if (!m_pModel)
	{
		m_pModel = new Model;
		if (!m_pModel->Load("Assets/Model/Enemy/Create.fbx", cg_Size, Model::Flip::XFlip))
		{
			MessageBox(NULL, "TurretEnemy_Model_error", "error", MB_OK);
		}
	}

	//---- ライフ ----
	m_nLife = m_nMaxLife = cg_MaxHP;

	//---- 当たり判定 ----
	m_Collision.radius = cg_Size * 0.5f;
}

CreateEnemy::~CreateEnemy()
{
	SAFE_DELETE(m_pModel);
}

void CreateEnemy::Update()
{
	if (!m_bActive) return;
	
}

void CreateEnemy::Draw()
{
	if (!m_bActive) return;
	if (m_bMission)
	{
		m_Icon.SetPos({ m_Pos.x, m_Pos.y + cg_DrawIconPos, m_Pos.z });
		m_Icon.Draw();
	}
	Enemy::ModelDraw(m_pModel);
	m_HPui.Draw(cg_MaxHP, m_nLife, { m_Pos.x, m_Pos.y + cg_DrawHpPos, m_Pos.z });
}

void CreateEnemy::SetMissionFlag(bool flag)
{
	m_bMission = flag;
}