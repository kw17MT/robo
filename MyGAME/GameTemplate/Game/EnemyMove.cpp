#include "stdafx.h"
#include "EnemyMove.h"

namespace
{
	const float MOVE_SPEED = 10.0f;
}

Vector3 EnemyMove::CalcNextPos(Vector3 position)
{
	Vector3 nextPos = position;
	if (nextPos.y >= 1000.0f)
	{
		m_isTop = true;
	}
	else if (nextPos.y <= 0.0f)
	{
		m_isTop = false;
	}

	if (m_isTop) {
		nextPos.y -= MOVE_SPEED;
	}
	else
	{
		nextPos.y += MOVE_SPEED;
	}
	

	return nextPos;
}