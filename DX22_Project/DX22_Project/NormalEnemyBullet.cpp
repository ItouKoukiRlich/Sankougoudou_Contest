//==== インクルード部 ====
#include"NormalEnemyBullet.h"
#include"TurretEnemy.h"

Player* NormalEnemyBullet::m_pPlayer = nullptr;

//==== 定数・マクロ定義 ====
#define EnemyNormalBulletEffect u"Assets/Effect/NormalEnemyBullet.efkefc"
namespace nameNormalEnemyBullete
{
	constexpr float cg_DeltaMove = nameTarretEnemy::cg_ShotLength / nameTarretEnemy::cg_nShotCountMax;
	constexpr int	cg_FlameToDestroy = (int)nameTarretEnemy::cg_nShotCountMax;//生存時間
}
using namespace nameNormalEnemyBullete;

NormalEnemyBullet::NormalEnemyBullet()
{
	EFK_INS->Load(EnemyNormalBulletEffect);	//エフェクト
	m_Collision.radius	= 0.5f;				//当たり判定
	m_nDamage			= 5;				//ダメージ
}

NormalEnemyBullet::~NormalEnemyBullet()
{
}

void NormalEnemyBullet::Update()
{
	if (!m_bActive) return;

	//ベクトルを元に前進
	DirectX::XMVECTOR v = m_Vec;
	DirectX::XMVectorScale(v, cg_DeltaMove);
	DirectX::XMFLOAT3 move;
	DirectX::XMStoreFloat3(&move, v);

	//弾の位置を設定
	m_Pos.x += move.x;
	m_Pos.y += move.y;
	m_Pos.z += move.z;

	//当たり判定を更新
	m_Collision.center = m_Pos;

	//エフェクトの位置を更新
	EFK_INS->SetPos(m_handle, m_Pos);

	m_nActiveCount++;	//カウントを進める
	if (m_nActiveCount > cg_FlameToDestroy)
	{
		m_bActive = false;			//弾を消す
		EFK_INS->Stop(m_handle);	//エフェクトの再生停止
	}
}

void NormalEnemyBullet::Draw()
{
	if (!m_bActive) return;
}

void NormalEnemyBullet::CreateBullet()
{
	
}

void NormalEnemyBullet::CreateBullet(DirectX::XMVECTOR vec, DirectX::XMFLOAT3 pos)
{
	m_Pos = pos;		//位置を設定
	m_Vec = vec;		//ベクトルを設定
	m_bActive = true;	//発射中にする
	m_nActiveCount = 0;	//カウントを初期化
	EFK_INS->Play(EnemyNormalBulletEffect, m_Pos, &m_handle, true, &m_Pos);
}

void NEB::SetCamera(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}