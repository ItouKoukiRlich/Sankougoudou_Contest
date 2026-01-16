//==== インクルード部 ====
#include"Player.h"
#include"Defines.h"
#include"ShaderList.h"
#include"Input.h"
#include"DirectX.h"
#include"Effect.h"
#include"MathStruct.h"
using namespace namePlayer;

//==== 定数・マクロ定義 ====
namespace namePlayer
{
	//---- モード切替関連 ----
	constexpr float cg_ChangeFlame	= 60.0f;										//切替にかかるフレーム
	constexpr float	cg_DeltaAngleX	= DegreeToRadian(90.0f / cg_ChangeFlame);		//１フレームの変化量
	constexpr float cg_DeltaAngleY	= -DegreeToRadian((360.0f / cg_ChangeFlame));	//１フレームの変化量

	//---- 回避関連 ----
	constexpr float cg_AvoidFlame = 20.0f;
	constexpr float cg_DeltaAvoidAngleY = DegreeToRadian((360.0f / cg_AvoidFlame));

	//---- スピードモードの際の設定 ----
	constexpr float cg_RadYspeed		= 30.0f;	//カメラの縦アングル
}

#define FILEPPP u"Assets/Effect/Heal.efkefc"
#define FILEDEM u"Assets/Effect/demo.efkefc"

Player::Player()
	:m_pCamera(nullptr)
	,m_pModelBody(nullptr)
	,m_pModelArm(nullptr)
	,m_pModelLeg(nullptr)
	,m_Angle(DirectX::XMFLOAT3{0.0f, 0.0f, 0.0f})
	,m_ArmAngle(0.0f)
	,m_nLife(cg_InitHp)
	,m_hdl(-1)
	,m_State(Player::State::eNormal)
	,m_ChangeCameraAngle(0.0f)
	,m_ChangeCount(0)
	,m_bBarrier(false)
	,m_SpeedBulletCount(0)
	,m_LookY(0.0f)
{
	//---- 弾の確保 ----
	m_pBullet		= new NormalBullet[cg_MaxNormalBullet];
	m_pSpeedBullet	= new SpeedBullet[cg_MaxSpeedBullet];

	//---- それぞれの方向の移動量をリセット ----
	for (int i = 0; i < Player::DirectionMax; i++)
		m_Move[i] = 0.0f;

	//---- モデルの確保 ----
	m_pModelBody = new Model;
	if (!m_pModelBody->Load("Assets/Model/PlayerBody.fbx", 1.0f))
		MessageBox(NULL, "PlayerModel_Body", "Error", MB_OK);

	m_pModelArm = new Model;
	if (!m_pModelArm->Load("Assets/Model/PlayerArm.fbx", 0.5f))
		MessageBox(NULL, "PlayerModel_Arm", "Error", MB_OK);

	m_pModelLeg = new Model;
	if (!m_pModelLeg->Load("Assets/Model/PlayerLeg.fbx", 0.5f))
		MessageBox(NULL, "PlayerModel_Leg", "Error", MB_OK);

	//---- 当たり判定 ----
	m_Collision.center = m_Pos;
	m_Collision.radius = 0.5f;
}

Player::~Player()
{
	SAFE_DELETE_ARRAY(m_pSpeedBullet);
	SAFE_DELETE_ARRAY(m_pBullet);
	SAFEDELETE(m_pModelLeg);
	SAFEDELETE(m_pModelArm);
	SAFEDELETE(m_pModelBody);
}

void Player::Update()
{
	//---- プレイヤーの更新処理 ----
	switch (m_State)
	{
	case Player::eNormal:
		Control();	//操作
		Move();		//移動計算
		break;

	case Player::eSpeed:
		Control_speed();
		Move_speed();
		break;

	case Player::eAvoid:
		VectorMove();
		m_Angle.y += cg_DeltaAvoidAngleY;
		m_ChangeCount++;
		if (m_ChangeCount > cg_AvoidFlame)
		{
			m_State = eNormal;
			m_Move[Player::MoveDirection::Up]		= 0.0f;
			m_Move[Player::MoveDirection::Forward]	= 0.0f;
			m_Move[Player::MoveDirection::Left]		= 0.0f;
			m_bBarrier = false;
		}
		break;

	case Player::eChangeToSpeeed:
		Control_speed();
		Move_speed();
		m_pCamera->AddRadY(m_ChangeCameraAngle);
		m_Angle.x += cg_DeltaAngleX;
		m_Angle.y += cg_DeltaAngleY;
		m_ChangeCount++;
		if (m_ChangeCount >= (int)cg_ChangeFlame)
			m_State = eSpeed;
		break;

	case Player::eChangeToNormal:
		m_pCamera->AddRadY(m_ChangeCameraAngle);
		m_Angle.x -= cg_DeltaAngleX;
		m_Angle.y -= cg_DeltaAngleY;
		m_ChangeCount++;
		if (m_ChangeCount >= (int)cg_ChangeFlame)
		{
			m_State = Player::eNormal;
			m_Angle.x = 0.0f;
		}
		break;
	}

	//---- 弾の更新処理 ----
	for (int i = 0; i < cg_MaxNormalBullet; ++i)
		m_pBullet[i].Update();
	for (int i = 0; i < cg_MaxSpeedBullet; ++i)
		m_pSpeedBullet[i].Update();
}

void Player::Draw()
{
	switch (m_State)
	{
	case Player::eNormal:
		DrawBody();	//本体
		//DrawArm();	//腕
		//DrawLeg();	//足
		break;

	case Player::eSpeed:
		DrawBody_speed();	//本体
		//DrawArm_speed();	//腕
		//DrawLeg_speed();	//足
		break;

	case Player::eAvoid:
		DrawBody();	//本体
		break;

	case Player::eChangeToSpeeed:
		DrawBody_speed();	//本体
		//DrawArm_speed();	//腕
		//DrawLeg_speed();	//足
		break;

	case Player::eChangeToNormal:
		DrawBody_speed();	//本体
		break;
	}

	//通常弾
	for (int i = 0; i < cg_MaxNormalBullet; ++i)
		m_pBullet[i].Draw();
	for (int i = 0; i < cg_MaxSpeedBullet; ++i)
		m_pSpeedBullet[i].Draw();
}

void Player::SetCamera(CameraGame* pCamera)
{
	m_pCamera = pCamera;
}

Camera* Player::GetCamera() const
{
	return m_pCamera;
}

int Player::GetHP() const
{
	return m_nLife;
}

void Player::MinusHP(int damage)
{
	m_nLife -= damage;
	if (m_nLife < 0) m_nLife = 0;
}

void Player::AddHP(int hp)
{
	m_nLife += hp;
	if (m_nLife > 100) m_nLife = 100;
}

Collision::Sphere Player::GetCollision() const
{
	return m_Collision;
}

DirectX::XMVECTOR Player::GetForwardVec()
{
	DirectX::XMFLOAT3 look			= m_pCamera->GetLook();								//注視点を入手
	DirectX::XMVECTOR LookVec		= DirectX::XMLoadFloat3(&look);						//注視点のベクトル
	DirectX::XMVECTOR PlayerVec		= DirectX::XMLoadFloat3(&m_Pos);					//プレイヤーベクトル
	DirectX::XMVECTOR vecForward	= DirectX::XMVectorSubtract(LookVec, PlayerVec);	//前方ベクトル
	vecForward = DirectX::XMVector3Normalize(vecForward);								//正規化

	return vecForward;
}

bool Player::GetBarrier() const
{
	return m_bBarrier;
}

void Player::Resste()
{
	m_Move[0] = 0.0f;
	m_Move[1] = 0.0f;
	m_Move[2] = 0.0f;
	m_State = eNormal;
	m_Angle = DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f };
}

NormalBullet* Player::GetNormalBullet()
{
	return m_pBullet;
}

SpeedBullet* Player::GetSpeedBullet()
{
	return m_pSpeedBullet;
}

void Player::ChangeMode()
{
	float radY = m_pCamera->GetRadY();	//カメラの縦の角度を入手
	m_LookY = 0.0f;

	switch (m_State)
	{
	case Player::eNormal:
		m_ChangeCameraAngle = cg_RadYspeed - radY;
		m_SpeedBulletCount = cm_SpeedBulletDelay;
		m_State = eChangeToSpeeed;
		break;

	case Player::eSpeed:
		m_ChangeCameraAngle = cg_RadYnormal - radY;
		m_State = Player::eChangeToNormal;
		break;
	}

	//切替のカウントを初期化
	m_ChangeCount = 0;
}

void Player::VectorMove()
{
	//---- 各方向のベクトルを求める ----

	//上ベクトル
	DirectX::XMFLOAT3	up = { 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR	vecUp = DirectX::XMLoadFloat3(&up);			//上ベクトル
	vecUp = DirectX::XMVector3Normalize(vecUp);	//正規化

	//直進ベクトル（カメラの注視点に向けて移動）
	DirectX::XMFLOAT3 look = m_pCamera->GetLook();								//注視点を入手
	DirectX::XMVECTOR LookVec = DirectX::XMLoadFloat3(&look);						//注視点のベクトル
	DirectX::XMVECTOR PlayerVec = DirectX::XMLoadFloat3(&m_Pos);					//プレイヤーベクトル
	DirectX::XMVECTOR vecForward = DirectX::XMVectorSubtract(LookVec, PlayerVec);	//前方ベクトル
	vecForward = DirectX::XMVector3Normalize(vecForward);								//正規化
	DirectX::XMVECTOR vecLook = vecForward;	//プレイヤーの回転で使うためベクトルの保存

	//左ベクトル
	DirectX::XMVECTOR vecLeft = DirectX::XMVector3Cross(vecForward, vecUp);		//左ベクトル
	vecLeft = DirectX::XMVector3Normalize(vecLeft);								//正規化

	//---- 各方向の移動量をベクトルと掛け合わせる ----
	vecUp = DirectX::XMVectorScale(vecUp, m_Move[Player::MoveDirection::Up]);
	vecForward = DirectX::XMVectorScale(vecForward, m_Move[Player::MoveDirection::Forward]);
	vecLeft = DirectX::XMVectorScale(vecLeft, m_Move[Player::MoveDirection::Left]);

	//---- 各方向のベクトルを合算 ----
	DirectX::XMFLOAT3 move;
	DirectX::XMVECTOR vecXZ = DirectX::XMVectorAdd(vecForward, vecLeft);
	DirectX::XMVECTOR vec = DirectX::XMVectorAdd(vecUp, vecXZ);
	DirectX::XMStoreFloat3(&move, vec);

	//---- 最終的な移動量を位置に合わせる ----
	m_Pos.x += move.x;
	m_Pos.y += move.y;
	m_Pos.z += move.z;

	//---- 当たり判定の位置も更新 ----
	m_Collision.center = m_Pos;
}
