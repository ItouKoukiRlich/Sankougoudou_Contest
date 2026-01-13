#ifndef ___MISSION_H___
#define ___MISSION_H___

//==== インクルード部 ====
#include"UI.h"

class Mission
{
public:
	enum Type
	{
		eTurret,
		eCreate,
		eNormal,
		eSuper,
		eMain,
		eTypeMax
	};

public:
	Mission();
	virtual ~Mission();
	virtual void Update();
	virtual void Draw();

	/// <summary>
	/// ミッション開始
	/// </summary>
	void MissionStart();

	/// <summary>
	/// ミッションのアクティブかを管理
	/// </summary>
	/// <param name="bActive">：アクティブならtrue</param>
	void SetActive(bool bActive);

	/// <summary>
	/// カウントをプラス
	/// </summary>
	void CountPlus();

	/// <summary>
	/// アクティブかどうか
	/// </summary>
	/// <returns>アクティブならtrue</returns>
	bool CheckActive() const;

	/// <summary>
	/// ミッションクリアかどうか
	/// </summary>
	/// <returns>クリアしていたらtrue</returns>
	bool CheckClear() const;

	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	static void Load();

	/// <summary>
	/// ミッションのUIを表示
	/// </summary>
	static void MissionMenuDraw();

	/// <summary>
	/// カウントリセット
	/// </summary>
	static void CountRisset();
private:
	void DrawNumber();

protected:
	enum TexTag
	{
		eMessage,
		eIcon,
		eTag,
		eMaxNumber,
		eCurrentNumber,
	};

protected:
	static UI m_ui;
	static Texture* m_pTexture;
	static int m_nActiveCount;

protected:
	DXf2 m_uvPos;			//UVの位置
	bool m_bActive	;		//ミッションがアクティブになっているか(アクティブならtrue)
	int  m_nMax;			//目標討伐数
	int  m_nCurrent;		//今の討伐数
};

#endif