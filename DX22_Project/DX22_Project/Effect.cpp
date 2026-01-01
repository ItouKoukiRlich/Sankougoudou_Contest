//==== インクルード部 ====
#include"Effect.h"
#include"DirectX.h"

Effect* Effect::m_pInstance = nullptr;

Effect::Effect()
	:m_efkTime(0)
	,m_pCamera(nullptr)
{
	//マネージャーの作成
	m_efkManager = Effekseer::Manager::Create(8000);
	
	//レンダラーの作成
	auto graphicsDevice = EffekseerRendererDX11::CreateGraphicsDevice(GetDevice(), GetContext());
	m_efkRenderer = EffekseerRendererDX11::Renderer::Create(graphicsDevice, 8000);
	m_efkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
	m_efkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
	m_efkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
	m_efkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
	m_efkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());
	m_efkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader());
	m_efkManager->SetModelLoader(m_efkRenderer->CreateModelLoader());
	m_efkManager->SetMaterialLoader(m_efkRenderer->CreateMaterialLoader());
	m_efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

Effect::~Effect()
{

}

void Effect::Update()
{
	//---- ループ再生するエフェクトの再生が終わっていないか確認 ----
	for (std::vector<Effect::LoopData>::iterator it = m_LoopHandle.begin(); it != m_LoopHandle.end(); ++it)
	{
		if (!m_efkManager->Exists(*(*it).pHandle))
		{
			//再生が終わっていたら再度再生(位置が動かない場合は保存してある位置から開始)
			if (!(*it).pPos)
				*(*it).pHandle = m_efkManager->Play(m_Effect.at((*it).file), (*it).pos);
			else
				*(*it).pHandle = m_efkManager->Play(m_Effect.at((*it).file), DXfloat3ToEFv3d(*(*it).pPos));
		}
	}

	//カメラが設定されていなければ処理しない
	if (!m_pCamera) return;

	//必要な情報を作成
	DirectX::XMFLOAT3	CameraPos	= m_pCamera->GetPos();
	DirectX::XMFLOAT3	CameraLook	= m_pCamera->GetLook();
	Effekseer::Matrix44 cameraMatrix;
	cameraMatrix.LookAtLH(	{ CameraPos.x, CameraPos.y, CameraPos.z },
							{ CameraLook.x, CameraLook.y, CameraLook.z },
							{ 0.0f, 1.0f, 0.0f });

	DirectX::XMFLOAT4X4 m = m_pCamera->GetProjectionMatrix(false);
	Effekseer::Matrix44 cm;
	cm.Values[0][0] = m._11;
	cm.Values[0][1] = m._12;
	cm.Values[0][2] = m._13;
	cm.Values[0][3] = m._14;
	cm.Values[1][0] = m._21;
	cm.Values[1][1] = m._22;
	cm.Values[1][2] = m._23;
	cm.Values[1][3] = m._24;
	cm.Values[2][0] = m._31;
	cm.Values[2][1] = m._32;
	cm.Values[2][2] = m._33;
	cm.Values[2][3] = m._34;
	cm.Values[3][0] = m._41;
	cm.Values[3][1] = m._42;
	cm.Values[3][2] = m._43;
	cm.Values[3][3] = m._44;


	Effekseer::Manager::LayerParameter layerParameter;
	layerParameter.ViewerPosition = { CameraPos.x, CameraPos.y, CameraPos.z };
	m_efkManager->SetLayerParameter(0, layerParameter);

	Effekseer::Manager::UpdateParameter updateParameter;
	m_efkManager->Update(updateParameter);

	m_efkRenderer->SetTime(m_efkTime / 60.0f);
	m_efkRenderer->SetProjectionMatrix(cm);
	m_efkRenderer->SetCameraMatrix(cameraMatrix);

	//時間を進める
	++m_efkTime;
	if (m_efkTime > 60) m_efkTime = 0;
}

void Effect::Draw()
{
	//カメラが設定されていなければ処理しない
	if (!m_pCamera) return;

	m_efkRenderer->BeginRendering();
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 100.0f;
	drawParameter.ViewProjectionMatrix = m_efkRenderer->GetCameraProjectionMatrix();;//cm
	m_efkManager->Draw(drawParameter);
	m_efkRenderer->EndRendering();
}

void Effect::Load(const char16_t* path)
{
	//既に読み込み済みかを確認(読み込み済みなら処理しない)
	if (m_Effect.find(path) != m_Effect.end()) 
		return;

	//エフェクトを読み込んで登録
	using PAIR = std::pair<const char16_t*, Effekseer::EffectRef>;
	m_Effect.insert(PAIR(path, Effekseer::Effect::Create(m_efkManager, path)));
}

void Effect::Play(const char16_t* path, DXf3 pos, Effekseer::Handle* pHandle, bool loop, DXf3* pPos)
{
	//読み込み済みか確認
	if (m_Effect.find(path) == m_Effect.end())
	{
		//読み込んでいなければ読み込む
		Load(path);
	}

	//再生したエフェクトのハンドルを設定
	*pHandle = m_efkManager->Play(m_Effect.at(path), { pos.x, pos.y, pos.z });

	//ループ再生する場合はそのエフェクトデータを保存
	if (loop)
	{
		Effect::LoopData data = {};
		data.pHandle	= pHandle;					//ハンドル
		data.file		= path;						//ファイル
		data.pos		= DXfloat3ToEFv3d(pos);		//再生の位置
		data.pPos		= pPos;						//再生の際の位置(固定ならnullptr)
		m_LoopHandle.push_back(data);
	}
}

void Effect::Stop(Effekseer::Handle handle)
{
	//エフェクトの停止
	m_efkManager->StopEffect(handle);

	//ループ再生リストにあるか確認
	for (std::vector<LoopData>::iterator it = m_LoopHandle.begin(); it != m_LoopHandle.end(); ++it)
	{
		if (*(*it).pHandle == handle)
		{
			//ループ再生リストから削除
			m_LoopHandle.erase(it);
			break;
		}
	}
}

void Effect::AddPos(Effekseer::Handle handle, DirectX::XMFLOAT3 move)
{
	m_efkManager->AddLocation(handle, DXfloat3ToEFv3d(move));
}

void Effect::SetPos(Effekseer::Handle handle, DirectX::XMFLOAT3 pos)
{
	m_efkManager->SetLocation(handle, DXfloat3ToEFv3d(pos));
}

void Effect::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}


Effect* Effect::GetInstance()
{
	//メモリに存在しなければ確保
	if (!m_pInstance)
		m_pInstance = new Effect;

	//ポインタを返す
	return m_pInstance;
}

void Effect::DeleteInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

Effekseer::Vector3D DXfloat3ToEFv3d(DirectX::XMFLOAT3 f3)
{
	return { f3.x, f3.y, f3.z };
}