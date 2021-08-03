#pragma once

typedef std::vector<int> HamBurger;

enum EnHamBurger
{
	enNon = -1,				//何もなし。
	enCheeseBurger = 0,		//チーズバーガー。
	enTomatoBurger,			//トマトバーガー。
	enEggBurger,			//エッグバーガー。
	enBasicBurger,			//普通のハンバーガー(1)
	enDoubleBurger,			//ダブルバーガー(2)
	enVegetableBurger,		//野菜バーガー(2)
	enBaconBurger,			//ベーコンバーガー(3)
	enBLTBurger,			//BLTバーガー(4)
	enOnionBurger,			//オニオンバーガー(5)
	enHighcalBurger,		//ハイカロリーバーガー(5)
	enOnOnionBurger,		//オンオニオンバーガー(5)
	enHamBurgerNum			//ハンバーガーの数。
};

/**
 * @brief ハンバーガーのレシピを作る。
*/
class HamBurgerFactory
{
private:
	HamBurgerFactory();
	~HamBurgerFactory() {}

public:
	/**
	 * @brief インスタンスを取得する。
	 * @return インスタンス
	*/
	static HamBurgerFactory& GetInstance()
	{
		static HamBurgerFactory instance;
		return instance;
	}

	/**
	 * @brief ハンバーガーを取得。
	 * @param enHamBurger ハンバーガーの種類
	 * @return ハンバーガーの情報を取得
	*/
	HamBurger& GetHamBurger(EnHamBurger enHamBurger)
	{
		return m_hamBurgers[enHamBurger];
	}
private:
	/**
	 * @brief ハンバーガーの配列。
	*/
	HamBurger m_hamBurgers[enHamBurgerNum];

};

//ハンバーガーファクトリーのインスタンスを取得。
static inline HamBurgerFactory& GetHamBurgerFactory()
{
	return HamBurgerFactory::GetInstance();
}
