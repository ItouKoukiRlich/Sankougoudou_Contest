//==== インクルード部 ====
#include"Enemy.h"
#include"ShaderList.h"

Camera* Enemy::m_pCamera = nullptr;

Enemy::Enemy()
	:m_nLife(0)
	,m_Angle({0.0f, 0.0f, 0.0f})
	,m_pBullet(nullptr)
	,m_bActive(false)
	,m_pModel(nullptr)
	,m_nBulletNum(0)
{

}

Enemy::~Enemy()
{

}

void Enemy::Update()
{

}

void Enemy::Draw()
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
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_UNLIT));

	// マテリアル別にメッシュを表示 
	for (unsigned int i = 0; i < m_pModel->GetMeshNum(); ++i)
	{
		// モデルのメッシュを取得 
		const Model::Mesh mesh = *m_pModel->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得 
		Model::Material material = *m_pModel->GetMaterial(mesh.materialID);

		// シェーダーへマテリアルを設定 
		ShaderList::SetMaterial(material);

		// モデルの描画 
		m_pModel->Draw(i);
	}
}

bool Enemy::CheckActive() const
{
	return m_bActive;
}

void Enemy::CreateEnemy(DXf3 pos)
{
	m_bActive	= true;
	m_Pos		= pos;
}

int Enemy::GetBulletNum() const
{
	return m_nBulletNum;
}

Bullet* Enemy::GetBullet() const
{
	return m_pBullet;
}

void Enemy::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}