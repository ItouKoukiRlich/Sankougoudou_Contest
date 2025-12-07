#ifndef ___DEBUGBLOCK_H___
#define ___DEBUGBLOCK_H___

//==== インクルード部 ====
#include<DirectXMath.h>
#include<Model.h>
#include<Camera.h>

class DebugBlock
{
public:
	DebugBlock();
	~DebugBlock();
	void Draw(DirectX::XMFLOAT3 pos, Camera* pCamera);

private:
	Model*	m_pModel;
};

#endif