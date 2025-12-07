//==== インクルード部 ====
#include"SceneGame.h"
#include"Defines.h"
#include"ShaderList.h"
#include"Geometory.h"

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
}

SceneGame::~SceneGame()
{
	SAFEDELETE(m_pPlayer);
	SAFEDELETE(m_pCamera);
}

void SceneGame::Update()
{
	m_pPlayer->Update();		//プレイヤー
	m_pCamera->Update();		//ゲーム内カメラ
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
}
