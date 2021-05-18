#pragma once

//ハンバーガーの種類を追加するときenumに種類を追加後、cpp側のコンストラクタでvectorに具材の種類を追加してやる。

//enum EnHamBurger
//{
//	enNon = -1,	//何もなし。
//	enCheeseBurger = 0,		//チーズバーガー。
//	enTomatoBurger,		//トマトバーガー。
//	enEggBurger,		//エッグバーガー。
//	//enTikimi
//	enHamBurgerNum			//ハンバーガーの数。
//};

#include "HamBurgerFactory.h"
#include "CLevel2D.h"
class SkinModelRender;
class PlayerGene;
class SpriteRender;
class Burger;
class CSoundSource;

class Counter : public IGameObject
{
private:
	Vector3 m_counterPos;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_burgerPos = Vector3::Zero;
	bool m_burgerCorrectFlag = false;		//提出されたバーガーが合っているフラグ。
	bool m_burgerMistakeFlag = false;		//提出されたバーガーが間違っているフラグ。

	Vector3 m_spritePos01 = { 500.0f,-200.0f,0.0f };
	Vector3 m_spritePos02 = { -500.0f,-200.0f,0.0f };
	bool m_spriteFlag01 = false;
	bool m_spriteFlag02 = false;
	int m_spriteTime01 = 0;
	int m_spriteTime02 = 0;

	bool m_spriteCompareFlagTrue[4][5] = { false };		//メニューと一致しているときの画像表示フラグ
	bool m_spriteCompareFlagFalse[4][5] = { false };	//メニューと不一致のときの画像表示フラグ
	int m_guzaiJudge[4][5] = { 2 };						//積まれた具材が合っているか？0,違っている。1,合っている。2,積まれていない。

	//表示できるメニューの種類。
	int menuNum = 3;

	//０．チーズ　１．エッグ　２．レタス　３．パテ　４．トマト

	/*int const burger01[3] = { 3,0,2 };
	int const burger02[4] = { 3,4,4,2 };
	int const burger03[4] = { 2,4,1,3 };
	typedef std::vector<int> HamBurger;
	HamBurger m_hamBurgers[enHamBurgerNum];*/

	//int const burger01[3] = { 3,0,2 };
	//int const burger02[4] = { 3,4,4,2 };
	//int const burger03[4] = { 2,4,0,3 };


	//カウンター番号　１が左、２が右
	int CounterNo = 0;
	//何段のハンバーガーを作ったのかの変数
	int StackNum = 0;

public:
	//モデルデータなどを初期化。
	Counter();
	bool Start();
	//Delete関数を呼び出し続ける。
	void Update();
	//バーガーを持った状態でBボタンを押すと診断開始。
	void Delete();
	void SetPosition(Vector3 pos) { m_counterPos = pos; m_position = pos; }
	void SetCounterNo(int num) { CounterNo = num; }
	//何段のハンバーガーをを作ったのかを記録するための関数。
	void SetStackNum(int num) { StackNum = num; }
	void SetStack0() { StackNum = 0; }

	//正しいバーガーかを調べる。
	bool Judge();
	void HamBurgerCompare();
	//表示している3つのハンバーガーを取得。
	EnHamBurger* GetShowHamBurgers()
	{
		return m_showHamBurgers;
	}
	//表示しているハンバーガーの配列。
	EnHamBurger m_showHamBurgers[11];

	int Delay = 0;

	PlayerGene* m_playerGene;
	SkinModelRender* m_skinModelRender;
	SpriteRender* m_spriteJudge01;
	SpriteRender* m_spriteJudge02;
	SpriteRender* m_spriteCompare[4][5];	//メニューと一致しているかの画像を表示
};