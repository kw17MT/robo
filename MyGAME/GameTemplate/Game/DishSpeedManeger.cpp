#include "stdafx.h"
#include "DishSpeedManeger.h"
#include <ctime>
#include <cstdlib>
#include <random>

void DishSpeedManeger::Update()
{
	countFrame++;
	if (countFrame >= 30) {
		elapseTime++;
		countFrame = 0;
	}

	if (elapseTime == speedUpTimeStart) {
		isSpeedUp = true;
	}

	if (elapseTime == speedUpTimeEnd) {
		isSpeedUp = false;
		elapseTime = 0;

		//TimeStart,End‚Ìƒ‰ƒ“ƒ_ƒ€ŠÖ”‚Å‚Ì‰Šú‰»
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<int> rand(10, 20);

		speedUpTimeStart = rand(mt);
		speedUpTimeEnd = speedUpTimeStart + 5;

	}
}