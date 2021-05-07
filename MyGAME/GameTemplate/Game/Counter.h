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
class SkinModelRender;
class PlayerGene;

class Counter : public IGameObject
{
private:
	Vector3 m_counterPos;
	Vector3 m_position = Vector3::Zero;

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

	int Delay = 0;

	PlayerGene* m_playerGene;
	SkinModelRender* m_skinModelRender;
};