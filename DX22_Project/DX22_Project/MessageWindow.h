#ifndef ___MESSAGEWINDOW_H___
#define ___MESSAGEWINDOW_H___

//==== インクルード部 ====
#include"UI.h"

class MessageWindow
{
public:
	enum MessageType
	{
		eStart = 0,
		eMission1,
		eMission2,
		eCutin1,
		eMission3,
		eSuper,
		eMission4,
		eTypeMax
	};

public:
	MessageWindow();
	~MessageWindow();
	void Update();
	void Draw();

	/// <summary>
	/// メッセージウィンドウを表示開始
	/// </summary>
	/// <param name="tyoe">：どのメッセージを描画するか</param>
	void Start(MessageWindow::MessageType type);

private:
	enum TexTag	//テクスチャのタグ
	{
		Base,
		Wave,
		Message,
		TexMax,
	};

private:
	UI		m_ui;
	float	m_fWaveU;		//ウェーブのテクスチャのU値
	int		m_nDrawCount;	//表示してるフレームのカウント
	bool	m_bFlag;		//処理するかのフラグ
};

#endif