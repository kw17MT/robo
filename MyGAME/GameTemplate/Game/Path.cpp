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
	//左側か右側か決める。
	if (guzaiNo == 1)
	{
		side = L"L";
	}
	else if (guzaiNo == 2)
	{
		side = L"R";
	}
	std::vector<Point> poinstList;
	level.Init(filePath, [&](ObjectData& objectData) {
		if (wcsncmp(objectData.name, side,1) == 0) {
			//文字列をint型に変換
			int pointNm = _wtoi(&objectData.name[1]);
			Point point;
			point.s_vector = objectData.Pos;
			point.s_number = pointNm;
			m_pointList[pointNm - 1] = point;
			//m_pointList.insert(pointNm, point);
			return false;
		}

		return false;
	});
}

Point* Path::GetNearPoint(const Vector3& pos)
{
	Point* point = &m_pointList[0];
	Vector3 diff = m_pointList[0].s_vector - pos;
	//今の場所から一番近いポイント探す
	for (int i = 1; i < m_pointList.size(); i++) {
		Vector3 diff2 = m_pointList[i].s_vector - pos;
		if (diff.LengthSq() > diff2.LengthSq()) {
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