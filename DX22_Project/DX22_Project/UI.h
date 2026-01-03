#ifndef ___UI_H___
#define ___UI_H___

//==== インクルード部 ====
#include"DirectX.h"
#include"Texture.h"
#include<vector>

//=== 定数・マクロ定義 ====
constexpr DXf2 UI_center = { 0.5f, 0.5f };	//UIの中心座標

class UI
{
public:
	struct UI_info
	{
		int			Number;		//番号(値を変更する際に指定する用)
		Texture*	pTexture;	//テクスチャ
		DXf2		pos;		//位置
		DXf2		size;		//サイズ
		DXf2		scale;		//拡大率
		DXf2		uv;			//UV座標
		DXf2		uvSize;		//UVサイズ
		DXf4		color;		//色
		float		Angle;		//回転の角度
		bool		bDraw;		//描画するならtrue
	};

public:
	UI();
	~UI();
	void Draw();

	/// <summary>
	/// UIを設定
	/// </summary>
	/// <param name="file">：ファイルパス</param>
	/// <param name="Number">：UIの番号(変更時、指定する時に使用)</param>
	/// <param name="pos">：描画位置</param>
	/// <param name="LoadScale">：読み込む際の拡大率</param>
	void SetUI(const char* file, int Number, DXf2 pos, float LoadScale = 1.0f);

	/// <summary>
	/// 色を変換
	/// </summary>
	/// <param name="Number">：変換したいUIの番号</param>
	/// <param name="r">：R値</param>
	/// <param name="g">：G値</param>
	/// <param name="b">：B値</param>
	/// <param name="a">：A値(透明度)</param>
	void ChangeColor(int Number, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

	/// <summary>
	/// 拡大率を変換
	/// </summary>
	/// <param name="Number">：変換したいUIの番号</param>
	/// <param name="Xscale">：横の拡大率</param>
	/// <param name="Yscale">：縦の拡大率</param>
	void ChangeScale(int Number, float Xscale, float Yscale);

	/// <summary>
	/// 回転の角度を変更
	/// </summary>
	/// <param name="Number">：変換したいUIの番号</param>
	/// <param name="Angle">：角度</param>
	void ChangeLotation(int Number, float Angle);

	/// <summary>
	/// UV座標を変更
	/// </summary>
	/// <param name="Number">：変換したいUIの番号</param>
	/// <param name="u">：u値</param>
	/// <param name="v">：v値</param>
	void ChangeUVpos(int Number, float u, float v);

	/// <summary>
	/// UVサイズを変更
	/// </summary>
	/// <param name="Number">：変更したいUIの番号</param>
	/// <param name="u">：uサイズ値</param>
	/// <param name="v">：vサイズ値</param>
	void ChangeUVsize(int Number, float u, float v);

	/// <summary>
	/// 位置を変更
	/// </summary>
	/// <param name="Number">：変更したいUIの番号</param>
	/// <param name="x">：x座標</param>
	/// <param name="y">：y座標</param>
	void ChangePos(int Number, float x, float y);

	/// <summary>
	/// サイズを変更
	/// </summary>
	/// <param name="Number">：変更したいUIの番号</param>
	/// <param name="x">：横サイズ</param>
	/// <param name="y">：縦サイズ</param>
	void ChangeSize(int Number, float x, float y);

	/// <summary>
	/// 描画する・しない を変更
	/// </summary>
	/// <param name="Number">：変更したいUIの番号</param>
	/// <param name="bDraw">：描画する(true)・描画しない(false)</param>
	void ChangeDraw(int Number, bool bDraw);

	/// <summary>
	/// 位置に加算
	/// </summary>
	/// <param name="Number">：変更したいUIの番号</param>
	/// <param name="x">：x座標に足す値</param>
	/// <param name="y">：y座標に足す値</param>
	void AddPos(int Number, float x, float y);

	/// <summary>
	/// 拡大率に加算
	/// </summary>
	/// <param name="Number">：変更したいUIの番号</param>
	/// <param name="x">：横の拡大率に加算する値</param>
	/// <param name="y">：縦の拡大率に加算する値</param>
	void AddScale(int Number, float x, float y);

	/// <summary>
	///	サイズを入手
	/// </summary>
	/// <param name="Number">：入手したいデータUIの番号</param>
	/// <returns>サイズ</returns>
	DirectX::XMFLOAT2 GetSize(int Number);

private:
	std::vector<UI_info> m_UI;
};

#endif