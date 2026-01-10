#ifndef ___PLAYER_H___
#define ___PLAYER_H___

//==== インクルード部 ====
#include"CameraGame.h"
#include"GameObject.h"
#include"Model.h"
#include"Effekseer.h"
#include"EffekseerRendererDX11.h"
#include"NormalBullet.h"
#include"SpeedBullet.h"
#include"Collision.h"

//==== 定数・マクロ定義 ====
namespace namePlayer
{
	constexpr int	cg_InitHp		= 100;		//ゲーム開始時のHP
	constexpr float cg_RadYnormal	= 30.0f;	//通常時のカメラのアングル
	constexpr int	cg_MaxNormalBullet = 3;
	constexpr int	cg_MaxSpeedBullet = 10;
}

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

	/// <summary>
	/// 設定してあるカメラポインタを入手
	/// </summary>
	/// <returns>カメラのポインタ</returns>
	Camera* GetCamera() const;

	/// <summary>
	/// プレイヤーのHPを入手
	/// </summary>
	/// <returns>：プレイヤーのHP</returns>
	int GetHP() const;

	/// <summary>
	/// HPを減少させる
	/// </summary>
	/// <param name="damage">：減少させる値</param>
	void MinusHP(int damage);

	/// <summary>
	/// HPを増やす
	/// </summary>
	/// <param name="hp">：増やす値</param>
	void AddHP(int hp);

	Collision::Sphere GetCollision() const;

	/// <summary>
	/// 正面のベクトルを入手
	/// </summary>
	/// <returns></returns>
	DirectX::XMVECTOR GetForwardVec();

	/// <summary>
	/// バリアを入手
	/// </summary>
	/// <returns>：無敵ならtrue</returns>
	bool GetBarrier() const;

private:
	//移動方向の列挙型
	enum MoveDirection
	{
		Forward = 0,
		Left,
		Up,
		DirectionMax
	};


	//ステート
	enum State
	{
		eNormal,
		eSpeed,
		eAvoid,
		eChangeToSpeeed,
		eChangeToNormal,
	};

private:
	static constexpr float	cm_Move				= 0.05f;
	static constexpr float	cm_IncreaseMove		= 0.01f;			//1フレームの増える移動量
	static constexpr float	cm_ChangeArmAngle	= 1.0f;				//1フレームで変更する手の角度
	static constexpr float	cm_ChangeAngle		= 1.0f;				//1フレームで変更する角度
	static constexpr float	cm_MaxArmAngle		= 10.0f;			//手の最大傾き角度
	static constexpr float	cm_MaxBodyAngleZ	= 10.0f;			//Z軸を軸に回転する体の最大角度
	static constexpr float	cm_MaxMove			= 0.05f;			//最高速度
	static constexpr int	cm_SpeedBulletDelay = 5;				//連続して弾を出す際に何フレーム置きに出すか
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

	/// <summary>
	/// プレイヤー操作
	/// </summary>
	void Control_speed();

	/// <summary>
	/// プレイヤーの移動ベクトルの計算
	/// </summary>
	void Move_speed();

	/// <summary>
	/// 本体の描画
	/// </summary>
	void DrawBody_speed();

	/// <summary>
	/// 手の描画
	/// </summary>
	void DrawArm_speed();

	/// <summary>
	/// 足の描画
	/// </summary>
	void DrawLeg_speed();

	/// <summary>
	/// プレイヤーのモード切替
	/// </summary>
	void ChangeMode();

	/// <summary>
	/// ベクトルの移動
	/// </summary>
	void VectorMove();

private:
	CameraGame* m_pCamera;						//ゲーム内カメラ情報
	Model*				m_pModelBody;			//プレイヤーモデル(本体)
	Model*				m_pModelArm;			//プレイヤーモデル(腕)
	Model*				m_pModelLeg;			//プレイヤーモデル(足)
	float				m_Move[DirectionMax];	//各方向の移動量
	DirectX::XMFLOAT3	m_Angle;				//プレイヤーの回転量
	float				m_ArmAngle;				//腕の回転する角度
	float				m_LookY;				//注視点の高さ
	unsigned int		m_SpeedBulletCount;		//弾のカウント（ディレイ用）


	Collision::Sphere	m_Collision;			//プレイヤーの当たり判定
	Effekseer::Handle m_hdl;

	//---- モード切替時のカメラの角度 ----
	float m_ChangeCameraAngle;
	int m_ChangeCount;

	//---- ステータス ----
	Player::State m_State;
	int		m_nLife;			//ライフ
	bool	m_bBarrier;			//バリア(無敵ならtrue)

	//---- 弾 ----
	NormalBullet*	m_pBullet;		//通常弾
	SpeedBullet*	m_pSpeedBullet;	//高速球
};

#endif