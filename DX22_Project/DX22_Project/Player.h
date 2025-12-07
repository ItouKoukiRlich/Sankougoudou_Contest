#ifndef ___PLAYER_H___
#define ___PLAYER_H___

//==== インクルード部 ====
#include"CameraGame.h"
#include"GameObject.h"
#include"Model.h"

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
	static constexpr float cm_Move			= 0.05f;
	static constexpr float cm_IncreaseMove	= 0.01f;		//1フレームの増える移動量
	static constexpr float cm_MaxMove		= 0.05f;			//最高速度

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

private:
	CameraGame* m_pCamera;				//ゲーム内カメラ情報
	Model*		m_pModelBody;			//プレイヤーモデル(本体)
	Model*		m_pModelArm;			//プレイヤーモデル(腕)
	Model*		m_pModelLeg;			//プレイヤーモデル(足)
	float		m_Move[DirectionMax];	//各方向の移動量
	float		m_fRotationAngleY;		//Y軸中心の回転度
	bool		m_bAction;				//操作が行われたか
};

#endif