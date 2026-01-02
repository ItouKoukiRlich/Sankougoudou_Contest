#ifndef ___EFFECT_H___
#define ___EFFECT_H___

//==== インクルード部 ====
#include<map>
#include<DirectXMath.h>
#include"DirectX.h"
#include"Effekseer.h"
#include"EffekseerRendererDX11.h"
#include"Camera.h"

using EFf3	= Effekseer::Vector3D;	//Effekseer::Vector3D
using EFhdl = Effekseer::Handle;	//Effekseer::Handle

//==== 定数・マクロ定義 ====
#define EFK_INS Effect::GetInstance()	//インスタンス取得の省略

//==== クラス ====
class Effect	//エフェクト管理
{
public:
	Effect();
	~Effect();
	void Update();
	void Draw();

	/// <summary>
	/// エフェクトの読み込み
	/// </summary>
	/// <param name="path">：エフェクトのファイルパス</param>
	void Load(const char16_t* path);

	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="path">：エフェクトのファイルパス</param>
	/// <param name="pos">：再生時の位置</param>
	/// <param name="pHandle">：再生エフェクトのハンドルを保存する変数のポインタ</param>
	/// <param name="loop">：ループ再生するかどうか(するならtrue)</param>
	/// <param name="pPos">：ループ再生する際のエフェクトの位置のポインタ(再生位置が動かないならnullptr)</param>
	void Play(const char16_t* path, DXf3 pos, Effekseer::Handle* pHandle, bool loop = false, DXf3* pPos = nullptr);

	/// <summary>
	/// エフェクトの再生を止める
	/// </summary>
	/// <param name="handle">：再生エフェクトのハンドル</param>
	void Stop(Effekseer::Handle handle);

	/// <summary>
	/// エフェクトの位置に値を加算
	/// </summary>
	/// <param name="handle">：変更を施すエフェクトのハンドル</param>
	/// <param name="move">：加算する値</param>
	void AddPos(Effekseer::Handle handle, DirectX::XMFLOAT3 move);

	/// <summary>
	/// エフェクトの位置を設定
	/// </summary>
	/// <param name="handle">：変更を施すエフェクトのハンドル</param>
	/// <param name="pos">：設定する位置</param>
	void SetPos(Effekseer::Handle handle, DirectX::XMFLOAT3 pos);

	/// <summary>
	/// カメラを設定
	/// </summary>
	/// <param name="pCamera">：カメラのアドレス</param>
	void SetCamera(Camera* pCamera);

	/// <summary>
	/// エフェクト管理するクラスのポインタを入手
	/// </summary>
	/// <returns></returns>
	static Effect* GetInstance();

	/// <summary>
	/// エフェクト管理するクラスの解放
	/// </summary>
	/// <returns></returns>
	static void DeleteInstance();

private://静的メンバ変数
	static Effect* m_pInstance;

private://構想体定義
	struct LoopData
	{
		Effekseer::Handle*	pHandle;
		const char16_t*		file;
		EFf3 pos;
		DXf3* pPos;
	};


private:
	//---- エフェクト関連 ----
	Effekseer::ManagerRef							m_efkManager;		//エフェクトマネージャー
	EffekseerRendererDX11::RendererRef				m_efkRenderer;		//エフェクトのレンダラー
	std::map<const char16_t*, Effekseer::EffectRef> m_Effect;			//エフェクトのデータを管理する
	std::vector<LoopData>							m_LoopHandle;		//ループ再生するエフェクト情報を保存
	int												m_efkTime;			//時間

	//---- その他必要な情報 ----
	Camera* m_pCamera;	//カメラのアドレス
};

//==== プロトタイプ宣言 ====

/// <summary>
/// DirectX::XMFLOAT3 を Effekseer::Vector3Dに変換
/// </summary>
/// <param name="f3">：変換したいDirectX::XMFLOAT3</param>
Effekseer::Vector3D DXfloat3ToEFv3d(DirectX::XMFLOAT3 f3);

#endif