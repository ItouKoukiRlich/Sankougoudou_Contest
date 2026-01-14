//==== インクルード部 ====
#include "CreateEnemy.h"
#include"Player.h"
#include"Sprite.h"

bool	CreateEnemy::m_bMission		= false;
Model*  CreateEnemy::m_pModel		= nullptr;
Texture* CreateEnemy::m_pTexture	= nullptr;

namespace nameCreateEnemy
{
	constexpr int	cg_MaxHP		= 10;
	constexpr float cg_DrawHpPos	= 4.0f;	//位置からどのくらい離れているか
	constexpr float cg_DrawIconPos	= 6.0f;	//位置からどのくらい離れているか
	constexpr float cg_Size			= 5.0f;
}
using namespace nameCreateEnemy;

CreateEnemy::CreateEnemy()
	:m_nAttackCount(0)
	,m_bAttckFlag(false)
	,m_Alpha(0.0f)
	,m_Upscal(0.0f)
{
	//---- モデル確保 ----
	if (!m_pModel)
	{
		m_pModel = new Model;
		if (!m_pModel->Load("Assets/Model/Enemy/Create.fbx", cg_Size, Model::Flip::XFlip))
		{
			MessageBox(NULL, "CreateEnemy_Model_error", "error", MB_OK);
		}
	}

	//---- テクスチャ確保 ----
	if (!m_pTexture)
	{
		m_pTexture = new Texture;
		if (FAILED(m_pTexture->Create("Assets/Texture/jamijami.png")))
		{
			MessageBox(NULL, "jamijami_error", "error", MB_OK);
		}
	}

	//---- ライフ ----
	m_nLife = m_nMaxLife = cg_MaxHP;

	//---- 当たり判定 ----
	m_Collision.radius = cg_Size * 0.5f;
}

CreateEnemy::~CreateEnemy()
{
	SAFE_DELETE(m_pTexture);
	SAFE_DELETE(m_pModel);
}

void CreateEnemy::Update()
{
	if (!m_bActive) return;
	
	//射程圏内にプレイヤーがいるか確認
	m_bAttckFlag = false;
	DirectX::XMFLOAT3 PlayerPos = m_pPlayer->GetPos();
	DirectX::XMVECTOR vecPos = DirectX::XMLoadFloat3(&m_Pos);
	DirectX::XMVECTOR vecPlayerPos = DirectX::XMLoadFloat3(&PlayerPos);
	DirectX::XMVECTOR Length = DirectX::XMVectorSubtract(vecPlayerPos, vecPos);
	float Distance;
	DirectX::XMStoreFloat(&Distance, DirectX::XMVector3Length(Length));
	if (Distance < 100.0f)
	{
		m_bAttckFlag = true;
		//射程圏内かつカウントが溜まっていたらダメージを与える
		if (m_nAttackCount > 300)
		{
			m_nAttackCount = 0;		//カウントをリセット
			m_pPlayer->MinusHP(1);	//HPを引く
		}
	}
	m_nAttackCount++;
}

void CreateEnemy::Draw()
{
	if (!m_bActive) return;
	if (m_bAttckFlag) DrawAttackEffect();
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

void CreateEnemy::DrawAttackEffect()
{
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);

	DirectX::XMMATRIX mCamInv = DirectX::XMMatrixIdentity();

	if (m_pCamera)
	{
		//スプライト表示用の行列を設定
		Sprite::SetView(m_pCamera->GetViewMatrix());
		Sprite::SetProjection(m_pCamera->GetProjectionMatrix());

		//カメラの行列からビルボード行列を計算
		DirectX::XMFLOAT4X4 view = m_pCamera->GetViewMatrix(false);
		mCamInv = DirectX::XMLoadFloat4x4(&view);
		mCamInv = DirectX::XMMatrixInverse(nullptr, mCamInv);
		DirectX::XMStoreFloat4x4(&view, mCamInv);
		view._41 = view._42 = view._43 = 0.0f;
		mCamInv = DirectX::XMLoadFloat4x4(&view);
	}

	//値をプラス
	m_Alpha += 0.01f;
	if (m_Alpha > 1.0f) m_Alpha = 0.0f;
	m_Upscal += 0.01f;
	if (m_Upscal > 1.0f) m_Upscal = 0.0f;

	//ビルボード込のワールド行列を計算
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f + m_Upscal, 1.0f + m_Upscal, 1.0f);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	DirectX::XMMATRIX mWorld = mCamInv * S * T;
	mWorld = DirectX::XMMatrixTranspose(mWorld);

	//スプライトに設定するワールド行列を用意
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, mWorld);

	//スプライトを描画
	Sprite::SetUVPos({ 0.0f, 0.0f });
	Sprite::SetUVScale({ 1.0f, 1.0f });
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f - m_Alpha });
	Sprite::SetOffset({ 0.0f, 0.0f });
	Sprite::SetSize({ 9.0f, 9.0f });
	Sprite::SetWorld(world);
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();
	SetRenderTargets(1, &pRTV, pDSV);
}
