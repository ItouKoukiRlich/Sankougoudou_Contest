//==== インクルード部 ====
#include "DebugBlock.h"
#include"Defines.h"
#include"ShaderList.h"

DebugBlock::DebugBlock()
	:m_pModel(nullptr)
{
	m_pModel = new Model;
	if (!m_pModel->Load("Assets/Model/Box.fbx", 0.2f))
		MessageBox(NULL, "PlayerModel_Body", "Error", MB_OK);
}

DebugBlock::~DebugBlock()
{
	SAFEDELETE(m_pModel);
}

void DebugBlock::Draw(DirectX::XMFLOAT3 pos, Camera* pCamera)
{
	//頂点シェーダーに渡す変換行列の変数を宣言
	DirectX::XMFLOAT4X4 fWVP[3] = {};
	fWVP[1] = pCamera->GetViewMatrix();
	fWVP[2] = pCamera->GetProjectionMatrix();

	//ワールド行列の作成
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX Translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	world = Scale * Translation;
	DirectX::XMStoreFloat4x4(&fWVP[0], DirectX::XMMatrixTranspose(world));

	//シェーダーへの変換行列を設定
	ShaderList::SetWVP(fWVP);

	//モデルに使用する頂点シェーダー、ピクセルシェーダーを設定
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_UNLIT));

	// マテリアル別にメッシュを表示 
	for (unsigned int i = 0; i < m_pModel->GetMeshNum(); ++i)
	{
		// モデルのメッシュを取得 
		const Model::Mesh mesh = *m_pModel->GetMesh(i);

		// メッシュに割り当てられているマテリアルを取得 
		Model::Material material = *m_pModel->GetMaterial(mesh.materialID);

		// シェーダーへマテリアルを設定 
		ShaderList::SetMaterial(material);

		// モデルの描画 
		m_pModel->Draw(i);
	}
}