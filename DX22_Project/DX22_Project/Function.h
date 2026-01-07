#ifndef ___FUNCTION_H___
#define ___FUNCTION_H___

//==== インクルード部 ====
#include<DirectXMath.h>

/// <summary>
/// XMFLOAT3を作成
/// </summary>
/// <param name="x">：float.xに入れる値</param>
/// <param name="y">：float.yに入れる値</param>
/// <param name="z">：float.zに入れる値</param>
/// <returns></returns>
inline DirectX::XMFLOAT3 MakeXMFloat3(float x, float y, float z)
{
	DirectX::XMFLOAT3 float3 = {};

	float3.x = x;
	float3.y = y;
	float3.z = z;

	return float3;
}

/// <summary>
/// (0.0f, 0.0f, 1.0f)方向を基準にベクトルが何度ずれた方向かを計算
/// </summary>
/// <param name="vec">：ベクトル</param>
/// <returns></returns>
inline float Y_LotationVec(DirectX::XMVECTOR vec)
{
	DirectX::XMFLOAT3 fZ = { 0.0f, 0.0f, 1.0f };
	DirectX::XMVECTOR vecZ = DirectX::XMLoadFloat3(&fZ);	//デフォルトの方向ベクトル
	vecZ = DirectX::XMVector3Normalize(vecZ);				//正規化

	float x1 = DirectX::XMVectorGetX(vecZ);
	float y1 = DirectX::XMVectorGetZ(vecZ);
	float x2 = DirectX::XMVectorGetX(vec);
	float y2 = DirectX::XMVectorGetZ(vec);

	float dot = x1 * x2 + y1 * y2;
	float det = x1 * y2 - y1 * x2;

	float AngleY = atan2f(det, dot);
	AngleY *= -1.0f;

	return AngleY;
}

#endif