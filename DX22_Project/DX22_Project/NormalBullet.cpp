//==== インクルード部 ====
#include"NormalBullet.h"
#include"Function.h"
#include"Player.h"
#include"Effect.h"

//==== 定数・マクロ定義 ====
#define NormalBulletEffect u"Assets/Effect/NormalBullet.efkefc"

Player* NormalBullet::m_pPlayer = nullptr;

namespace nameNormalBullete
{
	constexpr float cg_DeltaMove		= 0.05f;	//1フレームの移動量
	constexpr int	cg_FlameToDestroy	= 30;		//何フレーム後に消えるか
}
using namespace nameNormalBullete;

NormalBullet::NormalBullet(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	EFK_INS->Load(NormalBulletEffect);
}

NormalBullet::~NormalBullet()
{

}

void NormalBullet::Update()
{
	if (!m_bActive) return;	//発射していないなら処理しない

	//ベクトルを元に前進
	DirectX::XMVECTOR v = m_Vec;
	DirectX::XMVectorScale(v, cg_DeltaMove);
	DirectX::XMFLOAT3 move;
	DirectX::XMStoreFloat3(&move, v);

	//弾の位置を設定
	m_Pos.x += move.x;
	m_Pos.y += move.y;
	m_Pos.z += move.z;

	//弾の位置を決定
	EFK_INS->SetPos(m_handle, m_Pos);

	m_nActiveCount++;	//カウントを進める
	if (m_nActiveCount > cg_FlameToDestroy)
	{
		m_bActive = false;			//弾を消す
		EFK_INS->Stop(m_handle);	//エフェクトの再生停止
	}
}

void NormalBullet::Draw()
{
	if (!m_bActive) return;	//発射していないなら処理しない

#ifdef _DEBUG
	//m_block.Draw(m_Pos, m_pPlayer->GetCamera());
#endif
}

void NormalBullet::CreateBullet()
{
	//弾のベクトルを入手
	DirectX::XMVECTOR vec = m_pPlayer->GetForwardVec();
	m_Vec = vec;

	//弾の位置を設定(プレイヤーの前)
	float angle					= Y_LotationVec(vec);
	DirectX::XMFLOAT3 PlayerPos = m_pPlayer->GetPos();

	m_Pos.x = PlayerPos.x + 1.0f * sinf(angle);
	m_Pos.y = PlayerPos.y;
	m_Pos.z = PlayerPos.z + 1.0f * cosf(angle);

	//発射
	m_bActive		= true;												//発射状態にする
	m_nActiveCount	= 0;												//カウントを初期化
	EFK_INS->Play(NormalBulletEffect, m_Pos, &m_handle, true, &m_Pos);	//エフェクトの再生
}