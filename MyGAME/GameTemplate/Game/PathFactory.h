#pragma once
#include "Path.h"

enum EnLane
{
	enNonLane = -1,
	enNormalLane = 0,		//普通のレーン。
	enDeBuffLane = 1,		//デバフアイテムのレーン。
	enLaneNum = 2		//レーンの数。
};

class PathFactory
{
private:
	PathFactory();
	~PathFactory() {}

public:
	static PathFactory& GetInstance()
	{
		static PathFactory instance;
		return instance;
	}
	Path* GetPath(EnLane enLane, const int guzaiNo)
	{
		return &m_pathVector[guzaiNo - 1][enLane];
	}

private:
	std::vector<Path> m_pathVector[2];
};

