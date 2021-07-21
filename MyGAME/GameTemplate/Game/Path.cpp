#include "stdafx.h"
#include "Path.h"
#include "Level.h"

Path::Path()
{

}

Path::~Path()
{

}

void Path::Load(const char* filePath,int guzaiNo)
{
	Level level;
	const wchar_t* side;
	side = L"WayPoint";

	//std::vector<Point> poinstList;
	level.Init(filePath, [&](ObjectData& objectData) {
		//名前の上から8文字が WayPoint ならば
		if (wcsncmp(objectData.name, side, 8) == 0) {
			//文字列をint型に変換
			int pointNm = _wtoi(&objectData.name[8]);
			Point point;
			point.s_vector = objectData.Pos;
			point.s_number = pointNm;
			m_pointList[pointNm - 1] = point;
			return true;
		}

		return true;
	});
}

Point* Path::GetNearPoint(const Vector3& pos)
{
	//最初のポイントを持ってくる。
	Point* point = &m_pointList[0];
	//引数座標から最初のポイントに向かうベクトルを求める。
	Vector3 diff = m_pointList[0].s_vector - pos;
	//今の場所から一番近いポイント探す。
	for (int i = 1; i < 36/*m_pointList.size()*/; i++) {
		//引数座標からi番目のポイントに向かうベクトルを求める。
		Vector3 diff2 = m_pointList[i].s_vector - pos;
		//求めたベクトルの長さのほうが短かったら。
		if (diff.LengthSq() > diff2.LengthSq()) {
			//diffに求めたベクトルを代入。
			diff = diff2;
			point = &m_pointList[i];
		}
	}
	return point;
}

Point* Path::GetPoint(const int& number)
{
	//番号が最後のポイントだったら最初のポイント
	if (number == m_pointList.size()) {
		return &m_pointList[0];
	}
	else {
		return &m_pointList[number];
	}
}