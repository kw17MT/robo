#include "stdafx.h"
#include "RoboMove.h"

namespace 
{
const float MOVE_SPEED = 25.0f;
}

const Vector3 RoboMove::Move(const Vector3 currentPos)
{
	Vector3 nextPos = currentPos;

	if (g_pad[0]->GetLStickXF())
	{
		nextPos.x += MOVE_SPEED * g_pad[0]->GetLStickXF();
	}

	if (g_pad[0]->GetLStickYF())
	{
		nextPos.z += MOVE_SPEED * g_pad[0]->GetLStickYF();
	}

	if (g_pad[0]->IsPress(enButtonY))
	{
		nextPos.y += MOVE_SPEED * 0.5f;
	}

	if (g_pad[0]->IsPress(enButtonA))
	{
		nextPos.y -= MOVE_SPEED * 0.5f;
	}

	return nextPos;
}