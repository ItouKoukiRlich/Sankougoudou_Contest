#include "Geometory.h"

void Geometory::MakeBox()
{
	//--- 頂点の作成
	Vertex vtx[] = {
		//-z面
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},

		//z面
		{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
		{{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
		{{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
		
		//-x面
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
		{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
		
		//x面
		{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
		
		//-y面
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
		
		//y面
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
	};

	//--- インデックスの作成
	int idx[] = {
		 0, 1, 2, 1, 3, 2,	//-z面
		 4, 5, 6, 5, 7, 6,	// z面
		 8, 9,10, 9,11,10,	//-x面
		12,13,14,13,15,14,	// x面
		16,17,18,17,19,18,	//-y面
		20,21,22,21,23,22,	// y面
	};

	// バッファの作成
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxCount = sizeof(vtx) / sizeof(Vertex);
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = idx;
	desc.idxCount = sizeof(idx) / sizeof(int);
	desc.idxSize = sizeof(int);
	desc.topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pBox = new MeshBuffer();
	m_pBox->Create(desc);
}

void Geometory::MakeCylinder()
{
	const int	num = 8;
	const float pi	= 3.141592653589;
	const float degree = 360.0f / (float)num;
	const float radian = degree * (pi / 180.0f);

	//---- 頂点の作成 ----
	
	// 天面
	Vertex TopVtx[num + 1] = {};
	for (int i = 0; i < num + 1; i++)
	{
		//位置
		TopVtx[i].pos[0] = cosf(radian * i) * 0.5f;
		TopVtx[i].pos[1] = 0.5f;
		TopVtx[i].pos[2] = -1 * sinf(radian * i) * 0.5f;

		//座標
		TopVtx[i].uv[0] = cosf(radian * i);
		TopVtx[i].uv[1] = sinf(radian * i);
	}

	//底面
	Vertex UnderVtx[num + 1] = {};
	for (int i = 0; i < num + 1; i++)
	{
		//位置
		UnderVtx[i].pos[0] = cosf(radian * i) * 0.5f;
		UnderVtx[i].pos[1] = -0.5f;
		UnderVtx[i].pos[2] = sinf(radian * i) * 0.5f;

		//座標
		UnderVtx[i].uv[0] = cosf(radian * i);
		UnderVtx[i].uv[1] = sinf(radian * i);
	}

	
	//全部のデータを結合
	Vertex vtx[(num + 1) * 2] = {};
	for (int i = 0; i < num + 1; i++)
	{
		vtx[i] = TopVtx[i];
		vtx[num + 1 + i] = UnderVtx[i];
	}


	////--- インデックスの作成
	//// 天面、底面
	int idx[((num + 1) * 2) * 2] = { 0 };
	for (int i = 0; i < 6; i++)
	{
		idx[i * 3] = 0;
		idx[i * 3 + 1] = i + 1;
		idx[i * 3 + 2] = i + 2;

		idx[(num + 1) * 2 + (i * 3)] = num + 1;
		idx[(num + 1) * 2 + ((i * 3) + 1)] = num + 1 + i + 1;
		idx[(num + 1) * 2 + ((i * 3) + 2)] = num + 1 + i + 2;
	}

	//--- バッファの作成
	MeshBuffer::Description desc = {};
	desc.pVtx = vtx;
	desc.vtxCount = sizeof(vtx) / sizeof(Vertex);
	desc.vtxSize = sizeof(Vertex);
	desc.pIdx = idx;
	desc.idxCount = sizeof(idx) / sizeof(int);
	desc.idxSize = sizeof(int);
	desc.topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_pCylinder = new MeshBuffer();
	m_pCylinder->Create(desc);
}

void Geometory::MakeSphere()
{
	//--- 頂点の作成

	//--- インデックスの作成

	// バッファの作成
}