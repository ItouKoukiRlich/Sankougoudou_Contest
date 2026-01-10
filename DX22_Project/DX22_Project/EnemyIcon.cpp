//==== インクルード部 ====
#include "EnemyIcon.h"
#include"Sprite.h"

Texture* EnemyIcon::m_pTexture	= nullptr;
Camera* EnemyIcon::m_pCamera	= nullptr;

EnemyIcon::EnemyIcon()
{
	//---- テクスチャの確保 ----
	if (!m_pTexture)
	{
		m_pTexture = new Texture;
		if (FAILED(m_pTexture->Create("Assets/Texture/MissionIcon.png")))
		{
			MessageBox(NULL, "EnemyIcon texure error", "error", MB_OK);
		}
	}
}

EnemyIcon::~EnemyIcon()
{
	SAFE_DELETE(m_pTexture);
}

void EnemyIcon::Update()
{

}

void EnemyIcon::Draw()
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

	//ビルボード込のワールド行列を計算
	DirectX::XMMATRIX mWorld = mCamInv * DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	mWorld = DirectX::XMMatrixTranspose(mWorld);

	//スプライトに設定するワールド行列を用意
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, mWorld);

	//スプライトを描画
	Sprite::SetUVPos({ 0.0f, 0.0f });
	Sprite::SetUVScale({ 1.0f, 1.0f });
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Sprite::SetOffset({ 0.0f, 0.0f });
	Sprite::SetSize({ 2.0f, 2.0f });
	Sprite::SetWorld(world);
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();
}

void EnemyIcon::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}