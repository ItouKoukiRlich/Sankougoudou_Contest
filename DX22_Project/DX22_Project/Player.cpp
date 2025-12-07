//==== インクルード部 ====
#include"Player.h"
#include"Defines.h"
#include"ShaderList.h"
#include"Input.h"

Player::Player()
	:m_pCamera(nullptr)
	,m_pModelBody(nullptr)
	,m_pModelArm(nullptr)
	,m_pModelLeg(nullptr)
{
	//---- それぞれの方向の移動量をリセット ----
	for (int i = 0; i < Player::DirectionMax; i++)
		m_Move[i] = 0.0f;

	m_pModelBody = new Model;
	if (!m_pModelBody->Load("Assets/Model/PlayerBody.fbx", 1.0f))
		MessageBox(NULL, "PlayerModel_Body", "Error", MB_OK);
}

Player::~Player()
{
	SAFEDELETE(m_pModelLeg);
	SAFEDELETE(m_pModelArm);
	SAFEDELETE(m_pModelBody);
}

void Player::Update()
{
	Control();	//操作
}

void Player::Draw()
{
	DrawBody();//本体
	//腕
	//足
}

void Player::Control()
{
	DirectX::XMFLOAT3 move;

	//---- 上下移動 ----
	if (IsKeyPress('Q'))
	{
		m_Move[Player::MoveDirection::Up] += cm_IncreaseMove;	//移動量を増やす
		if (m_Move[Player::MoveDirection::Up] > cm_MaxMove)		//一定の移動量を越えない用意に補正
			m_Move[Player::MoveDirection::Up] = cm_MaxMove;
	}
	else if (IsKeyPress('E'))
	{

		m_Move[Player::MoveDirection::Up] -= cm_IncreaseMove;
		if (m_Move[Player::MoveDirection::Up] < -cm_MaxMove)
			m_Move[Player::MoveDirection::Up] = -cm_MaxMove;
	}

	//---- 前後移動 ----
	if (IsKeyPress('W'))
	{

		m_Move[Player::MoveDirection::Forward] += cm_IncreaseMove;
		if (m_Move[Player::MoveDirection::Forward] > cm_MaxMove)
			m_Move[Player::MoveDirection::Forward] = cm_MaxMove;
	}
	else if (IsKeyPress('S'))
	{

		m_Move[Player::MoveDirection::Forward] -= cm_IncreaseMove;
		if (m_Move[Player::MoveDirection::Forward] < -cm_MaxMove)
			m_Move[Player::MoveDirection::Forward] = -cm_MaxMove;
	}

	//---- 左右移動 ----
	if (IsKeyPress('D'))
	{
		m_Move[Player::MoveDirection::Left] -= cm_IncreaseMove;
		if (m_Move[Player::MoveDirection::Left] < -cm_MaxMove)
			m_Move[Player::MoveDirection::Left] = -cm_MaxMove;
	}
	else if (IsKeyPress('A'))
	{
		m_Move[Player::MoveDirection::Left] += cm_IncreaseMove;
		if (m_Move[Player::MoveDirection::Left] > cm_MaxMove)
			m_Move[Player::MoveDirection::Left] = cm_MaxMove;
	}

	//---- ボタンが離されたらリセット ----
	if (IsKeyRelease('Q')) m_Move[Player::MoveDirection::Up]		= 0.0f;
	if (IsKeyRelease('E')) m_Move[Player::MoveDirection::Up]		= 0.0f;
	if (IsKeyRelease('W')) m_Move[Player::MoveDirection::Forward]	= 0.0f;
	if (IsKeyRelease('S')) m_Move[Player::MoveDirection::Forward]	= 0.0f;
	if (IsKeyRelease('D')) m_Move[Player::MoveDirection::Left]		= 0.0f;
	if (IsKeyRelease('A')) m_Move[Player::MoveDirection::Left]		= 0.0f;
	
	//---- 各方向のベクトルを求める ----
	
	//上ベクトル
	DirectX::XMFLOAT3	up		= { 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR	vecUp	= DirectX::XMLoadFloat3(&up);			//上ベクトル
						vecUp	= DirectX::XMVector3Normalize(vecUp);	//正規化
	
	//直進ベクトル（カメラの注視点に向けて移動）
	DirectX::XMFLOAT3 look			= m_pCamera->GetLook();								//注視点を入手
	DirectX::XMVECTOR LookVec		= DirectX::XMLoadFloat3(&look);						//注視点のベクトル
	DirectX::XMVECTOR PlayerVec		= DirectX::XMLoadFloat3(&m_Pos);					//プレイヤーベクトル
	DirectX::XMVECTOR vecForward	= DirectX::XMVectorSubtract(LookVec, PlayerVec);	//前方ベクトル
	vecForward = DirectX::XMVector3Normalize(vecForward);								//正規化
	
	//左ベクトル
	DirectX::XMVECTOR vecLeft = DirectX::XMVector3Cross(vecForward, vecUp);		//左ベクトル
	vecLeft = DirectX::XMVector3Normalize(vecLeft);								//正規化

	//---- 各方向の移動量をベクトルと掛け合わせる ----
	vecUp		= DirectX::XMVectorScale(vecUp,			m_Move[Player::MoveDirection::Up]		);
	vecForward	= DirectX::XMVectorScale(vecForward,	m_Move[Player::MoveDirection::Forward]	);
	vecLeft		= DirectX::XMVectorScale(vecLeft,		m_Move[Player::MoveDirection::Left]		);

	//---- 各方向のベクトルを合算 ----
	DirectX::XMVECTOR	vec = DirectX::XMVectorAdd(vecUp, vecForward);
						vec = DirectX::XMVectorAdd(vec, vecLeft);
	DirectX::XMStoreFloat3(&move, vec);
	
	//---- 最終的な移動量を位置に合わせる ----
	m_Pos.x += move.x;
	m_Pos.y += move.y;
	m_Pos.z += move.z;
}

void Player::DrawBody()
{
	//頂点シェーダーに渡す変換行列の変数を宣言
	DirectX::XMFLOAT4X4 fWVP[3] = {};
	fWVP[1] = m_pCamera->GetViewMatrix();
	fWVP[2] = m_pCamera->GetProjectionMatrix();

	//ワールド行列の作成
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX Translation	= DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	DirectX::XMMATRIX Scale			= DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX Ry			= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(0.0f));
	world = Scale * Ry * Translation;
	DirectX::XMStoreFloat4x4(&fWVP[0], DirectX::XMMatrixTranspose(world));

	//シェーダーへの変換行列を設定
	ShaderList::SetWVP(fWVP);

	//モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
	m_pModelBody->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pModelBody->SetPixelShader(ShaderList::GetPS(ShaderList::PS_UNLIT));

	// マテリアル別にメッシュを表示 
	for (unsigned int i = 0; i < m_pModelBody->GetMeshNum(); ++i)
	{
		// モデルのメッシュを取得 
		const Model::Mesh mesh = *m_pModelBody->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得 
		Model::Material material = *m_pModelBody->GetMaterial(mesh.materialID);

		// シェーダーへマテリアルを設定 
		ShaderList::SetMaterial(material);

		// モデルの描画 
		m_pModelBody->Draw(i);
	}
}

void Player::SetCamera(CameraGame* pCamera)
{
	m_pCamera = pCamera;
}