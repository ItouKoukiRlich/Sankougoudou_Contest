//==== インクルード部 ====
#include"SceneGame.h"
#include"Defines.h"
#include"ShaderList.h"
#include"Geometory.h"
#include"Effect.h"
#include"Input.h"
#include"Function.h"
#include"TurretEnemy.h"
#include"NormalEnemyBullet.h"
#include"Collision.h"
using namespace nameSceneGame;
using namespace nmEnemyArray;

SceneGame::SceneGame()
	:m_pCamera(new CameraGame)
	,m_pPlayer(new Player)
	,m_pEnemy()
{
	RenderTarget* pRTV = GetDefaultRTV();	//レンダーターゲット
	DepthStencil* pDSV = GetDefaultDSV();	//デプス
	SetRenderTargets(1, &pRTV, pDSV);		//レンダーターゲットを設定
	SetDepthTest(true);						//奥行を認知
	Enemy::SetCamera(m_pCamera);
	TurretEnemy::SetPlayer(m_pPlayer);
	m_pPlayer->SetCamera(m_pCamera);		//プレイヤーにカメラを設定
	m_pCamera->SetPlayer(m_pPlayer);		//カメラ操作時に必要になるプレイヤーのアドレスを渡す
	EFK_INS->SetCamera(m_pCamera);			//カメラをエフェクト管理クラスに設定
	NEB::SetCamera(m_pPlayer);

	for (int i = 0; i < cg_MaxEnemy; i++)
	{
		if (i < cg_MaxTurret) m_pEnemy[i] = new TurretEnemy;
	}

	//---- 諸々のインスタンス化が終わり次第Game中のUIを作成 ----
	//プレイヤー関連のUI
	if (m_pPlayer)
	{
		m_GameUI.CreatePlayerLife(*m_pPlayer);
		m_GameUI.CreateWarningUI(*m_pPlayer);
	}

	CreateEnemy(nmEnemyArray::eTurret, { 0.0f, 0.0f, 10.0f });
}

SceneGame::~SceneGame()
{
	for (int i = 0; i < cg_MaxEnemy; i++)
	{
		delete m_pEnemy[i];
		m_pEnemy[i] = nullptr;
	}
	SAFEDELETE(m_pPlayer);
	SAFEDELETE(m_pCamera);
	EFK_INS->DeleteInstance();
}

void SceneGame::Update()
{
	m_pPlayer->Update();		//プレイヤー
	m_MessageWindow.Update();	//メッセージウィンドの更新処理
	m_pCamera->Update();		//ゲーム内カメラ
	for (int i = 0; i < cg_MaxEnemy; i++)
	{
		if (m_pEnemy[i]->CheckActive())
			m_pEnemy[i]->Update();
	}

	//---- ゲーム内オブジェクトの更新処理が終わってから当たり判定を確認 ----
	Collision::Sphere PlayerCollision = m_pPlayer->GetCollision();	//プレイヤーの当たり判定持ってくる
	Collision::Result result = {};
	for (int i = 0; i < cg_MaxEnemy; ++i)
	{
		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		//    敵の攻撃とプレイヤー当たり判定    //
		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		if (!m_pEnemy[i]->CheckActive()) continue;	//フィールドにいなければスキップ

		int num			= m_pEnemy[i]->GetBulletNum();	//敵の持つ弾の数を持ってくる
		if (num > 0)
		{
			Bullet* pBullet = m_pEnemy[i]->GetBullet();		//弾のポインタを持ってくる

			switch (num)
			{
			case 1:
				if (pBullet->GetActive())
				{
					result = Collision::Hit(pBullet->GetCollision(), PlayerCollision);
					if (result.isHit)
					{
						m_pPlayer->MinusHP(pBullet->GetDamage());
						pBullet->Stop();
					}
				}
				break;

			default:
				for (int i = 0; i < num; ++i) {
					if (pBullet[num].GetActive())
					{
						result = Collision::Hit(pBullet[num].GetCollision(), PlayerCollision);
						if (result.isHit)
						{
							m_pPlayer->MinusHP(pBullet[num].GetDamage());
							pBullet[num].Stop();
						}
					}
				}
				break;
			}
		}

		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		//    プレイヤーの弾と敵の当たり判定    //
		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

	}


	//---- 諸々の更新処理が終わってからUIに反映 ----
	m_GameUI.Update();
	//---- エフェクトの更新処理(最後にやる) ----
	Effect::GetInstance()->Update();
}

void SceneGame::Draw()
{
	//---- 仮置きしているボックスにカメラを設定 ----
	Geometory::SetView(m_pCamera->GetViewMatrix());
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix());

#ifdef _DEBUG
	m_pCamera->Draw();

	CreateBox({ 0.0f, 0.0f, cg_FieldHarfSize.z }, { cg_FieldSize.x, cg_FieldSize.y, 0.1f }, { 0.0f, 0.0f, 0.0f });
	CreateBox({ 0.0f, 0.0f, -cg_FieldHarfSize.z }, { cg_FieldSize.x, cg_FieldSize.y, 0.1f }, { 0.0f, 0.0f, 0.0f });
	CreateBox({ 0.0f, cg_FieldHarfSize.y, 0.0f }, { cg_FieldSize.x, 0.1f, cg_FieldSize.z }, { 0.0f, 0.0f, 0.0f });
	CreateBox({ 0.0f, -cg_FieldHarfSize.y, 0.0f }, { cg_FieldSize.x, 0.1f, cg_FieldSize.z }, { 0.0f, 0.0f, 0.0f });
	CreateBox({ cg_FieldHarfSize.x, 0.0f, 0.0f }, { 0.1f, cg_FieldSize.y, cg_FieldSize.z }, { 0.0f, 0.0f, 0.0f });
	CreateBox({ -cg_FieldHarfSize.x, 0.0f, 0.0f }, { 0.1f, cg_FieldSize.y, cg_FieldSize.z }, { 0.0f, 0.0f, 0.0f });
#endif

	m_pPlayer->Draw();
	m_MessageWindow.Draw();
	for (int i = 0; i < cg_MaxEnemy; i++)
	{
		if (m_pEnemy[i]->CheckActive())
			m_pEnemy[i]->Draw();
	}
	Effect::GetInstance()->Draw();

	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);
	m_GameUI.Draw();
	SetRenderTargets(1, &pRTV, pDSV);

}

void SceneGame::CreateEnemy(nmEnemyArray::Type type, DXf3 pos)
{
	switch (type)
	{
	case nmEnemyArray::eTurret:
		for (int i = cg_TurretStart; i < cg_MaxTurret; ++i)
		{
			//ゲームで使用中ならスキップ
			if (m_pEnemy[i]->CheckActive()) continue;
			
			//ゲームに設置
			m_pEnemy[i]->CreateEnemy(pos);
			break;
		}
		break;
	}
}
