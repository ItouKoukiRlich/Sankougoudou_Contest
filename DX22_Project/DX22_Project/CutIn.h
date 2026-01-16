#ifndef ___CUTIN_H___
#define ___CUTIN_H___

//==== インクルード部 ====
#include"Texture.h"
#include"Camera.h"

//==== 定数・マクロ定義 ====
namespace nameCutIn
{
	constexpr float fCutInLength = 310.0f;
	constexpr int nCutInLength = (int)fCutInLength;
}

class CutIn
{
public:
	enum CutType
	{
		eCut1,
		eCut2
	};

public:
	CutIn();
	~CutIn();
	void Update();
	void Draw();
	void SetCount(int num);
	void SetCutIn(CutType type);

private:
	Camera* m_pCamera;
	CutType m_Type;
	int m_nCount;
};

#endif