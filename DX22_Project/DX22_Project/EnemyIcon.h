#ifndef ___ENEMYICON_H___
#define ___ENEMYICON_H___

//==== インクルード部 ====
#include"Texture.h"
#include"GameObject.h"
#include"Camera.h"

class EnemyIcon : public GameObject
{
public:
	EnemyIcon();
	~EnemyIcon() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// カメラを設定
	/// </summary>
	/// <param name="pCamera"></param>
	static void SetCamera(Camera* pCamera);

private:
	static Texture* m_pTexture;
	static Camera*	m_pCamera;

};

#endif