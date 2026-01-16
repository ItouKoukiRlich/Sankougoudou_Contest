//==== インクルード部 ====
#include "MainEnemyBullet.h"
#include"Player.h"

Player* MainEnemyBullet::m_pPlayer = nullptr;

MainEnemyBullet::MainEnemyBullet()
	:m_nCount(0)
	,m_phase(phase1)
{
	EFK_INS->Load(u"Assets/Effect/NormalEnemyBullet.efkefc");	//エフェクト
	m_Collision.radius = 0.5f;				//当たり判定
	m_nDamage = 5;				//ダメージ
}

MainEnemyBullet::~MainEnemyBullet()
{
}

void MainEnemyBullet::Update()
{
	if (!m_bActive) return;
	switch (m_phase)
	{
	case phase1:
		m_Pos.y += 0.05f;
		m_Collision.center = m_Pos;
		m_nCount++;
		if (m_nCount >= 120)
		{
			m_phase = phase2;
			//プレイヤーの位置を入手してプレイヤーへのベクトルを決める
			DirectX::XMFLOAT3 PlayerPos = m_pPlayer->GetPos();								//プレイヤー位置入手
			DirectX::XMVECTOR vecPos = DirectX::XMLoadFloat3(&m_Pos);					//計算用に変換
			DirectX::XMVECTOR vecPlayerPos = DirectX::XMLoadFloat3(&PlayerPos);				//計算用に変換
			DirectX::XMVECTOR Length = DirectX::XMVectorSubtract(vecPlayerPos, vecPos);	//引き算
			Length = DirectX::XMVector3Normalize(Length);
			DirectX::XMStoreFloat3(&m_move, DirectX::XMVectorScale(Length, 0.5f));
			EFK_INS->SetPos(m_handle, m_Pos);
		}
		break;

	case phase2:
		//弾の位置を設定
		m_Pos.x += m_move.x;
		m_Pos.y += m_move.y;
		m_Pos.z += m_move.z;

		//当たり判定を更新
		m_Collision.center = m_Pos;

		//エフェクトの位置を更新
		EFK_INS->SetPos(m_handle, m_Pos);

		m_nActiveCount++;	//カウントを進める
		if (m_nActiveCount > 180)
		{
			m_bActive = false;			//弾を消す
			EFK_INS->Stop(m_handle);	//エフェクトの再生停止
		}
		break;
	}
}

void MainEnemyBullet::Draw()
{
	if (!m_bActive) return;
}

void MainEnemyBullet::CreateBullet(DXf3 pos)
{
	m_phase = phase1;
	m_nCount = 0;
	m_nActiveCount = 0;
	m_Pos = pos;
	m_Collision.center = pos;
	m_bActive = true;
	EFK_INS->Play(u"Assets/Effect/NormalEnemyBullet.efkefc", pos, &m_handle, true, &m_Pos);
}

void MainEnemyBullet::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}