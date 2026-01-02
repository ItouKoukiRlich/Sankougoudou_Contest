//==== インクルード部 ====
#include"UI.h"
#include"Sprite.h"
#include"MathStruct.h"

UI::UI()
{

}

UI::~UI()
{
	for (std::vector<UI_info>::iterator it = m_UI.begin(); it != m_UI.end(); ++it)
	{
		delete (*it).pTexture;
		(*it).pTexture = nullptr;
	}
}

void UI::Draw()
{
	//行列の設定
	DirectX::XMFLOAT4X4 world, view, proj;
	DirectX::XMMATRIX mView = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(	0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixTranspose(mView));
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixTranspose(mProj));
	Sprite::SetView(view);
	Sprite::SetProjection(proj);

	for (std::vector<UI_info>::iterator it = m_UI.begin(); it != m_UI.end(); ++it)
	{
		if (!(*it).bDraw) continue;

		//位置・サイズの計算
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation((*it).pos.x, (*it).pos.y, 0.0f);
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling((*it).scale.x, -((*it).scale.y), 1.0f);//(*it).size.x, -((*it).size.y), 1.0f);
		DirectX::XMMATRIX L = DirectX::XMMatrixRotationZ((*it).Angle);
		DirectX::XMMATRIX mWorld = S * L * T;
		
		//転置して読み取り専用のデータに変換
		DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mWorld));
		
		//描画
		Sprite::SetWorld(world);
		Sprite::SetSize((*it).size);
		Sprite::SetUVPos((*it).uv);
		Sprite::SetColor((*it).color);
		Sprite::SetTexture((*it).pTexture);
		Sprite::Draw();
	}
}

void UI::SetUI(const char* file, int Number, DXf2 pos, float LoadScale)
{
	UI::UI_info ui = {};
	
	ui.pTexture = new Texture;
	if (FAILED(ui.pTexture->Create(file)))
	{
		MessageBox(NULL, "texure error", "error", MB_OK);
	}
	
	ui.pos		= pos;
	float w = (float)ui.pTexture->GetWidth();
	float h = (float)ui.pTexture->GetHeight();
	w = w / h;
	h = 1;

	ui.Number = Number;
	ui.size		= { w * LoadScale, h * LoadScale};
	ui.scale	= { 1.0f, 1.0f };
	ui.uv		= { 0.0f, 0.0f };
	ui.color	= { 1.0f, 1.0f, 1.0f, 1.0f };
	ui.Angle	= 0.0f;
	ui.bDraw	= true;

	m_UI.push_back(ui);
}

void UI::ChangeColor(int Number, float r, float g, float b, float a)
{
	for (std::vector<UI::UI_info>::iterator it = m_UI.begin(); it != m_UI.end(); ++it)
	{
		//指定された番号のデータを変更するため番号と一致するか確認
		if ((*it).Number != Number) continue;

		//変更/変更後は処理する必要がないためループを抜ける
		(*it).color = MakeDXf4(r, g, b, a);
		break;
	}
}

void UI::ChangeScale(int Number, float Xscale, float Yscale)
{
	for (std::vector<UI::UI_info>::iterator it = m_UI.begin(); it != m_UI.end(); ++it)
	{
		//指定された番号のデータを変更するため番号と一致するか確認
		if ((*it).Number != Number) continue;

		//変更/変更後は処理する必要がないためループを抜ける
		(*it).scale = MakeDXf2(Xscale, Yscale);
		break;
	}
}

void UI::ChangeLotation(int Number, float Angle)
{
	for (std::vector<UI::UI_info>::iterator it = m_UI.begin(); it != m_UI.end(); ++it)
	{
		//指定された番号のデータを変更するため番号と一致するか確認
		if ((*it).Number != Number) continue;

		//変更/変更後は処理する必要がないためループを抜ける
		(*it).Angle = Angle;
		break;
	}
}

void UI::ChangeUVpos(int Number, float u, float v)
{
	for (std::vector<UI::UI_info>::iterator it = m_UI.begin(); it != m_UI.end(); ++it)
	{
		//指定された番号のデータを変更するため番号と一致するか確認
		if ((*it).Number != Number) continue;

		//変更/変更後は処理する必要がないためループを抜ける
		(*it).uv = MakeDXf2(u, v);
		break;
	}
}

void UI::ChangePos(int Number, float x, float y)
{
	for (std::vector<UI::UI_info>::iterator it = m_UI.begin(); it != m_UI.end(); ++it)
	{
		//指定された番号のデータを変更するため番号と一致するか確認
		if ((*it).Number != Number) continue;

		//変更/変更後は処理する必要がないためループを抜ける
		(*it).pos = MakeDXf2(x, y);
		break;
	}
}

void UI::ChangeDraw(int Number, bool bDraw)
{
	for (std::vector<UI::UI_info>::iterator it = m_UI.begin(); it != m_UI.end(); ++it)
	{
		//指定された番号のデータを変更するため番号と一致するか確認
		if ((*it).Number != Number) continue;

		//変更/変更後は処理する必要がないためループを抜ける
		(*it).bDraw = bDraw;
		break;
	}
}

void UI::AddPos(int Number, float x, float y)
{
	for (std::vector<UI::UI_info>::iterator it = m_UI.begin(); it != m_UI.end(); ++it)
	{
		//指定された番号のデータを変更するため番号と一致するか確認
		if ((*it).Number != Number) continue;

		//変更/変更後は処理する必要がないためループを抜ける
		(*it).pos = MakeDXf2((*it).pos.x + x, (*it).pos.y + y);
		break;
	}
}

void UI::AddScale(int Number, float x, float y)
{
	for (std::vector<UI::UI_info>::iterator it = m_UI.begin(); it != m_UI.end(); ++it)
	{
		//指定された番号のデータを変更するため番号と一致するか確認
		if ((*it).Number != Number) continue;

		//変更/変更後は処理する必要がないためループを抜ける
		(*it).scale = MakeDXf2((*it).scale.x + x, (*it).scale.y + y);
		break;
	}
}

DirectX::XMFLOAT2 UI::GetSize(int Number)
{
	for (std::vector<UI::UI_info>::iterator it = m_UI.begin(); it != m_UI.end(); ++it)
	{
		//指定された番号のデータを変更するため番号と一致するか確認
		if ((*it).Number != Number) continue;

		//変更/変更後は処理する必要がないためループを抜ける
		return (*it).size;
	}
	return {0.0f, 0.0f};
}