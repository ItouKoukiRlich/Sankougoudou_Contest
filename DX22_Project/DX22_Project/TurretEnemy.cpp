//==== インクルード部 ====
#include"TurretEnemy.h"
#include"Model.h"
#include"NormalEnemyBullet.h"
#include"Input.h"
using namespace nameTarretEnemy;

Player* TurretEnemy::m_pPlayer = nullptr;

TurretEnemy::TurretEnemy()
	:m_nCount(cg_nShotCountMax)	//最初から発射可能
{
	//---- モデル確保 ----
	m_pModel = new Model;
	if (!m_pModel->Load("Assets/Model/Enemy/Turret.fbx", 1.0f, Model::Flip::XFlip))
	{
		MessageBox(NULL, "TurretEnemy_Model_error", "error", MB_OK);
	}

	//---- 弾を確保 ----
	m_pBullet		= new NEB;
	m_nBulletNum	= 1;		//弾の数は１
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
	Enemy::Draw();
	m_pBullet->Draw();
}

void TurretEnemy::SetPlayer(Player* player)
{
	m_pPlayer = player;
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
