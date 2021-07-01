#pragma once

#include  "HamBurgerFactory.h"

class SpriteRender;
class MenuTimer;
class MissCounter;
class Counter;
class CSoundSource;
class CLevel2D;
class Level;
#include "Level2D.h"

static const int SHOW_HAMBURGER_SPECIES = 11;
static const int SHOW_HAMBURGER_NUMBER = 3;

/// <summary>
/// レベル2D処理のサンプルクラス。
/// </summary>
class CLevel2D : public IGameObject
{
	~CLevel2D();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

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

	//

	/**
	 * @brief ハンバーガーが一致しているかどうかを取得
	 * @param numbers 積み上げた具材の種類。
	 * @param size ハンバーガーの層数
	 * @param counterNo 作られたハンバーガーの番号
	 * @return TRUE＝すべて一致、FALSE＝どこかちがう
	*/
	bool GetIsMatchHamBurger(int* numbers, int size, int counterNo);

	/**
	 * @brief 画像を動かす関数。
	 * @param number 画像の状態 enMenuSlideState
	*/
	void SpriteSet(int number);

	/**
	 * @brief 画像の移動量を取得
	 * @param i 移動量
	 * @return 
	*/
	float GetSlideAmount(int i) { return m_slideAmount[i]; }

	enum enMenuSlideState {
		enStop,
		enSlideUp,
		enSlideDown,
		enSlideStateNum
	};

private:
	Level2D m_level2D;										//レベル2D。
	Vector3 m_slidePos[SHOW_HAMBURGER_NUMBER];				//メニューを上か下にスライド移動
	float m_slideAmount[SHOW_HAMBURGER_NUMBER] = { 0.0f };	//画像の上下移動量
	int m_slide[SHOW_HAMBURGER_NUMBER] = { 0 };			//メニュー画像のスライドフラグ。0で動かない、1で上にスライド、2で下にスライド。
	bool m_TimeUpSet[2] = { false };					//メニューのタイムオーバー中か？
	SpriteRender* sprite[enHamBurgerNum] = {nullptr};
	int m_randNum[SHOW_HAMBURGER_NUMBER] = { 0 };


	//3つのスプライトの座標。
	Vector3 m_spritePositions[SHOW_HAMBURGER_NUMBER];

	//表示しているハンバーガーの配列。
	EnHamBurger m_showHamBurgers[SHOW_HAMBURGER_SPECIES];

	//ハンバーガースプライトのデータ。ハンバーガーの数だけ用意する。
	Level2DObjectData m_level2DObjectDatas[enHamBurgerNum];
	MenuTimer* m_menuTimer[2] = { nullptr, nullptr };
	MissCounter* m_missCounter = nullptr;
	Counter* m_counter00 = nullptr;
	Counter* m_counter01 = nullptr;
	CSoundSource* m_slideSe[3] = { nullptr, nullptr, nullptr };
};