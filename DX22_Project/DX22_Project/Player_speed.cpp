//==== インクルード部 ====
#include"Player.h"
#include"Defines.h"
#include"ShaderList.h"
#include"Input.h"
#include"DirectX.h"
#include"Effect.h"
#include"Function.h"
using namespace namePlayer;

//==== 定数・マクロ定義 ====
namespace nmPlayer_Speed
{
	constexpr float cg_Speed = 0.1f;
	constexpr float cg_SpeedUpVal	= 0.5f;
	constexpr float cg_SpeedDownVal = -0.05f;
	constexpr float cg_SpeedSide = 0.01f;
}
using namespace nmPlayer_Speed;

void Player::Control_speed()
{
	//---- スピード状態は常に前進 ----
	m_Move[Player::MoveDirection::Forward] = cg_Speed;

	//---- 上下移動 ----
	if (IsKeyPress('Q'))
	{
		m_LookY += 0.01f;
		if (m_LookY > 0.5f) m_LookY = 0.5f;
	}
	else if (IsKeyPress('E'))
	{
		m_LookY -= 0.01f;
		if (m_LookY < -0.5f) m_LookY = -0.5f;
	}

	//注視点の位置でプレイヤーの回転する角度を求める
	if (m_LookY > 0.0f)
	{
		float ratio = m_LookY / 1.0f;
		float Angle = 20.0f * ratio;
		m_Angle.x	= DirectX::XMConvertToRadians(90.0f - Angle);
	}
	else if (m_LookY < 0.0f)
	{
		float y = m_LookY * -1.0f;
		float ratio = y / 1.0f;
		float Angle = 20.0f * ratio;
		m_Angle.x = DirectX::XMConvertToRadians(90.0f + Angle);
	}

	//---- 速度処理 ----
	if (IsKeyPress('W'))
	{//スピードアップ
		m_Move[Player::MoveDirection::Forward] += cg_SpeedUpVal;
	}
	else if (IsKeyPress('S'))
	{//スピードダウン
		m_Move[Player::MoveDirection::Forward] += cg_SpeedDownVal;
	}

	//---- 左右移動 ----
	if (IsKeyPress('D'))
	{
		m_Move[Player::MoveDirection::Left] = -cg_SpeedSide;
	}
	else if (IsKeyPress('A'))
	{
		m_Move[Player::MoveDirection::Left] = cg_SpeedSide;
	}

	//---- ボタンが離されたらリセット ----
	if (IsKeyRelease('Q')) m_Move[Player::MoveDirection::Up] = 0.0f;
	if (IsKeyRelease('E')) m_Move[Player::MoveDirection::Up] = 0.0f;
	if (IsKeyRelease('D')) m_Move[Player::MoveDirection::Left] = 0.0f;
	if (IsKeyRelease('A')) m_Move[Player::MoveDirection::Left] = 0.0f;

	//---- 弾の発射 ----
	m_SpeedBulletCount++;
	if (IsKeyPress(VK_SPACE))
	{
		if (m_SpeedBulletCount >= cm_SpeedBulletDelay)
		{
			m_SpeedBulletCount = 0;

			for (int i = 0; i < cg_MaxSpeedBullet; ++i)
			{
				//発射中か確認
				if (m_pSpeedBullet[i].GetActive()) continue;

				//直進ベクトル（カメラの注視点に向けて移動）
				DirectX::XMFLOAT3 look = m_pCamera->GetLook();
				look.y += m_LookY;
				DirectX::XMVECTOR LookVec = DirectX::XMLoadFloat3(&look);
				DirectX::XMVECTOR PlayerVec = DirectX::XMLoadFloat3(&m_Pos);
				DirectX::XMVECTOR vecForward = DirectX::XMVectorSubtract(LookVec, PlayerVec);
				vecForward = DirectX::XMVector3Normalize(vecForward);

				//	//弾のベクトルを入手
				DirectX::XMVECTOR vec = GetForwardVec();

				//弾の位置を設定(プレイヤーの前)
				float angle = Y_LotationVec(vec);
				DirectX::XMFLOAT3 Pos = {};
				Pos.x = m_Pos.x + 1.0f * sinf(angle);
				Pos.y = m_Pos.y + m_LookY;
				Pos.z = m_Pos.z + 1.0f * cosf(angle);

				m_pSpeedBullet[i].CreateBullet(vecForward, Pos);
				break;
			}
		}
		
	}

	//---- モード切替 ----
	if (IsKeyTrigger(VK_TAB))
	{
		m_Move[Player::MoveDirection::Forward] = 0.0f;
		ChangeMode();
	}
}

void Player::Move_speed()
{
	//---- 各方向のベクトルを求める ----

	//上ベクトル
	DirectX::XMFLOAT3	up = { 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR	vecUp = DirectX::XMLoadFloat3(&up);			//上ベクトル
	vecUp = DirectX::XMVector3Normalize(vecUp);	//正規化

	//直進ベクトル（カメラの注視点に向けて移動）
	DirectX::XMFLOAT3 look = m_pCamera->GetLook();									//注視点を入手
	look.y += m_LookY;
	DirectX::XMVECTOR LookVec = DirectX::XMLoadFloat3(&look);						//注視点のベクトル
	DirectX::XMVECTOR PlayerVec = DirectX::XMLoadFloat3(&m_Pos);					//プレイヤーベクトル
	DirectX::XMVECTOR vecForward = DirectX::XMVectorSubtract(LookVec, PlayerVec);	//前方ベクトル
	vecForward = DirectX::XMVector3Normalize(vecForward);							//正規化
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

	if (m_State == eNormal || m_State == eSpeed)
	{
		//---- プレイヤーの回転 ----
		//常に正面を向くようにする
		//角度０のベクトルを求める
		DirectX::XMFLOAT3 fZ = { 0.0f, 0.0f, 1.0f };
		DirectX::XMVECTOR vecZ = DirectX::XMLoadFloat3(&fZ);			//デフォルトの方向ベクトル
		vecZ = DirectX::XMVector3Normalize(vecZ);	//正規化

		float x1 = DirectX::XMVectorGetX(vecZ);
		float y1 = DirectX::XMVectorGetZ(vecZ);
		float x2 = DirectX::XMVectorGetX(vecLook);
		float y2 = DirectX::XMVectorGetZ(vecLook);

		float dot = x1 * x2 + y1 * y2;
		float det = x1 * y2 - y1 * x2;

		m_Angle.y = atan2f(det, dot);
		m_Angle.y *= -1.0f;
	}
}

void Player::DrawBody_speed()
{
	//頂点シェーダーに渡す変換行列の変数を宣言
	DirectX::XMFLOAT4X4 fWVP[3] = {};
	fWVP[1] = m_pCamera->GetViewMatrix();
	fWVP[2] = m_pCamera->GetProjectionMatrix();

	//ワールド行列の作成
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX Translation = DirectX::XMMatrixTranslation(m_Pos.x, m_Pos.y, m_Pos.z);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 0.5f);
	DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(m_Angle.x);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_Angle.y);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_Angle.z);
	world = Scale * Rx * Ry * Rz * Translation;
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

void Player::DrawArm_speed()
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
		(m_Pos.x + ArmPos.x * cosY,
			m_Pos.y + ArmPos.y,
			m_Pos.z + ArmPos.x * sinY);
		//拡大縮小行列(左右でモデルが反転する)
		DirectX::XMMATRIX Scale;
		switch (i)
		{
		case 0: Scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f); break;
		case 1: Scale = DirectX::XMMatrixScaling(-1.0f, 1.0f, 1.0f); break;
		}
		//回転行列
		DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(m_Angle.x);
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

void Player::DrawLeg_speed()
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
		(m_Pos.x + LegPos.x * cosA,
			m_Pos.y,
			m_Pos.z + LegPos.y);
		//拡大縮小行列(左右でモデルが反転する)
		DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(1.0f, 2.0f, 1.0f);
		//回転行列
		DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(m_Angle.x);
		DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_Angle.y);
		DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_Angle.z);
		world = Scale * Rx * Ry * Rz * Translation;
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