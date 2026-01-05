//==== インクルード部 ====
#include"SceneGame.h"
#include"Defines.h"
#include"ShaderList.h"
#include"Geometory.h"
#include"Effect.h"
#include"Input.h"

SceneGame::SceneGame()
	:m_pCamera(new CameraGame)
	,m_pPlayer(new Player)
{
	RenderTarget* pRTV = GetDefaultRTV();	//レンダーターゲット
	DepthStencil* pDSV = GetDefaultDSV();	//デプス
	SetRenderTargets(1, &pRTV, pDSV);		//レンダーターゲットを設定
	SetDepthTest(true);						//奥行を認知
	m_pPlayer->SetCamera(m_pCamera);		//プレイヤーにカメラを設定
	m_pCamera->SetPlayer(m_pPlayer);		//カメラ操作時に必要になるプレイヤーのアドレスを渡す
	EFK_INS->SetCamera(m_pCamera);			//カメラをエフェクト管理クラスに設定


	//---- 諸々のインスタンス化が終わり次第Game中のUIを作成 ----
	//プレイヤー関連のUI
	if (m_pPlayer)
	{
		m_GameUI.CreatePlayerLife(*m_pPlayer);
	}
}

SceneGame::~SceneGame()
{
	SAFEDELETE(m_pPlayer);
	SAFEDELETE(m_pCamera);
	EFK_INS->DeleteInstance();
}

void SceneGame::Update()
{
	m_pPlayer->Update();		//プレイヤー
	m_MessageWindow.Update();

	//---- 諸々の更新処理が終わってからUIに反映 ----
	m_GameUI.Update();

	m_pCamera->Update();		//ゲーム内カメラ
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
#endif

	m_pPlayer->Draw();
	m_MessageWindow.Draw();

	Effect::GetInstance()->Draw();

	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, nullptr);
	m_GameUI.Draw();
	SetRenderTargets(1, &pRTV, pDSV);

}
