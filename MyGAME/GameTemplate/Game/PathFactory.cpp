#include "stdafx.h"
#include "PathFactory.h"

namespace
{
	static const char* LANEPATH[enLaneNum];
}

PathFactory::PathFactory()
{
	LANEPATH[0] = "Assets/level/waypoint.tkl";
	LANEPATH[1] = "Assets/level/debuffwaypoint.tkl";


	for (int i = 0; i < enLaneNum; i++)
	{
		Path path1;
		path1.Load(LANEPATH[i], 1);
		m_pathVector[0].push_back(path1);

		Path path2;
		path2.Load(LANEPATH[i], 2);
		m_pathVector[1].push_back(path2);
	}
}
