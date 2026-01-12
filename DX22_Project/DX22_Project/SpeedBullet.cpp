//==== インクルード部 ====
#include "SpeedBullet.h"

//==== 定数・マクロ定義 ====
#define SpeedBulletEffect u"Assets/Effect/SpeedBullet.efkefc"
namespace nameSpeedBullete
{
	constexpr float cg_DeltaMove = 2.0f;	//移動量
	constexpr int	cg_FlameToDestroy = 30;	//何フレーム後に消えるか
}
using namespace nameSpeedBullete;

SpeedBullet::SpeedBullet()
{
	EFK_INS->Load(SpeedBulletEffect);
	m_nDamage = 1;
}

SpeedBullet::~SpeedBullet()
{
}

void SpeedBullet::Update()
{
	if (!m_bActive) return;	//発射していないなら処理しない

	//弾の位置を設定
	m_Pos.x += m_move.x;
	m_Pos.y += m_move.y;
	m_Pos.z += m_move.z;

	//弾の位置を決定
	EFK_INS->SetPos(m_handle, m_Pos);

	//当たり判定の位置も更新
	m_Collision.center = m_Pos;

	m_nActiveCount++;	//カウントを進める
	if (m_nActiveCount > cg_FlameToDestroy)
	{
		m_bActive = false;			//弾を消す
		EFK_INS->Stop(m_handle);	//エフェクトの再生停止
	}
}

void SpeedBullet::Draw()
{
}

void SpeedBullet::CreateBullet(DirectX::XMVECTOR vec, DirectX::XMFLOAT3 pos)
{
	DirectX::XMStoreFloat3(&m_move, DirectX::XMVectorScale(vec, cg_DeltaMove));
	m_Pos				= pos;	//位置を設定
	m_Collision.center	= pos;	//位置を設定
	m_bActive			= true;	//発射中にする
	m_nActiveCount		= 0;	//カウントを初期化
	EFK_INS->Play(SpeedBulletEffect, pos, &m_handle, true, &m_Pos);
}
