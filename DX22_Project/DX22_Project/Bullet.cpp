//==== インクルード部 ====
#include "Bullet.h"

Bullet::Bullet()
	:m_bActive(false)
	,m_nActiveCount(0)
	,m_handle(-1)
{
	DirectX::XMFLOAT3 f3 = { 0.0f, 0.0f, 0.0f };
	m_Vec = DirectX::XMLoadFloat3(&f3);
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

bool Bullet::GetActive() const
{
	return m_bActive;
}
