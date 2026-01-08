#ifndef ___FUNCTION_H___
#define ___FUNCTION_H___

//==== インクルード部 ====
#include<DirectXMath.h>
#include"Geometory.h"

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

/// <summary>
/// ボックスの行列情報を作成し、描画
/// </summary>
/// <param name="Position">：位置</param>
/// <param name="Size">：大きさ</param>
/// <param name="RoatationAngle">：角度</param>
/// <returns>変換行列</returns>
inline void CreateBox(DirectX::XMFLOAT3 Position, DirectX::XMFLOAT3 Size, DirectX::XMFLOAT3 RoatationAngle)
{
	//移動行列
	DirectX::XMMATRIX Translation = DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z);

	//拡大縮小行列
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(Size.x, Size.y, Size.z);

	//回転行列
	DirectX::XMMATRIX RotationX = DirectX::XMMatrixRotationX(RoatationAngle.x);
	DirectX::XMMATRIX RotationY = DirectX::XMMatrixRotationY(RoatationAngle.y);
	DirectX::XMMATRIX RotationZ = DirectX::XMMatrixRotationZ(RoatationAngle.z);

	//最終的な行列
	DirectX::XMMATRIX mat = Scale * RotationX * RotationY * RotationZ * Translation;

	//転置
	mat = DirectX::XMMatrixTranspose(mat);

	//XMFLOAT4X4に変換
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);

	Geometory::SetWorld(fMat);
	Geometory::DrawBox();
}

#endif