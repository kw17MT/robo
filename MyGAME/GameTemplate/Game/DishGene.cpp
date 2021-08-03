#include "stdafx.h"
#include "DishGene.h"
#include "Level.h"
#include "Dish.h"
#include "SoundSource.h"

#include <string>

namespace
{
	const int NAME_SIZE_WAYPOINT = 8;
	const int NAME_SIZE_HITOKETA = 9;
	const int NAME_SIZE_HUTAKETA = 10;
	const int LETTER_FIRST_FROM_LEFT = 8;
	const int LETTER_SECOND_FROM_LEFT = 9;
}

DishGene::~DishGene()
{
	for (int i = 0;i < m_popedDishNum; i++) {
		DeleteGO(m_Dish[i]);
	}
}

bool DishGene::Start()
{
	Level level;

	//WayPointと名前の付いたオブジェクトを探す。
	const wchar_t* wayPoint = L"WayPoint";
	std::vector<DishData> dishData;
	level.Init("Assets/level/dishWayPoint.tkl", [&](ObjectData& objectData) {
		//WayPointを含む名前のオブジェクトで、
		if (wcsncmp(objectData.name, wayPoint, NAME_SIZE_WAYPOINT) == 0) {
			int dishNumber = 0;
			//長さで1桁か二けたか見分ける
			//一桁だったらname[8]だけとってこればいい
			//WayPoint1~9の時
			if (wcslen(objectData.name) == NAME_SIZE_HITOKETA) {
				//数字部分の左から1番目をとってきて、皿の番号に設定する。
				dishNumber = _wtoi(&objectData.name[LETTER_FIRST_FROM_LEFT]);
				DishData dishdata;
				dishdata.s_dishPosition = objectData.Pos;
				dishdata.s_number = dishNumber;

				m_dishData[dishNumber - 1] = dishdata;
			}
			//二けただったらname[8][9]を順にとってきてcatする→intにかえてやる
			//WayPoint10~
			else
			{
				//数字部分の左から1番目、2番目をとってきて
				wchar_t c_num10 = objectData.name[LETTER_FIRST_FROM_LEFT];
				wchar_t c_num1 = objectData.name[LETTER_SECOND_FROM_LEFT];
				int i_num10 = _wtoi(&c_num10);
				int i_num1 = _wtoi(&c_num1);
				string str10 = to_string(i_num10);
				string str1 = to_string(i_num1);
				str10 = str10 + str1;
				//皿の番号に設定してやる
				dishNumber = atoi(str10.c_str());
				DishData dishdata;
				dishdata.s_dishPosition = objectData.Pos;
				dishdata.s_number = dishNumber;

				m_dishData[dishNumber - 1] = dishdata;
			}


			m_Dish[m_popedDishNum] = NewGO<Dish>(0);
			m_Dish[m_popedDishNum]->SetPosition(objectData.Pos);
			//皿を一つ出したので変数をインクリメント
			m_popedDishNum++;

			if (m_popedDishNum == m_dishMaxNum) return true;

			return true;
		}
		return true;
	});
	//ここまできたら全部出たはずなので稼働状態をFALSEに
	m_dishGeneState = false;

	return true;
}

Vector3 DishGene::GetDishPositionBasedNumber(int number)
{
	return m_dishData[number].s_dishPosition;
}