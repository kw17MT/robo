#include "stdafx.h"
#include "RoboMove.h"

namespace 
{
const float MOVE_SPEED = 100.0f;
}

const Vector3 RoboMove::Move(const Vector3 currentPos)
{
	Vector3 nextPos = currentPos;
	Vector3 forwardDirection = g_camera3D->GetForward();
	Vector3 sideDirection = g_camera3D->GetRight();

	if (g_pad[0]->GetLStickXF())
	{
		nextPos += sideDirection * MOVE_SPEED * g_pad[0]->GetLStickXF();
	}

	if (g_pad[0]->GetLStickYF())
	{
		nextPos += forwardDirection * MOVE_SPEED * g_pad[0]->GetLStickYF();
	}

	if (g_pad[0]->IsPress(enButtonY))
	{
		nextPos.y += MOVE_SPEED * 5.0f;
	}

	if (g_pad[0]->IsPress(enButtonA))
	{
		nextPos.y -= MOVE_SPEED * 5.0f;
	}

	return nextPos;
}