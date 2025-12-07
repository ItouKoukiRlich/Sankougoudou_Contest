#ifndef ___CAMERAGAME_H___
#define ___CAMERAGAME_H___

//==== インクルード部 ====
#include<DirectXMath.h>
#include"Camera.h"
#include"DebugBlock.h"

class Player;

class CameraGame : public Camera
{
public:
	CameraGame();
	~CameraGame() override;
	void Update() override;
	void Draw();

	/// <summary>
	/// プレイヤーのアドレスを設定(カメラ操作で必要になるため)
	/// </summary>
	/// <param name="pPlayer">：プレイヤーのアドレス</param>
	void SetPlayer(Player* pPlayer);

	//DirectX::XMFLOAT3 GetLook();

private:
	static constexpr float	cm_PlayerAndCameraRadius = 2.0f;
	const float				cm_Move		= 0.1f;						//移動量
	const DirectX::XMFLOAT3 cm_InitPos	= { 0.0f, 0.0f, -1.0f };	//回転抜きの初期位置

private:
	float	m_radXZ;		//カメラの横回転
	float	m_radY;			//カメラ縦回転
	float	m_radius;		//カメラと注意点の距離
	float	m_Degree;		//プレイヤーを中心に何度の場所に注視点があるか
	Player* m_pPlayer;		//プレイヤーのアドレス

#ifdef _DEBUG
	DebugBlock m_DebugBlock;	//デバッグ用のブロック
#endif
};

#endif