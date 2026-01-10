#ifndef ___SCENEGAME_H___
#define ___SCENEGAME_H___

//==== インクルード部 ====
#include"Scene.h"
#include"CameraGame.h"
#include"Player.h"
#include"MessageWindow.h"
#include"GameUI.h"
#include"Enemy.h"

//==== 定数・マクロ定義 ====
namespace nameSceneGame
{
	constexpr DXf3 cg_FieldSize		= { 1000.0f, 1000.0f , 1000.0f };			//フィールドのサイズ
	constexpr DXf3 cg_FieldHarfSize = { cg_FieldSize.x * 0.5f, cg_FieldSize.y * 0.5f , cg_FieldSize.z * 0.5f};
}

namespace nmEnemyArray
{
	enum Type
	{
		eTurret,
	};

	//---- タレット型敵 ----
	constexpr int cg_TurretStart	= 0;
	constexpr int cg_MaxTurret		= 10;

	constexpr int cg_MaxEnemy = cg_MaxTurret;
}

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame();
	void Update() override;
	void Draw() override;
	
	/// <summary>
	/// 敵を作成
	/// </summary>
	/// <param name="type">：作成する敵の種類</param>
	/// <param name="pos">：生成する位置</param>
	void CreateEnemy(nmEnemyArray::Type type, DXf3 pos);

private:
	CameraGame*		m_pCamera;			//ゲームカメラ
	Player*			m_pPlayer;			//プレイヤー
	MessageWindow	m_MessageWindow;	//メッセージUI
	GameUI			m_GameUI;			//ゲームのUI
	Enemy*			m_pEnemy[nmEnemyArray::cg_MaxEnemy];	//敵

};

#endif