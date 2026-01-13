//==== インクルード部 ====
#include"Mission.h"
#include"Sprite.h"

UI Mission::m_ui;
Texture* Mission::m_pTexture = nullptr;
int Mission::m_nActiveCount = 0;

//==== 定数・マクロ定義 ====
#define MISSION_UI_HEIGHT (0.07)
namespace nameMission
{
	constexpr DXf2 cg_UIpos		= { 0.13f, 0.1f };
	constexpr DXf2 CurrentPos	= { 0.21f, 0.1f };
	constexpr DXf2 MaxPos		= { 0.27f, 0.1f };
}
using namespace nameMission;

Mission::Mission()
	:m_bActive(false)
	,m_nCurrent(0)
	,m_nMax(0)
	,m_uvPos({0.0f, 0.0f})
{

}

Mission::~Mission()
{
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void Mission::Update()
{
	
}

void Mission::Draw()
{
	if (m_bActive)
	{
		m_ui.ChangePos(Mission::eMessage, cg_UIpos.x, cg_UIpos.y + m_nActiveCount * MISSION_UI_HEIGHT);
		m_ui.ChangeUVpos(Mission::eMessage, m_uvPos.x, m_uvPos.y);
		m_ui.Draw();
		DrawNumber();
	}
}

void Mission::MissionStart()
{
	m_bActive = true;	//ミッションをアクティブにする
}

void Mission::SetActive(bool bActive)
{
	m_bActive = bActive;
}

void Mission::CountPlus()
{
	m_nCurrent++;
}

bool Mission::CheckActive() const
{
	return m_bActive;
}

bool Mission::CheckClear() const
{
	if (m_nCurrent == m_nMax) return true;
	return false;
}

void Mission::Load()
{
	m_ui.SetUI("Assets/Texture/Mission/Font.png", Mission::TexTag::eMessage, { cg_UIpos.x, cg_UIpos.y }, 0.1f);
	m_ui.SetUI("Assets/Texture/Mission/Tag.png", Mission::TexTag::eTag,		{ 0.145f, 0.04f }, 0.05f);
	m_ui.SetUI("Assets/Texture/MIssionIcon.png", Mission::TexTag::eIcon,	{ 0.04f, 0.04f }, 0.06f);
	m_ui.ChangeScale(Mission::eMessage, 2.0f, 1.0f);
	m_ui.ChangeUVsize(Mission::eMessage, 1.0f, 1.0f / (float)Mission::eTypeMax);

	m_pTexture = new Texture;
	if (FAILED(m_pTexture->Create("Assets/Texture/Mission/Number.png"))) {
		MessageBox(NULL, "MissionMessage error", "error", MB_OK);
	}
}

void Mission::MissionMenuDraw()
{
	m_ui.ChangeDraw(Mission::TexTag::eMessage, false);
	m_ui.ChangeDraw(Mission::TexTag::eTag, true);
	m_ui.ChangeDraw(Mission::TexTag::eIcon, true);
	m_ui.Draw();
	m_ui.ChangeDraw(Mission::TexTag::eMessage, true);
	m_ui.ChangeDraw(Mission::TexTag::eTag, false);
	m_ui.ChangeDraw(Mission::TexTag::eIcon, false);
}

void Mission::CountRisset()
{
	m_nActiveCount = 0;
}

void Mission::DrawNumber()
{
	//行列の設定
	DirectX::XMFLOAT4X4 world, view, proj;
	DirectX::XMMATRIX mView = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixTranspose(mView));
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixTranspose(mProj));
	Sprite::SetView(view);
	Sprite::SetProjection(proj);

	for (int i = 0; i < 2; ++i)
	{
		DirectX::XMMATRIX T;
		//位置・サイズの計算
		switch (i)
		{
		case 0: T = DirectX::XMMatrixTranslation(CurrentPos.x, CurrentPos.y + m_nActiveCount * MISSION_UI_HEIGHT, 0.0f); break;
		case 1: T = DirectX::XMMatrixTranslation(MaxPos.x, MaxPos.y + m_nActiveCount * MISSION_UI_HEIGHT, 0.0f);			break;
		}
		
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);
		DirectX::XMMATRIX mWorld = S * T;

		//転置して読み取り専用のデータに変換
		DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mWorld));

		//描画
		Sprite::SetWorld(world);
		Sprite::SetSize({ 0.03f, 0.03f });
		switch (i)
		{
		case 0: Sprite::SetUVPos({ (float)m_nCurrent / 10.0f,	0.0f });	break;
		case 1: Sprite::SetUVPos({ (float)m_nMax	 / 10.0f,	0.0f});		break;
		}
		Sprite::SetUVScale({ 0.1f, 1.0f });
		Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Sprite::SetTexture(m_pTexture);
		Sprite::Draw();
	}
	//カウントを進める
	m_nActiveCount++;
}
