#ifndef ___CAMERA_H___
#define ___CAMERA_H___

//==== インクルード部 ====
#include<DirectXMath.h>

class Camera
{
public:
	Camera();
	virtual ~Camera();
	virtual void Update() = 0;
	
	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <param name="transpose"></param>
	/// <returns></returns>
	DirectX::XMFLOAT4X4 GetViewMatrix(bool transpose = true);

	/// <summary>
	/// プロジェクション行列の取得
	/// </summary>
	/// <param name="transpose"></param>
	/// <returns></returns>
	DirectX::XMFLOAT4X4 GetProjectionMatrix(bool transpose = true);

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 GetPos();

	/// <summary>
	/// 注視点の取得
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 GetLook();

	/// <summary>
	/// 注視点の設定
	/// </summary>
	/// <param name="look">：設定する注視点</param>
	void SetLook(DirectX::XMFLOAT3 look);

protected:
	DirectX::XMFLOAT3 m_pos;		//座標
	DirectX::XMFLOAT3 m_look;		//注意点
	DirectX::XMFLOAT3 m_up;			//上方ベクトル
	float m_fovy;					//画角
	float m_aspect;					//アスペクト比
	float m_near;					//ニアクリップ
	float m_far;					//ファークリップ


};

#endif