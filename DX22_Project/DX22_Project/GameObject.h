#ifndef ___GAMEOBJECT_H___
#define ___GAMEOBJECT_H___

//==== インクルード部 ====
#include<DirectXMath.h>

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	virtual void Update() = 0;
	virtual void Draw();

	/// <summary>
	/// 位置を設定
	/// </summary>
	/// <param name="pos">：位置</param>
	void SetPos(DirectX::XMFLOAT3 pos);

	/// <summary>
	/// 位置を入手
	/// </summary>
	/// <returns>位置</returns>
	DirectX::XMFLOAT3 GetPos() const;

protected:
	DirectX::XMFLOAT3 m_Pos;
};

#endif