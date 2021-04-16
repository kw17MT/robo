#include "stdafx.h"
#include "HamBurgerFactory.h"

HamBurgerFactory::HamBurgerFactory()
{
	//ハンバーガーのデータを作る。
	HamBurger cheese;
	cheese.push_back(3);
	cheese.push_back(0);
	cheese.push_back(2);
	m_hamBurgers[enCheeseBurger] = cheese;

	HamBurger tomato;
	tomato.push_back(3);
	tomato.push_back(4);
	tomato.push_back(4);
	tomato.push_back(2);
	m_hamBurgers[enTomatoBurger] = tomato;

	HamBurger egg;
	egg.push_back(2);
	egg.push_back(0);
	egg.push_back(4);
	egg.push_back(3);
	m_hamBurgers[enEggBurger] = egg;
}