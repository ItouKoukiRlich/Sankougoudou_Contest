#ifndef ___CREATEENEMY_H___
#define ___CREATEENEMY_H___

//==== インクルード部 ====
#include"Enemy.h"

class CreateEnemy : public Enemy
{
public:
	CreateEnemy();
	~CreateEnemy() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// ミッション対象のフラグを設定
	/// </summary>
	/// <param name="flag">：ミッション対象ならtrue</param>
	static void SetMissionFlag(bool flag);


	static void staticDraw(Camera* pCamera, DXf3 pos);

private:
	static Model* m_pModel;
	static bool m_bMission;
	static Texture* m_pTexture;

private:
	void DrawAttackEffect();

private:
	unsigned int m_nAttackCount;
	bool m_bAttckFlag;
	float m_Alpha;
	float m_Upscal;
};

#endif