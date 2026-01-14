//==== インクルード部 ====
#include"Enemy.h"
#include"ShaderList.h"
#include"NormalEnemy.h"
#include"Player.h"

Camera* Enemy::m_pCamera = nullptr;
Player* Enemy::m_pPlayer = nullptr;

Enemy::Enemy()
	:m_nLife(0)
	,m_Angle({0.0f, 0.0f, 0.0f})
	,m_pBullet(nullptr)
	,m_bActive(false)
	,m_nBulletNum(0)
{
	m_Collision.center = {};
	m_Collision.radius = 0.5f;
}

Enemy::~Enemy()
{

}

void Enemy::Update()
{

}

void Enemy::Draw()
{
	
}

void Enemy::ModelDraw(Model* pModel)
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
	pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_UNLIT));

	// マテリアル別にメッシュを表示 
	for (unsigned int i = 0; i < pModel->GetMeshNum(); ++i)
	{
		// モデルのメッシュを取得 
		const Model::Mesh mesh = *pModel->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得 
		Model::Material material = *pModel->GetMaterial(mesh.materialID);

		// シェーダーへマテリアルを設定 
		ShaderList::SetMaterial(material);

		// モデルの描画 
		pModel->Draw(i);
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
	m_Collision.center = pos;
}

void Enemy::CreateEnemyNormal(DXf3 pos, int type, DXf3 move)
{
	m_bActive = true;
	m_Pos = pos;
	m_Collision.center = pos;
	m_nLife = m_nMaxLife;
}

int Enemy::GetBulletNum() const
{
	return m_nBulletNum;
}

Bullet* Enemy::GetBullet() const
{
	return m_pBullet;
}

bool Enemy::MinusHP(int damage)
{
	m_nLife -= damage;
	if (m_nLife <= 0)
	{
		m_nLife = 0;
		m_bActive = false;
		if (m_pBullet) m_pBullet->Stop();
		return true;
	}

	return false;
}

void Enemy::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}

Collision::Sphere Enemy::GetCollision() const
{
	return m_Collision;
}

void Enemy::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}