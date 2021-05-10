#include "stdafx.h"
#include "HamBurgerFactory.h"

HamBurgerFactory::HamBurgerFactory()
{
	//ハンバーガーのデータを作る。
	//０．チーズ　
	//１．エッグ　
	//２．レタス　
	//３．パテ　
	//４．トマト
	//５．オニオン  ※追加具材
	//６．ベーコン　※追加具材

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
	egg.push_back(4);
	egg.push_back(1);
	egg.push_back(3);
	m_hamBurgers[enEggBurger] = egg;
	
	//以下、追加メニュー
	//普通のバーガー
	HamBurger basic;
	basic.push_back(3); //パティ
	m_hamBurgers[enBasicBurger] = basic;
	
	//ダブルバーガー
	HamBurger double_bur;
	double_bur.push_back(3); //パティ
	double_bur.push_back(3); //パティ
	m_hamBurgers[enDoubleBurger] = double_bur;

	//野菜バーガー
	HamBurger vegetable;
	vegetable.push_back(4); //トマト
	vegetable.push_back(5); //オニオン
	m_hamBurgers[enVegetableBurger] = vegetable;

	//ベーコンバーガー
	HamBurger bacon;
	bacon.push_back(2); //レタス
	bacon.push_back(0); //チーズ
	bacon.push_back(6); //ベーコン
	m_hamBurgers[enBaconBurger] = bacon;

	//BLTバーガー
	HamBurger blt;
	blt.push_back(4); //トマト
	blt.push_back(2); //レタス
	blt.push_back(2); //レタス
	blt.push_back(6); //ベーコン
	m_hamBurgers[enBLTBurger] = blt;

	//オニオンバーガー
	HamBurger onion;
	onion.push_back(5); //オニオン
	onion.push_back(4); //トマト
	onion.push_back(5); //オニオン
	onion.push_back(0); //チーズ
	onion.push_back(3); //パティ
	m_hamBurgers[enOnionBurger] = onion;

	//ハイカロリーバーガー
	HamBurger highcal;
	highcal.push_back(6); //ベーコン
	highcal.push_back(0); //チーズ
	highcal.push_back(3); //パティ
	highcal.push_back(0); //チーズ
	highcal.push_back(1); //エッグ
	m_hamBurgers[enHighcalBurger] = highcal;

	//オンオニオンバーガー
	HamBurger ononion;
	ononion.push_back(6); //ベーコン
	ononion.push_back(0); //チーズ
	ononion.push_back(5); //オニオン
	ononion.push_back(5); //オニオン
	ononion.push_back(5); //オニオン
	m_hamBurgers[enOnOnionBurger] = ononion;

}