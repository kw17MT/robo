#include "stdafx.h"
#include "PathFactory.h"

namespace
{
	static const char* LANEPATH[1];
}

PathFactory::PathFactory()
{
	LANEPATH[0] = "Assets/level/dishWayPoint.tkl";

	Path path1;
	path1.Load(LANEPATH[0], 1);
	m_pathVector.push_back(path1);
}
