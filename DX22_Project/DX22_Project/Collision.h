#ifndef ___COLLISION_H___
#define ___COLLISION_H___

//==== インクルード部 ====
#include<DirectXMath.h>

class Collision
{
public:

	struct Box	//立方体
	{
		DirectX::XMFLOAT3	center;
		DirectX::XMFLOAT3	size;
	};

	struct Sphere	//球
	{
		DirectX::XMFLOAT3	center;	//中心座標
		float				radius;	//半径
	};

	struct Result
	{
		bool				isHit;	//当たったかどうか
		DirectX::XMFLOAT3	dir;	//ヒット方向
	};

public:
	static Result Hit(Box a, Box b);
	static Result Hit(Sphere a, Sphere b);
private:

};

#endif