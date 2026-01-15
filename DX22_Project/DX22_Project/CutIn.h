#ifndef ___CUTIN_H___
#define ___CUTIN_H___

#include"Texture.h"
#include"Camera.h"

class CutIn
{
public:
	CutIn();
	~CutIn();
	void Update();
	void Draw();

private:
	Camera* m_pCamera;
};

#endif