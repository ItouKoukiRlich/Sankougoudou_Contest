#ifndef ___PLAYER_H___
#define ___PLAYER_H___

//==== インクルード部 ====
#include"CameraGame.h"
#include"GameObject.h"
#include"Model.h"
#include"Effekseer.h"
#include"EffekseerRendererDX11.h"

class Player : public GameObject
{
public:
	Player();
	~Player();
	void Update();
	void Draw();

	/// <summary>
	/// カメラを設定
	/// </summary>
	/// <param name="pCamera">：カメラのアドレス</param>
	void SetCamera(CameraGame* pCamera);

private:
	//移動方向の列挙型
	enum MoveDirection
	{
		Forward = 0,
		Left,
		Up,
		DirectionMax
	};

private:
	static constexpr float	cm_Move				= 0.05f;
	static constexpr float	cm_IncreaseMove		= 0.01f;			//1フレームの増える移動量
	static constexpr float	cm_ChangeArmAngle	= 1.0f;				//1フレームで変更する手の角度
	static constexpr float	cm_ChangeAngle		= 1.0f;				//1フレームで変更する角度
	static constexpr float	cm_MaxArmAngle		= 10.0f;			//手の最大傾き角度
	static constexpr float	cm_MaxBodyAngleZ	= 10.0f;			//Z軸を軸に回転する体の最大角度
	static constexpr float	cm_MaxMove			= 0.05f;			//最高速度
	const DirectX::XMFLOAT2 cm_ArmPos			= {0.75f, 0.0f};
	const DirectX::XMFLOAT2 cm_LegPos			= {0.25f, -1.0f};

private:
	
	/// <summary>
	/// プレイヤー操作
	/// </summary>
	void Control();

	/// <summary>
	/// プレイヤーの移動ベクトルの計算
	/// </summary>
	void Move();

	/// <summary>
	/// 本体の描画
	/// </summary>
	void DrawBody();

	/// <summary>
	/// 手の描画
	/// </summary>
	void DrawArm();

	/// <summary>
	/// 足の描画
	/// </summary>
	void DrawLeg();

private:
	CameraGame* m_pCamera;						//ゲーム内カメラ情報
	Model*				m_pModelBody;			//プレイヤーモデル(本体)
	Model*				m_pModelArm;			//プレイヤーモデル(腕)
	Model*				m_pModelLeg;			//プレイヤーモデル(足)
	float				m_Move[DirectionMax];	//各方向の移動量
	DirectX::XMFLOAT3	m_Angle;				//プレイヤーの回転量
	float				m_ArmAngle;				//腕の回転する角度
	
	Effekseer::Handle	efkHandle;		//エフェクトのハンドル
	Effekseer::Handle	efkHandle2;		//エフェクトのハンドル
};

#endif