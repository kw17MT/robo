#pragma once

#include  "HamBurgerFactory.h"

class SpriteRender;
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
	bool GetIsMatchHamBurger(int* numbers);
private:
	Level2D m_level2D;		//レベル2D。
	//Sprite m_sprite;			//スプライト。
	Vector3 m_position;		//座標。
	Vector3 m_scale;		//大きさ。

	SpriteRender* sprite[enHamBurgerNum];

	//3つのスプライトの座標。
	Vector3 m_spritePositions[SHOW_HAMBURGER_NUMBER];

	//表示しているハンバーガーの配列。
	EnHamBurger m_showHamBurgers[SHOW_HAMBURGER_NUMBER];

	//ハンバーガースプライトのデータ。ハンバーガーの数だけ用意する。
	Level2DObjectData m_level2DObjectDatas[enHamBurgerNum];
};