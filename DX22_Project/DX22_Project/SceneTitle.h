#ifndef ___SCENETITLE_H___
#define ___SCENETITLE_H___

//==== インクルード部 ====
#include"Scene.h"
#include"Camera.h"
#include"TitleUI.h"
#include"Effect.h"

class SceneTitle : public Scene
{
public:
	SceneTitle();
	~SceneTitle() override;
	void Update() override;
	void Draw()	override;

private:
	Camera*		m_pCamera;		//カメラ
	TitleUI		m_TitleUI;		//タイトルのUI
	EFhdl		m_Hndle;		//エフェクトのハンドル
	int			m_nEffectCount;	//エフェクトのカウント
};

#endif