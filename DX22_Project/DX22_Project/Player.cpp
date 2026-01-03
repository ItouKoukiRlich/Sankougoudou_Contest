//==== インクルード部 ====
#include"Player.h"
#include"Defines.h"
#include"ShaderList.h"
#include"Input.h"
#include"DirectX.h"
#include"Effect.h"

#define FILEPPP u"Assets/Effect/Heal.efkefc"
#define FILEDEM u"Assets/Effect/demo.efkefc"

Player::Player()
	:m_pCamera(nullptr)
	,m_pModelBody(nullptr)
	,m_pModelArm(nullptr)
	,m_pModelLeg(nullptr)
	,m_Angle(DirectX::XMFLOAT3{0.0f, 0.0f, 0.0f})
	,m_ArmAngle(0.0f)
{
	//---- それぞれの方向の移動量をリセット ----
	for (int i = 0; i < Player::DirectionMax; i++)
		m_Move[i] = 0.0f;

	m_pModelBody = new Model;
	if (!m_pModelBody->Load("Assets/Model/PlayerBody.fbx", 1.0f))
		MessageBox(NULL, "PlayerModel_Body", "Error", MB_OK);

	m_pModelArm = new Model;
	if (!m_pModelArm->Load("Assets/Model/PlayerArm.fbx", 0.5f))
		MessageBox(NULL, "PlayerModel_Arm", "Error", MB_OK);

	m_pModelLeg = new Model;
	if (!m_pModelLeg->Load("Assets/Model/PlayerLeg.fbx", 0.5f))
		MessageBox(NULL, "PlayerModel_Leg", "Error", MB_OK);
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
	Move();		//移動計算
}

void Player::Draw()
{
	DrawBody();	//本体
	DrawArm();	//腕
	DrawLeg();	//足
}

void Player::Control()
{
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
		//腕が傾くように補正を掛ける
		m_ArmAngle += cm_ChangeArmAngle;
		if (m_ArmAngle > cm_MaxArmAngle) m_ArmAngle = cm_MaxArmAngle;

		//移動量を掛ける
		m_Move[Player::MoveDirection::Forward] += cm_IncreaseMove;
		if (m_Move[Player::MoveDirection::Forward] > cm_MaxMove)
			m_Move[Player::MoveDirection::Forward] = cm_MaxMove;
	}
	else if (IsKeyPress('S'))
	{
		//腕が傾くように補正を掛ける
		m_ArmAngle -= cm_ChangeArmAngle;
		if (m_ArmAngle < -cm_MaxArmAngle) m_ArmAngle = -cm_MaxArmAngle;

		m_Move[Player::MoveDirection::Forward] -= cm_IncreaseMove;
		if (m_Move[Player::MoveDirection::Forward] < -cm_MaxMove)
			m_Move[Player::MoveDirection::Forward] = -cm_MaxMove;
	}

	//---- 左右移動 ----
	if (IsKeyPress('D'))
	{
		//体が傾くように補正を掛ける
		//m_Angle.z -= cm_ChangeAngle;
		//if (m_Angle.z < -cm_MaxBodyAngleZ) m_Angle.z = -cm_MaxBodyAngleZ;

		m_Move[Player::MoveDirection::Left] -= cm_IncreaseMove;
		if (m_Move[Player::MoveDirection::Left] < -cm_MaxMove)
			m_Move[Player::MoveDirection::Left] = -cm_MaxMove;
	}
	else if (IsKeyPress('A'))
	{
		//体が傾くように補正を掛ける
		//m_Angle.z += cm_ChangeAngle;
		//if (m_Angle.z > cm_MaxBodyAngleZ) m_Angle.z = cm_MaxBodyAngleZ;

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

	//---- 手の角度が自動的に戻るようにする ----
	//角度が０度より大きかったらマイナス。
	//０より小さかったらプラス
	//角度の変更料が同じだと変化しないため変更料に補正を掛ける
	if (m_ArmAngle >= 0.0f) m_ArmAngle -= cm_ChangeArmAngle * 0.5f;
	else m_ArmAngle += cm_ChangeArmAngle * 0.5f;

	//---- Z軸回転をしている場合、自動的に戻るようにする ----
	if (m_Angle.z > 0.0f)		m_Angle.z -= cm_ChangeAngle * 0.5f;
	else if (m_Angle.z < 0.0f)	m_Angle.z += cm_ChangeAngle * 0.5f;
	m_Angle.z = DirectX::XMConvertToRadians(m_Angle.z);	//最終的な角度をラジアンに変換
}

void Player::Move()
{
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
	DirectX::XMVECTOR vecLook = vecForward;	//プレイヤーの回転で使うためベクトルの保存

	//左ベクトル
	DirectX::XMVECTOR vecLeft = DirectX::XMVector3Cross(vecForward, vecUp);		//左ベクトル
	vecLeft = DirectX::XMVector3Normalize(vecLeft);								//正規化

	//---- 各方向の移動量をベクトルと掛け合わせる ----
	vecUp		= DirectX::XMVectorScale(vecUp, m_Move[Player::MoveDirection::Up]);
	vecForward	= DirectX::XMVectorScale(vecForward, m_Move[Player::MoveDirection::Forward]);
	vecLeft		= DirectX::XMVectorScale(vecLeft, m_Move[Player::MoveDirection::Left]);

	//---- 各方向のベクトルを合算 ----
	DirectX::XMFLOAT3 move;
	DirectX::XMVECTOR vecXZ = DirectX::XMVectorAdd(vecForward, vecLeft);
	DirectX::XMVECTOR vec	= DirectX::XMVectorAdd(vecUp, vecXZ);
	DirectX::XMStoreFloat3(&move, vec);

	//---- 最終的な移動量を位置に合わせる ----
	m_Pos.x += move.x;
	m_Pos.y += move.y;
	m_Pos.z += move.z;

	//---- プレイヤーの回転 ----
	//常に正面を向くようにする
	//角度０のベクトルを求める
	DirectX::XMFLOAT3 fZ	= { 0.0f, 0.0f, 1.0f };
	DirectX::XMVECTOR vecZ	= DirectX::XMLoadFloat3(&fZ);			//デフォルトの方向ベクトル
	vecZ					= DirectX::XMVector3Normalize(vecZ);	//正規化

	float x1 = DirectX::XMVectorGetX(vecZ);
	float y1 = DirectX::XMVectorGetZ(vecZ);
	float x2 = DirectX::XMVectorGetX(vecLook);
	float y2 = DirectX::XMVectorGetZ(vecLook);

	float dot = x1 * x2 + y1 * y2;
	float det = x1 * y2 - y1 * x2;

	m_Angle.y = atan2f(det, dot);
	m_Angle.y *= -1.0f;
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
	DirectX::XMMATRIX Scale			= DirectX::XMMatrixScaling(1.0f, 1.0f, 0.5f);
	DirectX::XMMATRIX Ry			= DirectX::XMMatrixRotationY(m_Angle.y);
	DirectX::XMMATRIX Rz			= DirectX::XMMatrixRotationZ(m_Angle.z);
	world = Scale * Ry * Rz * Translation;
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

void Player::DrawArm()
{
	//頂点シェーダーに渡す変換行列の変数を宣言
	DirectX::XMFLOAT4X4 fWVP[3] = {};
	fWVP[1] = m_pCamera->GetViewMatrix();
	fWVP[2] = m_pCamera->GetProjectionMatrix();

	//---- 注視点の位置 ----
	float cosY = cosf(-m_Angle.y);
	float sinY = sinf(-m_Angle.y);

	for (int i = 0; i < 2; i++)
	{
		//腕の位置を決定（2個目の描画時は位置が逆になる）
		DirectX::XMFLOAT2 ArmPos = cm_ArmPos;
		if (i == 1) ArmPos = DirectX::XMFLOAT2{ -cm_ArmPos.x, cm_ArmPos.y };

		//==== ワールド行列の作成 ====
		DirectX::XMMATRIX world;
		
		//移動行列
		DirectX::XMMATRIX Translation = DirectX::XMMatrixTranslation
		(	m_Pos.x + ArmPos.x * cosY,
			m_Pos.y + ArmPos.y,
			m_Pos.z + ArmPos.x * sinY);
		//拡大縮小行列(左右でモデルが反転する)
		DirectX::XMMATRIX Scale;
		switch (i)
		{
		case 0: Scale = DirectX::XMMatrixScaling( 1.0f, 1.0f, 1.0f); break;
		case 1: Scale = DirectX::XMMatrixScaling(-1.0f, 1.0f, 1.0f); break;
		}
		//回転行列
		DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_ArmAngle));
		DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_Angle.y);
		DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_Angle.z);
		world = Scale * Rx * Ry * Rz * Translation;
		DirectX::XMStoreFloat4x4(&fWVP[0], DirectX::XMMatrixTranspose(world));

		//シェーダーへの変換行列を設定
		ShaderList::SetWVP(fWVP);

		//モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
		m_pModelArm->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pModelArm->SetPixelShader(ShaderList::GetPS(ShaderList::PS_UNLIT));

		// マテリアル別にメッシュを表示 
		for (unsigned int i = 0; i < m_pModelArm->GetMeshNum(); ++i)
		{
			// モデルのメッシュを取得 
			const Model::Mesh mesh = *m_pModelArm->GetMesh(i);

			// メッシュに割り当てられているマテリアルを取得 
			Model::Material material = *m_pModelArm->GetMaterial(mesh.materialID);

			// シェーダーへマテリアルを設定 
			ShaderList::SetMaterial(material);

			// モデルの描画 
			m_pModelArm->Draw(i);
		}
	}
}

void Player::DrawLeg()
{
	//頂点シェーダーに渡す変換行列の変数を宣言
	DirectX::XMFLOAT4X4 fWVP[3] = {};
	fWVP[1] = m_pCamera->GetViewMatrix();
	fWVP[2] = m_pCamera->GetProjectionMatrix();

	//---- 注視点の位置 ----
	float cosA = cosf(-m_Angle.y);
	float sinA = sinf(-m_Angle.y);

	for (int i = 0; i < 2; i++)
	{
		//腕の位置を決定（2個目の描画時は位置が逆になる）
		DirectX::XMFLOAT2 LegPos = cm_LegPos;
		if (i == 1) LegPos = DirectX::XMFLOAT2{ -cm_LegPos.x, cm_LegPos.y };

		//==== ワールド行列の作成 ====
		DirectX::XMMATRIX world;

		//移動行列
		DirectX::XMMATRIX Translation = DirectX::XMMatrixTranslation
		(	m_Pos.x + LegPos.x * cosA,
			m_Pos.y + LegPos.y,
			m_Pos.z + LegPos.x * sinA);
		//拡大縮小行列(左右でモデルが反転する)
		DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(1.0f, 2.0f, 1.0f);
		//回転行列
		DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_Angle.y);
		DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(m_Angle.z));
		world = Scale * Ry * Rz * Translation;
		DirectX::XMStoreFloat4x4(&fWVP[0], DirectX::XMMatrixTranspose(world));

		//シェーダーへの変換行列を設定
		ShaderList::SetWVP(fWVP);

		//モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
		m_pModelLeg->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pModelLeg->SetPixelShader(ShaderList::GetPS(ShaderList::PS_UNLIT));

		// マテリアル別にメッシュを表示 
		for (unsigned int i = 0; i < m_pModelLeg->GetMeshNum(); ++i)
		{
			// モデルのメッシュを取得 
			const Model::Mesh mesh = *m_pModelLeg->GetMesh(i);

			// メッシュに割り当てられているマテリアルを取得 
			Model::Material material = *m_pModelLeg->GetMaterial(mesh.materialID);

			// シェーダーへマテリアルを設定 
			ShaderList::SetMaterial(material);

			// モデルの描画 
			m_pModelLeg->Draw(i);
		}
	}
}

void Player::SetCamera(CameraGame* pCamera)
{
	m_pCamera = pCamera;
}