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
#include"EnemyIcon.h"
#include"EnemyHP.h"
using namespace nameSceneGame;
using namespace nmEnemyArray;

SceneGame::SceneGame()
	:m_pCamera(new CameraGame)
	,m_pPlayer(new Player)
	,m_pMission(new Mission)
	,m_pEnemy()
	,m_phase(SceneGame::Phase::eGame)
	,m_nGameOverCount(0)
{
	RenderTarget* pRTV = GetDefaultRTV();	//レンダーターゲット
	DepthStencil* pDSV = GetDefaultDSV();	//デプス
	SetRenderTargets(1, &pRTV, pDSV);		//レンダーターゲットを設定
	SetDepthTest(true);						//奥行を認知
	Enemy::SetCamera(m_pCamera);
	TurretEnemy::SetPlayer(m_pPlayer);
	EnemyIcon::SetCamera(m_pCamera);
	EnemyHP::SetCamera(m_pCamera);
	m_Clear.SetPlayer(m_pPlayer);
	m_pPlayer->SetCamera(m_pCamera);		//プレイヤーにカメラを設定
	m_pCamera->SetPlayer(m_pPlayer);		//カメラ操作時に必要になるプレイヤーのアドレスを渡す
	EFK_INS->SetCamera(m_pCamera);			//カメラをエフェクト管理クラスに設定

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
	SAFEDELETE(m_pMission);
	SAFEDELETE(m_pPlayer);
	SAFEDELETE(m_pCamera);
	EFK_INS->DeleteInstance();
}

void SceneGame::Update()
{
	switch (m_phase)
	{
	case SceneGame::Phase::eGame:
		m_pPlayer->Update();		//プレイヤー
		m_MessageWindow.Update();	//メッセージウィンドの更新処理
		m_pCamera->Update();		//ゲーム内カメラ
		for (int i = 0; i < cg_MaxEnemy; i++)
		{
			if (m_pEnemy[i]->CheckActive())
				m_pEnemy[i]->Update();
		}

		//---- ゲーム内オブジェクトの更新処理が終わってから当たり判定を確認 ----
		Collision();
		//---- 諸々の更新処理が終わってからUIに反映 ----
		m_GameUI.Update();

		//---- 最後にゲームオーバーかを判定 ----
		if (m_pPlayer->GetHP() == 0) StartGameOver(GameOver::Type::eDead);
		else if (CheckOutField()) StartGameOver(GameOver::Type::eOutField);
		break;

	case SceneGame::Phase::eGameOver:
		m_GameOver.Update();
		break;

	case SceneGame::Phase::eDelay:
		m_nGameOverCount++;
		if (m_nGameOverCount == 30) m_phase = eGameOver;
		m_MessageWindow.Update();	//メッセージウィンドの更新処理
		m_pCamera->Update();		//ゲーム内カメラ
		for (int i = 0; i < cg_MaxEnemy; i++)
		{
			if (m_pEnemy[i]->CheckActive())
				m_pEnemy[i]->Update();
		}
		//---- 諸々の更新処理が終わってからUIに反映 ----
		m_GameUI.Update();
		break;

	case SceneGame::eClear:
		m_Clear.Update();
		break;
	}

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
	m_pMission->Draw();

	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	switch (m_phase)
	{
	case SceneGame::Phase::eGame:
		SetRenderTargets(1, &pRTV, nullptr);
		m_GameUI.Draw();
		SetRenderTargets(1, &pRTV, pDSV);
		break;

	case SceneGame::Phase::eGameOver:
		m_GameOver.Draw();
		break;

	case SceneGame::Phase::eDelay:
		SetRenderTargets(1, &pRTV, nullptr);
		m_GameUI.Draw();
		SetRenderTargets(1, &pRTV, pDSV);
		break;

	case SceneGame::eClear:
		SetRenderTargets(1, &pRTV, nullptr);
		m_Clear.Draw();
		SetRenderTargets(1, &pRTV, pDSV);
		break;
	}
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

void SceneGame::Collision()
{
	Collision::Sphere PlayerCollision = m_pPlayer->GetCollision();	//プレイヤーの当たり判定持ってくる
	NormalBullet* NormalBullet = m_pPlayer->GetNormalBullet();	//プレイヤーの弾情報を持ってくる
	SpeedBullet* SpeedBullet = m_pPlayer->GetSpeedBullet();	//プレイヤーのスピード弾情報を持ってくる
	Collision::Result result = {};
	for (int i = 0; i < cg_MaxEnemy; ++i)
	{
		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		//    敵の攻撃とプレイヤー当たり判定    //
		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		if (!m_pEnemy[i]->CheckActive()) continue;	//フィールドにいなければスキップ

		int num = m_pEnemy[i]->GetBulletNum();	//敵の持つ弾の数を持ってくる
		if (num > 0)
		{
			Bullet* pBullet = m_pEnemy[i]->GetBullet();		//弾のポインタを持ってくる

			//---- 弾とプレイヤーを比べる(無敵なら処理しない) ----
			if (!m_pPlayer->GetBarrier())
			{
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
		}

		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		//    プレイヤーの弾と敵の当たり判定    //
		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		Collision::Sphere EnemyCollision = m_pEnemy[i]->GetCollision();	//敵の当たり判定を入手

		//通常弾と比べる
		for (int nb = 0; nb < namePlayer::cg_MaxNormalBullet; ++nb)
		{
			if (!NormalBullet[nb].GetActive()) continue;	//発射してないなら処理しない
			result = Collision::Hit(EnemyCollision, NormalBullet[nb].GetCollision());
			if (result.isHit) {
				m_pEnemy[i]->MinusHP(NormalBullet[nb].GetDamage());
				NormalBullet[nb].Stop();
			}
		}

		//高速弾と比べる
		for (int sb = 0; sb < namePlayer::cg_MaxSpeedBullet; ++sb)
		{
			if (!SpeedBullet[sb].GetActive()) continue;	//発射してないなら処理しない
			result = Collision::Hit(EnemyCollision, SpeedBullet[sb].GetCollision());
			if (result.isHit) {
				m_pEnemy[i]->MinusHP(SpeedBullet[sb].GetDamage());
				SpeedBullet[sb].Stop();
			}
		}
	}
}

void SceneGame::StartGameOver(GameOver::Type type)
{
	m_nGameOverCount = 0;
	m_phase = SceneGame::Phase::eDelay;
	m_GameOver.Start(type);
}

bool SceneGame::CheckOutField()
{
	//プレイヤーがフィールドの端に近づいていないか確認
	DXf3 pos = m_pPlayer->GetPos();
	if (pos.x >  nameSceneGame::cg_FieldHarfSize.x ||
		pos.x < -nameSceneGame::cg_FieldHarfSize.x ||
		pos.y >  nameSceneGame::cg_FieldHarfSize.x ||
		pos.y < -nameSceneGame::cg_FieldHarfSize.y ||
		pos.z >  nameSceneGame::cg_FieldHarfSize.z ||
		pos.z < -nameSceneGame::cg_FieldHarfSize.z)
	{
		return true;
	}
	return false;
}
