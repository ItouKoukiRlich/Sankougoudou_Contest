//==== インクルード部 ====
#include"NormalBullet.h"
#include"Effect.h"

//==== 定数・マクロ定義 ====
#define NormalBulletEffect u"Assets/Effect/NormalBullet.efkefc"
namespace nameNormalBullete
{
	constexpr float cg_DeltaMove		= 1.0f;	//移動量
	constexpr int	cg_FlameToDestroy	= 30;	//何フレーム後に消えるか
}
using namespace nameNormalBullete;

NormalBullet::NormalBullet()
{
	EFK_INS->Load(NormalBulletEffect);
	m_nDamage = 2;
}

NormalBullet::~NormalBullet()
{
}

void NormalBullet::Update()
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

void NormalBullet::Draw()
{
}

void NormalBullet::CreateBullet(DirectX::XMVECTOR vec, DirectX::XMFLOAT3 pos)
{
	DirectX::XMStoreFloat3(&m_move, DirectX::XMVectorScale(vec, cg_DeltaMove));
	m_Pos				= pos;	//位置を設定
	m_Collision.center	= pos;	//位置を設定
	m_bActive			= true;	//発射中にする
	m_nActiveCount		= 0;	//カウントを初期化
	EFK_INS->Play(NormalBulletEffect, pos, &m_handle, true, &m_Pos);
}
