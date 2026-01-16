#ifndef ___SCENEGAME_H___
#define ___SCENEGAME_H___

//==== インクルード部 ====
#include"Scene.h"
#include"CameraGame.h"
#include"Player.h"
#include"MessageWindow.h"
#include"GameUI.h"
#include"Enemy.h"
#include"Mission.h"
#include"GameOver.h"
#include"Clear.h"
#include"CutIn.h"
#include"GameMenu.h"

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
		eCreate,
		eNormal,
		eSuper,
		eMain
	};

	//---- タレット型敵 ----
	constexpr int cg_TurretStart	= 0;
	constexpr int cg_MaxTurret		= 10;
	//---- ノーマルタイプ ----
	constexpr int cg_NormalStart	= 10;
	constexpr int cg_MaxNormal		= 10;
	//---- スーパータイプ ----
	constexpr int cg_SuperStart		= 20;
	constexpr int cg_MaxSuper		= 10;
	//---- 作成タイプ ----
	constexpr int cg_CreateStart	= 30;
	constexpr int cg_MaxCreate		= 5;
	//---- メインタイプ----
	constexpr int cg_MainStart		= 35;
	constexpr int cg_MaxMain		= 3;
	constexpr int cg_MaxEnemy = cg_MaxTurret + cg_MaxNormal + cg_MaxSuper + cg_MaxCreate + cg_MaxMain;
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
	void CreateEnemyField(nmEnemyArray::Type type, DXf3 pos);

	/// <summary>
	/// ノーマルタイプの敵を作成
	/// </summary>
	/// <param name="pos">：位置</param>
	/// <param name="type">：タイプ</param>
	/// <param name="move">：移動量</param>
	void CreateNormalEnemy(DXf3 pos, int type, DXf3 move = { 5.0f, 5.0f, 5.0f });

private:
	enum Phase
	{
		eGame,		//ゲーム
		eGameOver,	//ゲームオーバー
		eDelay,		//ゲームゲーム時の間
		eClear,		//クリア
		eCutIn,
		eCutIn2,
		eMenu,
		eStart,
	};

	enum GameStep
	{
		eStep1,
		eStep2,
		eStep3,
		eStep4,
		eStep5
	};

private:
	void Collision();
	void StartGameOver(GameOver::Type type);
	bool CheckOutField();
	/// <summary>
	/// ミッション対象の敵のカウントを進める
	/// </summary>
	/// <param name="i"></param>
	void EnemyMissionCount(int i);

	/// <summary>
	/// テクスチャメッセージの描画
	/// </summary>
	void TextureMessage();

	/// <summary>
	/// プレイヤーの位置をリセット
	/// </summary>
	void ResetPlayer();


	void GameDraw();

private:
	GameOver			m_GameOver;			//ゲームオーバー
	CameraGame*			m_pCamera;			//ゲームカメラ
	Player*				m_pPlayer;			//プレイヤー
	MessageWindow		m_MessageWindow;	//メッセージUI
	GameUI				m_GameUI;			//ゲームのUI
	Enemy*				m_pEnemy[nmEnemyArray::cg_MaxEnemy];	//敵
	Mission*			m_pMission[Mission::Type::eTypeMax];
	Clear				m_Clear;			//クリア
	SceneGame::Phase	m_phase;			//
	int					m_nGameOverCount;	//ゲームオーバー以降の間をカウント
	int					m_nGameCount;		//ゲームのカウント
	GameStep m_Step;
	CutIn m_CutIn;
	GameMenu m_menu;
	int startstep;
};

#endif