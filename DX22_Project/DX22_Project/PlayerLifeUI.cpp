//==== インクルード部 ====
#include "PlayerLifeUI.h"

namespace namePlayerLifeUI
{
	constexpr DXf2	cg_HpUIpos		= { 0.11f, 0.82f };
	constexpr DXf2	cg_HPBaseScale	= { 0.2f, 0.34f };
	constexpr DXf2	cg_HPScale		= { cg_HPBaseScale.x, cg_HPBaseScale.y};
	constexpr int	cg_MaxNum		= 10;
	constexpr float cg_NumChipSizeU	= 1.0f / (float)cg_MaxNum;
	constexpr DXf2 cg_HyakuPos		= { cg_HpUIpos.x - 0.04f, cg_HpUIpos.y + 0.0f };
	constexpr DXf2 cg_ZyuuPos		= { cg_HpUIpos.x + 0.00f, cg_HpUIpos.y + 0.0f };
	constexpr DXf2 cg_ItiPos		= { cg_HpUIpos.x + 0.04f, cg_HpUIpos.y + 0.0f };
	constexpr float cg_UnderPosY = 0.96f;

	constexpr DXf2  cg_SizeDecreasePerHP = { 0.002f, 0.0034f };						//HP１減る事に小さくなるサイズ
	constexpr float cg_PosDecreasePerHP	 = (cg_UnderPosY - cg_HpUIpos.y) / 100.0f;	//HP１減る事に移動する量
	constexpr int cg_ChangeOldHpUI		 = 30;	//HPが変動してから何フレーム後にUI変更をかけるか

	//ファイルパス
	constexpr char cg_NumChip[] = "Assets/Texture/HP_UI/number.png";
}
using namespace namePlayerLifeUI;



PlayerLifeUI::PlayerLifeUI(Player& player)
	:m_Player(player)
	,m_nLifePrev(namePlayer::cg_InitHp)
	,m_nOldLife(namePlayer::cg_InitHp)
	,m_nFlameCount(0)
{
	//---- テクスチャの読み込み ----
	m_ui.SetUI("Assets/Texture/HP_UI/HP_Base.png",	eBase,		cg_HpUIpos);
	m_ui.SetUI("Assets/Texture/HP_UI/HPprev.png",	eHPprev,	cg_HpUIpos);
	m_ui.SetUI("Assets/Texture/HP_UI/HP.png",		eHP,		cg_HpUIpos);
	m_ui.SetUI(cg_NumChip,	eHyaku,		cg_HyakuPos);
	m_ui.SetUI(cg_NumChip,	eZyuu,		cg_ZyuuPos);
	m_ui.SetUI(cg_NumChip,	eIti,		cg_ItiPos);

	//---- 値の調整 ----
	m_ui.ChangeScale(eHP,		cg_HPScale.x,		cg_HPScale.y);
	m_ui.ChangeScale(eHPprev,	cg_HPScale.x,		cg_HPScale.y);
	m_ui.ChangeScale(eBase,		cg_HPBaseScale.x,	cg_HPBaseScale.y);
	//m_ui.ChangeScale(eHyaku,		cg_HPBaseScale.x,	cg_HPBaseScale.y);
	m_ui.ChangeUVsize(eHyaku,	cg_NumChipSizeU, 1.0f);
	m_ui.ChangeUVsize(eZyuu,	cg_NumChipSizeU, 1.0f);
	m_ui.ChangeUVsize(eIti,		cg_NumChipSizeU, 1.0f);
	
	m_ui.ChangeSize(eHyaku, 0.04, 0.1f);
	m_ui.ChangeSize(eZyuu,	0.04, 0.1f);
	m_ui.ChangeSize(eIti,		0.04, 0.1f);
}

PlayerLifeUI::~PlayerLifeUI()
{

}

void PlayerLifeUI::Update()
{
	//---- プレイヤーのHPを入手 ----
	int hp = m_Player.GetHP();

	//---- プレイヤーのHP減少量を元にHPUIのサイズ・位置を変更 ----
	int		num		= m_nLifePrev - hp;
	float	fnumX	= (float)num * -cg_SizeDecreasePerHP.x;
	float	fnumY	= (float)num * -cg_SizeDecreasePerHP.y;
	m_ui.AddScale(eHP, fnumX, fnumY);
	float fnumYpos = (float)num * cg_PosDecreasePerHP;
	m_ui.AddPos(eHP, 0.0f, fnumYpos);
	
	//HPを保存
	m_nLifePrev = hp;

	//過去のダメージUIの調整
	if (m_nOldLife < hp)
	{//増えていたら今のHPに合わせる
		m_ui.AddScale(eHPprev, fnumX, fnumY);
		m_ui.AddPos(eHPprev, 0.0f, fnumYpos);
		m_nOldLife = hp;
	}
	if (m_nOldLife > hp)
	{//減っていたら
		m_nFlameCount++;

		if (m_nFlameCount > cg_ChangeOldHpUI)
		{
			m_ui.AddScale(eHPprev, -cg_SizeDecreasePerHP.x, -cg_SizeDecreasePerHP.y);
			m_ui.AddPos(eHPprev, 0.0f, cg_PosDecreasePerHP);
			m_nOldLife--;
		}

		//同じになったら値を初期化
		if (m_nOldLife == hp)
		{
			m_nFlameCount = 0;
		}
	}
	

	//---- プレイヤーのHPを元に数字の表示を設定 ----
	//百の位
	num = hp / 100;
	m_ui.ChangeUVpos(eHyaku, (float)num * cg_NumChipSizeU, 1.0f);
	hp = hp % 100;

	//十の位
	num = hp / 10;
	m_ui.ChangeUVpos(eZyuu, (float)num * cg_NumChipSizeU, 1.0f);
	hp = hp % 10;

	//一の位
	num = hp / 1;
	m_ui.ChangeUVpos(eIti, (float)num * cg_NumChipSizeU, 1.0f);
}

void PlayerLifeUI::Draw()
{
	m_ui.Draw();
}