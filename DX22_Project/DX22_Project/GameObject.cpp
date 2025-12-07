//==== インクルード部 ====
#include "GameObject.h"

GameObject::GameObject()
	:m_Pos({0.0f, 0.0f, 0.0f})
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{

}

void GameObject::Draw()
{

}

void GameObject::SetPos(DirectX::XMFLOAT3 pos)
{
	m_Pos = pos;
}

DirectX::XMFLOAT3 GameObject::GetPos() const
{
	return m_Pos;
}