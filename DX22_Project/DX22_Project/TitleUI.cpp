//==== インクルード部 ====
#include "TitleUI.h"
#include<math.h>
#include"MathStruct.h"

//==== 定数・マクロ定義 ====
namespace nameTitleUI
{
	constexpr DXf2	GearPos = { 0.5f, 0.95f };	//歯車のテクスチャの位置
	constexpr DXf2	TitlePos = { 0.5f, 0.25f };	//タイトルのテクスチャの位置
	constexpr DXf2	StartPos = { 0.5f, 0.4f };	//GAME_START のUIの位置
	constexpr DXf2	StartUnderLinePos = { 0.5f, 0.44f };	//GAME_START のアンダーラインの位置
	constexpr float BackU_deltaValue = 0.001f;			//1フレームで変化する背景u値

	//タイトルロゴ挿入演出
	constexpr DXf2	TitleInitPos			= { 1.5f, 0.25f };	//タイトルの初期位置(画面外)
	constexpr float TitleMoveFlame			= 20.0f;			//移動アニメーションにかかるフレーム
	constexpr float TitleChangeScaleFlame	= 30.0f;			//拡大縮小アニメーションにかかるフレーム
	constexpr float TitleReturnScaleFlame	= 10.0f;			//拡大縮小から元に戻るのにかかるフレーム
	constexpr DXf2	AnimeScale				= { 0.25f, 2.5f };	//タイトルのロゴが最大でどのくらい圧縮されるか

	constexpr int	TitleInsertFlame	= TitleMoveFlame + TitleChangeScaleFlame + TitleReturnScaleFlame;	//タイトル挿入のアニメーションにかかる時間
	constexpr float TitleMove = (TitlePos.x - TitleInitPos.x) / TitleMoveFlame;	//タイトル挿入の移動量
	constexpr DXf2 TitleScaledeltaValue = { (AnimeScale.x - 1.0f) / TitleChangeScaleFlame,
											(AnimeScale.y - 1.0f) / TitleChangeScaleFlame };//拡大縮小の変化量
	constexpr DXf2 TitleScaledeltaValueReturn = { (1.0f - AnimeScale.x) / TitleReturnScaleFlame,
												  (1.0f - AnimeScale.y) / TitleReturnScaleFlame };//拡大縮小の変化量
	//歯車挿入演出
	constexpr DXf2	GearInitPos = { 0.5f, 1.45f };								//歯車の初期位置(画面外)
	constexpr float GearMoveY = (GearPos.y - GearInitPos.y) / TitleInsertFlame;	//歯車の1フレームの移動量
}
using namespace nameTitleUI;

TitleUI::TitleUI()
	:m_Degree(0.0f)
	,m_GearDegree(0.0f)
	,m_BackU(0.0f)
	,m_nAnimeFlameCount(0)
{
	m_ui.SetUI("Assets/Texture/Back.png",  eBack,  UI_center);
	m_ui.SetUI("Assets/Texture/Title.png", eTitle, TitleInitPos, 0.2f);
	m_ui.SetUI("Assets/Texture/Gear.png",  eGear1, GearInitPos);
	m_ui.SetUI("Assets/Texture/Gear.png",  eGear2, GearInitPos, 0.8);
	m_ui.SetUI("Assets/Texture/GameStart/GameStart.png",			eStart,			StartPos,			0.04f);
	m_ui.SetUI("Assets/Texture/GameStart/GameStartUnderLine.png",	eStartUderLine,	StartUnderLinePos,	0.04f);

	//歯車の色を変更
	m_ui.ChangeColor(eGear1, 1.0f, 0.0f, 0.0f, 1.0f);
	m_ui.ChangeColor(eGear2, 0.0f, 0.0f, 1.0f, 1.0f);

	//アニメーションが終わるまで「GAME START」は描画しない
	m_ui.ChangeDraw(eStart, false);
	m_ui.ChangeDraw(eStartUderLine, false);
}

TitleUI::~TitleUI()
{

}

void TitleUI::Update()
{
	//---- アニメーションの処理 ----
	if ((float)m_nAnimeFlameCount < TitleInsertFlame)
	{
		AnimeProcess();	//アニメーション処理
	}
	//---- アニメーション終了後の処理 ----
	else
	{
		//---- 背景の処理 ----
		m_BackU -= BackU_deltaValue;
		m_ui.ChangeUVpos(eBack, m_BackU, 0.0f);

		//---- 歯車の回転 ----
		m_GearDegree += 1.0f;
		m_ui.ChangeLotation(eGear1, DegreeToRadian(m_GearDegree));
		m_ui.ChangeLotation(eGear2, -DegreeToRadian(m_GearDegree));

		//---- スタートのアンダーラインの拡大・点滅の処理 ----
		m_Degree += 1.0f;
		if (m_Degree > 180.0f) m_Degree = 0.0f;
		float rad = sinf(DegreeToRadian(m_Degree));
		m_ui.ChangeColor(eStartUderLine, 1.0f, 1.0f, 1.0f, rad);
		m_ui.ChangeScale(eStartUderLine, 1.0f, rad);
	}
}

void TitleUI::Draw()
{
	m_ui.Draw();
}

void TitleUI::AnimeProcess()
{
	//歯車
	m_ui.AddPos(eGear1, 0.0f, nameTitleUI::GearMoveY);
	m_ui.AddPos(eGear2, 0.0f, nameTitleUI::GearMoveY);

	//タイトル
	if ((float)m_nAnimeFlameCount < nameTitleUI::TitleMoveFlame)
	{
		m_ui.AddPos(eTitle, nameTitleUI::TitleMove, 0.0f);
	}
	else if ((float)m_nAnimeFlameCount < nameTitleUI::TitleMoveFlame + nameTitleUI::TitleChangeScaleFlame)
	{
		//拡大率を変更
		m_ui.AddScale(eTitle, nameTitleUI::TitleScaledeltaValue.x, nameTitleUI::TitleScaledeltaValue.y);

		//移動する
		DXf2 size = m_ui.GetSize(eTitle);
		size.x = size.x * 0.5f;
		m_ui.AddPos(eTitle, size.x * TitleScaledeltaValue.x, 0.0f);
	}
	else if ((float)m_nAnimeFlameCount < TitleMoveFlame + TitleChangeScaleFlame + TitleReturnScaleFlame)
	{
		//拡大率を変更
		m_ui.AddScale(eTitle, nameTitleUI::TitleScaledeltaValueReturn.x, nameTitleUI::TitleScaledeltaValueReturn.y);

		//移動する
		DXf2 size = m_ui.GetSize(eTitle);
		size.x = size.x * 0.5f;
		m_ui.AddPos(eTitle, size.x * TitleScaledeltaValueReturn.x, 0.0f);
	}

	//カウントを進める
	m_nAnimeFlameCount++;

	//アニメーションが終了する際の処理
	if (m_nAnimeFlameCount == TitleInsertFlame)
	{
		//アニメーション終了後「GAME START」は描画
		m_ui.ChangeDraw(eStart, true);
		m_ui.ChangeDraw(eStartUderLine, true);
	}

}
