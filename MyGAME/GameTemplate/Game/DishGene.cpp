#include "stdafx.h"
#include "DishGene.h"
#include "Level.h"
#include "Dish.h"

//テストで出してるだけ、消すこと
#include "Guzai.h"

DishGene::~DishGene()
{
	for (int i = 0;i < DishNum; i++) {
		DeleteGO(m_Dish[i]);
	}
}

bool DishGene::Start()
{
	Level level;

	const wchar_t* WayPoint = L"WayPoint";
	level.Init("Assets/level/WayPoint_new.tkl", [&](ObjectData& objectData) {
		if (wcsncmp(objectData.name, WayPoint, 8) == 0) {
			m_Dish[DishNum] = NewGO<Guzai/*Dish*/>(0);
			m_Dish[DishNum]->SetPosition(objectData.Pos);
			//m_Dish[DishNum]->SetGuzaiNo(1);
			DishNum++;
			return true;
		}
		





		});

	return true;
}

void DishGene::PopDishes()
{

}