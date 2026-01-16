//==== インクルード部 ====
#include "Bullet.h"
#include"Function.h"

Bullet::Bullet()
	:m_bActive(false)
	,m_nActiveCount(0)
	,m_handle(-1)
	,m_nDamage(1)
	,m_move({0.0f, 0.0f, 0.0f})
{
	m_Collision.center = m_Pos;
	m_Collision.radius = 0.5f;	//デフォルトで0.5f
}

Bullet::~Bullet()
{
}

void Bullet::Update()
{

}

void Bullet::Draw()
{

}

void Bullet::CreateBullet()
{
}

void Bullet::CreateBullet(DirectX::XMVECTOR vec, DirectX::XMFLOAT3 pos)
{
}

void Bullet::CreateBullet(DXf3 pos)
{
}

bool Bullet::GetActive() const
{
	return m_bActive;
}

Collision::Sphere Bullet::GetCollision() const
{
	return m_Collision;
}

int Bullet::GetDamage() const
{
	return m_nDamage;
}

void Bullet::Stop()
{
 	EFK_INS->Stop(m_handle);
	m_bActive = false;
}

void Bullet::DrawCollision() const
{
	float size = m_Collision.radius * 2.0f;
	CreateBox(m_Collision.center, { size, size, size }, { 0.0f, 0.0f, 0.0f });
}