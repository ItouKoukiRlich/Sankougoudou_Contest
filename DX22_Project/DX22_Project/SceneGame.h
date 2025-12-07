#ifndef ___SCENEGAME_H___
#define ___SCENEGAME_H___

//==== インクルード部 ====
#include"Scene.h"
#include"CameraGame.h"
#include"Player.h"

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame();
	void Update() override;
	void Draw() override;

private:
	CameraGame* m_pCamera;	//ゲームカメラ
	Player*		m_pPlayer;	//プレイヤー
	//フィールド

};

#endif