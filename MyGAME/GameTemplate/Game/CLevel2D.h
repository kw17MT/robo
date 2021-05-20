#pragma once

#include  "HamBurgerFactory.h"

class SpriteRender;
class MenuTimer;
class MissCounter;
class Counter;
class CSoundSource;
#include "Level2D.h"

static const int SHOW_HAMBURGER_NUMBER = 3;

/// <summary>
/// レベル2D処理のサンプルクラス。
/// </summary>
class CLevel2D : public IGameObject
{
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	//不要
	//Level2Dから渡されたname(パスの一部）を用いてSpriteRenderで使うconst char*型を作る。
	//const char* MakePathFromName(const char* name);

	//次に表示するハンバーガーを決めるお(´・ω・｀)！
	void Roulette(int number);

	//ハンバーガーの画像を表示するお！
	void ShowHamBurger(int number,EnHamBurger enHamBurger);
public:
	//表示している3つのハンバーガーを取得。
	EnHamBurger* GetShowHamBurgers()
	{
		return m_showHamBurgers;
	}

	//ハンバーガーが一致しているかどうかを取得
	bool GetIsMatchHamBurger(int* numbers, int size, int counterNo);

	//画像を動かす関数。
	void SpriteSet(int number);

	//画像の移動量を取得
	float GetSlideAmount(int i) { return m_slideAmount[i]; }
private:
	Level2D m_level2D;		//レベル2D。
	//Sprite m_sprite;			//スプライト。
	Vector3 m_position;		//座標。
	Vector3 m_scale;		//大きさ。
	Vector3 m_slidePos[SHOW_HAMBURGER_NUMBER];		//移動
	float m_slideAmount[SHOW_HAMBURGER_NUMBER] = { 0.0f }; //画像の上下移動量
	int m_slide[SHOW_HAMBURGER_NUMBER] = { 0 };			//メニュー画像のスライドフラグ。0で動かない、1で上にスライド、2で下にスライド。
	bool m_TimeUpSet[2] = { false };					//メニューのタイムオーバー中か？
	SpriteRender* sprite[enHamBurgerNum];
	int m_randNum[SHOW_HAMBURGER_NUMBER] = { 0 };


	//3つのスプライトの座標。
	Vector3 m_spritePositions[SHOW_HAMBURGER_NUMBER];

	//表示しているハンバーガーの配列。
	EnHamBurger m_showHamBurgers[SHOW_HAMBURGER_NUMBER];

	//ハンバーガースプライトのデータ。ハンバーガーの数だけ用意する。
	Level2DObjectData m_level2DObjectDatas[enHamBurgerNum];

	MenuTimer* m_menuTimer[2];
	MissCounter* m_missCounter;
	Counter* m_counter01;
	Counter* m_counter02;
	CSoundSource* m_slideSe[3];
};