//==== インクルード部 ====
#include"TurretEnemy.h"
#include"Model.h"
#include"NormalEnemyBullet.h"
#include"Input.h"
#include"ShaderList.h"
namespace nameTarretEnemy
{
	constexpr int cg_MaxHP = 4;
	constexpr float cg_DrawHpPos = 4.0f;	//位置からどのくらい離れているか
	constexpr float cg_DrawIconPos = 6.0f;	//位置からどのくらい離れているか
	constexpr float cg_Size = 3.0f;
}
using namespace nameTarretEnemy;

bool	TurretEnemy::m_bMission = false;
Model*	TurretEnemy::m_pModel	= nullptr;

TurretEnemy::TurretEnemy()
	:m_nCount(0)	//最初から発射可能
{
	//---- モデル確保 ----
	if (!m_pModel)
	{
		m_pModel = new Model;
		if (!m_pModel->Load("Assets/Model/Enemy/Turret.fbx", cg_Size, Model::Flip::XFlip))
		{
			MessageBox(NULL, "TurretEnemy_Model_error", "error", MB_OK);
		}
	}

	//---- 弾を確保 ----
	m_pBullet		= new NEB;
	m_nBulletNum	= 1;		//弾の数は１
	
	//---- ライフ ----
	m_nLife = m_nMaxLife = cg_MaxHP;

	//---- 当たり判定 ----
	m_Collision.radius = (cg_Size + 2.0f) * 0.5f;
}

TurretEnemy::~TurretEnemy()
{
	SAFE_DELETE(m_pBullet);
	SAFE_DELETE(m_pModel);
}

void TurretEnemy::Update()
{
	if (!m_bActive) return;

	m_pBullet->Update();

	//---- 弾の発射処理 ----
	//カウントが弾の発射カウントになったら発射
	if (m_nCount >= cg_nShotCountMax)
	{
		//射程圏内にプレイヤーがいるか確認
		DirectX::XMFLOAT3 PlayerPos		= m_pPlayer->GetPos();								//プレイヤー位置入手
		DirectX::XMVECTOR vecPos		= DirectX::XMLoadFloat3(&m_Pos);					//計算用に変換
		DirectX::XMVECTOR vecPlayerPos	= DirectX::XMLoadFloat3(&PlayerPos);				//計算用に変換
		DirectX::XMVECTOR Length		= DirectX::XMVectorSubtract(vecPlayerPos, vecPos);	//引き算
		float Distance;
		DirectX::XMStoreFloat(&Distance, DirectX::XMVector3Length(Length));					//距離を求める
		if (Distance <= cg_ShotLength)
		{
			//発射中なら撃たない
			if (!m_pBullet->GetActive())
			{
				//発射する
				Length = DirectX::XMVector3Normalize(Length);
				m_pBullet->CreateBullet(Length, m_Pos);
				m_nCount = 0;	//発射したらカウントは０
			}
		}
	}
	m_nCount++;
}

void TurretEnemy::Draw()
{
	if (!m_bActive) return;
	if (m_bMission)
	{
		m_Icon.SetPos({ m_Pos.x, m_Pos.y + cg_DrawIconPos, m_Pos.z });
		m_Icon.Draw();
	}
	ModelDraw(m_pModel);
	m_pBullet->Draw();
	m_HPui.Draw(cg_MaxHP, m_nLife, {m_Pos.x, m_Pos.y + cg_DrawHpPos, m_Pos.z});

	//float num = m_Collision.radius * 2.0f;
	//CreateBox(m_Pos, { num, num, num }, { 0.0f, 0.0f, 0.0f });
}

void TurretEnemy::SetMissionFlag(bool flag)
{
	m_bMission = flag;
}

void TurretEnemy::staticDraw(Camera* pCamera, DXf3 pos)
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

bool TurretEnemy::CheckShot()
{
	//---- プレイヤー位置入手 ----
	DirectX::XMFLOAT3 PlayerPos = m_pPlayer->GetPos();

	//---- プレイヤーとの距離を求める ----
	DirectX::XMVECTOR vecPos		= DirectX::XMLoadFloat3(&m_Pos);					//計算用に変換
	DirectX::XMVECTOR vecPlayerPos	= DirectX::XMLoadFloat3(&PlayerPos);				//計算用に変換
	DirectX::XMVECTOR ToPlayerVec	= DirectX::XMVectorSubtract(vecPlayerPos, vecPos);	//引き算
	float Distance;
	DirectX::XMStoreFloat(&Distance, DirectX::XMVector3Length(ToPlayerVec));			//距離を求める

	//---- 射程内か確認 ----
	if (Distance <= cg_ShotLength)	//長さが射程範囲内なら
		return true;

	return false;
}
