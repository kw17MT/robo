#include "stdafx.h"
#include "DishManager.h"
#include <ctime>
#include <cstdlib>
#include <random>

//void DishManeger
//{
//	countFrame++;
//	if (countFrame >= 30) {
//		elapseTime++;
//		countFrame = 0;
//	}
//
//	if (elapseTime == speedUpTimeStart) {
//		isSpeedUp = true;
//	}
//
//	if (elapseTime == speedUpTimeEnd) {
//		isSpeedUp = false;
//		elapseTime = 0;
//
//		//TimeStart,Endのランダム関数での初期化
//		std::random_device rnd;
//		std::mt19937 mt(rnd());
//		std::uniform_int_distribution<int> rand(10, 20);
//
//		speedUpTimeStart = rand(mt);
//		speedUpTimeEnd = speedUpTimeStart + 5;
//
//	}
//}