//==== インクルード部 ====
#include "MessageWindow.h"
#include"DirectX.h"

//==== 定数・マクロ定義 ====
namespace nameMessageWindow	//定数名などが重複しないようにnamespaceを確保
{
	//---- 表示位置 ----
	constexpr DXf2	BasePos		= { 0.9f, 0.3f };					//位置
	constexpr DXf2	WavePos		= { BasePos.x, BasePos.y + 0.2f };	//位置
	constexpr DXf2	MessagePos	= { BasePos.x, BasePos.y - 0.16f };	//位置
	
	//---- スケール(丁度いい感じになるように調整用) ----
	constexpr DXf2 BaseScale = { 0.8f, 1.2f };		//元
	constexpr DXf2 WaveScale = { 0.7f, 1.2	};		//波
	constexpr DXf2 FontSize	 = { 0.15f, 0.18f };	//文章
	
	//---- 挿入・消える際の演出関連 ----
	constexpr float WaveU_deltaValue	= 0.01f;	//1フレームで変化する背景u値
	constexpr int	DrawFlame			= 300;		//表示するフレーム
	constexpr float BaseAnimeFlame		= 5;		//挿入または消える演出にかかるフレーム
	constexpr float InitScale			= 0.001f;	//最初のスケール(段々と大きくなるため最初は小さくする)
	constexpr float Scale_deltaValue	= (BaseScale.y - InitScale) / BaseAnimeFlame;	//拡大率の変更量(1フレーム)
}
using namespace nameMessageWindow;

MessageWindow::MessageWindow()
	:m_fWaveU(0.0f)
	,m_nDrawCount(0)
	,m_bFlag(false)
{
	//テクスチャの読み込み
	m_ui.SetUI("Assets/Texture/Message/Wave.png", Wave, WavePos, 0.1f);
	m_ui.SetUI("Assets/Texture/Message/Font.png", Message, MessagePos);
	m_ui.SetUI("Assets/Texture/Message/MessageWindow.png", Base, BasePos, 0.5f);

	//UIのステータス調整
	m_ui.ChangeScale(Base,		BaseScale.x, InitScale);
	m_ui.ChangeScale(Wave,		WaveScale.x, WaveScale.y);
	m_ui.ChangeSize(Message,	FontSize.x,  FontSize.y);

	//挿入演出時は波と文章のテクスチャは描画しない
	m_ui.ChangeDraw(Message, false);
	m_ui.ChangeDraw(Wave, false);

	//UIのスケールを変更
	m_ui.ChangeUVsize(Message, 1.0f / (float)MessageWindow::MessageType::eTypeMax, 1.0f);
}

MessageWindow::~MessageWindow()
{
}

void MessageWindow::Update()
{
	if (!m_bFlag) return;

	//---- 状態別の処理 ----
	
	//挿入演出の処理
	if (m_nDrawCount < (int)BaseAnimeFlame)
	{
		m_ui.AddScale(Base, 0.0f, Scale_deltaValue);
	}
	//表示中の処理
	else if (m_nDrawCount < (int)BaseAnimeFlame + DrawFlame)
	{
		//ウェーブテクスチャのUV座標変更の処理
		m_ui.ChangeUVpos(Wave, m_fWaveU, 0.0f);
		m_fWaveU += WaveU_deltaValue;
	}
	//消える際の処理
	else if (m_nDrawCount < (int)BaseAnimeFlame * 2 + DrawFlame)
	{
		m_ui.AddScale(Base, 0.0f, -Scale_deltaValue);
	}

	//カウントを進める
	m_nDrawCount++;

	//挿入演出完了後に描画
	if (m_nDrawCount == (int)BaseAnimeFlame)
	{
		m_ui.ChangeDraw(Message, true);
		m_ui.ChangeDraw(Wave, true);
	}

	//消える演出の際は描画しない
	if (m_nDrawCount == (int)BaseAnimeFlame + DrawFlame)
	{
		m_ui.ChangeDraw(Message, false);
		m_ui.ChangeDraw(Wave, false);
	}

	//表示時間を越えたら描画終了
	if (m_nDrawCount >= (int)BaseAnimeFlame * 2 + DrawFlame)
	{
		m_bFlag			= false;	//描画終了
		m_fWaveU		= 0.0f;		//U値の初期化
	}
}

void MessageWindow::Draw()
{
	if (!m_bFlag) return;

	m_ui.Draw();
}

void MessageWindow::Start(MessageWindow::MessageType type)
{
	//表示開始
	//表示するメッセージによってUV座標を変更
	m_bFlag = true;
	m_ui.ChangeUVpos(Message, (1.0f / (float)MessageWindow::MessageType::eTypeMax) * (float)type, 0.0f);

	//表示終了時間になる前に呼び出されたらメッセージを上書きするためここで初期化
	m_nDrawCount = 0;
	m_ui.ChangeScale(Base, BaseScale.x, InitScale);
	m_ui.ChangeScale(Wave, WaveScale.x, WaveScale.y);
}