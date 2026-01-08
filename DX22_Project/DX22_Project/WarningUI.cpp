//==== インクルード部 ====
#include"WarningUI.h"
#include"MathStruct.h"
#include"SceneGame.h"

//==== 定数・マクロ定義 ====
namespace nameWarning
{
	constexpr DXf2	cg_UIpos			= { 0.5f, 0.15f };	//UIの表示位置
	constexpr float cg_DeltaMoveU		= 0.01f;			//1フレームのU座標の移動量
	constexpr int	cg_DeltaAngle		= 1;				//1フレームの角度の変更量
	constexpr float cg_CheckStartPos	= 10.0f;			//フィールドの端にどのくらい近づいたら警告を出すか
}
using namespace nameWarning;
#define WarningPath "Assets/Texture/Warning/"

WarningUI::WarningUI(Player& player)
	:m_Player(player)
	,m_Angle(0)
{
	m_ui.SetUI("Assets/Texture/Warning/back.png", eBack, cg_UIpos, 0.15f);
	m_ui.SetUI("Assets/Texture/Warning/font.png", eFont, cg_UIpos, 0.15f);
}

WarningUI::~WarningUI()
{

}

void WarningUI::Update()
{
	//プレイヤーがフィールドの端に近づいていないか確認
	DXf3 pos = m_Player.GetPos();
	if (CheckWarning())
	{
		m_ui.AddUVpos(eBack, cg_DeltaMoveU, 0.0f);
		m_ui.ChangeColor(eBack, 1.0f, 1.0f, 1.0f, sinf(DegreeToRadian((float)m_Angle)));
		m_ui.ChangeColor(eFont, 1.0f, 1.0f, 1.0f, sinf(DegreeToRadian((float)m_Angle)));
		m_Angle += cg_DeltaAngle;
		if (m_Angle >= 180) m_Angle = 0;
	}
}

void WarningUI::Draw()
{
	if (CheckWarning())
		m_ui.Draw();
}

bool WarningUI::CheckWarning()
{
	//プレイヤーがフィールドの端に近づいていないか確認
	DXf3 pos = m_Player.GetPos();
	if (pos.x >  nameSceneGame::cg_FieldHarfSize.x - cg_CheckStartPos||
		pos.x < -nameSceneGame::cg_FieldHarfSize.x + cg_CheckStartPos||
		pos.y >  nameSceneGame::cg_FieldHarfSize.x - cg_CheckStartPos||
		pos.y < -nameSceneGame::cg_FieldHarfSize.y + cg_CheckStartPos||
		pos.z >  nameSceneGame::cg_FieldHarfSize.z - cg_CheckStartPos||
		pos.z < -nameSceneGame::cg_FieldHarfSize.z + cg_CheckStartPos)
	{
		return true;
	}
	return false;
}
