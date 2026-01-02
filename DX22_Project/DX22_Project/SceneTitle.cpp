//==== インクルード部 ====
#include"SceneTitle.h"
#include"CameraTitle.h"
#include"Geometory.h"
#include"Input.h"

//==== 定数・マクロ定義 ====
#define SmokeJetEFK u"Assets/Effect/SmokeJet.efkefc"				//エフェクトのファイルパス

constexpr int	cg_SmokeJetIntervalFrames	= 300;					//何フレーム置きにエフェクトがでるか
constexpr DXf3	cg_SmokeJetPos				= {0.0f, 1.0f, 0.0f};	//ジェットエフェクトの位置

SceneTitle::SceneTitle()
	:m_pCamera(nullptr)
	,m_Hndle(-1)
	,m_nEffectCount(240)
{
	m_pCamera = new CameraTitle;
	EFK_INS->SetCamera(m_pCamera);
	EFK_INS->Load(SmokeJetEFK);
}

SceneTitle::~SceneTitle()
{
	EFK_INS->DeleteInstance();
	SAFE_DELETE(m_pCamera);
}

void SceneTitle::Update()
{
	m_pCamera->Update();
	m_TitleUI.Update();
	
	//エフェクトのカウントを進める
	m_nEffectCount++;
	if (m_nEffectCount >= cg_SmokeJetIntervalFrames)
	{
		m_nEffectCount = 0;
		EFK_INS->Play(SmokeJetEFK, cg_SmokeJetPos, &m_Hndle);
	}

#ifdef _DEBUG
	//デバッグ時はPでエフェクト発射可能
	if (IsKeyTrigger('P'))
	{
		EFK_INS->Play(SmokeJetEFK, cg_SmokeJetPos, &m_Hndle);
	}
#endif

	EFK_INS->Update();
}

void SceneTitle::Draw()
{
	//---- 仮置きしているボックスにカメラを設定 ----
	Geometory::SetView(m_pCamera->GetViewMatrix());
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix());
	m_TitleUI.Draw();
	EFK_INS->Draw();
}