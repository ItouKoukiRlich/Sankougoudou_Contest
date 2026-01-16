//==== インクルード部 ====
#include "CutIn.h"
#include"ShaderList.h"
#include"CameraCutIn.h"
#include"Geometory.h"
#include"SuperEnemy.h"
#include"MainEnemy.h"
#include"CreateEnemy.h"
#include"NormalEnemy.h"

namespace nameCutIn
{
	constexpr float cutl = fCutInLength;
	constexpr DXf3 pos1  = { -1.0f, 1.0f, -1.0f };
	constexpr DXf3 move1 = { -1.0f / cutl, 1.0f / cutl, (1.0f - 100.0f) / cutl};
	constexpr DXf3 move2 = {  10.0f / cutl, 0.0f / cutl, (1.0f - 100.0f) / cutl};
	constexpr DXf3 move3 = {  10.0f / cutl, 10.0f / cutl, (1.0f - 100.0f) / cutl};
	constexpr DXf3 move4 = {  -1.0f / cutl,-10.0f / cutl, (1.0f - 100.0f) / cutl};
}
using namespace nameCutIn;

CutIn::CutIn()
	:m_pCamera(nullptr)
	,m_Type(CutType::eCut1)
	,m_nCount(0)
{
	m_pCamera = new CameraCutIn;
}

CutIn::~CutIn()
{
	SAFE_DELETE(m_pCamera);
}

void CutIn::Update()
{
	switch (m_Type)
	{
	case CutIn::eCut1:
		break;

	case CutIn::eCut2:
		break;
	}
	m_nCount++;
}

void CutIn::Draw()
{
	//---- 仮置きしているボックスにカメラを設定 ----
	Geometory::SetView(m_pCamera->GetViewMatrix());
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix());

	switch (m_Type)
	{
	case CutIn::eCut1:
		SuperEnemy::staticDraw(m_pCamera, { 0.0f + move1.x * (float)m_nCount, 0.0f + move1.y * (float)m_nCount, 100.0f + move1.z * (float)m_nCount });
		CreateEnemy::staticDraw(m_pCamera, { 0.0f + move2.x * (float)m_nCount, 0.0f + move2.y * (float)m_nCount, 110.0f + move2.z * (float)m_nCount });
		NormalEnemy::staticDraw(m_pCamera, { 0.0f + move3.x * (float)m_nCount, 0.0f + move3.y * (float)m_nCount, 110.0f + move3.z * (float)m_nCount });
		NormalEnemy::staticDraw(m_pCamera, { 0.0f + move4.x * (float)m_nCount, 0.0f + move4.y * (float)m_nCount, 110.0f + move4.z * (float)m_nCount });
		break;

	case CutIn::eCut2:
		MainEnemy::staticDraw(m_pCamera, { 0.0f + move1.x * (float)m_nCount, 0.0f + move1.y * (float)m_nCount, 100.0f + move1.z * (float)m_nCount });
		MainEnemy::staticDraw(m_pCamera, { 0.0f + move2.x * (float)m_nCount, 0.0f + move2.y * (float)m_nCount, 110.0f + move2.z * (float)m_nCount });
		MainEnemy::staticDraw(m_pCamera, { 0.0f + move3.x * (float)m_nCount, 0.0f + move3.y * (float)m_nCount, 110.0f + move3.z * (float)m_nCount });
		break;
	}
}

void CutIn::SetCount(int num)
{
	m_nCount = num;
}

void CutIn::SetCutIn(CutType type)
{
	m_Type = type;
	m_nCount = 0;
}