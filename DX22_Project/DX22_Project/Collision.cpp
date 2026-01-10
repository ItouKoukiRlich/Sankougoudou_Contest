//==== インクルード部 ====
#include "Collision.h"

Collision::Result Collision::Hit(Box a, Box b)
{
    Result out = {};

    //計算用の型に変換
    DirectX::XMVECTOR vPosA     = DirectX::XMLoadFloat3(&a.center);
    DirectX::XMVECTOR vPosB     = DirectX::XMLoadFloat3(&b.center);
    DirectX::XMVECTOR vSizeA    = DirectX::XMLoadFloat3(&a.size);
    DirectX::XMVECTOR vSizeB    = DirectX::XMLoadFloat3(&b.size);

    //ボックスの半分のサイズ
    vSizeA = DirectX::XMVectorScale(vSizeA, 0.5f);
    vSizeB = DirectX::XMVectorScale(vSizeB, 0.5f);

    //ボックスの各軸の最大値、最小値を取得
    DirectX::XMVECTOR vMaxA = DirectX::XMVectorAdd(vPosA, vSizeA);
    DirectX::XMVECTOR vMinA = DirectX::XMVectorSubtract(vPosA, vSizeA);
    DirectX::XMVECTOR vMaxB = DirectX::XMVectorAdd(vPosB, vSizeB);
    DirectX::XMVECTOR vMinB = DirectX::XMVectorSubtract(vPosB, vSizeB);
    DirectX::XMFLOAT3 maxA, minA, maxB, minB;
    DirectX::XMStoreFloat3(&maxA, vMaxA);
    DirectX::XMStoreFloat3(&minA, vMinA);
    DirectX::XMStoreFloat3(&maxB, vMaxB);
    DirectX::XMStoreFloat3(&minB, vMinB);

    //判定前はヒットしていないとみなす
    out.isHit = false;

    //ボックスの各軸同士の関係から当たり判定を行う
    if (maxA.x >= minB.x && minA.x <= maxB.x)
    {
        if (maxA.y >= minB.y && minA.y <= maxB.y)
        {
            if (maxA.z >= minB.z && minA.z <= maxB.z)
            {
                //ヒット
                out.isHit = true;

                //ブロック同士の距離を計算
                DirectX::XMVECTOR vDist = DirectX::XMVectorSubtract(vPosA, vPosB);
                vDist = DirectX::XMVectorAbs(vDist);

                //ブロックのサイズから書く軸のめり込み量を計算
                DirectX::XMVECTOR vSumSIze = DirectX::XMVectorAdd(vSizeA, vSizeB);
                DirectX::XMVECTOR vOverlap = DirectX::XMVectorSubtract(vSumSIze, vDist);
                DirectX::XMFLOAT3 overlap;
                DirectX::XMStoreFloat3(&overlap, vOverlap);

                //各軸のめり込み量のうち、最小のめり込み量の方向へ跳ね返す
                if (overlap.x < overlap.y)
                {
                    if (overlap.x < overlap.z)
                        out.dir = { a.center.x < b.center.x ? -1.0f : 1.0f, 0.0f, 0.0f };
                    else
                        out.dir = { 0.0f, 0.0f, a.center.z < b.center.z ? -1.0f : 1.0f };
                }
                else
                {
                    if (overlap.y < overlap.z)
                        out.dir = { 0.0f, a.center.y < b.center.y ? -1.0f : 1.0f, 0.0f };
                    else
                        out.dir = { 0.0f, 0.0f, a.center.z < b.center.z ? -1.0f : 1.0f };
                }
            }
        }
    }

    return out;
}

Collision::Result Collision::Hit(Sphere a, Sphere b)
{
    Result out = {};

    //計算用の型に変換
    DirectX::XMVECTOR vPosA = DirectX::XMLoadFloat3(&a.center);
    DirectX::XMVECTOR vPosB = DirectX::XMLoadFloat3(&b.center);

    //2点の位置から距離ベクトルを計算
    DirectX::XMVECTOR vDist = DirectX::XMVectorSubtract(vPosA, vPosB);

    //距離ベクトルから長さを取得
    DirectX::XMVECTOR vLen = DirectX::XMVector3Length(vDist);
    float length;
    DirectX::XMStoreFloat(&length, vLen);

    //球の半径と２点間の距離から当たり判定を取得
    if (length < a.radius + b.radius) out.isHit = true;
    else out.isHit = false;

    return out;
}

Collision::Result Collision::Hit(Sphere a, Box b)
{
    Result out = {};

    //---- 円から最も近い四角形の点を求める ----
    DirectX::XMFLOAT3 HarfSize = { b.size.x * 0.5f, b.size.y * 0.5f, b.size.z * 0.5f };
    DirectX::XMFLOAT3 BoxMin = {
        b.center.x - HarfSize.x,
        b.center.y - HarfSize.y,
        b.center.z - HarfSize.z,
    };
    DirectX::XMFLOAT3 BoxMax = {
       b.center.x + HarfSize.x,
       b.center.y + HarfSize.y,
       b.center.z + HarfSize.z,
    };
    DirectX::XMFLOAT3 clamp = {};
    clamp.x = Clamp(a.center.x, BoxMin.x, BoxMax.x);
    clamp.y = Clamp(a.center.y, BoxMin.y, BoxMax.y);
    clamp.z = Clamp(a.center.z, BoxMin.z, BoxMax.z);
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&clamp);

    //---- 最も近い点と円の距離を求める ----
    DirectX::XMVECTOR PosCircle = DirectX::XMLoadFloat3(&a.center);
    DirectX::XMVECTOR Vec       = DirectX::XMVectorSubtract(Pos, PosCircle);
    DirectX::XMVECTOR Length    = DirectX::XMVector3Length(Vec);
    float fLength;
    DirectX::XMStoreFloat(&fLength, Length);

    //---- 距離が半径よりも小さければ当たっている ----
    if (fLength < a.radius) out.isHit = true;
    else out.isHit = false;

    return out;
}

float Collision::Clamp(float num, float min, float max)
{
    if (num < min) num = min;
    else if (num > max) num = max;

    return num;
}