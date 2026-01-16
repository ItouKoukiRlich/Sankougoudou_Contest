//==== インクルード部 ====
#include "MainEnemy.h"
#include"ShaderList.h"
#include"MainEnemyBullet.h"

bool	MainEnemy::m_bMission	= false;
Model*  MainEnemy::m_pModel		= nullptr;

namespace nameMainEnemy
{
	constexpr int	cg_MaxHP = 20;
	constexpr float cg_DrawHpPos = 4.0f;	//位置からどのくらい離れているか
	constexpr float cg_DrawIconPos = 6.0f;	//位置からどのくらい離れているか
	constexpr float cg_Size = 3.0f;
	constexpr int ShotTime = 180;	//何フレーム置きに発射するか
}
using namespace nameMainEnemy;

MainEnemy::MainEnemy()
	:m_nCount(0)
{
	//---- モデル確保 ----
	if (!m_pModel)
	{
		m_pModel = new Model;
		if (!m_pModel->Load("Assets/Model/Enemy/Main.fbx", cg_Size, Model::Flip::XFlip))
		{
			MessageBox(NULL, "MainEnemy_Model_error", "error", MB_OK);
		}
	}

	//---- 弾を確保 ----
	m_nBulletNum = 1;		//弾の数は１
	m_pBullet = new MainEnemyBullet;

	//---- ライフ ----
	m_nLife = m_nMaxLife = cg_MaxHP;

	//---- 当たり判定 ----
	m_Collision.radius = (cg_Size + 2.0f) * 0.5f;
}

MainEnemy::~MainEnemy()
{
	SAFE_DELETE(m_pBullet);
	SAFE_DELETE(m_pModel);
}

void MainEnemy::Update()
{
	m_nCount++;
	m_pBullet->Update();
	if (m_nCount >= ShotTime)
	{
		m_pBullet->CreateBullet(m_Pos);
		m_nCount = 0;
	}
}

void MainEnemy::Draw()
{
	if (!m_bActive) return;
	if (m_bMission)
	{
		m_Icon.SetPos({ m_Pos.x, m_Pos.y + cg_DrawIconPos, m_Pos.z });
		m_Icon.Draw();
	}
	ModelDraw(m_pModel);
	m_pBullet->Draw();
	m_HPui.Draw(cg_MaxHP, m_nLife, { m_Pos.x, m_Pos.y + cg_DrawHpPos, m_Pos.z });
}

void MainEnemy::SetMissionFlag(bool flag)
{
	m_bMission = flag;
}

void MainEnemy::staticDraw(Camera* pCamera, DXf3 pos)
{
	//==== 頂点シェーダーに渡す変換行列の変数を宣言 ====
	DirectX::XMFLOAT4X4 fWVP[3] = {};
	fWVP[1] = pCamera->GetViewMatrix();
	fWVP[2] = pCamera->GetProjectionMatrix();

	//==== ワールド行列の作成 ====
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX Translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	world = Scale * Translation;
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