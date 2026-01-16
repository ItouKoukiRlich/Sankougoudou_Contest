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
#include"MissionTurret.h"
#include"MissionCreate.h"
#include"MIssionNormal.h"
#include"MIssionSuper.h"
#include"MissionMain.h"
#include"NormalEnemy.h"
#include"CreateEnemy.h"
#include"SuperEnemy.h"
#include"MainEnemy.h"
#include"MainEnemyBullet.h"
using namespace nameSceneGame;
using namespace nmEnemyArray;

SceneGame::SceneGame()
	:m_pCamera(new CameraGame)
	,m_pPlayer(new Player)
	,m_pEnemy()
	,m_phase(SceneGame::Phase::eGame)
	,m_nGameOverCount(0)
	,m_nGameCount(0)
	,m_Step(GameStep::eStep1)
{
	RenderTarget* pRTV = GetDefaultRTV();	//レンダーターゲット
	DepthStencil* pDSV = GetDefaultDSV();	//デプス
	SetRenderTargets(1, &pRTV, pDSV);		//レンダーターゲットを設定
	SetDepthTest(true);						//奥行を認知
	Enemy::SetCamera(m_pCamera);
	Enemy::SetPlayer(m_pPlayer);
	MainEnemyBullet::SetPlayer(m_pPlayer);
	TurretEnemy::SetPlayer(m_pPlayer);
	EnemyIcon::SetCamera(m_pCamera);
	EnemyHP::SetCamera(m_pCamera);
	m_Clear.SetPlayer(m_pPlayer);
	m_pPlayer->SetCamera(m_pCamera);		//プレイヤーにカメラを設定
	m_pCamera->SetPlayer(m_pPlayer);		//カメラ操作時に必要になるプレイヤーのアドレスを渡す
	EFK_INS->SetCamera(m_pCamera);			//カメラをエフェクト管理クラスに設定
	Mission::Load();
	//---- 敵 ----
	for (int i = 0; i < cg_MaxEnemy; i++)
	{
		if (i < cg_MaxTurret)				  m_pEnemy[i] = new TurretEnemy;
		else if (i < cg_MaxNormal + cg_MaxTurret) m_pEnemy[i] = new NormalEnemy;
		else if (i < cg_MaxSuper + cg_MaxTurret + cg_MaxNormal) m_pEnemy[i] = new SuperEnemy;
		else if (i < cg_MaxTurret + cg_MaxNormal + cg_MaxSuper + cg_MaxCreate) m_pEnemy[i] = new CreateEnemy;
		else if (i < cg_MaxTurret + cg_MaxNormal + cg_MaxSuper + cg_MaxCreate + cg_MaxMain) m_pEnemy[i] = new MainEnemy;
	}

	//---- ミッション ----
	for (int i = 0; i < Mission::Type::eTypeMax; ++i)
	{
		switch (i)
		{
		case Mission::Type::eTurret: m_pMission[i] = new MissionTurret; break;
		case Mission::Type::eCreate: m_pMission[i] = new MissionCreate; break;
		case Mission::Type::eNormal: m_pMission[i] = new MIssionNormal; break;
		case Mission::Type::eSuper:  m_pMission[i] = new MissionSuper;  break;
		case Mission::Type::eMain:   m_pMission[i] = new MissionMain;   break;
		}
	}

	//---- 諸々のインスタンス化が終わり次第Game中のUIを作成 ----
	//プレイヤー関連のUI
	if (m_pPlayer)
	{
		m_GameUI.CreatePlayerLife(*m_pPlayer);
		m_GameUI.CreateWarningUI(*m_pPlayer);
	}

	//初期からいる敵を設置
	CreateEnemyField(eTurret, { 10.0f, 0.0f, 10.0f });
	CreateEnemyField(eTurret, { 0.0f, 0.0f, 10.0f });
	CreateEnemyField(eTurret, { -10.0f, 0.0f, 10.0f });
	
	
}

SceneGame::~SceneGame()
{
	for (int i = 0; i < cg_MaxEnemy; i++)
	{
		delete m_pEnemy[i];
		m_pEnemy[i] = nullptr;
	}
	for (int i = 0; i < Mission::Type::eTypeMax; ++i)
		SAFEDELETE(m_pMission[i]);
	SAFEDELETE(m_pPlayer);
	SAFEDELETE(m_pCamera);
	EFK_INS->DeleteInstance();
}

void SceneGame::Update()
{
	if (IsKeyTrigger('B'))
		ResetPlayer();

	m_MessageWindow.Update();	//メッセージウィンドの更新処理
	
	switch (m_phase)
	{
	case SceneGame::Phase::eGame:
		m_pPlayer->Update();		//プレイヤー
		m_pCamera->Update();		//ゲーム内カメラ
		for (int i = 0; i < cg_MaxEnemy; i++)
		{
			if (m_pEnemy[i]->CheckActive())
				m_pEnemy[i]->Update();
		}

		//---- テクスチャメッセージ ----
		TextureMessage();
		
		//---- ゲーム内オブジェクトの更新処理が終わってから当たり判定を確認 ----
		Collision();

		//---- 当たり判定を確認後ミッションクリアか確認 ----
		for (int i = 0; i < Mission::eTypeMax; ++i)
		{
			if (m_pMission[i]->CheckActive())
			{
				if (m_pMission[i]->CheckClear())
				{
					//ミッションを終了しクリアアニメーションを再生
					m_GameUI.PlayMissionEffect(MissionEffect::eAnimeClear);
					m_pMission[i]->SetActive(false);

					//敵のアイコン表示も終了
					switch (i)
					{
					case Mission::eTurret:  TurretEnemy::SetMissionFlag(false); break;
					case Mission::eCreate:  CreateEnemy::SetMissionFlag(false); break;
					case Mission::eNormal:  NormalEnemy::SetMissionFlag(false); break;
					case Mission::eSuper:   SuperEnemy::SetMissionFlag(false); break;
					case Mission::eMain:    MainEnemy::SetMissionFlag(false); break;
					}
				}
			}
		}

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

	case SceneGame::eCutIn:
		m_CutIn.Update();
		m_nGameCount++;
		if (m_nGameCount == 310)
		{
			m_nGameCount = 0;
			m_phase = eGame;
			m_Step = eStep3;
			m_CutIn.SetCutIn(CutIn::eCut2);

			//敵を生成
			CreateEnemyField(eCreate, { -10.0f, 0.0f, 10.0f });
			CreateEnemyField(eCreate, {   0.0f, 0.0f, 10.0f });
			CreateEnemyField(eCreate, {  10.0f, 0.0f, 10.0f });
			CreateEnemyField(eSuper,  { -10.0f, 0.0f, 20.0f });
			CreateEnemyField(eSuper,  {   0.0f, 0.0f, 20.0f });
			CreateEnemyField(eSuper,  {  10.0f, 0.0f, 20.0f });
			CreateEnemyField(eSuper,  {  0.0f, 5.0f, 20.0f });

			CreateNormalEnemy({   0.0f, 0.0f, 10.0f }, NormalEnemy::e21);
			CreateNormalEnemy({  20.0f, 0.0f, 20.0f }, NormalEnemy::e12tate);
			CreateNormalEnemy({ -20.0f, 0.0f, 20.0f }, NormalEnemy::e12tate);
			CreateNormalEnemy({   0.0f, 0.0f, 20.0f }, NormalEnemy::e21tate, {9.0f, 5.0f, 5.0f});
			CreateNormalEnemy({ -20.0f, 5.0f, 30.0f }, NormalEnemy::e21);
			CreateNormalEnemy({  20.0f, 5.0f, 30.0f }, NormalEnemy::e21);
			CreateNormalEnemy({ -20.0f, -5.0f, 30.0f }, NormalEnemy::e21);
			CreateNormalEnemy({  20.0f, -5.0f, 30.0f }, NormalEnemy::e21);

		}
		break;

	case SceneGame::eCutIn2:
		m_CutIn.Update();
		m_nGameCount++;
		if (m_nGameCount == 310)
		{
			m_nGameCount = 0;
			m_phase = eGame;
			m_Step = eStep5;
			m_nGameCount = 0;
			m_GameUI.PlayMissionEffect(MissionEffect::eAnimeStart);
			m_pMission[Mission::eMain]->MissionStart();
			MainEnemy::SetMissionFlag(true);
			//敵を生成
			CreateEnemyField(eMain, { -10.0f, 0.0f, 10.0f });
			CreateEnemyField(eMain, {   0.0f, 0.0f, 10.0f });
			CreateEnemyField(eMain, {  10.0f, 0.0f, 10.0f });
		}
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

	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();

#ifdef _DEBUG
	m_pCamera->Draw();

	CreateBox({ 0.0f, 0.0f, cg_FieldHarfSize.z }, { cg_FieldSize.x, cg_FieldSize.y, 0.1f }, { 0.0f, 0.0f, 0.0f });
	CreateBox({ 0.0f, 0.0f, -cg_FieldHarfSize.z }, { cg_FieldSize.x, cg_FieldSize.y, 0.1f }, { 0.0f, 0.0f, 0.0f });
	CreateBox({ 0.0f, cg_FieldHarfSize.y, 0.0f }, { cg_FieldSize.x, 0.1f, cg_FieldSize.z }, { 0.0f, 0.0f, 0.0f });
	CreateBox({ 0.0f, -cg_FieldHarfSize.y, 0.0f }, { cg_FieldSize.x, 0.1f, cg_FieldSize.z }, { 0.0f, 0.0f, 0.0f });
	CreateBox({ cg_FieldHarfSize.x, 0.0f, 0.0f }, { 0.1f, cg_FieldSize.y, cg_FieldSize.z }, { 0.0f, 0.0f, 0.0f });
	CreateBox({ -cg_FieldHarfSize.x, 0.0f, 0.0f }, { 0.1f, cg_FieldSize.y, cg_FieldSize.z }, { 0.0f, 0.0f, 0.0f });
#endif
	
	switch (m_phase)
	{
	case SceneGame::Phase::eGame:
		GameDraw();
		SetRenderTargets(1, &pRTV, nullptr);
		m_GameUI.Draw();
		SetRenderTargets(1, &pRTV, pDSV);
		break;

	case SceneGame::Phase::eGameOver:
		GameDraw();
		m_GameOver.Draw();
		break;

	case SceneGame::Phase::eDelay:
		GameDraw();
		SetRenderTargets(1, &pRTV, nullptr);
		m_GameUI.Draw();
		SetRenderTargets(1, &pRTV, pDSV);
		break;

	case SceneGame::eClear:
		GameDraw();
		SetRenderTargets(1, &pRTV, nullptr);
		m_Clear.Draw();
		SetRenderTargets(1, &pRTV, pDSV);
		break;

	case SceneGame::eCutIn:
		m_CutIn.Draw();
		SetRenderTargets(1, &pRTV, nullptr);
		m_MessageWindow.Draw();
		SetRenderTargets(1, &pRTV, pDSV);
		break;

	case SceneGame::eCutIn2:
		m_CutIn.Draw();
		SetRenderTargets(1, &pRTV, nullptr);
		m_MessageWindow.Draw();
		SetRenderTargets(1, &pRTV, pDSV);
		break;
	}
}

void SceneGame::CreateEnemyField(nmEnemyArray::Type type, DXf3 pos)
{
	switch (type)
	{
	case nmEnemyArray::eTurret:
		for (int i = cg_TurretStart; i < cg_MaxTurret; ++i){
			//ゲームで使用中ならスキップ
			if (m_pEnemy[i]->CheckActive()) continue;
			//ゲームに設置
			m_pEnemy[i]->CreateEnemy(pos);
			break;
		}
		break;

	case nmEnemyArray::eNormal:
		for (int i = cg_NormalStart; i < cg_MaxNormal + cg_MaxTurret; ++i){
			//ゲームで使用中ならスキップ
			if (m_pEnemy[i]->CheckActive()) continue;
			//ゲームに設置
			m_pEnemy[i]->CreateEnemy(pos);
			break;
		}
		break;

	case nmEnemyArray::eSuper:
		for (int i = cg_SuperStart; i < cg_MaxNormal + cg_MaxTurret + cg_MaxSuper; ++i) {
			//ゲームで使用中ならスキップ
			if (m_pEnemy[i]->CheckActive()) continue;
			//ゲームに設置
			m_pEnemy[i]->CreateEnemy(pos);
			break;
		}
		break;

	case nmEnemyArray::eCreate:
		for (int i = cg_CreateStart; i < cg_MaxNormal + cg_MaxTurret + cg_MaxSuper + cg_MaxCreate; ++i) {
			//ゲームで使用中ならスキップ
			if (m_pEnemy[i]->CheckActive()) continue;
			//ゲームに設置
			m_pEnemy[i]->CreateEnemy(pos);
			break;
		}
		break;

	case nmEnemyArray::eMain:
		for (int i = cg_MainStart; i < cg_MaxNormal + cg_MaxTurret + cg_MaxSuper + cg_MaxCreate + cg_MaxMain; ++i) {
			//ゲームで使用中ならスキップ
			if (m_pEnemy[i]->CheckActive()) continue;
			//ゲームに設置
			m_pEnemy[i]->CreateEnemy(pos);
			break;
		}
		break;
	}
}

void SceneGame::CreateNormalEnemy(DXf3 pos, int type, DXf3 move)
{
	for (int i = cg_NormalStart; i < cg_MaxNormal + cg_MaxTurret; ++i) {
		if (m_pEnemy[i]->CheckActive()) continue;
		//ゲームに設置
		m_pEnemy[i]->CreateEnemyNormal(pos, type, move);
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

		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		//    敵とプレイヤーの当たり判定    //
		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		Collision::Sphere EnemyCollision = m_pEnemy[i]->GetCollision();	//敵の当たり判定を入手
		//倒した敵がタレット型なら
		if (i < cg_MaxTurret)
		{
			
		}
		//倒した敵が小型タイプなら
		else if (i < cg_MaxTurret + cg_MaxNormal)
		{
			
		}
		else if (i < cg_MaxTurret + cg_MaxNormal + cg_MaxSuper)
		{
			result = Collision::Hit(PlayerCollision, EnemyCollision);
			if (result.isHit)
			{
				m_pEnemy[i]->Delete();	//敵を消す
				EnemyMissionCount(i);	//ミッションのカウントを進める
				m_pPlayer->MinusHP(nameEnemy::HitDamage);	//HPを削る
			}
		}

		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		//    プレイヤーの弾と敵の当たり判定    //
		//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
		//通常弾と比べる
		for (int nb = 0; nb < namePlayer::cg_MaxNormalBullet; ++nb)
		{
			if (!NormalBullet[nb].GetActive()) continue;	//発射してないなら処理しない
			result = Collision::Hit(EnemyCollision, NormalBullet[nb].GetCollision());
			if (result.isHit) {
				if (m_pEnemy[i]->MinusHP(NormalBullet[nb].GetDamage()))
					EnemyMissionCount(i);
				NormalBullet[nb].Stop();
			}
		}

		//高速弾と比べる
		for (int sb = 0; sb < namePlayer::cg_MaxSpeedBullet; ++sb)
		{
			if (!SpeedBullet[sb].GetActive()) continue;	//発射してないなら処理しない
			result = Collision::Hit(EnemyCollision, SpeedBullet[sb].GetCollision());
			if (result.isHit) {
				if (m_pEnemy[i]->MinusHP(SpeedBullet[sb].GetDamage()))
					EnemyMissionCount(i);
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

void SceneGame::EnemyMissionCount(int i)
{
	//倒した敵がタレット型なら
	if (i < cg_MaxTurret)
	{
		if (m_pMission[Mission::eTurret]->CheckActive())
			m_pMission[Mission::eTurret]->CountPlus();
	}
	//倒した敵が小型タイプなら
	else if (i < cg_MaxTurret + cg_MaxNormal)
	{
		if (m_pMission[Mission::eNormal]->CheckActive())
			m_pMission[Mission::eNormal]->CountPlus();
	}
	//倒した敵がスーパータイプなら
	else if (i < cg_MaxTurret + cg_MaxNormal + cg_MaxSuper)
	{
		if (m_pMission[Mission::eSuper]->CheckActive())
			m_pMission[Mission::eSuper]->CountPlus();
	}
	//倒した敵が作成タイプなら
	else if (i < cg_MaxTurret + cg_MaxNormal + cg_MaxSuper + cg_MaxCreate)
	{
		if (m_pMission[Mission::eCreate]->CheckActive())
			m_pMission[Mission::eCreate]->CountPlus();
	}
	//倒した敵がメインタイプなら
	else if (i < cg_MaxTurret + cg_MaxNormal + cg_MaxSuper + cg_MaxCreate + cg_MaxMain)
	{
		if (m_pMission[Mission::eMain]->CheckActive())
			m_pMission[Mission::eMain]->CountPlus();
	}
}

void SceneGame::TextureMessage()
{
	switch (m_Step)
	{
	case eStep1:
		if (m_nGameCount == 0)
			m_MessageWindow.Start(MessageWindow::eMission1);
		else if (m_nGameCount == 200)
		{
			m_GameUI.PlayMissionEffect(MissionEffect::eAnimeStart);
			m_pMission[Mission::eTurret]->MissionStart();
			TurretEnemy::SetMissionFlag(true);
			m_nGameCount = 0;
			m_Step = eStep2;
		}
		m_nGameCount++;
		break;

	case eStep2:
		//ミッションをクリアしているか確認
		if (m_pMission[Mission::eTurret]->CheckClear()) {
			m_nGameCount++;
			if (m_nGameCount > 180)
			{
				m_nGameCount = 0;
				m_phase = eCutIn;	//カットインが入る
				m_MessageWindow.Start(MessageWindow::eCutin1);
			}
		}
		break;

	case eStep3:
		if (m_nGameCount == 10)
			m_MessageWindow.Start(MessageWindow::eMission3);
		else if (m_nGameCount == 210)
		{
			m_GameUI.PlayMissionEffect(MissionEffect::eAnimeStart);
			m_pMission[Mission::eNormal]->MissionStart();
			m_pMission[Mission::eCreate]->MissionStart();
			m_pMission[Mission::eSuper ]->MissionStart();
			NormalEnemy::SetMissionFlag(true);
			CreateEnemy::SetMissionFlag(true);
			SuperEnemy::SetMissionFlag(true);
		}
		else if (m_nGameCount == 410)
			m_MessageWindow.Start(MessageWindow::eMission2);
		else if (m_nGameCount == 700)
		{
			m_MessageWindow.Start(MessageWindow::eSuper);
			m_nGameCount = 0;
			m_Step = eStep4;
		}
		m_nGameCount++;
		break;

	case eStep4:
		//クリアしているか条件を調べる
		if (m_pMission[Mission::eNormal]->CheckClear() &&
			m_pMission[Mission::eCreate]->CheckClear() &&
			m_pMission[Mission::eSuper]->CheckClear())
		{
			m_nGameCount++;
			if (m_nGameCount > 180)
			{
				m_nGameCount = 0;
				m_phase = eCutIn2;
				m_MessageWindow.Start(MessageWindow::eMission4);
			}
		}
		break;

	case eStep5:
		if (m_pMission[Mission::eMain]->CheckClear())
		{
			m_nGameCount++;
			if (m_nGameCount > 180)
			{
				m_phase = eClear;
			}
		}
	}
}

void SceneGame::ResetPlayer()
{
	m_pPlayer->SetPos({ 0.0f, 0.0f, 0.0f });	//プレイヤーの位置を初期位置に戻す
	m_pCamera->SetLook({ 0.0f, 0.0f, 5.65f });
	m_pCamera->SetPos({ 0, -2.5, -5.65 });
	m_pCamera->RessetDegree();
	m_pPlayer->Resste();//プレイヤーの移動量・モードもリセット
}

void SceneGame::GameDraw()
{
	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();

	m_pPlayer->Draw();
	for (int i = 0; i < cg_MaxEnemy; i++)
	{
		if (m_pEnemy[i]->CheckActive())
			m_pEnemy[i]->Draw();
	}
	Effect::GetInstance()->Draw();

	//==== ミッションの描画 ====
	SetRenderTargets(1, &pRTV, nullptr);
	m_MessageWindow.Draw();
	Mission::MissionMenuDraw();
	for (int i = 0; i < Mission::Type::eTypeMax; ++i)
		m_pMission[i]->Draw();
	Mission::CountRisset();
	SetRenderTargets(1, &pRTV, pDSV);
}
