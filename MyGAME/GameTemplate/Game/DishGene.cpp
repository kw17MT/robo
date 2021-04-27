#include "stdafx.h"
#include "DishGene.h"
#include "Level.h"
#include "Dish.h"


#include <string>

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
	std::vector<DishData> dishData;
	level.Init("Assets/level/WayPoint_new.tkl", [&](ObjectData& objectData) {
		if (wcsncmp(objectData.name, WayPoint, 8) == 0) {
			//’·‚³‚Å1Œ…‚©“ñ‚¯‚½‚©Œ©•ª‚¯‚é
			//ˆêŒ…‚¾‚Á‚½‚çname[9]‚¾‚¯‚Æ‚Á‚Ä‚±‚ê‚Î‚¢‚¢
			if (wcslen(objectData.name) == 9) {
				int dishNumber = _wtoi(&objectData.name[8]);
				DishData dishdata;
				dishdata.s_dishPosition = objectData.Pos;
				dishdata.s_number = dishNumber;

				m_dishData[dishNumber - 1] = dishdata;
			}

			//“ñ‚¯‚½‚¾‚Á‚½‚çname[9][10]‚ğ‡‚É‚Æ‚Á‚Ä‚«‚Äcat‚·‚é¨int‚É‚©‚¦‚Ä‚â‚é
			else
			{
				wchar_t c_num10 = objectData.name[8];
				wchar_t c_num1 = objectData.name[9];
				int i_num10 = _wtoi(&c_num10);
				int i_num1 = _wtoi(&c_num1);
				string str10 = to_string(i_num10);
				string str1 = to_string(i_num1);
				str10 = str10 + str1;
				//a = wcscat(&a, &objectData.name[9]);
				int dishNumber = atoi(str10.c_str());
				DishData dishdata;
				dishdata.s_dishPosition = objectData.Pos;
				dishdata.s_number = dishNumber;

				m_dishData[dishNumber - 1] = dishdata;
			}


			m_Dish[DishNum] = NewGO<Dish>(0);
			m_Dish[DishNum]->SetPosition(objectData.Pos);
			DishNum++;
			return true;
		}
	});

	DishGeneState = false;

	return true;
}

Vector3 DishGene::GetDishPositionBasedNumber(int number)
{
	return m_dishData[number].s_dishPosition;
}