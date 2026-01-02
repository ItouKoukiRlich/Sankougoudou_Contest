#ifndef ___MATHSTRUCT_H___
#define ___MATHSTRUCT_H___

//==== インクルード部 ====
#include<DirectXMath.h>

/// <summary>
/// DirextX::XMFLOAT2 を作成
/// </summary>
/// <param name="x">:x値</param>
/// <param name="y">:y値</param>
/// <returns></returns>
inline DirectX::XMFLOAT2 MakeDXf2(float x, float y)
{
	return{ x, y };
}

/// <summary>
/// DirextX::XMFLOAT3 を作成
/// </summary>
/// <param name="x">：x値</param>
/// <param name="y">：y値</param>
/// <param name="z">：z値</param>
/// <returns></returns>
inline DirectX::XMFLOAT3 MakeDXf3(float x, float y, float z)
{
	return { x, y, z };
}

/// <summary>
/// DirextX::XMFLOAT4 を作成
/// </summary>
/// <param name="x">：x値</param>
/// <param name="y">：y値</param>
/// <param name="z">：z値</param>
/// <param name="w">：w値</param>
/// <returns></returns>
inline DirectX::XMFLOAT4 MakeDXf4(float x, float y, float z, float w)
{
	return { x, y, z, w };
}

struct float3
{
	float x;
	float y;
	float z;
};
#define MakeFloat3(x,y,z) (float3{x,y,z})

#define PI (3.141592653589f)
#define DegreeToRadian(deg) (deg * (PI / 180.0f))
#define RadianToDegree(rad) (rad * (180.0f / PI))

#endif