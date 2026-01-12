//==== インクルード部 ====
#include "EnemyHP.h"
#include"Sprite.h"

Texture*	EnemyHP::m_pTexHP		= nullptr;
Texture*	EnemyHP::m_pTexHPBase	= nullptr;
Camera*		EnemyHP::m_pCamera		= nullptr;

EnemyHP::EnemyHP()
{
	//---- テクスチャの確保 ----
	if (!m_pTexHP){
		m_pTexHP = new Texture;
		if (FAILED(m_pTexHP->Create("Assets/Texture/EnemyHP/HP.png")))
		{
			MessageBox(NULL, "EnemyHP texure error", "error", MB_OK);
		}
	}
	if (!m_pTexHPBase) {
		m_pTexHPBase = new Texture;
		if (FAILED(m_pTexHPBase->Create("Assets/Texture/EnemyHP/HP_base.png")))
		{
			MessageBox(NULL, "EnemyHP texure error", "error", MB_OK);
		}
	}
}

EnemyHP::~EnemyHP()
{
	SAFE_DELETE(m_pTexHP);
	SAFE_DELETE(m_pTexHPBase);
}

void EnemyHP::Draw(int MaxHP, int CurrentHP, DXf3 pos, DXf2 scale)
{
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

	//---- HPバーのベース ----
	DirectX::XMMATRIX mWorld = mCamInv * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	mWorld = DirectX::XMMatrixTranspose(mWorld);

	//スプライトに設定するワールド行列を用意
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, mWorld);

	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);

	//スプライトを描画
	Sprite::SetUVPos({ 0.0f, 0.0f });
	Sprite::SetUVScale({ 1.0f, 1.0f });
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Sprite::SetOffset({ 0.0f, 0.0f });
	Sprite::SetSize({3.0f * scale.x, 1.0f * scale.y});
	Sprite::SetWorld(world);
	Sprite::SetTexture(m_pTexHPBase);
	Sprite::Draw();

	//---- HPの表示 ----
	float ratio = (float)CurrentHP / (float)MaxHP;

	mWorld = mCamInv * DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	mWorld = DirectX::XMMatrixTranspose(mWorld);
	
	//スプライトに設定するワールド行列を用意
	DirectX::XMStoreFloat4x4(&world, mWorld);
	Sprite::SetWorld(world);
	Sprite::SetSize({ 3.0f * scale.x * ratio * 0.95f, 1.0f * scale.y * 0.8f });
	Sprite::SetOffset({ (3.0f * scale.x * 0.5f * 0.95f) * -(1.0f - ratio), 0.0f});
	Sprite::SetTexture(m_pTexHP);
	Sprite::Draw();
	Sprite::SetOffset({0.0f, 0.0f });
	SetRenderTargets(1, &pRTV,pDSV);
}

void EnemyHP::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}