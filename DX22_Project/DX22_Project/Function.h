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
DirectX::XMFLOAT3 MakeXMFloat3(float x, float y, float z)
{
	DirectX::XMFLOAT3 float3 = {};

	float3.x = x;
	float3.y = y;
	float3.z = z;

	return float3;
}

#endif