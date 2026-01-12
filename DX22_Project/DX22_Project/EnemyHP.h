#ifndef ___ENEMYHP_H___
#define ___ENEMYHP_H___

//==== インクルード部 ====
#include"DirectX.h"
#include"Texture.h"
#include"Camera.h"

class EnemyHP
{
public:
	EnemyHP();
	~EnemyHP();
	void Draw(int MaxHP, int CurrentHP, DXf3 pos, DXf2 scale = {1.0f, 1.0f});
	static void SetCamera(Camera* pCamera);
private:
	static Texture* m_pTexHP;
	static Texture* m_pTexHPBase;
	static Camera* m_pCamera;
};

#endif