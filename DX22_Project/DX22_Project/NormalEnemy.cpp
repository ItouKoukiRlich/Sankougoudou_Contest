//==== インクルード部 ====
#include"NormalEnemy.h"
#include"MathStruct.h"

bool NormalEnemy::m_bMission = false;

//==== 定数・マクロ定義 ====
namespace nameNormalEnemy
{
	constexpr int	cg_MaxHP		= 1;
	constexpr float cg_DrawHpPos	= 2.0f;	//位置からどのくらい離れているか
	constexpr float cg_DrawIconPos	= 4.0f;	//位置からどのくらい離れているか
	constexpr float cg_Size			= 1.0f;
	constexpr float cg_Degree		= 2.0f;
	constexpr float cg_Degree2		= 4.0f;
}
using namespace nameNormalEnemy;

NormalEnemy::NormalEnemy()
	:m_CenterPos({ 0.0f, 0.0f, 15.0f })
	,m_Move({ 5.0f, 5.0f, 5.0f })
	,m_Degree(0.0f)
	,m_Degree2(0.0f)
	,m_neType(NormalEnemyType::e12)
{
	//---- モデル確保 ----
	m_pModel = new Model;
	if (!m_pModel->Load("Assets/Model/Enemy/Normal.fbx", cg_Size, Model::Flip::XFlip))
	{
		MessageBox(NULL, "TurretEnemy_Model_error", "error", MB_OK);
	}

	//---- ライフ ----
	m_nLife = cg_MaxHP;

	//---- 当たり判定 ----
	m_Collision.radius = cg_Size * 0.5f;
}

NormalEnemy::~NormalEnemy()
{
	SAFE_DELETE(m_pBullet);
	SAFE_DELETE(m_pModel);
}

void NormalEnemy::Update()
{
	if (!m_bActive) return;

	//角度を求める
	float radian  = sinf(DegreeToRadian(m_Degree ));
	float radian2 = sinf(DegreeToRadian(m_Degree2));

	//位置の更新
	switch (m_neType)
	{
	case NormalEnemyType::e21:
		m_Pos.x = m_CenterPos.x + radian * m_Move.x;
		m_Pos.y = m_CenterPos.y;
		m_Pos.z = m_CenterPos.z + radian2 * m_Move.z;
		break;

	case NormalEnemyType::e12:
		m_Pos.x = m_CenterPos.x + radian2 * m_Move.x;
		m_Pos.y = m_CenterPos.y;
		m_Pos.z = m_CenterPos.z + radian  * m_Move.z;
		break;

	case NormalEnemyType::e21tate:
		m_Pos.x = m_CenterPos.x + radian  * m_Move.x;
		m_Pos.y = m_CenterPos.y + radian2 * m_Move.y;
		m_Pos.z = m_CenterPos.z;
		break;

	case NormalEnemyType::e12tate:
		m_Pos.x = m_CenterPos.x + radian2  * m_Move.x;
		m_Pos.y = m_CenterPos.y + radian  * m_Move.y;
		m_Pos.z = m_CenterPos.z;
		break;
	}

	//角度の値をプラス
	m_Degree += cg_Degree;
	m_Degree2 += cg_Degree2;
	if (m_Degree >= 360.0f) m_Degree = 0.0f;
	if (m_Degree2 >= 360.0f) m_Degree2 = 0.0f;

	//当たり判定の位置を更新
	m_Collision.center = m_Pos;
}

void NormalEnemy::Draw()
{
	if (!m_bActive) return;
	if (m_bMission)
	{
		m_Icon.SetPos({ m_Pos.x, m_Pos.y + cg_DrawIconPos, m_Pos.z });
		m_Icon.Draw();
	}
	Enemy::Draw();
	//m_pBullet->Draw();
	m_HPui.Draw(cg_MaxHP, m_nLife, { m_Pos.x, m_Pos.y + cg_DrawHpPos, m_Pos.z });
}

void NormalEnemy::SetMissionFlag(bool flag)
{
	m_bMission = flag;
}

void NormalEnemy::CreateEnemyNormal(DXf3 pos, int type, DXf3 move)
{
	m_Pos = pos;
	m_CenterPos = pos;
	m_Collision.center = pos;
	m_Move = move;
	m_bActive = true;
	m_nLife = cg_MaxHP;
}
